#pragma once

#ifdef HZ_PLATFORM_WINDOWS

extern Horizon::Application* Horizon::CreateApplication();

int main(int argc, char** argv)
{
	Horizon::Log::Init();
	HZ_PROFILE_BEGIN_SESSION("Startup", "HorizonProfile-Startup.json");
	auto app = Horizon::CreateApplication();
	HZ_PROFILE_END_SESSION();

	HZ_PROFILE_BEGIN_SESSION("Runtime", "HorizonProfile-Runtime.json");
	app->Run();
	HZ_PROFILE_END_SESSION();

	HZ_PROFILE_BEGIN_SESSION("Shutdown", "HorizonProfile-Shutdown.json");
	delete app;
	HZ_PROFILE_END_SESSION();
}

#endif