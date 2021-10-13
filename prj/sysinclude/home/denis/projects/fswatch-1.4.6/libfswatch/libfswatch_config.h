/* libfswatch_config.h.  Generated from libfswatch_config.h.in by configure.  */
/* libfswatch_config.h.in.  Generated from configure.ac by autoheader.  */

/* Define to 1 if translation of program messages to the user's native
   language is requested. */
#define ENABLE_NLS 1

/* Define to 1 if you have the Mac OS X function CFLocaleCopyCurrent in the
   CoreFoundation framework. */
/* #undef HAVE_CFLOCALECOPYCURRENT */

/* Define to 1 if you have the Mac OS X function CFPreferencesCopyAppValue in
   the CoreFoundation framework. */
/* #undef HAVE_CFPREFERENCESCOPYAPPVALUE */

/* Define to 1 if you have the <CoreServices/CoreServices.h> header file. */
/* #undef HAVE_CORESERVICES_CORESERVICES_H */

/* define if the compiler supports basic C++11 syntax */
/* #undef HAVE_CXX11 */

/* Define if <mutex> is available. */
#define HAVE_CXX_MUTEX 1

/* Define if the thread_local storage specified is available. */
#define HAVE_CXX_THREAD_LOCAL 1

/* Define if std::unique_ptr in <memory> is available. */
#define HAVE_CXX_UNIQUE_PTR 1

/* Define if the GNU dcgettext() function is already present or preinstalled.
   */
#define HAVE_DCGETTEXT 1

/* Define to 1 if you have the declaration of `kevent', and to 0 if you don't.
   */
#define HAVE_DECL_KEVENT 0

/* Define to 1 if you have the declaration of `kqueue', and to 0 if you don't.
   */
#define HAVE_DECL_KQUEUE 0

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the <fcntl.h> header file. */
#define HAVE_FCNTL_H 1

/* Define if the file events are supported by OS X FSEvents API. */
/* #undef HAVE_FSEVENTS_FILE_EVENTS */

/* Define if the GNU gettext() function is already present or preinstalled. */
#define HAVE_GETTEXT 1

/* Define if you have the iconv() function and it works. */
/* #undef HAVE_ICONV */

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the `modf' function. */
#define HAVE_MODF 1

/* Define to 1 if you have the `realpath' function. */
#define HAVE_REALPATH 1

/* Define to 1 if you have the `regcomp' function. */
#define HAVE_REGCOMP 1

/* Define to 1 if you have the `select' function. */
#define HAVE_SELECT 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if `st_mtime' is a member of `struct stat'. */
#define HAVE_STRUCT_STAT_ST_MTIME 1

/* Define to 1 if `st_mtimespec' is a member of `struct stat'. */
/* #undef HAVE_STRUCT_STAT_ST_MTIMESPEC */

/* Define to 1 if you have the <sys/event.h> header file. */
/* #undef HAVE_SYS_EVENT_H */

/* Define to 1 if you have the <sys/inotify.h> header file. */
#define HAVE_SYS_INOTIFY_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if you have the <unordered_map> header file. */
#define HAVE_UNORDERED_MAP 1

/* Define to 1 if you have the <unordered_set> header file. */
#define HAVE_UNORDERED_SET 1

/* Define to 1 if the system has the type `_Bool'. */
/* #undef HAVE__BOOL */

/* Define to the sub-directory where libtool stores uninstalled libraries. */
#define LT_OBJDIR ".libs/"

/* Name of package */
#define PACKAGE "libfswatch"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "enrico.m.crisostomo@gmail.com"

/* Define to the full name of this package. */
#define PACKAGE_NAME "libfswatch"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "libfswatch 1.4.6"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "libfswatch"

/* Define to the home page for this package. */
#define PACKAGE_URL "https://github.com/emcrisostomo/fswatch"

/* Define to the version of this package. */
#define PACKAGE_VERSION "1.4.6"

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Version number of package */
#define VERSION "1.4.6"

/* Define for Solaris 2.5.1 so the uint32_t typedef from <sys/synch.h>,
   <pthread.h>, or <semaphore.h> is not used. If the typedef were allowed, the
   #define below would cause a syntax error. */
/* #undef _UINT32_T */

/* Define to `int' if <sys/types.h> does not define. */
/* #undef mode_t */

/* Define to `unsigned int' if <sys/types.h> does not define. */
/* #undef size_t */

/* Define to the type of an unsigned integer type of width exactly 32 bits if
   such a type exists and the standard includes do not define it. */
/* #undef uint32_t */
