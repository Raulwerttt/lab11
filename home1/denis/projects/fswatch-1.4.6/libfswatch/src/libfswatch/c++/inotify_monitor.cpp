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
#include "inotify_monitor.h"
#include <limits.h>
#include <unistd.h>
#include <stdio.h>
#include <sstream>
#include <ctime>
#include <cmath>
#include <sys/select.h>
#include "libfswatch_exception.h"
#include "../c/libfswatch_log.h"
#include "libfswatch_map.h"
#include "libfswatch_set.h"
#include "path_utils.h"

using namespace std;

namespace fsw
{

  struct inotify_monitor_impl
  {
    int inotify_monitor_handle = -1;
    vector<event> events;
    /*
     * A map of file names by descriptor is kept in sync because the name field
     * of the inotify_event structure is present only when it identifies a
     * child of a watched directory.  In all the other cases, we store the path
     * for easy retrieval.
     */
    fsw_hash_set<int> watched_descriptors;
    fsw_hash_map<string, int> path_to_wd;
    /*
     * Since the inotify API maintains only works with watch
     * descriptors a cache maintaining a relationship between a watch
     * descriptor and the path used to get it is required to be able to map an
     * event to the path it refers to.  From man inotify:
     * 
     *   The inotify API identifies events via watch descriptors.  It is the
     *   application's responsibility to cache a mapping (if one is needed)
     *   between watch descriptors and pathnames.  Be aware that directory
     *   renamings may affect multiple cached pathnames.
     */
    fsw_hash_map<int, string> wd_to_path;
    fsw_hash_set<int> descriptors_to_remove;
    fsw_hash_set<int> watches_to_remove;
    time_t curr_time;
  };

  static const unsigned int BUFFER_SIZE = (10 * ((sizeof (struct inotify_event)) + NAME_MAX + 1));

  REGISTER_MONITOR_IMPL(inotify_monitor, inotify_monitor_type);

  inotify_monitor::inotify_monitor(vector<string> paths_to_monitor,
                                   FSW_EVENT_CALLBACK * callback,
                                   void * context) :
    monitor(paths_to_monitor, callback, context),
    impl(new inotify_monitor_impl())
  {
_akvs_probe("19:11",'f','i');

    impl->inotify_monitor_handle = ::inotify_init();

    if (impl->inotify_monitor_handle == -1)
    {
{_akvs_probe("19:15",'b','i');

      ::perror("inotify_init");
      
_akvs_probe("19:11",'f','o');
throw libfsw_exception(_("Cannot initialize inotify."));
    
_akvs_probe("19:15",'b','o');}
}
  
_akvs_probe("19:11",'f','o');
}

  inotify_monitor::~inotify_monitor()
  {
_akvs_probe("19:16",'f','i');

    // close inotify watchers
    for (auto inotify_desc_pair : impl->watched_descriptors)
    {
{_akvs_probe("19:17",'b','i');

      ostringstream log;
      log << _("Removing: ") << inotify_desc_pair << "\n";
      libfsw_log(log.str().c_str());

      if (::inotify_rm_watch(impl->inotify_monitor_handle, inotify_desc_pair))
      {
{_akvs_probe("19:20",'b','i');

        ::perror("inotify_rm_watch");
      
_akvs_probe("19:20",'b','o');}
}
    
_akvs_probe("19:17",'b','o');}
}

    // close inotify
    if (impl->inotify_monitor_handle > 0)
    {
{_akvs_probe("19:21",'b','i');

      ::close(impl->inotify_monitor_handle);
    
_akvs_probe("19:21",'b','o');}
}

    delete impl;
  
_akvs_probe("19:16",'f','o');
}

