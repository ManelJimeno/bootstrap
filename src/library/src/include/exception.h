/**
 * Part of https://github.com/ManelJimeno/bootstrap (C) 2022
 * Authors: Manel Jimeno <manel.jimeno@gmail.com>
 * License: http://www.opensource.org/licenses/mit-license.php MIT
 */

#pragma once
#include "helpers.h"
#include <exception>
#include <string>

namespace project_library
{

class Exception : public std::exception
{
  public:
    LIBRARY_API explicit Exception(std::string msg);
    LIBRARY_API char const* what() const noexcept override;

  private:
    std::string m_msg;
};

class NotImplemented : public Exception
{
    using Exception::Exception;
};

class FileNotFound : public Exception
{
    using Exception::Exception;
};

} // namespace project_library
