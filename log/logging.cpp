
#include "logging.h"

// variables

logging::sources::severity_channel_logger_mt<boost::log::trivial::severity_level> logger::lg = 
	logging::sources::severity_channel_logger_mt<boost::log::trivial::severity_level>(logging::keywords::channel = "net");
logging::attributes::mutable_constant<std::string> logger::rotation_date = 
	logging::attributes::mutable_constant<std::string>(dt::to_iso_extended_string(
		dt::second_clock::local_time() + dt::hours(4)).substr(0, 10));
boost::shared_ptr<sink_t> logger::sink = nullptr;

bool logger::init(std::string strLogPath, std::string strLogName) {
	// 初始化日志
	logging::add_common_attributes();
	boost::shared_ptr<logging::core > core = logging::core::get();
	core->add_global_attribute("RotationDate", rotation_date);
	boost::shared_ptr<logging::sinks::text_multifile_backend> backend =
		boost::make_shared<logging::sinks::text_multifile_backend>();
	// Set up the file naming pattern
	backend->set_file_name_composer(logging::sinks::file::as_file_name_composer(
		logexpr::stream << strLogPath << "/"
		<< logexpr::attr<logging::trivial::severity_level>("Severity")
		<< "/" << strLogName << "."
		// 文件名还是带上Level，否者copy到一起的时候分不清
		<< logexpr::attr<logging::trivial::severity_level>("Severity") << "."
		<< logexpr::attr<std::string>("RotationDate") << ".log"));
	
	sink = boost::shared_ptr<sink_t>(new sink_t(backend));
	// Set the formatter
	sink->set_formatter(logexpr::stream
		<< "datetime=" << logexpr::format_date_time<boost::posix_time::ptime>("TimeStamp","%Y-%m-%d %H:%M:%S.%f")
		<< logexpr::smessage);
	core->add_sink(sink);
	return true;
}

void logger::update_rotation_time() {
	dt::ptime t = dt::second_clock::local_time();
	if (t.time_of_day().hours() <= 20)
		return;
	t += dt::hours(4); // 以20点为界切割一个交易日，选择20的原因是这样使得所有的数据可以用一天表示，原油也可以
	rotation_date.set(dt::to_iso_extended_string(t).substr(0, 10));
}

void logger::stop() {
	if (sink == nullptr)
		return;
	boost::shared_ptr<logging::core > core = logging::core::get();
	core->remove_sink(sink);
#ifdef USING_ASYNC_LOG_MODE
	sink->stop();
#endif // USING_ASYNC_LOG_MODE
	sink->flush();
	sink.reset();
}
