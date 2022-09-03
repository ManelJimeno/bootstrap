/**
 * Part of https://github.com/ManelJimeno/bootstrap (C) 2022
 * Authors: Manel Jimeno <manel.jimeno@gmail.com>
 * License: http://www.opensource.org/licenses/mit-license.php MIT
 */

#pragma once

#include "exception.h"
#include "settings.h"

class ApplicationSettings : public project_library::Settings
{
  public:
    /**
     * Constructor
     */
    ApplicationSettings();

    /**
     * load values, if the configuration file does not exist, it creates it and stores default values
     */
    void load();

    /**
     * save values
     */
    void save();

    /**
     * Settings
     */
    std::string value1;
    int value2;
    double value3;
    bool value4;
};
