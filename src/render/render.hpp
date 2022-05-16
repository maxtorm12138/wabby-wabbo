#ifndef _WABBY_RENDERER_RENDERER_HPP
#define _WABBY_RENDERER_RENDERER_HPP

// boost
#include "boost/config.hpp"
#include "boost/noncopyable.hpp"

namespace wabby::render
{

class BOOST_SYMBOL_EXPORT backend : boost::noncopyable
{
public:
    virtual ~backend();

public:
    virtual void begin_frame() = 0;

    virtual void end_frame() = 0;

    virtual void begin_render_pass() = 0;

    virtual void end_render_pass() = 0;
};

}

#endif