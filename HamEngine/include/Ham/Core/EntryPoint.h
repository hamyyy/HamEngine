#pragma once
#include "Ham/Core/Application.h"

#include <iostream>

extern Ham::Application *Ham::CreateApplication(Ham::ApplicationCommandLineArgs args);

int main(int argc, char **argv)
{
	HAM_PROFILE_BEGIN_SESSION();
	HAM_PROFILE_APP("Ham Engine");
	Ham::Log::Init();
	auto app = Ham::CreateApplication({argc, argv});
	app->Init();
	if (app->Run())
	{
		HAM_CORE_ERROR("Error running Ham Engine Application");
		HAM_CORE_ERROR("Quitting...");
		return 1;
	}
	delete app;
	HAM_PROFILE_END_SESSION();
}