  bool inotify_monitor::add_watch(const string &path,
                                  const struct stat &fd_stat)
  {
_akvs_probe("19:22",'f','i');

    // TODO: Consider optionally adding the IN_EXCL_UNLINK flag.
    int inotify_desc = ::inotify_add_watch(impl->inotify_monitor_handle,
                                           path.c_str(),
                                           IN_ALL_EVENTS);

    if (inotify_desc == -1)
    {
{_akvs_probe("19:26",'b','i');

      ::perror("inotify_add_watch");
    
_akvs_probe("19:26",'b','o');}
}
    else
    {
{_akvs_probe("19:27",'b','i');

      impl->watched_descriptors.insert(inotify_desc);
      impl->wd_to_path[inotify_desc] = path;
      impl->path_to_wd[path] = inotify_desc;

      ostringstream log;
      log << _("Added: ") << path << "\n";
      libfsw_log(log.str().c_str());
    
_akvs_probe("19:27",'b','o');}
}

    
{_akvs_probe("19:22",'f','o');}
return (inotify_desc != -1);
  }

  void inotify_monitor::scan(const string &path, const bool accept_non_dirs)
  {
_akvs_probe("19:29",'f','i');

    struct stat fd_stat;
    if (!stat_path(path, fd_stat)) 
{_akvs_probe("19:33",'b','i');

{_akvs_probe("19:29",'f','o');}
return;
_akvs_probe("19:33",'b','o');}


    /*
     * When watching a directory the inotify API will return change events of
     * first-level children.  Therefore, we do not need to manually add a watch
     * for a child unless it is a directory.  By default, accept_non_dirs is
     * true to allow watching a file when first invoked on a node.
     */
    if (!accept_non_dirs && !S_ISDIR(fd_stat.st_mode)) 
{_akvs_probe("19:34",'b','i');

{_akvs_probe("19:29",'f','o');}
return;
_akvs_probe("19:34",'b','o');}

    else 
{_akvs_probe("19:35",'b','i');
if (follow_symlinks && S_ISLNK(fd_stat.st_mode))
    {
{_akvs_probe("19:36",'b','i');

      string link_path;
      if (read_link_path(path, link_path))
        
{_akvs_probe("19:38",'b','i');
scan(link_path/*, fn */, accept_non_dirs);
_akvs_probe("19:38",'b','o');}


      
{_akvs_probe("19:29",'f','o');}
return;
    
_akvs_probe("19:36",'b','o');}
}
_akvs_probe("19:35",'b','o');}


    if (!S_ISDIR(fd_stat.st_mode) && !accept_path(path)) 
{_akvs_probe("19:39",'b','i');

{_akvs_probe("19:29",'f','o');}
return;
_akvs_probe("19:39",'b','o');}

    if (!add_watch(path, fd_stat /*, fn */)) 
{_akvs_probe("19:40",'b','i');

{_akvs_probe("19:29",'f','o');}
return;
_akvs_probe("19:40",'b','o');}

    if (!recursive || !S_ISDIR(fd_stat.st_mode)) 
{_akvs_probe("19:41",'b','i');

{_akvs_probe("19:29",'f','o');}
return;
_akvs_probe("19:41",'b','o');}


    vector<string> children;
    get_directory_children(path, children);

    for (const string &child : children)
    {
{_akvs_probe("19:43",'b','i');

      if (child.compare(".") == 0 || child.compare("..") == 0) 
{_akvs_probe("19:45",'b','i');
continue;
_akvs_probe("19:45",'b','o');}


      /*
       * Scan children but only watch directories.
       */
      scan(path + "/" + child /*, fn */, false);
    
_akvs_probe("19:43",'b','o');}
}
  
_akvs_probe("19:29",'f','o');
}

  bool inotify_monitor::is_watched(const string & path)
  {
_akvs_probe("19:46",'f','i');

    {bool  __akvs2_return_value = ( (impl->path_to_wd.find(path) != impl->path_to_wd.end()));

_akvs_probe("19:46",'f','o');

return __akvs2_return_value;}
  }

  void inotify_monitor::scan_root_paths()
  {
_akvs_probe("19:48",'f','i');

    for (string &path : paths)
    {
{_akvs_probe("19:49",'b','i');

      if (!is_watched(path)) 
{_akvs_probe("19:51",'b','i');
scan(path);
_akvs_probe("19:51",'b','o');}

    
_akvs_probe("19:49",'b','o');}
}
  
_akvs_probe("19:48",'f','o');
}

