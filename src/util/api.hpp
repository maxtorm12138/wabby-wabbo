#ifndef _WABBY_UTIL_API_HPP
#define _WABBY_UTIL_API_HPP

#if defined _WIN32 || defined __CYGWIN__
  #if defined WABBY_API_BUILDING_DLL && !defined WABBY_API_IMPORT
    #ifdef __GNUC__
      #define WABBY_API_EXPORT __attribute__ ((dllexport))
    #else
      #define WABBY_API_EXPORT __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #else
    #ifdef __GNUC__
      #define WABBY_API_EXPORT __attribute__ ((dllimport))
    #else
      #define WABBY_API_EXPORT __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #endif
  #define WABBY_API_HIDE
#else
  #if __GNUC__ >= 4
    #define WABBY_API_EXPORT __attribute__ ((visibility ("default")))
    #define WABBY_API_HIDE  __attribute__ ((visibility ("hidden")))
  #else
    #define WABBY_API_EXPORT 
    #define WABBY_API_HIDE
  #endif
#endif

#define WABBY_MAKE_VERSION(major, minor, patch) \
    ((((uint32_t)(major)) << 22) | (((uint32_t)(minor)) << 12) | ((uint32_t)(patch)))

#endif