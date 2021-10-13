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
#include "monitor.h"
#include "libfswatch_exception.h"
#include <cstdlib>
#ifdef HAVE_REGCOMP
#  include <regex.h>
#endif

/*
 * Conditionally include monitor headers for default construction.
 */
#if defined(HAVE_FSEVENTS_FILE_EVENTS)
#  include "fsevents_monitor.h"
#endif
#if defined(HAVE_SYS_EVENT_H)
#  include "kqueue_monitor.h"
#endif
#if defined(HAVE_SYS_INOTIFY_H)
#  include "inotify_monitor.h"
#endif
#include "poll_monitor.h"

#include <iostream>

using namespace std;

namespace fsw
{

  struct compiled_monitor_filter
  {
#ifdef HAVE_REGCOMP    
    regex_t regex;
    fsw_filter_type type;
#endif
  };

  monitor::monitor(std::vector<std::string> paths,
                   FSW_EVENT_CALLBACK * callback,
                   void * context) :
    paths(paths), callback(callback), context(context)
  {
_akvs_probe("17:3",'f','i');

    if (callback == nullptr)
    {
{_akvs_probe("17:7",'b','i');

      
_akvs_probe("17:3",'f','o');
throw libfsw_exception(_("Callback cannot be null."), FSW_ERR_CALLBACK_NOT_SET);
    
_akvs_probe("17:7",'b','o');}
}
  
_akvs_probe("17:3",'f','o');
}

  void monitor::set_latency(double latency)
  {
_akvs_probe("17:8",'f','i');

    if (latency < 0)
    {
{_akvs_probe("17:10",'b','i');

      
_akvs_probe("17:8",'f','o');
throw libfsw_exception(_("Latency cannot be negative."), FSW_ERR_INVALID_LATENCY);
    
_akvs_probe("17:10",'b','o');}
}

    this->latency = latency;
  
_akvs_probe("17:8",'f','o');
}

  void monitor::set_recursive(bool recursive)
  {
_akvs_probe("17:11",'f','i');

    this->recursive = recursive;
  
_akvs_probe("17:11",'f','o');
}

  void monitor::add_filter(const monitor_filter &filter)
  {
_akvs_probe("17:13",'f','i');

    regex_t regex;
    int flags = 0;

    if (!filter.case_sensitive) 
{_akvs_probe("17:17",'b','i');
flags |= REG_ICASE;
_akvs_probe("17:17",'b','o');}

    if (filter.extended) 
{_akvs_probe("17:18",'b','i');
flags |= REG_EXTENDED;
_akvs_probe("17:18",'b','o');}


    if (::regcomp(&regex, filter.text.c_str(), flags))
    {
{_akvs_probe("17:19",'b','i');

      string err = _("An error occurred during the compilation of ") + filter.text;
      
_akvs_probe("17:13",'f','o');
throw libfsw_exception(err, FSW_ERR_INVALID_REGEX);
    
_akvs_probe("17:19",'b','o');}
}

    this->filters.push_back({regex, filter.type});
  
_akvs_probe("17:13",'f','o');
}

  void monitor::set_filters(const std::vector<monitor_filter> &filters)
  {
_akvs_probe("17:21",'f','i');

#ifdef HAVE_REGCOMP
    for (const monitor_filter &filter : filters)
    {
{_akvs_probe("17:23",'b','i');

      add_filter(filter);
    
_akvs_probe("17:23",'b','o');}
}
#endif
  
_akvs_probe("17:21",'f','o');
}

  void monitor::set_follow_symlinks(bool follow)
  {
_akvs_probe("17:25",'f','i');

    follow_symlinks = follow;
  
_akvs_probe("17:25",'f','o');
}

  bool monitor::accept_path(const string &path)
  {
_akvs_probe("17:27",'f','i');

    {bool  __akvs2_return_value = ( accept_path(path.c_str()));

_akvs_probe("17:27",'f','o');

return __akvs2_return_value;}
  }

  bool monitor::accept_path(const char *path)
  {
_akvs_probe("17:29",'f','i');

#ifdef HAVE_REGCOMP
    for (auto &filter : filters)
    {
{_akvs_probe("17:31",'b','i');

      if (::regexec(&filter.regex, path, 0, nullptr, 0) == 0)
      {
{_akvs_probe("17:33",'b','i');

        
{_akvs_probe("17:29",'f','o');}
return filter.type == fsw_filter_type::filter_include;
      
_akvs_probe("17:33",'b','o');}
}
    
_akvs_probe("17:31",'b','o');}
}
#endif

    
{_akvs_probe("17:29",'f','o');}
return true;
  }

  void * monitor::get_context()
  {
_akvs_probe("17:34",'f','i');

    
{_akvs_probe("17:34",'f','o');}
return context;
  }

  void monitor::set_context(void * context)
  {
_akvs_probe("17:35",'f','i');

    this->context = context;
  
_akvs_probe("17:35",'f','o');
}

  monitor::~monitor()
  {
_akvs_probe("17:37",'f','i');

#ifdef HAVE_REGCOMP
    for (auto &re : filters)
    {
{_akvs_probe("17:38",'b','i');

      ::regfree(&re.regex);
    
_akvs_probe("17:38",'b','o');}
}

    filters.clear();
#endif
  
_akvs_probe("17:37",'f','o');
}

