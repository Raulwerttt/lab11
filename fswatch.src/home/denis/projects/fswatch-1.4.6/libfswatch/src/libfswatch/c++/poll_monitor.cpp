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
#ifdef HAVE_CONFIG_H
#  include "libfswatch_config.h"
#endif

#include "gettext_defs.h"
#include "poll_monitor.h"
#include <unistd.h>
#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include "c/libfswatch_log.h"
#include "path_utils.h"
#include "libfswatch_map.h"

using namespace std;

#if defined HAVE_STRUCT_STAT_ST_MTIME
#  define FSW_MTIME(stat) (stat.st_mtime)
#  define FSW_CTIME(stat) (stat.st_ctime)
#elif defined HAVE_STRUCT_STAT_ST_MTIMESPEC
#  define FSW_MTIME(stat) (stat.st_mtimespec.tv_sec)
#  define FSW_CTIME(stat) (stat.st_ctimespec.tv_sec)
#endif

namespace fsw
{

  typedef struct poll_monitor::poll_monitor_data
  {
    fsw_hash_map<std::string, poll_monitor::watched_file_info> tracked_files;
  }
  poll_monitor_data;

  REGISTER_MONITOR_IMPL(poll_monitor, poll_monitor_type);

  poll_monitor::poll_monitor(vector<string> paths,
                             FSW_EVENT_CALLBACK * callback,
                             void * context) :
    monitor(paths, callback, context)
  {
_akvs_probe("21:3",'f','i');

    previous_data = new poll_monitor_data();
    new_data = new poll_monitor_data();
    time(&curr_time);
  
_akvs_probe("21:3",'f','o');
}

  poll_monitor::~poll_monitor()
  {
_akvs_probe("21:7",'f','i');

    delete previous_data;
    delete new_data;
  
_akvs_probe("21:7",'f','o');
}

  bool poll_monitor::initial_scan_callback(const string &path,
                                           const struct stat &stat)
  {
_akvs_probe("21:8",'f','i');

    if (previous_data->tracked_files.count(path))
      
{_akvs_probe("21:11",'b','i');

{_akvs_probe("21:8",'f','o');}
return false;
_akvs_probe("21:11",'b','o');}


    watched_file_info wfi{FSW_MTIME(stat), FSW_CTIME(stat)};
    previous_data->tracked_files[path] = wfi;

    
{_akvs_probe("21:8",'f','o');}
return true;
  }

  bool poll_monitor::intermediate_scan_callback(const string &path,
                                                const struct stat &stat)
  {
_akvs_probe("21:13",'f','i');

    if (new_data->tracked_files.count(path)) 
{_akvs_probe("21:16",'b','i');

{_akvs_probe("21:13",'f','o');}
return false;
_akvs_probe("21:16",'b','o');}


    watched_file_info wfi{FSW_MTIME(stat), FSW_CTIME(stat)};
    new_data->tracked_files[path] = wfi;

    if (previous_data->tracked_files.count(path))
    {
{_akvs_probe("21:18",'b','i');

      watched_file_info pwfi = previous_data->tracked_files[path];
      vector<fsw_event_flag> flags;

      if (FSW_MTIME(stat) > pwfi.mtime)
      {
{_akvs_probe("21:21",'b','i');

        flags.push_back(fsw_event_flag::Updated);
      
_akvs_probe("21:21",'b','o');}
}

      if (FSW_CTIME(stat) > pwfi.ctime)
      {
{_akvs_probe("21:22",'b','i');

        flags.push_back(fsw_event_flag::AttributeModified);
      
_akvs_probe("21:22",'b','o');}
}

      if (flags.size() > 0 && accept_path(path))
      {
{_akvs_probe("21:23",'b','i');

        events.push_back({path, curr_time, flags});
      
_akvs_probe("21:23",'b','o');}
}

      previous_data->tracked_files.erase(path);
    
_akvs_probe("21:18",'b','o');}
}
    else
    {
{_akvs_probe("21:24",'b','i');

      vector<fsw_event_flag> flags;
      flags.push_back(fsw_event_flag::Created);

      if (accept_path(path))
      {
{_akvs_probe("21:26",'b','i');

        events.push_back({path, curr_time, flags});
      
_akvs_probe("21:26",'b','o');}
}
    
_akvs_probe("21:24",'b','o');}
}

    
{_akvs_probe("21:13",'f','o');}
return true;
  }

  bool poll_monitor::add_path(const string &path,
                              const struct stat &fd_stat,
                              poll_monitor_scan_callback poll_callback)
  {
_akvs_probe("21:27",'f','i');

    {bool  __akvs2_return_value = ( ((*this).*(poll_callback))(path, fd_stat));

_akvs_probe("21:27",'f','o');

return __akvs2_return_value;}
  }

