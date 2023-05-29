#include "Ham/Util/Watcher.h"

namespace Ham
{
    filewatch::FileWatch<std::string> *FileWatcher::s_FileWatch = nullptr;

    void FileWatcher::Watch(std::string filePath, std::function<void()> OnFileChanged)
    {
        s_FileWatch = new filewatch::FileWatch<std::string>(filePath, [OnFileChanged](const std::string &path, const filewatch::Event change_type)
                                                            { if (change_type == filewatch::Event::modified){ OnFileChanged();} });
    }
} // namespace Ham
