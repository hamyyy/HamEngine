#pragma once

#include <FileWatch.hpp>
#include <string>
#include <unordered_map>

namespace Ham
{
    class FileWatcher
    {
    public:
        static std::function<void()> Watch(std::string filePath, std::function<void()> OnFileChanged);

    private:
        static std::unordered_map<std::string, filewatch::FileWatch<std::string>> s_FileWatchs;
    };
} // namespace Ham
