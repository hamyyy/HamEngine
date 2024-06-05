#pragma once

#include "Ham/Util/ConcurrentQueue.h"

#include <functional>
#include <memory>
#include <queue>
#include <string>

namespace Ham::Events {

class Event {
 public:
  virtual const char* GetName() const = 0;
  bool Handled = false;
};

class EventHandler {
 public:
  EventHandler(std::shared_ptr<Event> event) : m_Event(event) {}

  template <typename T>
  bool Dispatch(const std::function<bool(T&)>& func)
  {
    if (!m_Event->Handled && typeid(*m_Event) == typeid(T)) {
      m_Event->Handled = func(static_cast<T&>(*m_Event));
    }
    return m_Event->Handled;
  }

 private:
  std::shared_ptr<Event> m_Event;
};

class EventQueue {
 public:
  template <typename T, typename... Args>
  void PushEvent(Args&&... args)
  {
    m_Queue.enqueue(std::make_shared<T>(std::forward<Args>(args)...));
  }

  std::shared_ptr<Event> PollEvent()
  {
    std::shared_ptr<Event> event;
    bool found = m_Queue.try_dequeue(event);
    if (!found) {
      throw std::runtime_error("No events in queue");
    }
    return event;
  }

  // Returns the approximate size of the queue
  size_t Size() const { return m_Queue.size_approx(); }

 private:
  moodycamel::ConcurrentQueue<std::shared_ptr<Event>> m_Queue;
};

extern EventQueue g_GlobalEventQueue;

template <typename T, typename... Args>
inline static void PushEvent(Args&&... args)
{
  g_GlobalEventQueue.PushEvent<T>(std::forward<Args>(args)...);
}

inline static std::shared_ptr<Event> PollEvent()
{
  return std::move(g_GlobalEventQueue.PollEvent());
}

inline static size_t QueueSize()
{
  return g_GlobalEventQueue.Size();
}

}  // namespace Ham::Events
