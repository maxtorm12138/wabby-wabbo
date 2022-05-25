#ifndef WABBY_API_HPP
#define WABBY_API_HPP

#if defined _WIN32 || defined __CYGWIN__
#  ifdef WABBY_API_BUILDING_DLL
#    ifdef __GNUC__
#      define WABBY_API_PUBLIC __attribute__( ( dllexport ) )
#    else
#      define WABBY_API_PUBLIC __declspec( dllexport )  // Note: actually gcc seems to also supports this syntax.
#    endif
#  else
#    ifdef __GNUC__
#      define WABBY_API_PUBLIC __attribute__( ( dllimport ) )
#    else
#      define WABBY_API_PUBLIC __declspec( dllimport )  // Note: actually gcc seems to also supports this syntax.
#    endif
#  endif
#  define WABBY_API_PRIVATE
#else
#  if __GNUC__ >= 4
#    define WABBY_API_PUBLIC  __attribute__( ( visibility( "default" ) ) )
#    define WABBY_API_PRIVATE __attribute__( ( visibility( "hidden" ) ) )
#  else
#    define WABBY_API_PUBLIC
#    define WABBY_API_PRIVATE
#  endif
#endif

#define WABBY_API_MAKE_VERSION( major, minor, patch ) ( ( ( (uint32_t)( major ) ) << 22 ) | ( ( (uint32_t)( minor ) ) << 12 ) | ( (uint32_t)( patch ) ) )

#endif