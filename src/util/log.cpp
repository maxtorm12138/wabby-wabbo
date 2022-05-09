#include "log.hpp"

namespace wawy::util
{

std::string_view serverity_name(log_severity severity)
{
	static const char *names[] = 
	{
		"debug",
		"info",
		"warning",
		"error",
		"fatal"
	};
	return names[severity];
}

void logger::add_console_sink(std::ostream &os)
{
	sinks_.emplace_back(os);
}

std::ofstream &logger::add_file_sink(std::ofstream os)
{
	auto p_os = std::make_unique<std::ofstream>(std::move(os));
	sinks_.emplace_back(*sinks_holder_.emplace_back(std::move(p_os)));
	return static_cast<std::ofstream &>(sinks_.back().get());
}

std::ostream &logger::add_sink(std::unique_ptr<std::ostream> os)
{
	sinks_.emplace_back(*sinks_holder_.emplace_back(std::move(os)));
	return static_cast<std::ostream &>(sinks_.back().get());
}

void logger::add_record(log_severity severity, std::string_view message)
{
	for (auto sink : sinks_)
	{
		sink.get() << fmt::format("<{}> {}\n", serverity_name(severity), message);
	}
}

}