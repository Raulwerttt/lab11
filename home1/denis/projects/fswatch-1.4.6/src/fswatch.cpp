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
#  include "config.h"
#endif
#include "gettext.h"
#include "fswatch.h"
#include "fswatch_log.h"
#include <iostream>
#include <sstream>
#include <csignal>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cerrno>
#include <vector>
#include "libfswatch/c++/monitor.h"

#ifdef HAVE_GETOPT_LONG
#  include <getopt.h>
#endif

#define _(String) gettext(String)

using namespace std;

static string decode_event_flag_name(fsw_event_flag flag);

/*
 * Event formatting types and routines.
 */
static void print_event_flags(const event & evt);
static void print_event_path(const event & evt);
static void print_event_timestamp(const event & evt);

static int printf_event_validate_format(const string & fmt);

struct printf_event_callbacks
{
  void (*format_f)(const event & evt);
  void (*format_p)(const event & evt);
  void (*format_t)(const event & evt);
};

struct printf_event_callbacks event_format_callbacks
{
  print_event_flags,
  print_event_path,
  print_event_timestamp
};

static int printf_event(const string & fmt,
                        const event & evt,
                        const struct printf_event_callbacks & callback,
                        ostream & os = cout);

static const unsigned int TIME_FORMAT_BUFF_SIZE = 128;

static fsw::monitor *active_monitor = nullptr;
static vector<monitor_filter> filters;
static bool _0flag = false;
static bool _1flag = false;
static int batch_marker_flag = false;
static bool Eflag = false;
static bool fflag = false;
static bool Iflag = false;
static bool lflag = false;
static bool Lflag = false;
static bool mflag = false;
static bool nflag = false;
static bool oflag = false;
static bool rflag = false;
static bool tflag = false;
static bool uflag = false;
static bool vflag = false;
static int version_flag = false;
static bool xflag = false;
static double lvalue = 1.0;
static string monitor_name;
static string tformat = "%c";
static string batch_marker = decode_event_flag_name(fsw_event_flag::NoOp);
static int format_flag = false;
static string format;
static string event_flag_separator = " ";

/*
 * OPT_* variables are used as getopt_long values for long options that do not
 * have a short option equivalent.
 */
static const int OPT_BATCH_MARKER = 128;
static const int OPT_FORMAT = 129;
static const int OPT_EVENT_FLAG_SEPARATOR = 130;

bool is_verbose()
{
_akvs_probe("26:35",'f','i');

  
{_akvs_probe("26:35",'f','o');}
return vflag;
}

static void list_monitor_types(ostream& stream)
{
_akvs_probe("26:36",'f','i');

  stream << _("Available monitors in this platform:\n\n");

  for (const auto & type : fsw::monitor_factory::get_types())
  {
{_akvs_probe("26:38",'b','i');

    stream << "  " << type << "\n";
  
_akvs_probe("26:38",'b','o');}
}

_akvs_probe("26:36",'f','o');
}

static void print_version(ostream& stream)
{
_akvs_probe("26:40",'f','i');

  stream << PACKAGE_STRING << "\n";
  stream << "Copyright (C) 2014, 2015, Enrico M. Crisostomo <enrico.m.crisostomo@gmail.com>.\n";
  stream << _("License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\n");
  stream << _("This is free software: you are free to change and redistribute it.\n");
  stream << _("There is NO WARRANTY, to the extent permitted by law.\n");
  stream << "\n";
  stream << _("Written by Enrico M. Crisostomo.");
  stream << endl;

_akvs_probe("26:40",'f','o');
}

