/**
 * Part of https://github.com/ManelJimeno/bootstrap (C) 2022
 * Authors: Manel Jimeno <manel.jimeno@gmail.com>
 * License: http://www.opensource.org/licenses/mit-license.php MIT
 */

#include "settings_impl.h"
#include "Poco/Exception.h"
#include "Poco/File.h"
#include "Poco/FileStream.h"
#include "Poco/Util/FilesystemConfiguration.h"
#include "Poco/Util/IniFileConfiguration.h"
#include "Poco/Util/JSONConfiguration.h"
#include "Poco/Util/PropertyFileConfiguration.h"
#include "Poco/Util/XMLConfiguration.h"
#ifdef _WIN32
#include "Poco/Util/WinRegistryConfiguration.h"
#endif

#define MAP_VALUE_EXCEPTION(x)                                                                                         \
    try                                                                                                                \
    {                                                                                                                  \
        x;                                                                                                             \
    }                                                                                                                  \
    catch (Poco::SyntaxException & syntax)                                                                             \
    {                                                                                                                  \
        throw project_library::SyntaxException(syntax.displayText());                                                  \
    }                                                                                                                  \
    catch (Poco::NotFoundException & notFound)                                                                         \
    {                                                                                                                  \
        throw project_library::NotFoundException(notFound.displayText());                                              \
    }

namespace project_library
{

Poco::AutoPtr<Poco::Util::AbstractConfiguration> factory(const Poco::Path& rootFolder, const std::string& filename,
                                                         Settings::Format format)
{
    Poco::Util::AbstractConfiguration* ptr;
    switch (format)
    {
    case Settings::Format::Filesystem: {
        Poco::Path filePath(rootFolder, filename);
        ptr = new Poco::Util::FilesystemConfiguration(filePath.toString());
        break;
    }
    case Settings::Format::JSON:
        ptr = new Poco::Util::JSONConfiguration();
        break;
    case Settings::Format::IniFile:
        ptr = new Poco::Util::IniFileConfiguration();
        break;
    case Settings::Format::XML:
        ptr = new Poco::Util::XMLConfiguration();
        break;
    case Settings::Format::PropertyFile:
        ptr = new Poco::Util::PropertyFileConfiguration();
        break;
#ifdef _WIN32
    case Settings::Format::WinRegistry:
        ptr = new Poco::Util::WinRegistryConfiguration(filename);
        break;
#endif
    }
    Poco::AutoPtr<Poco::Util::AbstractConfiguration> smart_ptr(ptr);
    return smart_ptr;
}

SettingsImpl::SettingsImpl(const std::string& filename, const std::string& pathSuffix, const bool inConfigHome,
                           Settings::Format format) noexcept
    : m_filename(filename), m_format(format), m_suffix(pathSuffix),
      m_rootFolder(inConfigHome ? Poco::Path::configHome() : Poco::Path::current(), m_suffix)
{
    m_config = factory(m_rootFolder, filename, format);
    createFolders();
}

bool SettingsImpl::exists(const std::string& key) const
{
    MAP_VALUE_EXCEPTION(return m_config->has(key));
}

std::string SettingsImpl::getString(const std::string& key) const
{
    MAP_VALUE_EXCEPTION(return m_config->getString(key))
}

int SettingsImpl::getInt(const std::string& key) const
{
    MAP_VALUE_EXCEPTION(return m_config->getInt(key))
}

double SettingsImpl::getDouble(const std::string& key) const
{
    MAP_VALUE_EXCEPTION(return m_config->getDouble(key))
}

bool SettingsImpl::getBool(const std::string& key) const
{
    MAP_VALUE_EXCEPTION(return m_config->getBool(key))
}

void SettingsImpl::setBool(const std::string& key, bool value)
{
    m_config->setBool(key, value);
}

void SettingsImpl::setDouble(const std::string& key, double value)
{
    m_config->setDouble(key, value);
}

void SettingsImpl::setInt(const std::string& key, int value)
{
    m_config->setInt(key, value);
}

void SettingsImpl::setString(const std::string& key, std::string value)
{
    m_config->setString(key, value);
}

void SettingsImpl::createFolders()
{
#ifdef _WIN32
    if (m_format == project_library::Settings::Format::WinRegistry)
        return;
#endif
    Poco::File newFolder(m_rootFolder.toString());
    if (!newFolder.exists())
    {
        newFolder.createDirectories();
    }
}

void SettingsImpl::load()
{
#ifdef _WIN32
    if (m_format == project_library::Settings::Format::WinRegistry)
        return;
#endif
    Poco::Path filePath(m_rootFolder, m_filename);
    try
    {
        switch (m_format)
        {
        case Settings::Format::Filesystem:
            break;
        case Settings::Format::IniFile:
            m_config.cast<Poco::Util::IniFileConfiguration>()->load(filePath.toString());
            break;
        case Settings::Format::JSON:
            m_config.cast<Poco::Util::JSONConfiguration>()->load(filePath.toString());
            break;
        case Settings::Format::XML:
            m_config.cast<Poco::Util::XMLConfiguration>()->load(filePath.toString());
            break;
        case Settings::Format::PropertyFile:
            m_config.cast<Poco::Util::PropertyFileConfiguration>()->load(filePath.toString());
            break;
#ifdef _WIN32
        case Settings::Format::WinRegistry:
            break;
#endif
        }
    }
    catch (Poco::FileNotFoundException& e)
    {
        throw FileNotFound(e.displayText());
    }
}

void SettingsImpl::save()
{
#ifdef _WIN32
    if (m_format == project_library::Settings::Format::WinRegistry)
        return;
#endif
    if (m_format == Settings::Format::IniFile)
    {
        throw NotImplemented("This implementation of a Configuration only reads properties from a legacy Windows "
                             "initialization (.ini) file, and cannot able to save the info. ");
    }
    // The filesystem create the data file when we do a setXXXX
    if (m_format == Settings::Format::Filesystem)
    {
        return;
    }

    Poco::Path filePath(m_rootFolder, m_filename);
    auto value = filePath.toString();
    Poco::FileOutputStream fos(value);

    switch (m_format)
    {
    case Settings::Format::IniFile:
        break;
    case Settings::Format::Filesystem:
        break;
    case Settings::Format::JSON:
        m_config.cast<Poco::Util::JSONConfiguration>()->save(fos);
        break;
    case Settings::Format::XML:
        m_config.cast<Poco::Util::XMLConfiguration>()->save(fos);
        break;
    case Settings::Format::PropertyFile:
        m_config.cast<Poco::Util::PropertyFileConfiguration>()->save(fos);
        break;
#ifdef _WIN32
    case Settings::Format::WinRegistry:
        break;
#endif
    }
}

} // namespace project_library
