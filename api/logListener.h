
#ifndef API_LOG_LISTENER_H_
#define API_LOG_LISTENER_H_
#include "event_system.h"
#include "spdlog/spdlog.h"

#include <string>
#include <string_view>


static void
Log_display(std::string_view message);

void
Log_display(std::string_view message)
{
  // todos add fake applicative command
  spdlog::info(std::string("[Log]: ") + std::string(message.data()));
}

void
HandleServiceRegisteredEventLog(const EnergyService & service)
{
  Log_display(
      "Service has registered with name " +
      std::string(service.getServiceName()) + " and with type " +
      std::to_string(static_cast<int>(service.getServiceType())));
}

void
SetupLogEventHandlers(EventSystem & events_system)
{
  events_system.Subscribe(
      "A energy service registered", HandleServiceRegisteredEventLog);
}
#endif // API_LOG_LISTENER_H_
