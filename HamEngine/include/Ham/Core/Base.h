#pragma once

#include "Ham/Core/PlatformDetection.h"

#include <memory>

#ifdef HAM_DEBUG
	#if defined(HAM_PLATFORM_WINDOWS)
		#define HAM_DEBUGBREAK() __debugbreak()
	#elif defined(HAM_PLATFORM_LINUX)
		#include <signal.h>
		#define HAM_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
#else
	#define HAM_DEBUGBREAK()
#endif

#define HAM_EXPAND_MACRO(x) x
#define HAM_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#define HAM_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Hazel {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}

#include "Ham/Core/Log.h"
#include "Ham/Core/Assert.h"