/**
 * Part of https://github.com/ManelJimeno/bootstrap (C) 2022
 * Authors: Manel Jimeno <manel.jimeno@gmail.com>
 * License: http://www.opensource.org/licenses/mit-license.php MIT
 */

#include "application_settings.h"

ApplicationSettings::ApplicationSettings()
    : project_library::Settings("settings.json", "application", true, project_library::Settings::Format::JSON),
      value1("string"), value2(123), value3(321.123), value4(false)
{
}

void ApplicationSettings::load()
{
    try
    {
        Settings::load();
        value1 = getString("application.value1");
        value2 = getInt("application.value2");
        value3 = getDouble("application.value3");
        value4 = getBool("application.value4");
    }
    catch (project_library::FileNotFound)
    {
        save();
    }
}

void ApplicationSettings::save()
{
    setString("application.value1", value1);
    setInt("application.value2", value2);
    setDouble("application.value3", value3);
    setBool("application.value4", value4);
    Settings::save();
}
