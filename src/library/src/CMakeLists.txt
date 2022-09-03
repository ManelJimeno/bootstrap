#
# Library.
#
# Part of https://github.com/ManelJimeno/bootstrap(C) 2022
#
# Authors: Manel Jimeno <manel.jimeno@gmail.com>
#
# License: http://www.opensource.org/licenses/mit-license.php MIT
#

set(SOURCES exception.cpp settings.cpp settings_impl.cpp)
set(LIBRARIES Poco::Poco)
set(PUBLIC_HEADERS include)
set(PRIVATE_HEADERS .)

config_target(
    LIBRARY
    CPP
    TARGET
    ${LIBRARY_NAME}
    SOURCES
    ${SOURCES}
    PUBLIC_LIBRARIES
    ${LIBRARIES}
    PUBLIC_INCLUDE_DIRECTORIES
    ${PUBLIC_HEADERS}
    PRIVATE_INCLUDE_DIRECTORIES
    ${PRIVATE_HEADERS})

# Set the DLLEXPORT variable to export symbols in windows
target_compile_definitions(${LIBRARY_NAME} PRIVATE LIBRARY_EXPORTS)

add_input_folder_to_doc(${CMAKE_CURRENT_SOURCE_DIR})

install(
    TARGETS ${LIBRARY_NAME}
    BUNDLE DESTINATION . COMPONENT ${component_runtime}
    RUNTIME DESTINATION bin COMPONENT ${component_runtime}
    ARCHIVE DESTINATION lib COMPONENT ${component_archive}
    LIBRARY DESTINATION lib COMPONENT ${component_library})
