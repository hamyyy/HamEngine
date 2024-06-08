#include "Ham/Core/Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/pattern_formatter.h>

#include <memory>

namespace Ham {

std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
std::shared_ptr<spdlog::logger> Log::s_ClientLogger;
std::string Log::s_ClientPattern = "%^[%T] %n: %v%$";
std::string Log::s_Filename = "Ham.log";
std::shared_ptr<spdlog::sinks::ringbuffer_sink_mt> Log::s_RingBufferSink = nullptr;

void Log::Init(std::string name)
{
  std::vector<spdlog::sink_ptr> coreLogSinks;
  coreLogSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
  coreLogSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(s_Filename, true));
  s_RingBufferSink = std::make_shared<spdlog::sinks::ringbuffer_sink_mt>(1);
  coreLogSinks.push_back(s_RingBufferSink);

  coreLogSinks[0]->set_pattern("%^[%T] %n: %v%$");
  coreLogSinks[0]->set_level(spdlog::level::debug);

  coreLogSinks[1]->set_pattern("[%T] [%l] %n: %v");
  coreLogSinks[1]->set_level(spdlog::level::trace);
  coreLogSinks[2]->set_pattern("%v");

  s_CoreLogger = std::make_shared<spdlog::logger>("HAM", begin(coreLogSinks), end(coreLogSinks));
  spdlog::register_logger(s_CoreLogger);
  s_CoreLogger->set_level(spdlog::level::trace);
  s_CoreLogger->flush_on(spdlog::level::trace);

  std::vector<spdlog::sink_ptr> clientLogSinks;
  clientLogSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
  clientLogSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(s_Filename, true));
  clientLogSinks.push_back(s_RingBufferSink);

  clientLogSinks[0]->set_pattern("%^[%T] %n: %v%$");
  clientLogSinks[0]->set_level(spdlog::level::debug);
  clientLogSinks[1]->set_pattern(s_ClientPattern);
  clientLogSinks[1]->set_level(spdlog::level::trace);

  s_ClientLogger = std::make_shared<spdlog::logger>(name, begin(clientLogSinks), end(clientLogSinks));
  spdlog::register_logger(s_ClientLogger);
  s_ClientLogger->set_level(spdlog::level::trace);
  s_ClientLogger->flush_on(spdlog::level::trace);
}

void Log::SetPattern(std::string pattern)
{
  s_ClientLogger->sinks()[0]->set_pattern(pattern);
  s_ClientPattern = pattern;
}

void Log::SetName(std::string name)
{
  if (s_ClientLogger)
    s_ClientLogger.reset();

  std::vector<spdlog::sink_ptr> clientLogSinks;
  clientLogSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
  clientLogSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(s_Filename, true));
  clientLogSinks[0]->set_pattern("%^[%T] %n: %v%$");
  clientLogSinks[1]->set_pattern(s_ClientPattern);

  s_ClientLogger = std::make_shared<spdlog::logger>(name, begin(clientLogSinks), end(clientLogSinks));
  spdlog::register_logger(s_ClientLogger);
  s_ClientLogger->set_level(spdlog::level::trace);
  s_ClientLogger->flush_on(spdlog::level::trace);
}

std::string Log::GetLastMessage()
{
  auto s = s_RingBufferSink->last_formatted(1)[0];
  // remove last instance of SPDLOG_EOL (newline)
  s.erase(s.find_last_not_of(SPDLOG_EOL) + 1);
  return s;
}

}  // namespace Ham
