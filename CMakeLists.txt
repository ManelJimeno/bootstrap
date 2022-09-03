#
# Main cmake file.
#
# Part of https://github.com/ManelJimeno/bootstrap (C) 2022
#
# Authors: Manel Jimeno <manel.jimeno@gmail.com>
#
# License: http://www.opensource.org/licenses/mit-license.php MIT
#

cmake_minimum_required(VERSION 3.22.3-3.23)

# Load required programs
find_package(Python3 "3.8" QUIET REQUIRED)

# Add cmake-tools
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/tools/cmake")
include(cmake-tools)

# Project definition
project(
    MyProject
    VERSION "${PRODUCT_VERSION}"
    DESCRIPTION "My Project description"
    LANGUAGES CXX)

# Options
option(BUILD_UNIT_TESTS "Build unit tests" ON)

# These values are loaded from the project_customization.txt file APPLICATION_NAME, LIBRARY_NAME, COPYRIGHT_PROJECT,
# AUTHOR_PROJECT

# Conan settings

set(CONAN_PACKAGES
    zlib/1.2.13
    libiconv/1.17
    pcre2/10.40
    sqlite3/3.39.3
    poco/1.12.2
    doxygen/1.9.4
    gtest/cci.20210126
    qt/6.3.1)

set(CONAN_PACKAGES_TO_FIND Poco)
set(CONAN_BUILD_OPTIONS *:shared=True xapian-core:shared=False libelf:shared=False)
if(APPLE)
    list(APPEND CONAN_BUILD_OPTIONS poco:enable_data_mysql=False)
endif()
set(CONAN_BUILD_MISSING ON)
if(BUILD_UNIT_TESTS)
    list(APPEND CONAN_PACKAGES_TO_FIND GTest)
endif()

conan_configure(REQUIRES ${CONAN_PACKAGES} OPTIONS ${CONAN_BUILD_OPTIONS} FIND_PACKAGES ${CONAN_PACKAGES_TO_FIND})

set(CUSTOM_ENVIRONMENT "QT6_DIR=${CONAN_QT_ROOT}" "DYLD_LIBRARY_PATH=\"${CMAKE_BINARY_DIR}/lib\""
                       "LD_LIBRARY_PATH=\"${CMAKE_BINARY_DIR}/lib\"")

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    list(APPEND CUSTOM_ENVIRONMENT "QT_DEBUG_PLUGINS=1"
         "QT_QPA_PLATFORM_PLUGIN_PATH=${CONAN_QT_ROOT}/res/archdatadir/plugins")
endif()

write_conan_environment_file(OUTPUT_FOLDER "${CMAKE_BINARY_DIR}" SET ${CONFIG_CUSTOM_ENVIRONMENT})

# Create targets
add_subdirectory(src)

if(CMAKE_TOOLS_BUILD_DOC)
    set(PRODUCT_DOC_TEMPLATES ${CMAKE_SOURCE_DIR}/docs)
    set(PRODUCT_DOC_OUTPUT ${CMAKE_BINARY_DIR}/docs/sphinx/output)
    create_documentation_target()
    install(
        DIRECTORY ${PRODUCT_DOC_OUTPUT}
        DESTINATION ${INSTALL_DOCDIR}
        COMPONENT ${COMPONENT_DOCUMENTATION})
endif()

# Installer creation settings
if(CMAKE_TOOLS_BUILD_INSTALLER)
    # Custom values
    set(CPACK_NSIS_CONTACT "${AUTHOR_PROJECT}")
    set(CPACK_PACKAGE_VENDOR "${AUTHOR_PROJECT}")
    # Configuring install
    configure_installer(PROGRAM_NAME ${APPLICATION_NAME})
endif()
