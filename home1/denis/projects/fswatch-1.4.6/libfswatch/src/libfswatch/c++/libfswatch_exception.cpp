#include "_akvs_probe.h"
/* 
 * Copyright (C) 2014, Enrico M. Crisostomo
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "libfswatch_exception.h"
#include "gettext_defs.h"

using namespace std;

namespace fsw
{

  libfsw_exception::libfsw_exception(string cause, int code) :
    cause(cause), code(code)
  {
_akvs_probe("18:0",'f','i');

  
_akvs_probe("18:0",'f','o');
}

  const char * libfsw_exception::what() const noexcept
  {
_akvs_probe("18:3",'f','i');

    {const char * __akvs2_return_value = ( (string(_("Error: ")) + this->cause).c_str());

_akvs_probe("18:3",'f','o');

return __akvs2_return_value;}
  }

  int libfsw_exception::error_code() const noexcept
  {
_akvs_probe("18:4",'f','i');

    
{_akvs_probe("18:4",'f','o');}
return code;
  }

  libfsw_exception::operator int() const noexcept
  {
_akvs_probe("18:5",'f','i');

    
{_akvs_probe("18:5",'f','o');}
return code;
  }

  libfsw_exception::~libfsw_exception() noexcept
  {
_akvs_probe("18:6",'f','i');

  
_akvs_probe("18:6",'f','o');
}
}
