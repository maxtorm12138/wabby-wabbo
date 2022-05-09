#include "log.hpp"

namespace wawy::util
{

void logger::add_console_sink(std::ostream &os)
{
	sinks_.emplace_back(os);
}

void logger::add_file_sink(std::ofstream os)
{
	auto p_os = std::make_unique<std::ofstream>(std::move(os));
	sinks_.emplace_back(*sinks_holder_.emplace_back(std::move(p_os)));
}

void logger::do_add_record(log_severity severity, std::string_view msg)
{
	for (auto sink : sinks_)
	{
		sink.get() << "<" << severity << "> " << msg << "\n";
	}
}

}