static void usage(ostream& stream)
{
_akvs_probe("26:42",'f','i');

#ifdef HAVE_GETOPT_LONG
  stream << PACKAGE_STRING << "\n\n";
  stream << _("Usage:\n");
  stream << PACKAGE_NAME << _(" [OPTION] ... path ...\n");
  stream << "\n";
  stream << _("Options:\n");
  stream << " -0, --print0          " << _("Use the ASCII NUL character (0) as line separator.\n");
  stream << " -1, --one-event       " << _("Exit fswatch after the first set of events is received.\n");
  stream << "     --batch-marker    " << _("Print a marker at the end of every batch.\n");
#  ifdef HAVE_REGCOMP
  stream << " -e, --exclude=REGEX   " << _("Exclude paths matching REGEX.\n");
  stream << " -E, --extended        " << _("Use extended regular expressions.\n");
#  endif
  stream << "     --format=FORMAT   " << _("Use the specified record format.") << "\n";
  stream << " -f, --format-time     " << _("Print the event time using the specified format.\n");
  stream << " -h, --help            " << _("Show this message.\n");
#  ifdef HAVE_REGCOMP
  stream << " -i, --include=REGEX   " << _("Include paths matching REGEX.\n");
  stream << " -I, --insensitive     " << _("Use case insensitive regular expressions.\n");
#  endif
  stream << " -l, --latency=DOUBLE  " << _("Set the latency.\n");
  stream << " -L, --follow-links    " << _("Follow symbolic links.\n");
  stream << " -m, --monitor=NAME    " << _("Use the specified monitor.\n");
  stream << " -n, --numeric         " << _("Print a numeric event mask.\n");
  stream << " -o, --one-per-batch   " << _("Print a single message with the number of change events.\n");
  stream << " -r, --recursive       " << _("Recurse subdirectories.\n");
  stream << " -t, --timestamp       " << _("Print the event timestamp.\n");
  stream << " -u, --utc-time        " << _("Print the event time as UTC time.\n");
  stream << " -v, --verbose         " << _("Print verbose output.\n");
  stream << "     --version         " << _("Print the version of ") << PACKAGE_NAME << _(" and exit.\n");
  stream << " -x, --event-flags     " << _("Print the event flags.\n");
  stream << "     --event-flag-separator=STRING\n";
  stream << "                       " << _("Print event flags using the specified separator.") << "\n";
  stream << "\n";
#else
  string option_string = "[";
  option_string += "01";
#  ifdef HAVE_REGCOMP
  option_string += "eE";
#  endif
  option_string += "fh";
#  ifdef HAVE_REGCOMP
  option_string += "i";
#  endif
  option_string += "lLmnortuvx";
  option_string += "]";

  stream << PACKAGE_STRING << "\n\n";
  stream << "Syntax:\n";
  stream << PACKAGE_NAME << " " << option_string << " path ...\n";
  stream << "\n";
  stream << "Usage:\n";
  stream << " -0  Use the ASCII NUL character (0) as line separator.\n";
  stream << " -1  Exit fswatch after the first set of events is received.\n";
#  ifdef HAVE_REGCOMP
  stream << " -e  Exclude paths matching REGEX.\n";
  stream << " -E  Use extended regular expressions.\n";
#  endif
  stream << " -f  Print the event time stamp with the specified format.\n";
  stream << " -h  Show this message.\n";
#  ifdef HAVE_REGCOMP
  stream << " -i  Use case insensitive regular expressions.\n";
  stream << " -i  Include paths matching REGEX.\n";
#  endif
  stream << " -l  Set the latency.\n";
  stream << " -m  Use the specified monitor.\n";
  stream << " -L  Follow symbolic links.\n";
  stream << " -n  Print a numeric event masks.\n";
  stream << " -o  Print a single message with the number of change events in the current\n";
  stream << "     batch.\n";
  stream << " -r  Recurse subdirectories.\n";
  stream << " -t  Print the event timestamp.\n";
  stream << " -u  Print the event time as UTC time.\n";
  stream << " -v  Print verbose output.\n";
  stream << " -x  Print the event flags.\n";
  stream << "\n";
#endif

  list_monitor_types(stream);

  stream << _("\nSee the man page for more information.\n\n");

  stream << _("Report bugs to <") << PACKAGE_BUGREPORT << ">.\n";
  stream << PACKAGE << _(" home page: <") << PACKAGE_URL << ">.";
  stream << endl;

_akvs_probe("26:42",'f','o');
}

static void close_stream()
{
_akvs_probe("26:44",'f','i');

  if (active_monitor)
  {
{_akvs_probe("26:45",'b','i');

    delete active_monitor;

    active_monitor = nullptr;
  
_akvs_probe("26:45",'b','o');}
}

_akvs_probe("26:44",'f','o');
}

static void close_handler(int signal)
{
_akvs_probe("26:46",'f','i');

  close_stream();

  fsw_log(_("Done.\n"));
  exit(FSW_EXIT_OK);

_akvs_probe("26:46",'f','o');
}

