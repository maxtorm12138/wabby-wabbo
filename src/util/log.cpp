#include "log.hpp"

// boost
#include "boost/date_time.hpp"

namespace wabby::util
{

std::string_view serverity_name(log_severity::severity severity)
{
	static const char *names[] = 
	{
		"debug",
		"info",
		"warning",
		"error",
		"fatal"
	};
	return names[static_cast<int>(severity)];
}

logger::logger(logger &&other) noexcept :
	sinks_holder_(std::move(other.sinks_holder_)),
	sinks_(std::move(other.sinks_))
{
}

logger &logger::operator=(logger &&other) noexcept
{
	sinks_holder_ = std::move(other.sinks_holder_);
	sinks_ = std::move(other.sinks_);
	return *this;
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

void logger::add_record(log_severity::severity severity, std::string_view message)
{
	auto now = boost::posix_time::to_iso_extended_string(boost::posix_time::microsec_clock::local_time());
	auto real_message = fmt::format("<{}|{}> {}\n", now, serverity_name(severity), message);
	for (auto sink : sinks_)
	{
		sink.get() << real_message;
	}
}

}