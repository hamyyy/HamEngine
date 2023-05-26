#pragma once
#include "Ham/Application.h"

#include <iostream>

extern Ham::Application* Ham::CreateApplication();

int main(int argc, char** argv)
{
	// Hazel::Log::Init();

	// HZ_PROFILE_BEGIN_SESSION("Startup", "HazelProfile-Startup.json");
	auto app = Ham::CreateApplication(); // { argc, argv };
	// HZ_PROFILE_END_SESSION();

	// HZ_PROFILE_BEGIN_SESSION("Runtime", "HazelProfile-Runtime.json");
	if (app->Run())
	{
		std::cout << "Error setting up Ham Application" << std::endl;
		return 1;
	}
	// HZ_PROFILE_END_SESSION();

	// HZ_PROFILE_BEGIN_SESSION("Shutdown", "HazelProfile-Shutdown.json");
	delete app;
	// HZ_PROFILE_END_SESSION();
}
