#ifndef _API_EVENT_SYSTEM_H_
#define _API_EVENT_SYSTEM_H_
#include "service.h"
#include "spdlog/spdlog.h"

#include <functional>
#include <map>
#include <string>
#include <string_view>
#include <vector>

class EventSystem
{
public:

  void
  Subscribe(
      std::string_view event_type,
      const std::function<void(const gridcode::EnergyService &)> & function)
  {
    m_subscribers[event_type.data()].push_back(function);
  }

  void
  PostEvent(
      std::string_view event_type,
      const gridcode::EnergyService & energyService)
  {
    for (const auto & callable : m_subscribers[event_type.data()]) {
      callable(energyService);
    }
  }

  [[maybe_unused]] void
  ListSubscribers()
  {
    for (const auto & [event, fun] : m_subscribers) {
      spdlog::info(
          "EventSystem type: " + event + " subscribers " +
          std::to_string(m_subscribers[event].size()));
    }
  }

private:

  std::map<
      std::string,
      std::vector<std::function<void(const gridcode::EnergyService &)>>>
      m_subscribers;
};
#endif // _API_EVENT_SYSTEM_H_
