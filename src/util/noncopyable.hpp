#pragma once

namespace wawy::util
{
class noncopyable
{
public:
    noncopyable(const noncopyable &other) = delete;
    noncopyable &operator=(const noncopyable &other) = delete; 
    ~noncopyable() = default;
};
}