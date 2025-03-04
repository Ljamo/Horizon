#pragma once

#include <memory>

#ifdef HZ_PLATFORM_WINDOWS
#if HZ_DYNAMIC_LINK
	#ifdef HZ_BUILD_DLL
		#define HORIZON_API __declspec(dllexport)
	#else
		#define HORIZON_API __declspec(dllimport)
	#endif //
#else
#define HORIZON_API
#endif
#else
	#error Horizon only supports Windows!
#endif // 

#ifdef HZ_DEBUG
#define HZ_ENABLE_ASSERTS
#endif

#ifdef HZ_ENABLE_ASSERTS
	#define HZ_ASSERT(x, ...) { if(!(x)) { HZ_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define HZ_CORE_ASSERT(x, ...) { if(!(x)) { HZ_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define HZ_ASSERT(x, ...)
	#define HZ_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define HZ_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#define HZ_SET_2D() Horizon::Renderer::SetDimension(Horizon::RendererAPI::Dimension::_2D)
#define HZ_SET_3D() Horizon::Renderer::SetDimension(Horizon::RendererAPI::Dimension::_3D)

namespace Horizon {

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