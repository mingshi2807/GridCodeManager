
#ifndef API_LOG_LISTENER_H_
#define API_LOG_LISTENER_H_
#include "event_system.h"
#include "log.h"
#include "service.h"

#include <type_traits>

template <typename T>
constexpr auto
toUnderlying(T && t) noexcept
{
  return static_cast<std::underlying_type_t<T>>(std::forward<T>(t));
}

template <typename E>
auto
toEnumString(const E & e) -> std::string
{
  switch (e) {
    case E::TSO:
      return "TSO";
    case E::DSO:
      return "DSO";
    default:
      return "Unknown";
  }
}

void
HandleServiceRegisteredEventLog(const gridcode::EnergyService & service)
{
  Log_display(
      "Service has registered with name " +
      std::string(service.getServiceName()) + " and with type " +
      toEnumString(service.getServiceType()));
  /* std::to_string(toUnderlying(service.getServiceType()))); */
}

void
SetupLogEventHandlers(EventSystem & events_system)
{
  events_system.Subscribe(
      "service_registered", HandleServiceRegisteredEventLog);
}
#endif // API_LOG_LISTENER_H_
