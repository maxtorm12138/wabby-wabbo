#ifndef _WAWY_UTIL_LOG_HPP
#define _WAWY_UTIL_LOG_HPP

// module
#include "noncopyable.hpp"

// std
#include "iostream"
#include "ostream"
#include "fstream"
#include "vector"

// fmt
#include "fmt/format.h"

namespace wawy::util
{

enum log_severity
{
	debug = 0,
	info,
	warning,
	error,
	fatal
};


class logger : public wawy::util::noncopyable
{
public:
	void add_console_sink(std::ostream &os = std::cerr);
	void add_file_sink(std::ofstream os);

	template<typename ... Args>
	void add_record(log_severity severity, std::string_view fmt, Args &&... args)
	{
		std::string formated_msg = fmt::format(fmt, std::forward<Args>(args)...);
		do_add_record(severity, formated_msg);
	}

private:
	void do_add_record(log_severity severity, std::string_view msg);

private:
	std::vector<std::unique_ptr<std::ostream>> sinks_holder_;
	std::vector<std::reference_wrapper<std::ostream>> sinks_;
};

}

template<typename ostream>
ostream &operator<<(ostream &os, wawy::util::log_severity severity)
{
	static const char *names[] = 
	{
		"debug",
		"info",
		"warning",
		"error",
		"fatal"
	};

	os << names[severity];
	return os;
}

#define WLOG(LOGGER, SEVERITY, FMT, ...) (LOGGER).add_record(SEVERITY, FMT, __VA_OPT__(,))

#endif