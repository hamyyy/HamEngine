#pragma once

#include "Ham/Core/Base.h"

#define HAM_USE_PROFILER_NONE   0
#define HAM_USE_PROFILER_OPTICK 1
#define HAM_USE_PROFILER_TRACY  2

#define HAM_ENABLE_PROFILING HAM_USE_PROFILER_TRACY

#if HAM_ENABLE_PROFILING == HAM_USE_PROFILER_OPTICK

#include <optick.config.h>
#include <optick.h>

#define HAM_PROFILE_APP(...)           OPTICK_APP(__VA_ARGS__)
#define HAM_PROFILE_THREAD(...)        OPTICK_THREAD(__VA_ARGS__)
#define HAM_PROFILE_FRAME(...)         OPTICK_FRAME(__VA_ARGS__)
#define HAM_PROFILE_SCOPE_NAMED(...)   OPTICK_EVENT(__VA_ARGS__)
#define HAM_PROFILE_SCOPE(...)         OPTICK_EVENT(__VA_ARGS__)
#define HAM_PROFILE_BEGIN_SESSION(...) OPTICK_START_CAPTURE(__VA_ARGS__)
#define HAM_PROFILE_END_SESSION(...)   OPTICK_STOP_CAPTURE(__VA_ARGS__)
#define HAM_PROFILE_SAVE_SESSION(...)  OPTICK_SAVE_CAPTURE(__VA_ARGS__)

#elif HAM_ENABLE_PROFILING == HAM_USE_PROFILER_TRACY

#include <tracy/Tracy.hpp>

#define HAM_PROFILE_APP(...)         ZoneScopedN(__VA_ARGS__)
#define HAM_PROFILE_THREAD(...)      ZoneScopedN(__VA_ARGS__)
#define HAM_PROFILE_FRAME(...)       FrameMarkNamed(__VA_ARGS__)
#define HAM_PROFILE_SCOPE_NAMED(...) ZoneScopedN(__VA_ARGS__)
#define HAM_PROFILE_SCOPE()          ZoneScoped
#define HAM_PROFILE_BEGIN_SESSION(...)
#define HAM_PROFILE_END_SESSION(...)
#define HAM_PROFILE_SAVE_SESSION(...)

#else

#define HAM_PROFILE_APP(...)
#define HAM_PROFILE_THREAD(...)
#define HAM_PROFILE_FRAME(...)
#define HAM_PROFILE_SCOPE(...)
#define HAM_PROFILE_BEGIN_SESSION(...)
#define HAM_PROFILE_END_SESSION(...)
#define HAM_PROFILE_SAVE_SESSION(...)

#endif