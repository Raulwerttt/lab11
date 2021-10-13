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
#include <iostream>
#ifdef HAVE_CXX_MUTEX
#  include <mutex>
#  include <atomic>
#endif
#include <ctime>
#include <stdlib.h>
#include <cstring>
#include <memory>
#include "libfswatch.h"
#include "../c++/libfswatch_map.h"
#include "../c++/filter.h"
#include "../c++/monitor.h"
#include "../c++/libfswatch_exception.h"

using namespace std;
using namespace fsw;

typedef struct FSW_SESSION
{
  FSW_HANDLE handle;
  vector<string> paths;
  fsw_monitor_type type;
  fsw::monitor * monitor;
  FSW_CEVENT_CALLBACK callback;
  double latency;
  bool recursive;
  bool follow_symlinks;
  vector<monitor_filter> filters;
#ifdef HAVE_CXX_MUTEX
  atomic<bool> running;
#endif
} FSW_SESSION;

static bool srand_initialized = false;

#ifdef HAVE_CXX_UNIQUE_PTR
static fsw_hash_map<FSW_HANDLE, unique_ptr<FSW_SESSION>> sessions;
#else
static fsw_hash_map<FSW_HANDLE, FSW_SESSION *> sessions;
#endif

#ifdef HAVE_CXX_MUTEX
#  ifdef HAVE_CXX_UNIQUE_PTR
static fsw_hash_map<FSW_HANDLE, unique_ptr<mutex>> session_mutexes;
#  else
static fsw_hash_map<FSW_HANDLE, mutex *> session_mutexes;
#  endif
static std::mutex session_mutex;
#endif

#if defined(HAVE_CXX_THREAD_LOCAL)
static FSW_THREAD_LOCAL unsigned int last_error;
#endif

// Default library callback.
static FSW_EVENT_CALLBACK libfsw_cpp_callback_proxy;
static FSW_SESSION * get_session(const FSW_HANDLE handle);

static int create_monitor(FSW_HANDLE handle, const fsw_monitor_type type);
static FSW_STATUS fsw_set_last_error(const int error);

/*
 * Library initialization routine.  Currently, libfswatch only initializes
 * gettext.
 */
int fsw_init_library()
{
_akvs_probe("5:16",'f','i');

  // Trigger gettext operations
#ifdef ENABLE_NLS
  bindtextdomain(PACKAGE, LOCALEDIR);
#endif

  
{_akvs_probe("5:16",'f','o');}
return FSW_OK;
}

typedef struct fsw_callback_context
{
  FSW_HANDLE handle;
  FSW_CEVENT_CALLBACK callback;
} fsw_callback_context;

