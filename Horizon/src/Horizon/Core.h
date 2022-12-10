#pragma once

#ifdef HZ_PLATFORM_WINDOWS
	#ifdef HZ_BUILD_DLL
		#define HORIZON_API __declspec(dllexport)
	#else
		#define HORIZON_API __declspec(dllimport)
	#endif //
#else
	#error Horizon only supports Windows!
#endif // 
