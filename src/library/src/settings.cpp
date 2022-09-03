/**
 * Part of https://github.com/ManelJimeno/bootstrap (C) 2022
 * Authors: Manel Jimeno <manel.jimeno@gmail.com>
 * License: http://www.opensource.org/licenses/mit-license.php MIT
 */

#include "settings.h"
#include "settings_impl.h"

namespace project_library
{

Settings::Settings(const std::string& filename, const std::string& pathSuffix, const bool inConfigHome,
                   Settings::Format format) noexcept
    : m_pImpl(new SettingsImpl(filename, pathSuffix, inConfigHome, format))
{
}

Settings::~Settings() = default;

bool Settings::exists(const std::string& key) const
{
    return m_pImpl->exists(key);
}

std::string Settings::getString(const std::string& key) const
{
    return m_pImpl->getString(key);
}

int Settings::getInt(const std::string& key) const
{
    return m_pImpl->getInt(key);
}

double Settings::getDouble(const std::string& key) const
{
    return m_pImpl->getDouble(key);
}

bool Settings::getBool(const std::string& key) const
{
    return m_pImpl->getBool(key);
}

void Settings::setBool(const std::string& key, bool value)
{
    m_pImpl->setBool(key, value);
}

void Settings::setDouble(const std::string& key, double value)
{
    m_pImpl->setDouble(key, value);
}

void Settings::setInt(const std::string& key, int value)
{
    m_pImpl->setInt(key, value);
}

void Settings::setString(const std::string& key, std::string value)
{
    m_pImpl->setString(key, value);
}

void Settings::load()
{
    m_pImpl->load();
}

void Settings::save()
{
    m_pImpl->save();
}

} // namespace project_library