void libfsw_cpp_callback_proxy(const std::vector<event> & events,
                               void * context_ptr)
{
_akvs_probe("5:20",'f','i');

  // TODO: A C friendly error handler should be notified instead of throwing an exception.
  if (!context_ptr)
    
{_akvs_probe("5:23",'b','i');

_akvs_probe("5:20",'f','o');
throw int(FSW_ERR_MISSING_CONTEXT);
_akvs_probe("5:23",'b','o');}


  const fsw_callback_context * context = static_cast<fsw_callback_context *> (context_ptr);

  fsw_cevent * const cevents = static_cast<fsw_cevent *> (::malloc(sizeof (fsw_cevent) * events.size()));

  if (cevents == nullptr)
    
{_akvs_probe("5:26",'b','i');

_akvs_probe("5:20",'f','o');
throw int(FSW_ERR_MEMORY);
_akvs_probe("5:26",'b','o');}


  for (unsigned int i = 0; i < events.size(); ++i)
  {
{_akvs_probe("5:28",'b','i');

    fsw_cevent * cevt = &cevents[i];
    const event & evt = events[i];

    // Copy event into C event wrapper.
    const string path = evt.get_path();

    // Copy std::string into char * buffer and null-terminate it.
    cevt->path = static_cast<char *> (::malloc(sizeof (char *) * (path.length() + 1)));
    if (!cevt->path) 
{_akvs_probe("5:32",'b','i');

_akvs_probe("5:20",'f','o');
throw int(FSW_ERR_MEMORY);
_akvs_probe("5:32",'b','o');}


    ::strncpy(cevt->path, path.c_str(), path.length());
    cevt->path[path.length()] = '\0';

    cevt->evt_time = evt.get_time();

    const vector<fsw_event_flag> flags = evt.get_flags();
    cevt->flags_num = flags.size();

    if (!cevt->flags_num) 
{_akvs_probe("5:34",'b','i');
cevt->flags = nullptr;
_akvs_probe("5:34",'b','o');}

    else
    {
{_akvs_probe("5:35",'b','i');

      cevt->flags = static_cast<fsw_event_flag *> (::malloc(sizeof (fsw_event_flag) * cevt->flags_num));
      if (!cevt->flags) 
{_akvs_probe("5:36",'b','i');

_akvs_probe("5:20",'f','o');
throw int(FSW_ERR_MEMORY);
_akvs_probe("5:36",'b','o');}

    
_akvs_probe("5:35",'b','o');}
}

    for (unsigned int e = 0; e < cevt->flags_num; ++e)
    {
{_akvs_probe("5:38",'b','i');

      cevt->flags[e] = flags[e];
    
_akvs_probe("5:38",'b','o');}
}
  
_akvs_probe("5:28",'b','o');}
}

  // TODO manage C++ exceptions from C code
  (*(context->callback))(cevents, events.size());

  // Deallocate memory allocated by events.
  for (unsigned int i = 0; i < events.size(); ++i)
  {
{_akvs_probe("5:40",'b','i');

    fsw_cevent * cevt = &cevents[i];

    if (cevt->flags) 
{_akvs_probe("5:42",'b','i');
::free(static_cast<void *> (cevt->flags));
_akvs_probe("5:42",'b','o');}

    ::free(static_cast<void *> (cevt->path));
  
_akvs_probe("5:40",'b','o');}
}

  ::free(static_cast<void *> (cevents));

_akvs_probe("5:20",'f','o');
}

FSW_HANDLE fsw_init_session(const fsw_monitor_type type)
{
_akvs_probe("5:43",'f','i');

#ifdef HAVE_CXX_MUTEX
  std::lock_guard<std::mutex> session_lock(session_mutex);
#endif

  if (!srand_initialized)
  {
{_akvs_probe("5:46",'b','i');

    srand(time(nullptr));
    srand_initialized = true;
  
_akvs_probe("5:46",'b','o');}
}

  int handle;

  do
  {
{_akvs_probe("5:48",'b','i');

    handle = rand();
  
_akvs_probe("5:48",'b','o');}
}
  while (sessions.find(handle) != sessions.end());

  FSW_SESSION *session = new FSW_SESSION{};

  session->handle = handle;
  session->type = type;

  // Store the handle and a mutex to guard access to session instances.
#ifdef HAVE_CXX_UNIQUE_PTR
  sessions[handle] = unique_ptr<FSW_SESSION>(session);
#else
  sessions[handle] = session;
#endif

#ifdef HAVE_CXX_MUTEX
#  ifdef HAVE_CXX_UNIQUE_PTR
  session_mutexes[handle] = unique_ptr<mutex>(new mutex);
#  else
  session_mutexes[handle] = new mutex;
#  endif
#endif

  
{_akvs_probe("5:43",'f','o');}
return handle;
}

int create_monitor(const FSW_HANDLE handle, const fsw_monitor_type type)
{
_akvs_probe("5:50",'f','i');

  try
  {
    FSW_SESSION * session = get_session(handle);

    // Check sufficient data is present to build a monitor.
    if (!session->callback)
      
{_akvs_probe("5:54",'b','i');
{int  __akvs2_return_value = ( fsw_set_last_error(int(FSW_ERR_CALLBACK_NOT_SET)));

_akvs_probe("5:50",'f','o');

return __akvs2_return_value;}
_akvs_probe("5:54",'b','o');}


    if (session->monitor)
      
{_akvs_probe("5:55",'b','i');
{int  __akvs2_return_value = ( fsw_set_last_error(int(FSW_ERR_MONITOR_ALREADY_EXISTS)));

_akvs_probe("5:50",'f','o');

return __akvs2_return_value;}
_akvs_probe("5:55",'b','o');}


    if (!session->paths.size())
      
{_akvs_probe("5:56",'b','i');
{int  __akvs2_return_value = ( fsw_set_last_error(int(FSW_ERR_PATHS_NOT_SET)));

_akvs_probe("5:50",'f','o');

return __akvs2_return_value;}
_akvs_probe("5:56",'b','o');}


    fsw_callback_context * context_ptr = new fsw_callback_context;
    context_ptr->callback = session->callback;
    context_ptr->handle = session->handle;

    monitor * current_monitor = monitor::create_monitor(type,
                                                        session->paths,
                                                        libfsw_cpp_callback_proxy,
                                                        context_ptr);
    session->monitor = current_monitor;
  }
  catch (libfsw_exception ex)
  {
{_akvs_probe("5:60",'b','i');

    {int  __akvs2_return_value = ( fsw_set_last_error(int(ex)));

_akvs_probe("5:50",'f','o');

return __akvs2_return_value;}
  
_akvs_probe("5:60",'b','o');}
}
  catch (int error)
  {
{_akvs_probe("5:62",'b','i');

    {int  __akvs2_return_value = ( fsw_set_last_error(error));

_akvs_probe("5:50",'f','o');

return __akvs2_return_value;}
  
_akvs_probe("5:62",'b','o');}
}

  {int  __akvs2_return_value = ( fsw_set_last_error(FSW_OK));

_akvs_probe("5:50",'f','o');

return __akvs2_return_value;}
}