static bool validate_latency(double latency, ostream &ost, ostream &est)
{
_akvs_probe("26:48",'f','i');

  if (lvalue == 0.0)
  {
{_akvs_probe("26:52",'b','i');

    est << _("Invalid value: ") << optarg << endl;
    
{_akvs_probe("26:48",'f','o');}
return false;
  
_akvs_probe("26:52",'b','o');}
}

  if (errno == ERANGE || lvalue == HUGE_VAL)
  {
{_akvs_probe("26:53",'b','i');

    est << _("Value out of range: ") << optarg << endl;
    
{_akvs_probe("26:48",'f','o');}
return false;
  
_akvs_probe("26:53",'b','o');}
}

  if (is_verbose())
  {
{_akvs_probe("26:54",'b','i');

    ost << _("Latency set to: ") << lvalue << endl;
  
_akvs_probe("26:54",'b','o');}
}

  
{_akvs_probe("26:48",'f','o');}
return true;
}

static void register_signal_handlers()
{
_akvs_probe("26:55",'f','i');

  struct sigaction action;
  action.sa_handler = close_handler;
  sigemptyset(&action.sa_mask);
  action.sa_flags = 0;

  if (sigaction(SIGTERM, &action, nullptr) == 0)
  {
{_akvs_probe("26:57",'b','i');

    fsw_log(_("SIGTERM handler registered.\n"));
  
_akvs_probe("26:57",'b','o');}
}
  else
  {
{_akvs_probe("26:58",'b','i');

    cerr << _("SIGTERM handler registration failed.") << endl;
  
_akvs_probe("26:58",'b','o');}
}

  if (sigaction(SIGABRT, &action, nullptr) == 0)
  {
{_akvs_probe("26:59",'b','i');

    fsw_log(_("SIGABRT handler registered.\n"));
  
_akvs_probe("26:59",'b','o');}
}
  else
  {
{_akvs_probe("26:60",'b','i');

    cerr << _("SIGABRT handler registration failed.") << endl;
  
_akvs_probe("26:60",'b','o');}
}

  if (sigaction(SIGINT, &action, nullptr) == 0)
  {
{_akvs_probe("26:61",'b','i');

    fsw_log(_("SIGINT handler registered.\n"));
  
_akvs_probe("26:61",'b','o');}
}
  else
  {
{_akvs_probe("26:62",'b','i');

    cerr << _("SIGINT handler registration failed") << endl;
  
_akvs_probe("26:62",'b','o');}
}

_akvs_probe("26:55",'f','o');
}

static string decode_event_flag_name(fsw_event_flag flag)
{
_akvs_probe("26:63",'f','i');

  switch (flag)
  {
  case fsw_event_flag::NoOp:
    
_akvs_probe("26:65",'b','i');
{string  __akvs2_return_value = ( "NoOp");

_akvs_probe("26:63",'f','o');

return __akvs2_return_value;}
  
_akvs_probe("26:65",'b','o');
case fsw_event_flag::PlatformSpecific:
    
_akvs_probe("26:66",'b','i');
{string  __akvs2_return_value = ( "PlatformSpecific");

_akvs_probe("26:63",'f','o');

return __akvs2_return_value;}
  
_akvs_probe("26:66",'b','o');
case fsw_event_flag::Created:
    
_akvs_probe("26:67",'b','i');
{string  __akvs2_return_value = ( "Created");

_akvs_probe("26:63",'f','o');

return __akvs2_return_value;}
  
_akvs_probe("26:67",'b','o');
case fsw_event_flag::Updated:
    
_akvs_probe("26:68",'b','i');
{string  __akvs2_return_value = ( "Updated");

_akvs_probe("26:63",'f','o');

return __akvs2_return_value;}
  
_akvs_probe("26:68",'b','o');
case fsw_event_flag::Removed:
    
_akvs_probe("26:69",'b','i');
{string  __akvs2_return_value = ( "Removed");

_akvs_probe("26:63",'f','o');

return __akvs2_return_value;}
  
_akvs_probe("26:69",'b','o');
case fsw_event_flag::Renamed:
    
_akvs_probe("26:70",'b','i');
{string  __akvs2_return_value = ( "Renamed");

_akvs_probe("26:63",'f','o');

return __akvs2_return_value;}
  
_akvs_probe("26:70",'b','o');
case fsw_event_flag::OwnerModified:
    
_akvs_probe("26:71",'b','i');
{string  __akvs2_return_value = ( "OwnerModified");

_akvs_probe("26:63",'f','o');

return __akvs2_return_value;}
  
_akvs_probe("26:71",'b','o');
case fsw_event_flag::AttributeModified:
    
_akvs_probe("26:72",'b','i');
{string  __akvs2_return_value = ( "AttributeModified");

_akvs_probe("26:63",'f','o');

return __akvs2_return_value;}
  
_akvs_probe("26:72",'b','o');
case fsw_event_flag::MovedFrom:
    
_akvs_probe("26:73",'b','i');
{string  __akvs2_return_value = ( "MovedFrom");

_akvs_probe("26:63",'f','o');

return __akvs2_return_value;}
  
_akvs_probe("26:73",'b','o');
case fsw_event_flag::MovedTo:
    
_akvs_probe("26:74",'b','i');
{string  __akvs2_return_value = ( "MovedTo");

_akvs_probe("26:63",'f','o');

return __akvs2_return_value;}
  
_akvs_probe("26:74",'b','o');
case fsw_event_flag::IsFile:
    
_akvs_probe("26:75",'b','i');
{string  __akvs2_return_value = ( "IsFile");

_akvs_probe("26:63",'f','o');

return __akvs2_return_value;}
  
_akvs_probe("26:75",'b','o');
case fsw_event_flag::IsDir:
    
_akvs_probe("26:76",'b','i');
{string  __akvs2_return_value = ( "IsDir");

_akvs_probe("26:63",'f','o');

return __akvs2_return_value;}
  
_akvs_probe("26:76",'b','o');
case fsw_event_flag::IsSymLink:
    
_akvs_probe("26:77",'b','i');
{string  __akvs2_return_value = ( "IsSymLink");

_akvs_probe("26:63",'f','o');

return __akvs2_return_value;}
  
_akvs_probe("26:77",'b','o');
case fsw_event_flag::Link:
    
_akvs_probe("26:78",'b','i');
{string  __akvs2_return_value = ( "Link");

_akvs_probe("26:63",'f','o');

return __akvs2_return_value;}
  
_akvs_probe("26:78",'b','o');
default:
    {string  __akvs2_return_value = ( "<Unknown>");

_akvs_probe("26:63",'f','o');

return __akvs2_return_value;}
  }

_akvs_probe("26:63",'f','o');
}

