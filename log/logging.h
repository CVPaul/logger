#ifndef __LOGGING_H__
#define __LOGGING_H__

// #define USING_ASYNC_LOG_MODE // using sync log sink for debug reason

#include <string>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/sinks/async_frontend.hpp>
#include <boost/log/sinks/text_multifile_backend.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>

namespace dt = boost::posix_time;
namespace logging = boost::log;
namespace logexpr = boost::log::expressions;

#ifdef USING_ASYNC_LOG_MODE
typedef logging::sinks::asynchronous_sink<logging::sinks::text_multifile_backend,
	logging::sinks::unbounded_fifo_queue> sink_t;
#else
typedef logging::sinks::synchronous_sink<logging::sinks::text_multifile_backend> sink_t;
#endif // USING_ASYNC_LOG_MODE

class logger {
private:
	logger() {};
public:
	static logging::sources::severity_channel_logger_mt<boost::log::trivial::severity_level> lg;
	static boost::shared_ptr<sink_t> sink;
	static logging::attributes::mutable_constant<std::string> rotation_date;
public:
	// functions
	static bool init(std::string strLogPath, std::string strLogName);
	static void stop();
	static void update_rotation_time();
};

#define LOG_EXTRA_INFO "||scope=" << __FUNCTION__ << "@" << __FILE__ << ":"  << __LINE__ << "||"

#define logger_trace	BOOST_LOG_SEV(logger::lg,logging::trivial::severity_level::trace) << LOG_EXTRA_INFO
#define logger_debug	BOOST_LOG_SEV(logger::lg,logging::trivial::severity_level::debug) << LOG_EXTRA_INFO
#define logger_info		BOOST_LOG_SEV(logger::lg,logging::trivial::severity_level::info) << LOG_EXTRA_INFO
#define logger_warning	BOOST_LOG_SEV(logger::lg,logging::trivial::severity_level::warning) << LOG_EXTRA_INFO
#define logger_error	BOOST_LOG_SEV(logger::lg,logging::trivial::severity_level::error) << LOG_EXTRA_INFO
#define logger_fatal	BOOST_LOG_SEV(logger::lg,logging::trivial::severity_level::fatal) << LOG_EXTRA_INFO

#endif // __LOGGING_H__