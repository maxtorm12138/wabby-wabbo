#ifndef _WABBY_CORE_ENGINE_HPP
#define _WABBY_CORE_ENGINE_HPP

// std
#include "string_view"

// boost
#include "boost/noncopyable.hpp"
#include "boost/config.hpp"

namespace wabby::core
{

class engine_impl;

class BOOST_SYMBOL_EXPORT engine : public boost::noncopyable
{
public:
    engine(std::string_view application_name, uint32_t application_version);
    ~engine();

public:
    void run();

private:
    engine_impl *impl_;
};
}

#endif