static vector<string> decode_event_flag_names(vector<fsw_event_flag> flags)
{
_akvs_probe("26:80",'f','i');

  vector<string> names;

  for (fsw_event_flag flag : flags)
  {
{_akvs_probe("26:83",'b','i');

    names.push_back(decode_event_flag_name(flag));
  
_akvs_probe("26:83",'b','o');}
}

  
{_akvs_probe("26:80",'f','o');}
return names;
}

static void print_event_path(const event & evt)
{
_akvs_probe("26:85",'f','i');

  cout << evt.get_path();

_akvs_probe("26:85",'f','o');
}

static void print_event_timestamp(const event & evt)
{
_akvs_probe("26:87",'f','i');

  const time_t & evt_time = evt.get_time();

  char time_format_buffer[TIME_FORMAT_BUFF_SIZE];
  struct tm * tm_time = uflag ? gmtime(&evt_time) : localtime(&evt_time);

  string date =
    strftime(time_format_buffer,
             TIME_FORMAT_BUFF_SIZE,
             tformat.c_str(),
             tm_time) ? string(time_format_buffer) : string(_("<date format error>"));

  cout << date;

_akvs_probe("26:87",'f','o');
}

static void print_event_flags(const event & evt)
{
_akvs_probe("26:93",'f','i');

  const vector<fsw_event_flag> & flags = evt.get_flags();

  if (nflag)
  {
{_akvs_probe("26:96",'b','i');

    int mask = 0;
    for (const fsw_event_flag &flag : flags)
    {
{_akvs_probe("26:98",'b','i');

      mask += static_cast<int> (flag);
    
_akvs_probe("26:98",'b','o');}
}

    cout << mask;
  
_akvs_probe("26:96",'b','o');}
}
  else
  {
{_akvs_probe("26:100",'b','i');

    vector<string> flag_names = decode_event_flag_names(flags);

    for (int i = 0; i < flag_names.size(); ++i)
    {
{_akvs_probe("26:103",'b','i');

      const string &name = flag_names[i];
      cout << name;

      // Event flag separator is currently hard-coded.
      if (i != flag_names.size() - 1) 
{_akvs_probe("26:105",'b','i');
cout << event_flag_separator;
_akvs_probe("26:105",'b','o');}

    
_akvs_probe("26:103",'b','o');}
}
  
_akvs_probe("26:100",'b','o');}
}

_akvs_probe("26:93",'f','o');
}

static void print_end_of_event_record()
{
_akvs_probe("26:106",'f','i');

  if (_0flag)
  {
{_akvs_probe("26:107",'b','i');

    cout << '\0';
    cout.flush();
  
_akvs_probe("26:107",'b','o');}
}
  else
  {
{_akvs_probe("26:108",'b','i');

    cout << endl;
  
_akvs_probe("26:108",'b','o');}
}

_akvs_probe("26:106",'f','o');
}

