#pragma once

#include <Ham/Core/Log.h>

#include <FileWatcher/FileWatcher.h>

#include <string>
#include <functional>
#include <unordered_map>

namespace Ham {
struct FileListenerData;
class FileListener : public FW::FileWatchListener {
 public:
  void handleFileAction(FW::WatchID watchid, const FW::String& dir, const FW::String& filename, FW::Action action);

 private:
  std::unordered_map<std::string, FileListenerData> m_WatcherList;
  friend class FileWatcher;
};

class FileWatcher {
 public:
  static void Init()
  {
    s_FileListener = new FileListener();
  }
  static void Shutdown()
  {
    delete s_FileListener;
  }

  static void Update();
  static void Watch(std::string filePath, std::function<void()> OnFileChanged);
  static void Unwatch(std::string filePath);

 private:
  static FileListener* s_FileListener;
};

struct FileListenerData {
  std::function<void()> OnFileChanged;
  std::shared_ptr<FW::FileWatcher> fileWatcher;
};

}  // namespace Ham