#pragma once
#include "Ham/Core/Application.h"
#include "Ham/Util/Watcher.h"

extern Ham::Application *Ham::CreateApplication(Ham::ApplicationCommandLineArgs args);

int main(int argc, char **argv)
{
  HAM_PROFILE_BEGIN_SESSION();
  HAM_PROFILE_APP("Ham Engine");
  try {
    Ham::Log::Init();
    Ham::UUIDGenerator::Init();
    auto app = Ham::CreateApplication({argc, argv});
    app->Init();
    app->Run();
    delete app;
  }
  catch (const std::exception &e) {
    HAM_CORE_ERROR("Application Error: {0}", e.what());
    return 1;
  }
  HAM_PROFILE_END_SESSION();
}
