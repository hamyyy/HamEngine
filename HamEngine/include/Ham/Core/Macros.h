#pragma once

#ifdef SOURCE_ASSETS_PATH
#define ASSETS_PATH std::string(SOURCE_ASSETS_PATH)
#else
#define ASSETS_PATH std::string("assets/")
#endif