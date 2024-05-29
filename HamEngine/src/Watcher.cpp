#include "Ham/Util/Watcher.h"
#include "Ham/Util/File.h"

#include <filesystem>

namespace Ham {

FileListener* FileWatcher::s_FileListener = nullptr;

// FileWatcher::FileWatcher() {}

void FileListener::handleFileAction(FW::WatchID watchid, const FW::String& dir, const FW::String& filename, FW::Action action)
{
  if (action != FW::Action::Modified) return;
  auto filePath = dir + (char)std::filesystem::path::preferred_separator + filename;
  HAM_CORE_TRACE("File {0} changed", filePath);
  if (m_WatcherList.find(filePath) != m_WatcherList.end()) {
    m_WatcherList[filePath].OnFileChanged();
  }
}

void FileWatcher::Update()
{
  for (auto& [filePath, data] : s_FileListener->m_WatcherList) {
    data.fileWatcher->update();
  }
}

void FileWatcher::Watch(std::string filePath, std::function<void()> OnFileChanged)
{
  std::filesystem::path path(filePath);
  path.make_preferred();
  filePath = path.string();

  HAM_CORE_TRACE("Watching file {0}", filePath);

  if (std::filesystem::is_directory(path)) {
    HAM_CORE_ERROR("Cannot watch directory {0}. Must specify a file", filePath);
    return;
  }

  if (s_FileListener->m_WatcherList.find(filePath) != s_FileListener->m_WatcherList.end()) {
    HAM_CORE_WARN("File {0} is already being watched", filePath);
    return;
  }

  auto dirPath = path.parent_path().string();

  std::shared_ptr<FW::FileWatcher> watcher;
  for (auto& [watchedPath, data] : s_FileListener->m_WatcherList) {
    if (watchedPath.find(dirPath) != std::string::npos) {
      watcher = data.fileWatcher;
      break;
    }
  }

  if (!watcher) {
    watcher = std::make_shared<FW::FileWatcher>();
    try {
      watcher->addWatch(dirPath, s_FileListener, true);
    }
    catch (std::exception& e) {
      HAM_CORE_ERROR("Error watching directory: {0}", e.what());
      return;
    }
  }

  s_FileListener->m_WatcherList[filePath] = FileListenerData{std::move(OnFileChanged), watcher};
}

void FileWatcher::Unwatch(std::string filePath)
{
  std::filesystem::path path(filePath);
  path.make_preferred();
  filePath = path.string();

  HAM_CORE_TRACE("Unwatching file {0}", filePath);
  if (s_FileListener->m_WatcherList.find(filePath) == s_FileListener->m_WatcherList.end()) {
    HAM_CORE_WARN("File {0} is not being watched", filePath);
    return;
  }
  s_FileListener->m_WatcherList.erase(filePath);
}

}  // namespace Ham