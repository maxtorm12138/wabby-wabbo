#ifndef _WABBY_CORE_ENGINE_HPP
#define _WABBY_CORE_ENGINE_HPP

// std
#include "string"

// boost
#include "boost/config.hpp"
#include "boost/noncopyable.hpp"

namespace wabby::core
{

  class engine_impl;

  class BOOST_SYMBOL_EXPORT engine : public boost::noncopyable
  {
  public:
    engine( std::string application_name, uint32_t application_version );
    ~engine();

  public:
    void run();

  private:
    engine_impl * impl_;
  };
}  // namespace wabby::core

#endif