  void inotify_monitor::preprocess_dir_event(struct inotify_event * event)
  {
_akvs_probe("19:52",'f','i');

    vector<fsw_event_flag> flags;

    if (event->mask & IN_ISDIR) 
{_akvs_probe("19:55",'b','i');
flags.push_back(fsw_event_flag::IsDir);
_akvs_probe("19:55",'b','o');}

    if (event->mask & IN_MOVE_SELF) 
{_akvs_probe("19:56",'b','i');
flags.push_back(fsw_event_flag::Updated);
_akvs_probe("19:56",'b','o');}

    if (event->mask & IN_UNMOUNT) 
{_akvs_probe("19:57",'b','i');
flags.push_back(fsw_event_flag::PlatformSpecific);
_akvs_probe("19:57",'b','o');}


    if (flags.size())
    {
{_akvs_probe("19:58",'b','i');

      impl->events.push_back({impl->wd_to_path[event->wd], impl->curr_time, flags});
    
_akvs_probe("19:58",'b','o');}
}
  
_akvs_probe("19:52",'f','o');
}

  void inotify_monitor::preprocess_node_event(struct inotify_event * event)
  {
_akvs_probe("19:59",'f','i');

    vector<fsw_event_flag> flags;

    if (event->mask & IN_ACCESS) 
{_akvs_probe("19:62",'b','i');
flags.push_back(fsw_event_flag::PlatformSpecific);
_akvs_probe("19:62",'b','o');}

    if (event->mask & IN_ATTRIB) 
{_akvs_probe("19:63",'b','i');
flags.push_back(fsw_event_flag::AttributeModified);
_akvs_probe("19:63",'b','o');}

    if (event->mask & IN_CLOSE_NOWRITE) 
{_akvs_probe("19:64",'b','i');
flags.push_back(fsw_event_flag::PlatformSpecific);
_akvs_probe("19:64",'b','o');}

    if (event->mask & IN_CLOSE_WRITE) 
{_akvs_probe("19:65",'b','i');
flags.push_back(fsw_event_flag::Updated);
_akvs_probe("19:65",'b','o');}

    if (event->mask & IN_CREATE) 
{_akvs_probe("19:66",'b','i');
flags.push_back(fsw_event_flag::Created);
_akvs_probe("19:66",'b','o');}

    if (event->mask & IN_DELETE) 
{_akvs_probe("19:67",'b','i');
flags.push_back(fsw_event_flag::Removed);
_akvs_probe("19:67",'b','o');}

    if (event->mask & IN_MODIFY) 
{_akvs_probe("19:68",'b','i');
flags.push_back(fsw_event_flag::Updated);
_akvs_probe("19:68",'b','o');}

    if (event->mask & IN_MOVED_FROM)
    {
{_akvs_probe("19:69",'b','i');

      flags.push_back(fsw_event_flag::Removed);
      flags.push_back(fsw_event_flag::MovedFrom);
    
_akvs_probe("19:69",'b','o');}
}
    if (event->mask & IN_MOVED_TO)
    {
{_akvs_probe("19:70",'b','i');

      flags.push_back(fsw_event_flag::Created);
      flags.push_back(fsw_event_flag::MovedTo);
    
_akvs_probe("19:70",'b','o');}
}
    if (event->mask & IN_OPEN) 
{_akvs_probe("19:71",'b','i');
flags.push_back(fsw_event_flag::PlatformSpecific);
_akvs_probe("19:71",'b','o');}


    // Build the file name.
    ostringstream filename_stream;
    filename_stream << impl->wd_to_path[event->wd];

    if (event->len > 1)
    {
{_akvs_probe("19:73",'b','i');

      filename_stream << "/";
      filename_stream << event->name;
    
_akvs_probe("19:73",'b','o');}
}

    if (flags.size())
    {
{_akvs_probe("19:74",'b','i');

      impl->events.push_back({filename_stream.str(), impl->curr_time, flags});
    
_akvs_probe("19:74",'b','o');}
}

    {
      ostringstream log;
      log << _("Generic event: ") << event->wd << "::" << filename_stream.str() << "\n";
      libfsw_log(log.str().c_str());
    }

    /*
     * inotify automatically removes the watch of a watched item that has been
     * removed and posts an IN_IGNORED event after an IN_DELETE_SELF.
     */
    if (event->mask & IN_IGNORED)
    {
{_akvs_probe("19:76",'b','i');

      ostringstream log;
      log << "IN_IGNORED: " << event->wd << "::" << filename_stream.str() << "\n";
      libfsw_log(log.str().c_str());

      impl->descriptors_to_remove.insert(event->wd);
    
_akvs_probe("19:76",'b','o');}
}

    /*
     * inotify sends an IN_MOVE_SELF event when a watched object is moved into
     * the same filesystem and keeps watching it.  Since its path has changed,
     * we remove the watch so that recreation is attempted at the next
     * iteration.
     * 
     * Beware that a race condition exists which may result in events go
     * unnoticed when a watched file x is removed and a new file named x is
     * created thereafter.  In this case, fswatch could be blocked on ::read
     * and it would not have any chance to create a new watch descriptor for x
     * until an event is received and ::read unblocks.
     */
    if (event->mask & IN_MOVE_SELF)
    {
{_akvs_probe("19:78",'b','i');

      ostringstream log;
      log << "IN_MOVE_SELF: " << event->wd << "::" << filename_stream.str() << "\n";
      libfsw_log(log.str().c_str());

      impl->watches_to_remove.insert(event->wd);
      impl->descriptors_to_remove.insert(event->wd);
    
_akvs_probe("19:78",'b','o');}
}

    /*
     * An file could be moved to a path which is being observed.  The clobbered
     * file is handled by the corresponding IN_DELETE_SELF event.
     */

    /*
     * inotify automatically removes the watch of the object the IN_DELETE_SELF
     * event is related to.
     */
    if (event->mask & IN_DELETE_SELF)
    {
{_akvs_probe("19:80",'b','i');

      ostringstream log;
      log << "IN_DELETE_SELF: " << event->wd << "::" << filename_stream.str() << "\n";
      libfsw_log(log.str().c_str());

      impl->descriptors_to_remove.insert(event->wd);
    
_akvs_probe("19:80",'b','o');}
}
  
_akvs_probe("19:59",'f','o');
}

