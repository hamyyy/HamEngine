#pragma once

#include <whereami.h>
#include <filesystem>

#include <string>

namespace Ham::FileSystem
{
    static std::filesystem::path GetExecutablePath()
    {
        static int length = -1;
        static std::string pathStr = "";
        static std::filesystem::path path;

        if (length == -1)
        {
            length = wai_getExecutablePath(nullptr, 0, nullptr);
            pathStr = std::string(length, '\0');
            wai_getExecutablePath((char *)pathStr.c_str(), length, nullptr);

            path = std::filesystem::path(pathStr).make_preferred();
        }

        return path;
    }

    static std::filesystem::path GetExecutableDir()
    {
        static std::string pathStr = "";
        static std::filesystem::path path;

        if (pathStr == "")
        {
            pathStr = GetExecutablePath().string();
            pathStr = pathStr.substr(0, pathStr.find_last_of(std::filesystem::path::preferred_separator));

            path = std::filesystem::path(pathStr).make_preferred();
        }

        return path;
    }

    static std::filesystem::path GetAssetsDir()
    {
        static std::string pathStr = "";
        static std::filesystem::path path;

        if (pathStr == "")
        {
            pathStr = GetExecutableDir().string();
            pathStr += "/assets";

            path = std::filesystem::path(pathStr).make_preferred();
        }

        return path;
    }

} // namespace Ham

#ifdef SOURCE_ASSETS_PATH
#define ASSETS_PATH std::string(SOURCE_ASSETS_PATH)
#else
#define ASSETS_PATH Ham::FileSystem::GetAssetsDir().string()
#endif

#ifdef SOURCE_ASSETS_PATH_CORE
#define ASSETS_PATH_CORE std::string(SOURCE_ASSETS_PATH_CORE)
#else
#define ASSETS_PATH_CORE Ham::FileSystem::GetAssetsDir().string()
#endif