static void write_batch_marker()
{
_akvs_probe("26:109",'f','i');

  if (batch_marker_flag)
  {
{_akvs_probe("26:110",'b','i');

    cout << batch_marker;
    print_end_of_event_record();
  
_akvs_probe("26:110",'b','o');}
}

_akvs_probe("26:109",'f','o');
}

static void write_one_batch_event(const vector<event> &events)
{
_akvs_probe("26:111",'f','i');

  cout << events.size();
  print_end_of_event_record();

  write_batch_marker();

_akvs_probe("26:111",'f','o');
}

static void write_events(const vector<event> &events)
{
_akvs_probe("26:113",'f','i');

  for (const event &evt : events)
  {
{_akvs_probe("26:115",'b','i');

    printf_event(format, evt, event_format_callbacks);
    print_end_of_event_record();
  
_akvs_probe("26:115",'b','o');}
}

  write_batch_marker();

  if (_1flag)
  {
{_akvs_probe("26:117",'b','i');

    ::exit(FSW_EXIT_OK);
  
_akvs_probe("26:117",'b','o');}
}

_akvs_probe("26:113",'f','o');
}

static void process_events(const vector<event> &events, void * context)
{
_akvs_probe("26:118",'f','i');

  if (oflag)
    
{_akvs_probe("26:121",'b','i');
write_one_batch_event(events);
_akvs_probe("26:121",'b','o');}

  else
    
{_akvs_probe("26:122",'b','i');
write_events(events);
_akvs_probe("26:122",'b','o');}


_akvs_probe("26:118",'f','o');
}

static void start_monitor(int argc, char ** argv, int optind)
{
_akvs_probe("26:123",'f','i');

  // parsing paths
  vector<string> paths;

  for (auto i = optind; i < argc; ++i)
  {
{_akvs_probe("26:129",'b','i');

    char *real_path = ::realpath(argv[i], nullptr);
    string path(real_path ? real_path : argv[i]);

    if (real_path)
    {
{_akvs_probe("26:132",'b','i');

      ::free(real_path);
    
_akvs_probe("26:132",'b','o');}
}

    fsw_log(_("Adding path: "));
    fsw_log(path.c_str());
    fsw_log("\n");

    paths.push_back(path);
  
_akvs_probe("26:129",'b','o');}
}

  if (mflag)
    
{_akvs_probe("26:133",'b','i');
active_monitor = fsw::monitor_factory::create_monitor_by_name(monitor_name,
                                                                  paths,
                                                                  process_events);
_akvs_probe("26:133",'b','o');}

  else
    
{_akvs_probe("26:134",'b','i');
active_monitor = fsw::monitor::create_default_monitor(paths,
                                                          process_events);
_akvs_probe("26:134",'b','o');}


  /* 
   * libfswatch supports case sensitivity and extended flags to be set on any
   * filter but fswatch does not.  For the time being, we apply the same flags
   * to every filter.
   */
  for (auto & filter : filters)
  {
{_akvs_probe("26:135",'b','i');

    filter.case_sensitive = !Iflag;
    filter.extended = Eflag;
  
_akvs_probe("26:135",'b','o');}
}

  active_monitor->set_latency(lvalue);
  active_monitor->set_recursive(rflag);
  active_monitor->set_filters(filters);
  active_monitor->set_follow_symlinks(Lflag);

  active_monitor->start();

_akvs_probe("26:123",'f','o');
}

