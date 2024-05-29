#pragma once

#include "Ham/Core/Math.h"

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#pragma warning(pop)

#include <memory>

namespace Ham
{

    class Log
    {
    public:
        static void Init(std::string name = "APP");
        static void SetPattern(std::string pattern);
        static void SetName(std::string name);

        static std::shared_ptr<spdlog::logger> GetCoreLogger() { return s_CoreLogger; }
        static std::shared_ptr<spdlog::logger> GetClientLogger() { return s_ClientLogger; }

    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
        static std::string s_ClientPattern;
        static std::string s_Filename;
    };

}

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

// Client log macros
#define HAM_TRACE(...)
#define HAM_INFO(...)
#define HAM_WARN(...)
#define HAM_ERROR(...)
#define HAM_CRITICAL(...)

#else

// Core log macros
#define HAM_CORE_TRACE(...) ::Ham::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define HAM_CORE_INFO(...) ::Ham::Log::GetCoreLogger()->info(__VA_ARGS__)
#define HAM_CORE_WARN(...) ::Ham::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define HAM_CORE_ERROR(...) ::Ham::Log::GetCoreLogger()->error(__VA_ARGS__)
#define HAM_CORE_CRITICAL(...) ::Ham::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define HAM_TRACE(...) ::Ham::Log::GetClientLogger()->trace(__VA_ARGS__)
#define HAM_INFO(...) ::Ham::Log::GetClientLogger()->info(__VA_ARGS__)
#define HAM_WARN(...) ::Ham::Log::GetClientLogger()->warn(__VA_ARGS__)
#define HAM_ERROR(...) ::Ham::Log::GetClientLogger()->error(__VA_ARGS__)
#define HAM_CRITICAL(...) ::Ham::Log::GetClientLogger()->critical(__VA_ARGS__)

#endif