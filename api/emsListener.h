
#ifndef _API_EMS_LISTENER_H_
#define _API_EMS_LISTENER_H_
#include "ems.h"
#include "event_system.h"
#include "service.h"

void
HandleServiceRegisteredEvent(const gridcode::EnergyService & service)
{
  PostEmsMessage(
      "Operating mode registered is " + std::string(service.getServiceName()));
}

void
SetupEmsEventHandlers(EventSystem & events_system)
{
  events_system.Subscribe("service_registered", HandleServiceRegisteredEvent);
}

#endif // _API_EMS_LISTENER_H_
