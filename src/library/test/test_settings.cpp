/**
 * Part of https://github.com/ManelJimeno/bootstrap (C) 2022
 * Authors: Manel Jimeno <manel.jimeno@gmail.com>
 * License: http://www.opensource.org/licenses/mit-license.php MIT
 */

#include "settings.h"
#include <gtest/gtest.h>

using namespace project_library;

TEST(Settings, Filesystem_save)
{
    Settings settings("settings", "appdata", false, Settings::Format::Filesystem);
    settings.setString("section.value1", "string");
    settings.setInt("section.value2", 123);
    settings.setDouble("section.value3", 321.123);
    settings.setBool("section.value4", false);
    settings.save();
}

TEST(Settings, Filesystem_load)
{
    Settings settings("settings", "appdata", false, Settings::Format::Filesystem);
    settings.load();
    EXPECT_EQ(settings.getString("section.value1"), "string");
    EXPECT_EQ(settings.getInt("section.value2"), 123);
    EXPECT_EQ(settings.getDouble("section.value3"), 321.123);
    EXPECT_EQ(settings.getBool("section.value4"), false);
}

TEST(Settings, JSON_save)
{
    Settings settings("settings.json", "appdata", false, Settings::Format::JSON);
    settings.setString("section.value1", "string");
    settings.setInt("section.value2", 123);
    settings.setDouble("section.value3", 321.123);
    settings.setBool("section.value4", false);
    settings.save();
}

TEST(Settings, JSON_load)
{
    Settings settings("settings.json", "appdata", false, Settings::Format::JSON);
    settings.load();
    EXPECT_EQ(settings.getString("section.value1"), "string");
    EXPECT_EQ(settings.getInt("section.value2"), 123);
    EXPECT_EQ(settings.getDouble("section.value3"), 321.123);
    EXPECT_EQ(settings.getBool("section.value4"), false);
}

TEST(Settings, IniFile_save)
{
    Settings settings("settings.ini", "appdata", false, Settings::Format::IniFile);
    settings.setString("section.value1", "string");
    settings.setInt("section.value2", 123);
    settings.setDouble("section.value3", 321.123);
    settings.setBool("section.value4", false);
    EXPECT_ANY_THROW(settings.save());
}

TEST(Settings, IniFile_load)
{
    Settings settings("settings.ini", "appdata", false, Settings::Format::IniFile);
    settings.load();
    EXPECT_EQ(settings.getString("section.value1"), "string");
    EXPECT_EQ(settings.getInt("section.value2"), 123);
    EXPECT_EQ(settings.getDouble("section.value3"), 321.123);
    EXPECT_EQ(settings.getBool("section.value4"), false);
}

TEST(Settings, XML_save)
{
    Settings settings("settings.xml", "appdata", false, Settings::Format::XML);
    settings.setString("section.value1", "string");
    settings.setInt("section.value2", 123);
    settings.setDouble("section.value3", 321.123);
    settings.setBool("section.value4", false);
    settings.save();
}

TEST(Settings, XML_load)
{
    Settings settings("settings.xml", "appdata", false, Settings::Format::XML);
    settings.load();
    EXPECT_EQ(settings.getString("section.value1"), "string");
    EXPECT_EQ(settings.getInt("section.value2"), 123);
    EXPECT_EQ(settings.getDouble("section.value3"), 321.123);
    EXPECT_EQ(settings.getBool("section.value4"), false);
}

TEST(Settings, PropertyFile_save)
{
    Settings settings("settings.prop", "appdata", false, Settings::Format::PropertyFile);
    settings.setString("section.value1", "string");
    settings.setInt("section.value2", 123);
    settings.setDouble("section.value3", 321.123);
    settings.setBool("section.value4", false);
    settings.save();
}

TEST(Settings, PropertyFile_load)
{
    Settings settings("settings.prop", "appdata", false, Settings::Format::PropertyFile);
    settings.load();
    EXPECT_EQ(settings.getString("section.value1"), "string");
    EXPECT_EQ(settings.getInt("section.value2"), 123);
    EXPECT_EQ(settings.getDouble("section.value3"), 321.123);
    EXPECT_EQ(settings.getBool("section.value4"), false);
}

#ifdef _WIN32
TEST(Settings, WinRegistry_save)
{
    Settings settings("HKEY_CURRENT_USER\\Software\\Manel\\Test", "", false, Settings::Format::WinRegistry);
    settings.setString("value1", "string");
    settings.setInt("value2", 123);
    settings.setDouble("value3", 321.123);
    settings.setBool("value4", false);
    settings.save();
}

TEST(Settings, WinRegistry_load)
{
    Settings settings("HKEY_CURRENT_USER\\Software\\Manel\\Test", "", false, Settings::Format::WinRegistry);
    settings.load();
    EXPECT_EQ(settings.getString("value1"), "string");
    EXPECT_EQ(settings.getInt("value2"), 123);
    EXPECT_EQ(settings.getDouble("value3"), 321.123);
    EXPECT_EQ(settings.getBool("value4"), false);
}
#endif