int fsw_add_path(const FSW_HANDLE handle, const char * path)
{
_akvs_probe("5:63",'f','i');

  if (!path)
    
{_akvs_probe("5:66",'b','i');
{int  __akvs2_return_value = ( fsw_set_last_error(int(FSW_ERR_INVALID_PATH)));

_akvs_probe("5:63",'f','o');

return __akvs2_return_value;}
_akvs_probe("5:66",'b','o');}


  try
  {
#ifdef HAVE_CXX_MUTEX
    std::lock_guard<std::mutex> session_lock(session_mutex);
#endif
    FSW_SESSION * session = get_session(handle);

    session->paths.push_back(path);
  }
  catch (int error)
  {
{_akvs_probe("5:70",'b','i');

    {int  __akvs2_return_value = ( fsw_set_last_error(error));

_akvs_probe("5:63",'f','o');

return __akvs2_return_value;}
  
_akvs_probe("5:70",'b','o');}
}

  {int  __akvs2_return_value = ( fsw_set_last_error(FSW_OK));

_akvs_probe("5:63",'f','o');

return __akvs2_return_value;}
}

int fsw_set_callback(const FSW_HANDLE handle, const FSW_CEVENT_CALLBACK callback)
{
_akvs_probe("5:71",'f','i');

  if (!callback)
    
{_akvs_probe("5:74",'b','i');
{int  __akvs2_return_value = ( fsw_set_last_error(int(FSW_ERR_INVALID_CALLBACK)));

_akvs_probe("5:71",'f','o');

return __akvs2_return_value;}
_akvs_probe("5:74",'b','o');}


  try
  {
#ifdef HAVE_CXX_MUTEX
    std::lock_guard<std::mutex> session_lock(session_mutex);
#endif
    FSW_SESSION * session = get_session(handle);

    session->callback = callback;
  }
  catch (int error)
  {
{_akvs_probe("5:78",'b','i');

    {int  __akvs2_return_value = ( fsw_set_last_error(error));

_akvs_probe("5:71",'f','o');

return __akvs2_return_value;}
  
_akvs_probe("5:78",'b','o');}
}

  {int  __akvs2_return_value = ( fsw_set_last_error(FSW_OK));

_akvs_probe("5:71",'f','o');

return __akvs2_return_value;}
}

int fsw_set_latency(const FSW_HANDLE handle, const double latency)
{
_akvs_probe("5:79",'f','i');

  if (latency < 0)
    
{_akvs_probe("5:82",'b','i');
{int  __akvs2_return_value = ( fsw_set_last_error(int(FSW_ERR_INVALID_LATENCY)));

_akvs_probe("5:79",'f','o');

return __akvs2_return_value;}
_akvs_probe("5:82",'b','o');}


  try
  {
#ifdef HAVE_CXX_MUTEX
    std::lock_guard<std::mutex> session_lock(session_mutex);
#endif
    FSW_SESSION * session = get_session(handle);

    session->latency = latency;
  }
  catch (int error)
  {
{_akvs_probe("5:86",'b','i');

    {int  __akvs2_return_value = ( fsw_set_last_error(error));

_akvs_probe("5:79",'f','o');

return __akvs2_return_value;}
  
_akvs_probe("5:86",'b','o');}
}

  {int  __akvs2_return_value = ( fsw_set_last_error(FSW_OK));

_akvs_probe("5:79",'f','o');

return __akvs2_return_value;}
}

