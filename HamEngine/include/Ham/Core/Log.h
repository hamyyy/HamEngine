#pragma once

#include "Ham/Core/Math.h"

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/ringbuffer_sink.h>
#pragma warning(pop)

#include <memory>
#include <stdio.h>

namespace Ham {

class Log {
 public:
  static void Init(std::string name = "APP");
  static void SetPattern(std::string pattern);
  static void SetName(std::string name);

  static std::shared_ptr<spdlog::logger> GetCoreLogger() { return s_CoreLogger; }
  static std::shared_ptr<spdlog::logger> GetClientLogger() { return s_ClientLogger; }
  static std::string GetLastMessage();

 private:
  static std::shared_ptr<spdlog::logger> s_CoreLogger;
  static std::shared_ptr<spdlog::logger> s_ClientLogger;
  static std::string s_ClientPattern;
  static std::string s_Filename;
  static std::shared_ptr<spdlog::sinks::ringbuffer_sink_mt> s_RingBufferSink;
};

}  // namespace Ham

// template <typename OStream, int Dim>
// inline OStream &operator<<(OStream &os, const Ham::math::vec<Dim> &vector)
// {
//     return os << Ham::math::to_string(vector);
// }

// template <typename OStream, int W, int H>
// inline OStream &operator<<(OStream &os, const Ham::math::mat<W, H> &matrix)
// {
//     return os << Ham::math::to_string(matrix);
// }

// template <typename OStream>
// inline OStream &operator<<(OStream &os, const Ham::math::quat &quaternion)
// {
//     return os << Ham::math::to_string(quaternion);
// }

#ifdef HAM_DISABLE_LOGGING

// Core log macros
#define HAM_CORE_TRACE(...)
#define HAM_CORE_INFO(...)
#define HAM_CORE_WARN(...)
#define HAM_CORE_ERROR(...)
#define HAM_CORE_CRITICAL(...)

#define HAM_CORE_TRACE_ONCE(...)
#define HAM_CORE_INFO_ONCE(...)
#define HAM_CORE_WARN_ONCE(...)
#define HAM_CORE_ERROR_ONCE(...)
#define HAM_CORE_CRITICAL_ONCE(...)

#define HAM_CORE_TRACE_REPEATED(...)
#define HAM_CORE_INFO_REPEATED(...)
#define HAM_CORE_WARN_REPEATED(...)
#define HAM_CORE_ERROR_REPEATED(...)
#define HAM_CORE_CRITICAL_REPEATED(...)

// Client log macros
#define HAM_TRACE(...)
#define HAM_INFO(...)
#define HAM_WARN(...)
#define HAM_ERROR(...)
#define HAM_CRITICAL(...)

#define HAM_TRACE_ONCE(...)
#define HAM_INFO_ONCE(...)
#define HAM_WARN_ONCE(...)
#define HAM_ERROR_ONCE(...)
#define HAM_CRITICAL_ONCE(...)

#define HAM_TRACE_REPEATED(...)
#define HAM_INFO_REPEATED(...)
#define HAM_WARN_REPEATED(...)
#define HAM_ERROR_REPEATED(...)
#define HAM_CRITICAL_REPEATED(...)

#else

// Core log macros
#define HAM_CORE_TRACE(...)    ::Ham::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define HAM_CORE_INFO(...)     ::Ham::Log::GetCoreLogger()->info(__VA_ARGS__)
#define HAM_CORE_WARN(...)     ::Ham::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define HAM_CORE_ERROR(...)    ::Ham::Log::GetCoreLogger()->error(__VA_ARGS__)
#define HAM_CORE_CRITICAL(...) ::Ham::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define HAM_TRACE(...)         ::Ham::Log::GetClientLogger()->trace(__VA_ARGS__)
#define HAM_INFO(...)          ::Ham::Log::GetClientLogger()->info(__VA_ARGS__)
#define HAM_WARN(...)          ::Ham::Log::GetClientLogger()->warn(__VA_ARGS__)
#define HAM_ERROR(...)         ::Ham::Log::GetClientLogger()->error(__VA_ARGS__)
#define HAM_CRITICAL(...)      ::Ham::Log::GetClientLogger()->critical(__VA_ARGS__)

#define HAM_INTERNAL_LOG_ONCE_IMPL(type, ...)   \
  {                                             \
    static bool ham_core_info_once_flag = true; \
    if (ham_core_info_once_flag) {              \
      HAM_##type##(__VA_ARGS__);                \
      ham_core_info_once_flag = false;          \
    }                                           \
  }

