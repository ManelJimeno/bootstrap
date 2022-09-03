#
# Part of https://github.com/ManelJimeno/bootstrap (C) 2022
#
# Authors: Manel Jimeno <manel.jimeno@gmail.com>
#
# License: http://www.opensource.org/licenses/mit-license.php MIT
#

add_cpp_test(TARGET test_settings LIBRARIES ${LIBRARY_NAME})

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin/appdata)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/settings.ini DESTINATION ${CMAKE_BINARY_DIR}/bin/appdata)
