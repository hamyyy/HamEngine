#pragma once

#include "Ham/Util/ConcurrentQueue.h"
#include "Ham/Core/Assert.h"

#include <functional>
#include <memory>
#include <queue>
#include <string>
#include <type_traits>

namespace Ham::Events {

class Event {
 public:
  virtual const char* GetName() const = 0;
  bool Handled = false;

  template <typename T>
  bool Is() const
  {
    return typeid(T) == typeid(*this);
  }

  template <typename T>
  const T& As() const
  {
    HAM_CORE_ASSERT((Is<T>()), fmt::format("Event::As: Event is not of type {}", typeid(T).name()));
    return *static_cast<const T*>(this);
  }
};

class EventHandler {
 public:
  EventHandler() = default;
  EventHandler(std::shared_ptr<Event> event) : m_Event(event) {}

  void SetEvent(std::shared_ptr<Event> event) { m_Event = event; }

  template <typename T>
  bool Dispatch(const std::function<bool(T&)>& func)
  {
    if (!m_Event->Handled && typeid(*m_Event) == typeid(T)) {
      m_Event->Handled = func(static_cast<T&>(*m_Event));
    }
    return m_Event->Handled;
  }

 private:
  std::shared_ptr<Event> m_Event = nullptr;
};

class SubscriberBase : public std::enable_shared_from_this<SubscriberBase> {
 public:
  virtual ~SubscriberBase() = default;
  virtual bool Dispatch(std::shared_ptr<Event> event) = 0;
};

template <typename T>
class Subscriber : public SubscriberBase {
 public:
  Subscriber(const std::function<void(const T&)>& func) : m_Func(func) {}

  bool Dispatch(std::shared_ptr<Event> event)
  {
    if (!event->Handled && typeid(*event) == typeid(T)) {
      m_Func(static_cast<T&>(*event));
    }
    return event->Handled;
  }

 private:
  friend class SubscriberPool;
  std::function<void(const T&)> m_Func;
};

class SubscriberPool {
 public:
  template <typename T>
  std::shared_ptr<Events::Subscriber<T>> Add(const std::function<void(const T&)>& func)
  {
    auto s = std::make_shared<Subscriber<T>>(func);
    m_Subscribers.push_back(std::weak_ptr<Subscriber<T>>(s));
    HAM_CORE_TRACE("Added subscriber to pool. Total: {}", m_Subscribers.size());
    return s;
  }

  void Remove(std::shared_ptr<SubscriberBase> subscriber)
  {
    auto it = std::find_if(m_Subscribers.begin(), m_Subscribers.end(),
                           [&subscriber](const std::weak_ptr<SubscriberBase>& weakSubscriber) {
                             if (auto locked = weakSubscriber.lock()) {
                               return locked == subscriber;
                             }
                             return false;
                           });
    if (it != m_Subscribers.end()) {
      m_Subscribers.erase(it);
    }
  }

  void Dispatch(std::shared_ptr<Event> event)
  {
    std::vector<std::weak_ptr<SubscriberBase>> validSubscribers;

    for (auto& weakSubscriber : m_Subscribers) {
      if (auto subscriber = weakSubscriber.lock()) {
        subscriber->Dispatch(event);
        validSubscribers.push_back(weakSubscriber);
      }
    }

    m_Subscribers.swap(validSubscribers);
    HAM_CORE_TRACE_IF(m_Subscribers.size() != validSubscribers.size(), "Cleaned up {} subscriber(s). Total: {}", std::abs((int)(m_Subscribers.size() - validSubscribers.size())), m_Subscribers.size());
  }

  void Cleanup()
  {
    m_Subscribers.erase(
        std::remove_if(m_Subscribers.begin(), m_Subscribers.end(),
                       [](const std::weak_ptr<SubscriberBase>& weakSubscriber) {
                         return weakSubscriber.expired();
                       }),
        m_Subscribers.end());
  }

 private:
  std::vector<std::weak_ptr<SubscriberBase>> m_Subscribers;
};

class EventQueue {
 public:
  template <typename T, typename... Args>
  void Emit(Args&&... args)
  {
    m_Queue.enqueue(std::make_shared<T>(std::forward<Args>(args)...));
  }

  std::shared_ptr<Event> PollEvent()
  {
    std::shared_ptr<Event> event;
    bool found = m_Queue.try_dequeue(event);
    if (!found) return nullptr;
    return event;
  }

  // Returns the approximate size of the queue
  size_t Size() const { return m_Queue.size_approx(); }

 private:
  moodycamel::ConcurrentQueue<std::shared_ptr<Event>> m_Queue;
};

extern EventQueue g_GlobalEventQueue;

template <typename T, typename... Args>
inline static void Emit(Args&&... args)
{
  g_GlobalEventQueue.Emit<T>(std::forward<Args>(args)...);
}

/*
 * Polls the next event from the global event queue
 * @return The next event in the queue or nullptr if the queue is empty
 * @note This function is thread-safe
 */
inline static std::shared_ptr<Event> PollEvent()
{
  return std::move(g_GlobalEventQueue.PollEvent());
}

inline static size_t QueueSize()
{
  return g_GlobalEventQueue.Size();
}

}  // namespace Ham::Events

namespace Ham {
typedef std::shared_ptr<Events::SubscriberBase> Subscriber;
}  // namespace Ham
