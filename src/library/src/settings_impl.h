/**
 * Part of https://github.com/ManelJimeno/bootstrap (C) 2022
 * Authors: Manel Jimeno <manel.jimeno@gmail.com>
 * License: http://www.opensource.org/licenses/mit-license.php MIT
 */

#pragma once
#include "Poco/AutoPtr.h"
#include "Poco/Path.h"
#include "Poco/Util/AbstractConfiguration.h"
#include "settings.h"
#include <memory>
#include <string>

namespace project_library
{

class SettingsImpl
{
  public:
    /**
     * Constructor
     * if the selected configuration system is stored on disk, on Unix systems,
     * this is the '~/.config/'. On Windows systems, this is '%APPDATA%' (typically C:\Users\user\AppData\Roaming).
     * @param filename filename that store the settings
     * @param pathSuffix is a suffix to add to the path
     */
    explicit SettingsImpl(const std::string& filename, const std::string& pathSuffix, const bool inConfigHome,
                          Settings::Format format) noexcept;

    /**
     * Returns the boolean value of the property with the given name. If the value contains references
     * to other properties (${<property>}), these are expanded.
     * @param key
     * @return the boolean value of the property with the given name
     * @throw NotFoundException if the key does not exist
     * @throw SyntaxException if the property can not be converted to a boolean, the following string values
     * can be converted into a boolean:
     *  - numerical values: non zero becomes true, zero becomes false
     *  - strings: true, yes, on become true, false, no, off become false
     */
    bool getBool(const std::string& key) const;

    /**
     * Returns the double value of the property with the given name. If the value contains references
     * to other properties (${<property>}), these are expanded.
     * @param key
     * @return the double value of the property with the given name
     * @throw NotFoundException if the key does not exist
     * @throw SyntaxException if the property can not be converted to a boolean.
     */
    double getDouble(const std::string& key) const;

    /**
     * Returns the int value of the property with the given name. If the value contains references
     * to other properties (${<property>}), these are expanded.
     * @param key
     * @return the int value of the property with the given name
     * @throw NotFoundException if the key does not exist
     * @throw SyntaxException if the property can not be converted to a boolean.
     */
    int getInt(const std::string& key) const;

    /**
     * Returns the string value of the property with the given name. If the value contains references
     * to other properties (${<property>}), these are expanded.
     * @param key
     * @return the string value of the property with the given name
     * @throw NotFoundException if the key does not exist
     * @throw SyntaxException if the property can not be converted to a boolean.
     */
    std::string getString(const std::string& key) const;

    /**
     * Sets the property with the given key to the given value. An already existing value for the key is overwritten.
     * @param key
     * @param value
     */
    void setBool(const std::string& key, bool value);

    /**
     * Sets the property with the given key to the given value. An already existing value for the key is overwritten.
     * @param key
     * @param value
     */
    void setDouble(const std::string& key, double value);

    /**
     * Sets the property with the given key to the given value. An already existing value for the key is overwritten.
     * @param key
     * @param value
     */
    void setInt(const std::string& key, int value);

    /**
     * Sets the property with the given key to the given value. An already existing value for the key is overwritten.
     * @param key
     * @param value
     */
    void setString(const std::string& key, std::string value);

    /**
     *
     * @param key
     * @return true if and only if the property with the given key exists.
     */
    bool exists(const std::string& key) const;

    /**
     * Load the values from the config source
     */
    void load();

    /**
     * Save the values to the config source
     */
    void save();

  private:
    /**
     * Create the necessary folders to store the settings
     */
    void createFolders();

    Poco::AutoPtr<Poco::Util::AbstractConfiguration> m_config;
    std::string m_filename;
    std::string m_suffix;
    Settings::Format m_format;
    Poco::Path m_rootFolder;
};

} // namespace project_library