#define HAM_INTERNAL_LOG_REPEATED_IMPL(type, ...)                                                                                                                                          \
  {                                                                                                                                                                                        \
    static int ham_internal_counter = 1;                                                                                                                                                   \
    std::string ham_internal_current_message = fmt::format(__VA_ARGS__);                                                                                                                   \
    std::string ham_internal_last_message_check = ham_internal_counter == 1 ? ham_internal_current_message : fmt::format("({0}) {1}", ham_internal_counter, ham_internal_current_message); \
    std::string ham_internal_last_message = ::Ham::Log::GetLastMessage();                                                                                                                  \
    if (ham_internal_last_message == ham_internal_last_message_check) {                                                                                                                    \
      /*remove the last line from the log and replace it with the new one */                                                                                                               \
      printf("\033[F\033[K");                                                                                                                                                              \
      HAM_##type##("({0}) {1}", ++ham_internal_counter, ham_internal_current_message);                                                                                                     \
    }                                                                                                                                                                                      \
    else {                                                                                                                                                                                 \
      HAM_##type##(__VA_ARGS__);                                                                                                                                                           \
      ham_internal_counter = 1;                                                                                                                                                            \
    }                                                                                                                                                                                      \
  }

#define HAM_CORE_TRACE_ONCE(...)    HAM_INTERNAL_LOG_ONCE_IMPL(CORE_TRACE, __VA_ARGS__)
#define HAM_CORE_INFO_ONCE(...)     HAM_INTERNAL_LOG_ONCE_IMPL(CORE_INFO, __VA_ARGS__)
#define HAM_CORE_WARN_ONCE(...)     HAM_INTERNAL_LOG_ONCE_IMPL(CORE_WARN, __VA_ARGS__)
#define HAM_CORE_ERROR_ONCE(...)    HAM_INTERNAL_LOG_ONCE_IMPL(CORE_ERROR, __VA_ARGS__)
#define HAM_CORE_CRITICAL_ONCE(...) HAM_INTERNAL_LOG_ONCE_IMPL(CORE_CRITICAL, __VA_ARGS__)

#define HAM_CORE_TRACE_REPEATED(...)    HAM_INTERNAL_LOG_REPEATED_IMPL(CORE_TRACE, __VA_ARGS__)
#define HAM_CORE_INFO_REPEATED(...)     HAM_INTERNAL_LOG_REPEATED_IMPL(CORE_INFO, __VA_ARGS__)
#define HAM_CORE_WARN_REPEATED(...)     HAM_INTERNAL_LOG_REPEATED_IMPL(CORE_WARN, __VA_ARGS__)
#define HAM_CORE_ERROR_REPEATED(...)    HAM_INTERNAL_LOG_REPEATED_IMPL(CORE_ERROR, __VA_ARGS__)
#define HAM_CORE_CRITICAL_REPEATED(...) HAM_INTERNAL_LOG_REPEATED_IMPL(CORE_CRITICAL, __VA_ARGS__)

#define HAM_TRACE_ONCE(...)    HAM_INTERNAL_LOG_ONCE_IMPL(TRACE, __VA_ARGS__)
#define HAM_INFO_ONCE(...)     HAM_INTERNAL_LOG_ONCE_IMPL(INFO, __VA_ARGS__)
#define HAM_WARN_ONCE(...)     HAM_INTERNAL_LOG_ONCE_IMPL(WARN, __VA_ARGS__)
#define HAM_ERROR_ONCE(...)    HAM_INTERNAL_LOG_ONCE_IMPL(ERROR, __VA_ARGS__)
#define HAM_CRITICAL_ONCE(...) HAM_INTERNAL_LOG_ONCE_IMPL(CRITICAL, __VA_ARGS__)

#define HAM_TRACE_REPEATED(...)    HAM_INTERNAL_LOG_REPEATED_IMPL(TRACE, __VA_ARGS__)
#define HAM_INFO_REPEATED(...)     HAM_INTERNAL_LOG_REPEATED_IMPL(INFO, __VA_ARGS__)
#define HAM_WARN_REPEATED(...)     HAM_INTERNAL_LOG_REPEATED_IMPL(WARN, __VA_ARGS__)
#define HAM_ERROR_REPEATED(...)    HAM_INTERNAL_LOG_REPEATED_IMPL(ERROR, __VA_ARGS__)
#define HAM_CRITICAL_REPEATED(...) HAM_INTERNAL_LOG_REPEATED_IMPL(CRITICAL, __VA_ARGS__)

#endif