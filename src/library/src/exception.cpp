/**
 * Part of https://github.com/ManelJimeno/bootstrap (C) 2022
 * Authors: Manel Jimeno <manel.jimeno@gmail.com>
 * License: http://www.opensource.org/licenses/mit-license.php MIT
 */

#include "exception.h"

namespace project_library
{

Exception::Exception(std::string msg) : m_msg(std::move(msg))
{
}

char const* Exception::what() const noexcept
{
    return m_msg.c_str();
}

} // namespace project_library
