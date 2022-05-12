#ifndef _WAWY_CORE_ENGINE_HPP
#define _WAWY_CORE_ENGINE_HPP

// module
#include "util/noncopyable.hpp"
#include "util/api.hpp"

// std
#include "string_view"

namespace wabby::core
{

class engine_impl;

class WAWY_API_EXPORT engine : public wabby::util::noncopyable
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