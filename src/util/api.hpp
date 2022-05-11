#ifndef _WAWY_UTIL_API_HPP
#define _WAWY_UTIL_API_HPP

#if defined _WIN32 || defined __CYGWIN__
  #if defined WAWY_API_BUILDING_DLL && !defined WAWY_API_IMPORT
    #ifdef __GNUC__
      #define WAWY_API_EXPORT __attribute__ ((dllexport))
    #else
      #define WAWY_API_EXPORT __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #else
    #ifdef __GNUC__
      #define WAWY_API_EXPORT __attribute__ ((dllimport))
    #else
      #define WAWY_API_EXPORT __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #endif
  #define WAWY_API_HIDE
#else
  #if __GNUC__ >= 4
    #define WAWY_API_EXPORT __attribute__ ((visibility ("default")))
    #define WAWY_API_HIDE  __attribute__ ((visibility ("hidden")))
  #else
    #define WAWY_API_EXPORT 
    #define WAWY_API_HIDE
  #endif
#endif

#define WAWY_MAKE_VERSION(major, minor, patch) \
    ((((uint32_t)(major)) << 22) | (((uint32_t)(minor)) << 12) | ((uint32_t)(patch)))

#endif