  void poll_monitor::scan(const string &path, poll_monitor_scan_callback fn)
  {
_akvs_probe("21:31",'f','i');

    struct stat fd_stat;
    if (!stat_path(path, fd_stat)) 
{_akvs_probe("21:35",'b','i');

{_akvs_probe("21:31",'f','o');}
return;
_akvs_probe("21:35",'b','o');}


    if (follow_symlinks && S_ISLNK(fd_stat.st_mode))
    {
{_akvs_probe("21:36",'b','i');

      string link_path;
      if (read_link_path(path, link_path))
        
{_akvs_probe("21:38",'b','i');
scan(link_path, fn);
_akvs_probe("21:38",'b','o');}


      
{_akvs_probe("21:31",'f','o');}
return;
    
_akvs_probe("21:36",'b','o');}
}

    if (!S_ISDIR(fd_stat.st_mode) && !accept_path(path)) 
{_akvs_probe("21:39",'b','i');

{_akvs_probe("21:31",'f','o');}
return;
_akvs_probe("21:39",'b','o');}

    if (!add_path(path, fd_stat, fn)) 
{_akvs_probe("21:40",'b','i');

{_akvs_probe("21:31",'f','o');}
return;
_akvs_probe("21:40",'b','o');}

    if (!recursive) 
{_akvs_probe("21:41",'b','i');

{_akvs_probe("21:31",'f','o');}
return;
_akvs_probe("21:41",'b','o');}

    if (!S_ISDIR(fd_stat.st_mode)) 
{_akvs_probe("21:42",'b','i');

{_akvs_probe("21:31",'f','o');}
return;
_akvs_probe("21:42",'b','o');}


    vector<string> children;
    get_directory_children(path, children);

    for (string &child : children)
    {
{_akvs_probe("21:44",'b','i');

      if (child.compare(".") == 0 || child.compare("..") == 0) 
{_akvs_probe("21:46",'b','i');
continue;
_akvs_probe("21:46",'b','o');}


      scan(path + "/" + child, fn);
    
_akvs_probe("21:44",'b','o');}
}
  
_akvs_probe("21:31",'f','o');
}

  void poll_monitor::find_removed_files()
  {
_akvs_probe("21:47",'f','i');

    vector<fsw_event_flag> flags;
    flags.push_back(fsw_event_flag::Removed);

    for (auto &removed : previous_data->tracked_files)
    {
{_akvs_probe("21:49",'b','i');

      if (accept_path(removed.first))
      {
{_akvs_probe("21:51",'b','i');

        events.push_back({removed.first, curr_time, flags});
      
_akvs_probe("21:51",'b','o');}
}
    
_akvs_probe("21:49",'b','o');}
}
  
_akvs_probe("21:47",'f','o');
}

  void poll_monitor::swap_data_containers()
  {
_akvs_probe("21:52",'f','i');

    delete previous_data;
    previous_data = new_data;
    new_data = new poll_monitor_data();
  
_akvs_probe("21:52",'f','o');
}

  void poll_monitor::collect_data()
  {
_akvs_probe("21:53",'f','i');

    poll_monitor_scan_callback fn = &poll_monitor::intermediate_scan_callback;

    for (string &path : paths)
    {
{_akvs_probe("21:55",'b','i');

      scan(path, fn);
    
_akvs_probe("21:55",'b','o');}
}

    find_removed_files();
    swap_data_containers();
  
_akvs_probe("21:53",'f','o');
}

  void poll_monitor::collect_initial_data()
  {
_akvs_probe("21:57",'f','i');

    poll_monitor_scan_callback fn = &poll_monitor::initial_scan_callback;

    for (string &path : paths)
    {
{_akvs_probe("21:59",'b','i');

      scan(path, fn);
    
_akvs_probe("21:59",'b','o');}
}
  
_akvs_probe("21:57",'f','o');
}

  void poll_monitor::notify_events()
  {
_akvs_probe("21:61",'f','i');

    if (events.size())
    {
{_akvs_probe("21:62",'b','i');

      callback(events, context);
      events.clear();
    
_akvs_probe("21:62",'b','o');}
}
  
_akvs_probe("21:61",'f','o');
}

  void poll_monitor::run()
  {
_akvs_probe("21:63",'f','i');

    collect_initial_data();

    while (true)
    {
{_akvs_probe("21:64",'b','i');

#ifdef DEBUG
      libfsw_log(_("Done scanning.\n"));
#endif

      ::sleep(latency < MIN_POLL_LATENCY ? MIN_POLL_LATENCY : latency);

      time(&curr_time);

      collect_data();
      notify_events();
    
_akvs_probe("21:64",'b','o');}
}
  
_akvs_probe("21:63",'f','o');
}
}
