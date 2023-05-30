#include "Ham/Util/Watcher.h"

namespace Ham
{
    std::unordered_map<std::string, filewatch::FileWatch<std::string>> FileWatcher::s_FileWatchs;

    std::function<void()> FileWatcher::Watch(std::string filePath, std::function<void()> OnFileChanged)
    {

        s_FileWatchs.emplace(filePath, filewatch::FileWatch<std::string>(filePath, [OnFileChanged](const std::string &path, const filewatch::Event change_type)
                                                                         { if (change_type == filewatch::Event::modified){ OnFileChanged();} }));

        return [filePath]()
        {
            for (auto &[key, val] : s_FileWatchs)
                if (key == filePath)
                {
                    s_FileWatchs.erase(key);
                    break;
                }
        };
    }
} // namespace Ham
