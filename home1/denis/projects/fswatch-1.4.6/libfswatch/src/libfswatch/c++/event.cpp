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
#include "event.h"

using namespace std;

event::event(string path, time_t evt_time, vector<fsw_event_flag> flags) :
  path(path), evt_time(evt_time), evt_flags(flags)
{
_akvs_probe("11:0",'f','i');


_akvs_probe("11:0",'f','o');
}

event::~event()
{
_akvs_probe("11:4",'f','i');


_akvs_probe("11:4",'f','o');
}

string event::get_path() const
{
_akvs_probe("11:5",'f','i');

  {string  __akvs2_return_value = ( path);

_akvs_probe("11:5",'f','o');

return __akvs2_return_value;}
}

time_t event::get_time() const
{
_akvs_probe("11:6",'f','i');

  
{_akvs_probe("11:6",'f','o');}
return evt_time;
}

vector<fsw_event_flag> event::get_flags() const
{
_akvs_probe("11:7",'f','i');

  
{_akvs_probe("11:7",'f','o');}
return evt_flags;
}
