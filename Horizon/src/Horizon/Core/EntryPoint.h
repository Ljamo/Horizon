#pragma once

#ifdef HZ_PLATFORM_WINDOWS

extern Horizon::Application* Horizon::CreateApplication();

int main(int argc, char** argv)
{
	Horizon::Log::Init();
	HZ_CORE_WARN("Initializeed CORE LOG!");
	int a = 5;
	HZ_INFO("HZ INFO LOG! Var={0}", a);

	auto app = Horizon::CreateApplication();
	app->Run();
	delete app;
}

#endif