int fsw_set_recursive(const FSW_HANDLE handle, const bool recursive)
{
_akvs_probe("5:87",'f','i');

  try
  {
#ifdef HAVE_CXX_MUTEX
    std::lock_guard<std::mutex> session_lock(session_mutex);
#endif
    FSW_SESSION * session = get_session(handle);

    session->recursive = recursive;
  }
  catch (int error)
  {
{_akvs_probe("5:93",'b','i');

    {int  __akvs2_return_value = ( fsw_set_last_error(error));

_akvs_probe("5:87",'f','o');

return __akvs2_return_value;}
  
_akvs_probe("5:93",'b','o');}
}

  {int  __akvs2_return_value = ( fsw_set_last_error(FSW_OK));

_akvs_probe("5:87",'f','o');

return __akvs2_return_value;}
}

int fsw_set_follow_symlinks(const FSW_HANDLE handle,
                            const bool follow_symlinks)
{
_akvs_probe("5:94",'f','i');

  try
  {
#ifdef HAVE_CXX_MUTEX
    std::lock_guard<std::mutex> session_lock(session_mutex);
#endif
    FSW_SESSION * session = get_session(handle);

    session->follow_symlinks = follow_symlinks;
  }
  catch (int error)
  {
{_akvs_probe("5:100",'b','i');

    {int  __akvs2_return_value = ( fsw_set_last_error(error));

_akvs_probe("5:94",'f','o');

return __akvs2_return_value;}
  
_akvs_probe("5:100",'b','o');}
}

  {int  __akvs2_return_value = ( fsw_set_last_error(FSW_OK));

_akvs_probe("5:94",'f','o');

return __akvs2_return_value;}
}

int fsw_add_filter(const FSW_HANDLE handle,
                   const fsw_cmonitor_filter filter)
{
_akvs_probe("5:101",'f','i');

  try
  {
#ifdef HAVE_CXX_MUTEX
    std::lock_guard<std::mutex> session_lock(session_mutex);
#endif
    FSW_SESSION * session = get_session(handle);

    session->filters.push_back({filter.text, filter.type, filter.case_sensitive, filter.extended});
  }
  catch (int error)
  {
{_akvs_probe("5:107",'b','i');

    {int  __akvs2_return_value = ( fsw_set_last_error(error));

_akvs_probe("5:101",'f','o');

return __akvs2_return_value;}
  
_akvs_probe("5:107",'b','o');}
}

  {int  __akvs2_return_value = ( fsw_set_last_error(FSW_OK));

_akvs_probe("5:101",'f','o');

return __akvs2_return_value;}
}

#ifdef HAVE_CXX_MUTEX

template <typename T>
class monitor_start_guard
{
  atomic<T> & a;
  T val;

public:

  monitor_start_guard(atomic<T> & a,
                      T val,
                      memory_order sync = memory_order_seq_cst)
    : a(a), val(val)
  {
_akvs_probe("5:111",'f','i');

  
_akvs_probe("5:111",'f','o');
}

  ~monitor_start_guard()
  {
_akvs_probe("5:115",'f','i');

    a.store(val, memory_order_release);
  
_akvs_probe("5:115",'f','o');
}
};
#endif

