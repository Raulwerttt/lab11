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
#include "fswatch_log.h"
#include "fswatch.h"
#include <iostream>

using namespace std;

void fsw_log(const char * msg)
{
_akvs_probe("28:0",'f','i');

  if (is_verbose())
  {
{_akvs_probe("28:2",'b','i');

    cout << msg;
  
_akvs_probe("28:2",'b','o');}
}

_akvs_probe("28:0",'f','o');
}