  void inotify_monitor::preprocess_event(struct inotify_event * event)
  {
_akvs_probe("19:82",'f','i');

    if (event->mask & IN_Q_OVERFLOW)
    {
{_akvs_probe("19:84",'b','i');

      
_akvs_probe("19:82",'f','o');
throw libfsw_exception(_("Event queue overflowed."));
    
_akvs_probe("19:84",'b','o');}
}

    preprocess_dir_event(event);
    preprocess_node_event(event);
  
_akvs_probe("19:82",'f','o');
}

  void inotify_monitor::notify_events()
  {
_akvs_probe("19:85",'f','i');

    if (impl->events.size())
    {
{_akvs_probe("19:86",'b','i');

      ostringstream log;
      log << _("Notifying events #: ") << impl->events.size() << "\n";
      libfsw_log(log.str().c_str());

      callback(impl->events, context);
      impl->events.clear();
    
_akvs_probe("19:86",'b','o');}
}
  
_akvs_probe("19:85",'f','o');
}

  void inotify_monitor::remove_watch(int wd)
  {
_akvs_probe("19:88",'f','i');

    /*
     * No need to remove the inotify watch because it is removed automatically
     * when a watched element is deleted. 
     */
    impl->wd_to_path.erase(wd);
  
_akvs_probe("19:88",'f','o');
}

