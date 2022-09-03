#
# Application.
#
# Part of https://github.com/ManelJimeno/bootstrap (C) 2022
#
# Authors: Manel Jimeno <manel.jimeno@gmail.com>
#
# License: http://www.opensource.org/licenses/mit-license.php MIT
#
include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SOURCES main.cpp application_resources.qrc application_settings.cpp)

if(WIN32)
    # Adding the icon resource file
    list(APPEND SOURCES icon.rc)
endif()

set(LIBRARIES Qt6::Core Qt6::Gui Qt6::Widgets ${LIBRARY_NAME})

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(CONSOLE "CONSOLE")
endif()

config_target(
    CPP
    ${CONSOLE}
    TARGET
    ${APPLICATION_NAME}
    SOURCES
    ${SOURCES}
    PUBLIC_LIBRARIES
    ${LIBRARIES})

add_input_folder_to_doc(${CMAKE_CURRENT_SOURCE_DIR})

# Setting install program & QT6 platform folder
install(
    TARGETS ${APPLICATION_NAME}
    BUNDLE DESTINATION . COMPONENT ${COMPONENT_RUNTIME}
    RUNTIME DESTINATION bin COMPONENT ${COMPONENT_RUNTIME}
    ARCHIVE DESTINATION lib COMPONENT ${COMPONENT_ARCHIVE}
    LIBRARY DESTINATION lib COMPONENT ${COMPONENT_LIBRARY})

install(
    DIRECTORY ${CONAN_QT_ROOT}/res/archdatadir/plugins/platforms
    DESTINATION ${INSTALL_BINDIR}
    COMPONENT ${COMPONENT_LIBRARY})