static void parse_opts(int argc, char ** argv)
{
_akvs_probe("26:137",'f','i');

  int ch;
  ostringstream short_options;

  short_options << "01f:hl:Lm:nortuvx";
#ifdef HAVE_REGCOMP
  short_options << "e:Ei:I";
#endif
  short_options << "k";

#ifdef HAVE_GETOPT_LONG
  int option_index = 0;
  static struct option long_options[] = {
    { "print0", no_argument, nullptr, '0'},
    { "one-event", no_argument, nullptr, '1'},
    { "batch-marker", optional_argument, nullptr, OPT_BATCH_MARKER},
#  ifdef HAVE_REGCOMP
    { "exclude", required_argument, nullptr, 'e'},
    { "extended", no_argument, nullptr, 'E'},
#  endif
    { "format", required_argument, nullptr, OPT_FORMAT},
    { "format-time", required_argument, nullptr, 'f'},
    { "help", no_argument, nullptr, 'h'},
#  ifdef HAVE_REGCOMP
    { "include", required_argument, nullptr, 'i'},
    { "insensitive", no_argument, nullptr, 'I'},
#  endif
    { "latency", required_argument, nullptr, 'l'},
    { "follow-links", no_argument, nullptr, 'L'},
    { "monitor", required_argument, nullptr, 'm'},
    { "numeric", no_argument, nullptr, 'n'},
    { "one-per-batch", no_argument, nullptr, 'o'},
    { "recursive", no_argument, nullptr, 'r'},
    { "timestamp", no_argument, nullptr, 't'},
    { "utc-time", no_argument, nullptr, 'u'},
    { "verbose", no_argument, nullptr, 'v'},
    { "version", no_argument, &version_flag, true},
    { "event-flags", no_argument, nullptr, 'x'},
    { "event-flag-separator", required_argument, nullptr, OPT_EVENT_FLAG_SEPARATOR},
    { nullptr, 0, nullptr, 0}
  };

  while ((ch = getopt_long(argc,
                           argv,
                           short_options.str().c_str(),
                           long_options,
                           &option_index)) != -1)
  {
{_akvs_probe("26:144",'b','i');

#else
  while ((ch = getopt(argc, argv, short_options.str().c_str())) != -1)
  {
#endif

    switch (ch)
    {
    case '0':
      
_akvs_probe("26:145",'b','i');
_0flag = true;
      
_akvs_probe("26:145",'b','o');
break;

    
_akvs_probe("26:145",'b','o');
case '1':
      
_akvs_probe("26:146",'b','i');
_1flag = true;
      
_akvs_probe("26:146",'b','o');
break;

#ifdef HAVE_REGCOMP
    
_akvs_probe("26:146",'b','o');
case 'e':
      
_akvs_probe("26:147",'b','i');
filters.push_back({optarg, fsw_filter_type::filter_exclude});
      
_akvs_probe("26:147",'b','o');
break;

    
_akvs_probe("26:147",'b','o');
case 'E':
      
_akvs_probe("26:148",'b','i');
Eflag = true;
      
_akvs_probe("26:148",'b','o');
break;
#endif

    
_akvs_probe("26:148",'b','o');
case 'f':
      
_akvs_probe("26:149",'b','i');
fflag = true;
      tformat = string(optarg);
      
_akvs_probe("26:149",'b','o');
break;

    
_akvs_probe("26:149",'b','o');
case 'h':
      
_akvs_probe("26:150",'b','i');
usage(cout);
      ::exit(FSW_EXIT_OK);

#ifdef HAVE_REGCOMP
    
_akvs_probe("26:150",'b','o');
case 'i':
      
_akvs_probe("26:151",'b','i');
filters.push_back({optarg, fsw_filter_type::filter_include});
      
_akvs_probe("26:151",'b','o');
break;

    
_akvs_probe("26:151",'b','o');
case 'I':
      
_akvs_probe("26:152",'b','i');
Iflag = true;
      
_akvs_probe("26:152",'b','o');
break;
#endif

    
_akvs_probe("26:152",'b','o');
case 'l':
      
_akvs_probe("26:153",'b','i');
lflag = true;
      lvalue = strtod(optarg, nullptr);

      if (!validate_latency(lvalue, cout, cerr))
      {
{_akvs_probe("26:154",'b','i');

        exit(FSW_EXIT_LATENCY);
      
_akvs_probe("26:154",'b','o');}
}

      
_akvs_probe("26:153",'b','o');
break;

    
_akvs_probe("26:153",'b','o');
case 'L':
      
_akvs_probe("26:155",'b','i');
Lflag = true;
      
_akvs_probe("26:155",'b','o');
break;

    
_akvs_probe("26:155",'b','o');
case 'm':
      
_akvs_probe("26:156",'b','i');
mflag = true;
      monitor_name = string(optarg);
      
_akvs_probe("26:156",'b','o');
break;

    
_akvs_probe("26:156",'b','o');
case 'n':
      
_akvs_probe("26:157",'b','i');
nflag = true;
      xflag = true;
      
_akvs_probe("26:157",'b','o');
break;

    
_akvs_probe("26:157",'b','o');
case 'o':
      
_akvs_probe("26:158",'b','i');
oflag = true;
      
_akvs_probe("26:158",'b','o');
break;

    
_akvs_probe("26:158",'b','o');
case 'r':
      
_akvs_probe("26:159",'b','i');
rflag = true;
      
_akvs_probe("26:159",'b','o');
break;

    
_akvs_probe("26:159",'b','o');
case 't':
      
_akvs_probe("26:160",'b','i');
tflag = true;
      
_akvs_probe("26:160",'b','o');
break;

    
_akvs_probe("26:160",'b','o');
case 'u':
      
_akvs_probe("26:161",'b','i');
uflag = true;
      
_akvs_probe("26:161",'b','o');
break;

    
_akvs_probe("26:161",'b','o');
case 'v':
      
_akvs_probe("26:162",'b','i');
vflag = true;
      
_akvs_probe("26:162",'b','o');
break;

    
_akvs_probe("26:162",'b','o');
case 'x':
      
_akvs_probe("26:163",'b','i');
xflag = true;
      
_akvs_probe("26:163",'b','o');
break;

    
_akvs_probe("26:163",'b','o');
case OPT_BATCH_MARKER:
      
_akvs_probe("26:164",'b','i');
if (optarg) 
{_akvs_probe("26:165",'b','i');
batch_marker = optarg;
_akvs_probe("26:165",'b','o');}

      batch_marker_flag = true;
      
_akvs_probe("26:164",'b','o');
break;

    
_akvs_probe("26:164",'b','o');
case OPT_FORMAT:
      
_akvs_probe("26:166",'b','i');
format_flag = true;
      format = optarg;
      
_akvs_probe("26:166",'b','o');
break;

    
_akvs_probe("26:166",'b','o');
case OPT_EVENT_FLAG_SEPARATOR:
      
_akvs_probe("26:167",'b','i');
event_flag_separator = optarg;
      
_akvs_probe("26:167",'b','o');
break;
      
    
_akvs_probe("26:167",'b','o');
case '?':
      
_akvs_probe("26:168",'b','i');
usage(cerr);
      exit(FSW_EXIT_UNK_OPT);
    
_akvs_probe("26:168",'b','o');
}
  
_akvs_probe("26:144",'b','o');}
}

  if (version_flag)
  {
{_akvs_probe("26:169",'b','i');

    print_version(cout);
    ::exit(FSW_EXIT_OK);
  
_akvs_probe("26:169",'b','o');}
}

  // --format is incompatible with any other format option.
  if (format_flag && (tflag || xflag))
  {
{_akvs_probe("26:170",'b','i');

    cerr << _("--format is incompatible with any other format option such as -t and -x.") << endl;
    ::exit(FSW_EXIT_FORMAT);
  
_akvs_probe("26:170",'b','o');}
}

  if (format_flag && oflag)
  {
{_akvs_probe("26:171",'b','i');

    cerr << _("--format is incompatible with -o.") << endl;
    ::exit(FSW_EXIT_FORMAT);
  
_akvs_probe("26:171",'b','o');}
}

  // If no format was specified use:
  //   * %p as the default.
  //   * -t adds "%t " at the beginning of the format.
  //   * -x adds " %f" at the end of the format.
  //   * '\n' is used as record separator unless -0 is used, in which case '\0'
  //     is used instead.
  if (format_flag)
  {
{_akvs_probe("26:172",'b','i');

    // Test the user format
    if (printf_event_validate_format(format) < 0)
    {
{_akvs_probe("26:173",'b','i');

      cerr << _("Invalid format.") << endl;
      ::exit(FSW_EXIT_FORMAT);
    
_akvs_probe("26:173",'b','o');}
}
  
_akvs_probe("26:172",'b','o');}
}
  else
  {
{_akvs_probe("26:174",'b','i');

    // Build event format.
    if (tflag)
    {
{_akvs_probe("26:175",'b','i');

      format = "%t ";
    
_akvs_probe("26:175",'b','o');}
}

    format += "%p";

    if (xflag)
    {
{_akvs_probe("26:176",'b','i');

      format += " %f";
    
_akvs_probe("26:176",'b','o');}
}
  
_akvs_probe("26:174",'b','o');}
}

_akvs_probe("26:137",'f','o');
}

static void format_noop(const event & evt)
{
_akvs_probe("26:177",'f','i');


_akvs_probe("26:177",'f','o');
}

static int printf_event_validate_format(const string & fmt)
{
_akvs_probe("26:179",'f','i');


  struct printf_event_callbacks noop_callbacks
  {
    format_noop,
    format_noop,
    format_noop
  };

  const vector<fsw_event_flag> flags;
  const event empty("", 0, flags);
  ostream noop_stream(nullptr);

  {int  __akvs2_return_value = ( printf_event(fmt, empty, noop_callbacks, noop_stream));

_akvs_probe("26:179",'f','o');

return __akvs2_return_value;}
}

static int printf_event(const string & fmt,
                        const event & evt,
                        const struct printf_event_callbacks & callback,
                        ostream & os)
{
_akvs_probe("26:185",'f','i');

  /*
   * %t - time (further formatted using -f and strftime.
   * %p - event path
   * %f - event flags (event separator will be formatted with a separate option)
   */
  for (auto i = 0; i < format.length(); ++i)
  {
{_akvs_probe("26:191",'b','i');

    // If the character does not start a format directive, copy it as it is.
    if (format[i] != '%')
    {
{_akvs_probe("26:192",'b','i');

      os << format[i];
      
_akvs_probe("26:191",'b','o');
continue;
    
_akvs_probe("26:192",'b','o');}
}

    // If this is the end of the string, dump an error.
    if (i == format.length() - 1)
    {
{_akvs_probe("26:193",'b','i');

      
{_akvs_probe("26:185",'f','o');}
return -1;
    
_akvs_probe("26:193",'b','o');}
}

    // Advance to next format and check which directive it is.
    const char c = format[++i];

    switch (c)
    {
    case '%':
      
_akvs_probe("26:195",'b','i');
os << '%';
      
_akvs_probe("26:195",'b','o');
break;
    
_akvs_probe("26:195",'b','o');
case '0':
      
_akvs_probe("26:196",'b','i');
os << '\0';
      
_akvs_probe("26:196",'b','o');
break;
    
_akvs_probe("26:196",'b','o');
case 'n':
      
_akvs_probe("26:197",'b','i');
os << '\n';
      
_akvs_probe("26:197",'b','o');
break;
    
_akvs_probe("26:197",'b','o');
case 'f':
      
_akvs_probe("26:198",'b','i');
callback.format_f(evt);
      
_akvs_probe("26:198",'b','o');
break;
    
_akvs_probe("26:198",'b','o');
case 'p':
      
_akvs_probe("26:199",'b','i');
callback.format_p(evt);
      
_akvs_probe("26:199",'b','o');
break;
    
_akvs_probe("26:199",'b','o');
case 't':
      
_akvs_probe("26:200",'b','i');
callback.format_t(evt);
      
_akvs_probe("26:200",'b','o');
break;
    
_akvs_probe("26:200",'b','o');
default:
      
{_akvs_probe("26:185",'f','o');}
return -1;
    }
  
_akvs_probe("26:191",'b','o');}
}

  
{_akvs_probe("26:185",'f','o');}
return 0;
}

int main(int argc, char ** argv)
{
_akvs_probe("26:202",'f','i');

  // Trigger gettext operations
#ifdef ENABLE_NLS
  setlocale(LC_ALL, "");
  bindtextdomain(PACKAGE, LOCALEDIR);
  textdomain(PACKAGE);
#endif

  parse_opts(argc, argv);

  // validate options
  if (optind == argc)
  {
{_akvs_probe("26:205",'b','i');

    cerr << _("Invalid number of arguments.") << endl;
    ::exit(FSW_EXIT_UNK_OPT);
  
_akvs_probe("26:205",'b','o');}
}

  if (mflag && !fsw::monitor_factory::exists_type(monitor_name))
  {
{_akvs_probe("26:206",'b','i');

    cerr << _("Invalid monitor name.") << endl;
    ::exit(FSW_EXIT_MONITOR_NAME);
  
_akvs_probe("26:206",'b','o');}
}

  // configure and start the monitor
  try
  {
    // registering handlers to clean up resources
    register_signal_handlers();
    ::atexit(close_stream);

    // configure and start the monitor loop
    start_monitor(argc, argv, optind);
  }
  catch (exception & conf)
  {
{_akvs_probe("26:208",'b','i');

    cerr << _("An error occurred and the program will be terminated.\n");
    cerr << conf.what() << endl;

    
{_akvs_probe("26:202",'f','o');}
return FSW_EXIT_ERROR;
  
_akvs_probe("26:208",'b','o');}
}
  catch (...)
  {
{_akvs_probe("26:209",'b','i');

    cerr << _("An unknown error occurred and the program will be terminated.") << endl;

    
{_akvs_probe("26:202",'f','o');}
return FSW_EXIT_ERROR;
  
_akvs_probe("26:209",'b','o');}
}

  
{_akvs_probe("26:202",'f','o');}
return FSW_EXIT_OK;
}


#define _AKVS_PROBE_IMPLEMENTATION_ 1
#include "_akvs_probe.h"