  void inotify_monitor::process_pending_events()
  {
_akvs_probe("19:90",'f','i');

    // Remove watches.
    auto wtd = impl->watches_to_remove.begin();

    while (wtd != impl->watches_to_remove.end())
    {
{_akvs_probe("19:92",'b','i');

      if (::inotify_rm_watch(impl->inotify_monitor_handle, *wtd) != 0)
      {
{_akvs_probe("19:93",'b','i');

        ::perror("inotify_rm_watch");
      
_akvs_probe("19:93",'b','o');}
}
      else
      {
{_akvs_probe("19:94",'b','i');

        ostringstream log;
        log << _("Removed: ") << *wtd << "\n";
        libfsw_log(log.str().c_str());
      
_akvs_probe("19:94",'b','o');}
}

      impl->watches_to_remove.erase(wtd++);
    
_akvs_probe("19:92",'b','o');}
}

    // Clean up descriptors.
    auto fd = impl->descriptors_to_remove.begin();

    while (fd != impl->descriptors_to_remove.end())
    {
{_akvs_probe("19:97",'b','i');

      const string & curr_path = impl->wd_to_path[*fd];
      impl->path_to_wd.erase(curr_path);
      impl->wd_to_path.erase(*fd);
      impl->watched_descriptors.erase(*fd);

      impl->descriptors_to_remove.erase(fd++);
    
_akvs_probe("19:97",'b','o');}
}
  
_akvs_probe("19:90",'f','o');
}

  void inotify_monitor::run()
  {
_akvs_probe("19:99",'f','i');

    char buffer[BUFFER_SIZE];

    while (true)
    {
{_akvs_probe("19:101",'b','i');

      process_pending_events();

      scan_root_paths();

      // If no files can be watched, sleep and repeat the loop.
      if (!impl->watched_descriptors.size())
      {
{_akvs_probe("19:102",'b','i');

        ::sleep(latency);
        
_akvs_probe("19:101",'b','o');
continue;
      
_akvs_probe("19:102",'b','o');}
}

      // Use select to timeout on file descriptor read the amount specified by
      // the monitor latency.  This way, the monitor has a chance to update its
      // watches with at least the periodicity expected by the user.
      fd_set set;
      struct timeval timeout;

      FD_ZERO(&set);
      FD_SET(impl->inotify_monitor_handle, &set);
      double sec;
      double frac = ::modf(this->latency, &sec);
      timeout.tv_sec = sec;
      timeout.tv_usec = 1000 * 1000 * frac;

      int rv = ::select(impl->inotify_monitor_handle + 1,
                        &set,
                        nullptr,
                        nullptr,
                        &timeout);

      if (rv == -1)
      {
{_akvs_probe("19:111",'b','i');

        
_akvs_probe("19:99",'f','o');
throw libfsw_exception(_("::select() on inotify descriptor encountered an error."));
      
_akvs_probe("19:111",'b','o');}
}

      // In case of read timeout just repeat the loop.
      if (rv == 0)
      {
{_akvs_probe("19:112",'b','i');

        continue;
      
_akvs_probe("19:112",'b','o');}
}

      ssize_t record_num = ::read(impl->inotify_monitor_handle,
                                  buffer,
                                  BUFFER_SIZE);

      {
        ostringstream log;
        log << _("Number of records: ") << record_num << "\n";
        libfsw_log(log.str().c_str());
      }

      if (!record_num)
      {
{_akvs_probe("19:115",'b','i');

        
_akvs_probe("19:99",'f','o');
throw libfsw_exception(_("::read() on inotify descriptor read 0 records."));
      
_akvs_probe("19:115",'b','o');}
}

      if (record_num == -1)
      {
{_akvs_probe("19:116",'b','i');

        ::perror("read()");
        
_akvs_probe("19:99",'f','o');
throw libfsw_exception(_("::read() on inotify descriptor returned -1."));
      
_akvs_probe("19:116",'b','o');}
}

      time(&impl->curr_time);

      for (char *p = buffer; p < buffer + record_num;)
      {
{_akvs_probe("19:118",'b','i');

        struct inotify_event * event = reinterpret_cast<struct inotify_event *> (p);

        preprocess_event(event);

        p += (sizeof (struct inotify_event)) + event->len;
      
_akvs_probe("19:118",'b','o');}
}

      notify_events();
      ::sleep(latency);
    
_akvs_probe("19:101",'b','o');}
}
  
_akvs_probe("19:99",'f','o');
}
}
