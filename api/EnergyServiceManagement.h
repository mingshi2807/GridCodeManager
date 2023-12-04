
#ifndef API_ENERGYSERVICES_MANAGEMENT_H_
#define API_ENERGYSERVICES_MANAGEMENT_H_
#include "ServicesDB.h"
#include "event_system.h"

class EnergyServiceManagement
{
public:

  explicit EnergyServiceManagement(
      EnergyServiceDB & service_db, EventSystem & event_system)
      : m_database{service_db}
      , m_event_system{event_system}
  {}

  void
  RegisterNewService(std::string_view name, EnergyServiceType type)
  {
    EnergyService a_service = m_database.Create_Service(name, type);

    m_event_system.PostEvent("An new energy service registered", a_service);
  }

  void
  ResetServiceTypeToDefault(std::string_view name, EnergyServiceType type)
  {
    const auto found_service = m_database.FindService(name, type);
    if (found_service.has_value()) {
      EnergyService a_service = found_service.value();
      a_service.setServiceName("FCR");
      a_service.setServiceType(EnergyServiceType::TSO);

      m_event_system.PostEvent("Energy service resetted", a_service);
    }
  }

private:

  EnergyServiceDB m_database;
  EventSystem m_event_system;
};
#endif // API_ENERGYSERVICES_MANAGEMENT_H_
