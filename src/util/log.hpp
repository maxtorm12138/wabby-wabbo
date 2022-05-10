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
namespace log_severity
{
enum severity
{
	debug = 0,
	info,
	warning,
	error,
	fatal
};
}

class logger : public wawy::util::noncopyable
{
public:
	logger() = default;
	logger(logger &&other) noexcept;
	logger &operator=(logger &&other) noexcept;
	~logger() = default;

public:
	void add_console_sink(std::ostream &os = std::cerr);
	std::ofstream &add_file_sink(std::ofstream os);
	std::ostream &add_sink(std::unique_ptr<std::ostream> os);

	void add_record(log_severity::severity severity, std::string_view message);
	
private:
	std::vector<std::unique_ptr<std::ostream>> sinks_holder_;
	std::vector<std::reference_wrapper<std::ostream>> sinks_;
};

}


#define WLOG(LOGGER, SEVERITY, FMT, ...) \
	do\
	{\
		std::string formated_message = fmt::format(FMT, __VA_ARGS__);\
		LOGGER.add_record(SEVERITY, formated_message);\
	}\
	while(false)

#endif