int fsw_start_monitor(const FSW_HANDLE handle)
{
_akvs_probe("5:116",'f','i');

  try
  {
#ifdef HAVE_CXX_MUTEX
    unique_lock<mutex> session_lock(session_mutex, defer_lock);
    session_lock.lock();
#endif

    FSW_SESSION * session = get_session(handle);

#ifdef HAVE_CXX_MUTEX
    if (session->running.load(memory_order_acquire))
      
{_akvs_probe("5:120",'b','i');
{int  __akvs2_return_value = ( fsw_set_last_error(int(FSW_ERR_MONITOR_ALREADY_RUNNING)));

_akvs_probe("5:116",'f','o');

return __akvs2_return_value;}
_akvs_probe("5:120",'b','o');}


#  ifdef HAVE_CXX_UNIQUE_PTR
    unique_ptr<mutex> & sm = session_mutexes.at(handle);
    lock_guard<mutex> lock_sm(*sm.get());
#  else
    mutex * sm = session_mutexes.at(handle);
    lock_guard<mutex> lock_sm(*sm);
#  endif

    session_lock.unlock();
#endif

    if (!session->monitor)
      
{_akvs_probe("5:123",'b','i');
create_monitor(handle, session->type);
_akvs_probe("5:123",'b','o');}


    session->monitor->set_filters(session->filters);
    session->monitor->set_follow_symlinks(session->follow_symlinks);
    if (session->latency) 
{_akvs_probe("5:124",'b','i');
session->monitor->set_latency(session->latency);
_akvs_probe("5:124",'b','o');}

    session->monitor->set_recursive(session->recursive);

#ifdef HAVE_CXX_MUTEX
    session->running.store(true, memory_order_release);
    monitor_start_guard<bool> guard(session->running, false);
#endif

    session->monitor->start();
  }
  catch (int error)
  {
{_akvs_probe("5:127",'b','i');

    {int  __akvs2_return_value = ( fsw_set_last_error(error));

_akvs_probe("5:116",'f','o');

return __akvs2_return_value;}
  
_akvs_probe("5:127",'b','o');}
}

  {int  __akvs2_return_value = ( fsw_set_last_error(FSW_OK));

_akvs_probe("5:116",'f','o');

return __akvs2_return_value;}
}

int fsw_destroy_session(const FSW_HANDLE handle)
{
_akvs_probe("5:128",'f','i');

  int ret = FSW_OK;

  try
  {
#ifdef HAVE_CXX_MUTEX
    std::lock_guard<std::mutex> session_lock(session_mutex);
#endif
    FSW_SESSION * session = get_session(handle);

#ifdef HAVE_CXX_MUTEX
#  ifdef HAVE_CXX_UNIQUE_PTR
    const unique_ptr<mutex> & sm = session_mutexes[handle];
    lock_guard<mutex> sm_lock(*sm.get());
#  else
    mutex * sm = session_mutexes[handle];
    lock_guard<mutex> sm_lock(*sm);
#  endif
#endif

    if (session->monitor)
    {
{_akvs_probe("5:135",'b','i');

      void * context = session->monitor->get_context();

      if (!context)
      {
{_akvs_probe("5:137",'b','i');

        session->monitor->set_context(nullptr);
        delete static_cast<fsw_callback_context *> (context);
      
_akvs_probe("5:137",'b','o');}
}
      delete session->monitor;
    
_akvs_probe("5:135",'b','o');}
}

    sessions.erase(handle);
  }
  catch (int error)
  {
{_akvs_probe("5:139",'b','i');

    ret = error;
  
_akvs_probe("5:139",'b','o');}
}

  #ifdef HAVE_CXX_MUTEX
    session_mutexes.erase(handle);
  #endif

  {int  __akvs2_return_value = ( fsw_set_last_error(FSW_OK));

_akvs_probe("5:128",'f','o');

return __akvs2_return_value;}
}

FSW_SESSION * get_session(const FSW_HANDLE handle)
{
_akvs_probe("5:140",'f','i');

  if (sessions.find(handle) == sessions.end())
    
{_akvs_probe("5:142",'b','i');

_akvs_probe("5:140",'f','o');
throw int(FSW_ERR_SESSION_UNKNOWN);
_akvs_probe("5:142",'b','o');}


#ifdef HAVE_CXX_UNIQUE_PTR
  {FSW_SESSION * __akvs2_return_value = ( sessions[handle].get());

_akvs_probe("5:140",'f','o');

return __akvs2_return_value;}
#else
  return sessions[handle];
#endif
}

int fsw_set_last_error(const int error)
{
_akvs_probe("5:143",'f','i');

#if defined(HAVE_CXX_THREAD_LOCAL)
  last_error = error;
#endif

  
{_akvs_probe("5:143",'f','o');}
return error;
}

int fsw_last_error()
{
_akvs_probe("5:145",'f','i');

#if defined(HAVE_CXX_THREAD_LOCAL)
  
{_akvs_probe("5:145",'f','o');}
return last_error;
#else
  return fsw_set_last_error(FSW_ERR_UNSUPPORTED_OPERATION);
#endif
}

bool fsw_is_verbose()
{
_akvs_probe("5:146",'f','i');

  
{_akvs_probe("5:146",'f','o');}
return false;
}
