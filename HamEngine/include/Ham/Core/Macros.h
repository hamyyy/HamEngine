#pragma once

#ifdef SOURCE_ASSETS_PATH
#ifndef ASSETS_PATH
#define ASSETS_PATH std::string(SOURCE_ASSETS_PATH)
#endif
#else
#ifndef ASSETS_PATH
#define ASSETS_PATH std::string("")
#endif
#endif