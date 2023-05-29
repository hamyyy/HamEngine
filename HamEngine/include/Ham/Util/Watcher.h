#pragma once

#include <FileWatch.hpp>
#include <string>

namespace Ham
{
    class FileWatcher
    {
    public:
        static void Watch(std::string filePath, std::function<void()> OnFileChanged);

    private:
        static filewatch::FileWatch<std::string> *s_FileWatch;
    };
} // namespace Ham
