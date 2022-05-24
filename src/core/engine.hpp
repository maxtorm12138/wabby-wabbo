#ifndef WABBY_CORE_ENGINE_HPP
#define WABBY_CORE_ENGINE_HPP

// boost
#include "boost/config.hpp"

namespace wabby::core
{

  struct BOOST_SYMBOL_EXPORT engine_setup_info
  {
    const char * application_name;
    uint32_t     application_version;
    uint32_t     width;
    uint32_t     height;
  };

  class engine_impl;

  class BOOST_SYMBOL_EXPORT engine
  {
  public:
    engine();

    engine( const engine & other ) = delete;

    engine & operator=( const engine & other ) = delete;

    ~engine();

  public:
    void setup( const engine_setup_info & setup_info );

    void run();

    void teardown();

  private:
    engine_impl * impl_;
  };
}  // namespace wabby::core

#endif