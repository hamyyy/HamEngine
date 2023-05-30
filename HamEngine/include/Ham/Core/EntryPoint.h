#pragma once
#include "Ham/Core/Application.h"

extern Ham::Application *Ham::CreateApplication(Ham::ApplicationCommandLineArgs args);

int main(int argc, char **argv)
{
	HAM_PROFILE_BEGIN_SESSION();
	HAM_PROFILE_APP("Ham Engine");
	try
	{
		Ham::Log::Init();
		Ham::UUIDGenerator::Init();
		auto app = Ham::CreateApplication({argc, argv});
		app->Init();
		if (app->Run())
		{
			HAM_CORE_ERROR("Error running Ham Engine Application");
			HAM_CORE_ERROR("Quitting...");
			return 1;
		}
		delete app;
	}
	catch (const std::exception &e)
	{
		HAM_CORE_ERROR("Error running Ham Engine Application: {0}", e.what());
		HAM_CORE_ERROR("Quitting...");
		return 1;
	}
	HAM_PROFILE_END_SESSION();
}
