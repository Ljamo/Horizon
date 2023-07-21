#pragma once

#ifdef HZ_PLATFORM_WINDOWS

extern Horizon::Application* Horizon::CreateApplication();

int main(int argc, char** argv)
{
	Horizon::Log::Init();
	HZ_CORE_WARN("Initialized CORE LOG!");

	auto app = Horizon::CreateApplication();
	app->Run();
	delete app;
}

#endif