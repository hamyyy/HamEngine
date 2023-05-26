#pragma once
#include "Ham/Application.h"

#include <iostream>

extern Ham::Application *Ham::CreateApplication(Ham::ApplicationCommandLineArgs args);

int main(int argc, char **argv)
{
	Ham::Log::Init();

	// HZ_PROFILE_BEGIN_SESSION("Startup", "HazelProfile-Startup.json");
	auto app = Ham::CreateApplication({argc, argv});
	// HZ_PROFILE_END_SESSION();

	app->Init();

	// HZ_PROFILE_BEGIN_SESSION("Runtime", "HazelProfile-Runtime.json");
	if (app->Run())
	{
		HAM_CORE_ERROR("Error running Ham Engine Application");
		HAM_CORE_ERROR("Quitting...");
		return 1;
	}
	// HZ_PROFILE_END_SESSION();

	// HZ_PROFILE_BEGIN_SESSION("Shutdown", "HazelProfile-Shutdown.json");
	delete app;
	// HZ_PROFILE_END_SESSION();
}
