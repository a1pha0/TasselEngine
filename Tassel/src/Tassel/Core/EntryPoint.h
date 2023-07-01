#pragma once

#ifdef TASSEL_PLATFORM_WINDOWS

extern Tassel::Application* Tassel::CreateApplication();

int main(int argc, char** argv)
{
	Tassel::Log::Init();

	TASSEL_CORE_INFO("Hello, Tassel Engine!");

	auto app = Tassel::CreateApplication();
	app->Run();
	delete app;
	return 0;
}

#endif