  monitor * monitor::create_default_monitor(std::vector<std::string> paths,
                                            FSW_EVENT_CALLBACK * callback,
                                            void * context)
  {
_akvs_probe("17:40",'f','i');

#if defined(HAVE_FSEVENTS_FILE_EVENTS)
    return new fsevents_monitor(paths, callback, context);
#elif defined(HAVE_SYS_EVENT_H)
    return new kqueue_monitor(paths, callback, context);
#elif defined(HAVE_SYS_INOTIFY_H)
    {fsw::monitor * __akvs2_return_value = ( new inotify_monitor(paths, callback, context));

_akvs_probe("17:40",'f','o');

return __akvs2_return_value;}
#else
    return new poll_monitor(paths, callback, context);
#endif
  }

  monitor * monitor::create_monitor(fsw_monitor_type type,
                                    std::vector<std::string> paths,
                                    FSW_EVENT_CALLBACK * callback,
                                    void * context)
  {
_akvs_probe("17:44",'f','i');

    switch (type)
    {
    case system_default_monitor_type:
      
_akvs_probe("17:49",'b','i');
{fsw::monitor * __akvs2_return_value = ( monitor::create_default_monitor(paths, callback, context));

_akvs_probe("17:44",'f','o');

return __akvs2_return_value;}

    
_akvs_probe("17:49",'b','o');
case fsevents_monitor_type:
#if defined(HAVE_FSEVENTS_FILE_EVENTS)
      return new fsevents_monitor(paths, callback, context);
#else
      
_akvs_probe("17:50",'b','i');

_akvs_probe("17:44",'f','o');
throw libfsw_exception("Unsupported monitor.", FSW_ERR_UNKNOWN_MONITOR_TYPE);
#endif      

    
_akvs_probe("17:50",'b','o');
case kqueue_monitor_type:
#if defined(HAVE_SYS_EVENT_H)
      return new kqueue_monitor(paths, callback, context);
#else
      
_akvs_probe("17:51",'b','i');

_akvs_probe("17:44",'f','o');
throw libfsw_exception("Unsupported monitor.", FSW_ERR_UNKNOWN_MONITOR_TYPE);
#endif      

    
_akvs_probe("17:51",'b','o');
case inotify_monitor_type:
#if defined(HAVE_SYS_INOTIFY_H)
      
_akvs_probe("17:52",'b','i');
{fsw::monitor * __akvs2_return_value = ( new inotify_monitor(paths, callback, context));

_akvs_probe("17:44",'f','o');

return __akvs2_return_value;}
#else
      throw libfsw_exception("Unsupported monitor.", FSW_ERR_UNKNOWN_MONITOR_TYPE);
#endif      

    
_akvs_probe("17:52",'b','o');
case poll_monitor_type:
      
_akvs_probe("17:53",'b','i');
{fsw::monitor * __akvs2_return_value = ( new poll_monitor(paths, callback, context));

_akvs_probe("17:44",'f','o');

return __akvs2_return_value;}

    
_akvs_probe("17:53",'b','o');
default:
      
_akvs_probe("17:44",'f','o');
throw libfsw_exception("Unsupported monitor.", FSW_ERR_UNKNOWN_MONITOR_TYPE);
    }
  
_akvs_probe("17:44",'f','o');
}

  void monitor::start()
  {
_akvs_probe("17:55",'f','i');

#ifdef HAVE_CXX_MUTEX
    lock_guard<mutex> run_guard(run_mutex);
#endif
    this->run();
  
_akvs_probe("17:55",'f','o');
}

  map<string, fsw_monitor_type> & monitor_factory::type_by_string()
  {
_akvs_probe("17:57",'f','i');

    static map<string, fsw_monitor_type> type_by_string_map;

    
{_akvs_probe("17:57",'f','o');}
return type_by_string_map;
  };

  monitor * monitor_factory::create_monitor_by_name(const std::string& name,
                                                    std::vector<std::string> paths,
                                                    FSW_EVENT_CALLBACK * callback,
                                                    void * context)
  {
_akvs_probe("17:59",'f','i');

    auto i = type_by_string().find(name);

    if (i == type_by_string().end())
      
{_akvs_probe("17:65",'b','i');

{_akvs_probe("17:59",'f','o');}
return nullptr;
_akvs_probe("17:65",'b','o');}

    else
      
{_akvs_probe("17:66",'b','i');
{fsw::monitor * __akvs2_return_value = ( monitor::create_monitor(i->second, paths, callback, context));

_akvs_probe("17:59",'f','o');

return __akvs2_return_value;}
_akvs_probe("17:66",'b','o');}

  
_akvs_probe("17:59",'f','o');
}

  bool monitor_factory::exists_type(const std::string& name)
  {
_akvs_probe("17:67",'f','i');

    auto i = type_by_string().find(name);

    {bool  __akvs2_return_value = ( (i != type_by_string().end()));

_akvs_probe("17:67",'f','o');

return __akvs2_return_value;}
  }

  void monitor_factory::register_type(const std::string& name, fsw_monitor_type type)
  {
_akvs_probe("17:70",'f','i');

    type_by_string()[name] = type;
  
_akvs_probe("17:70",'f','o');
}

  vector<string> monitor_factory::get_types()
  {
_akvs_probe("17:73",'f','i');

    vector<string> types;

    for (auto & i : type_by_string())
    {
{_akvs_probe("17:75",'b','i');

      types.push_back(i.first);
    
_akvs_probe("17:75",'b','o');}
}

    
{_akvs_probe("17:73",'f','o');}
return types;
  }

  monitor_registrant::monitor_registrant(const std::string & name, fsw_monitor_type type)
  {
_akvs_probe("17:77",'f','i');

    monitor_factory::register_type(name, type);
  
_akvs_probe("17:77",'f','o');
}
}
