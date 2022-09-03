/**
 * Part of https://github.com/ManelJimeno/bootstrap (C) 2022
 * Authors: Manel Jimeno <manel.jimeno@gmail.com>
 * License: http://www.opensource.org/licenses/mit-license.php MIT
 */

#pragma once
#include "exception.h"
#include "helpers.h"
#include <string>

namespace project_library
{

class NotFoundException : public Exception
{
    using Exception::Exception;
};

class SyntaxException : public Exception
{
    using Exception::Exception;
};

class SettingsImpl;

class Settings
{
    DISABLE_COPY(Settings)
  public:
    /**
     * This enum
     */
    enum class Format
    {
#ifdef _WIN32
        WinRegistry,
#endif
        Filesystem,
        JSON,
        IniFile,
        XML,
        PropertyFile
    };

    /**
     * Constructor
     *
     * @param filename filename that store the settings
     * @param pathSuffix is a suffix to add to the path
     * @param inConfigHome if false uses the current path, if true uses a config home folder, on Unix systems is the
     * '~/.config/', on Windows systems, this is '%APPDATA%' (typically C:\Users\user\AppData\Roaming).
     */
    LIBRARY_API explicit Settings(const std::string& filename, const std::string& pathSuffix = "",
                                  const bool inConfigHome = true, Format format = Format::IniFile) noexcept;

    /**
     * Destructor
     */
    LIBRARY_API ~Settings();

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
    LIBRARY_API bool getBool(const std::string& key) const;

    /**
     * Returns the double value of the property with the given name. If the value contains references
     * to other properties (${<property>}), these are expanded.
     * @param key
     * @return the double value of the property with the given name
     * @throw NotFoundException if the key does not exist
     * @throw SyntaxException if the property can not be converted to a boolean.
     */
    LIBRARY_API double getDouble(const std::string& key) const;

    /**
     * Returns the int value of the property with the given name. If the value contains references
     * to other properties (${<property>}), these are expanded.
     * @param key
     * @return the int value of the property with the given name
     * @throw NotFoundException if the key does not exist
     * @throw SyntaxException if the property can not be converted to a boolean.
     */
    LIBRARY_API int getInt(const std::string& key) const;

    /**
     * Returns the string value of the property with the given name. If the value contains references
     * to other properties (${<property>}), these are expanded.
     * @param key
     * @return the string value of the property with the given name
     * @throw NotFoundException if the key does not exist
     * @throw SyntaxException if the property can not be converted to a boolean.
     */
    LIBRARY_API std::string getString(const std::string& key) const;

    /**
     * Sets the property with the given key to the given value. An already existing value for the key is overwritten.
     * @param key
     * @param value
     */
    LIBRARY_API void setBool(const std::string& key, bool value);

    /**
     * Sets the property with the given key to the given value. An already existing value for the key is overwritten.
     * @param key
     * @param value
     */
    LIBRARY_API void setDouble(const std::string& key, double value);

    /**
     * Sets the property with the given key to the given value. An already existing value for the key is overwritten.
     * @param key
     * @param value
     */
    LIBRARY_API void setInt(const std::string& key, int value);

    /**
     * Sets the property with the given key to the given value. An already existing value for the key is overwritten.
     * @param key
     * @param value
     */
    LIBRARY_API void setString(const std::string& key, std::string value);

    /**
     *
     * @param key
     * @return true if and only if the property with the given key exists.
     */
    LIBRARY_API bool exists(const std::string& key) const;

    /**
     * Load the values from the config source
     */
    LIBRARY_API void load();

    /**
     * Save the values to the config source
     */
    LIBRARY_API void save();

  private:
    PIMPL(SettingsImpl)
};

} // namespace project_library
