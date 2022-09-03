#
# Part of https://github.com/ManelJimeno/bootstrap (C) 2022
#
# Authors: Manel Jimeno <manel.jimeno@gmail.com>
#
# License: http://www.opensource.org/licenses/mit-license.php MIT
#

add_subdirectory(src)

if(BUILD_UNIT_TESTS)
    add_subdirectory(test)
endif()
