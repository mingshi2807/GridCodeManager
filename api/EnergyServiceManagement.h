
#ifndef API_ENERGYSERVICES_MANAGEMENT_H_
#define API_ENERGYSERVICES_MANAGEMENT_H_
#include "ServicesDB.h"
#include "event_system.h"
#include "service.h"

class EnergyServiceManagement
{
public:

  explicit EnergyServiceManagement(
      EnergyServiceDB & service_db, EventSystem & event_system)
      : m_database{service_db}
      , m_event_system{event_system}
  {}

  void
  RegisterNewService(
      const std::string_view name, const gridcode::EnergyServiceType & type)
  {
    gridcode::EnergyService e_service = m_database.Create_Service(name, type);

    m_event_system.PostEvent("service_registered", e_service);
  }

  void
  ResetServiceTypeToDefault(
      std::string_view name, gridcode::EnergyServiceType type)
  {
    const auto found_service = m_database.FindService(name, type);
    if (found_service.has_value()) {
      gridcode::EnergyService a_service = found_service.value();
      a_service.setServiceName("FCR");
      a_service.setServiceType(gridcode::EnergyServiceType::TSO);

      m_event_system.PostEvent("Energy service resetted", a_service);
    }
  }

private:

  EnergyServiceDB m_database;
  EventSystem m_event_system;
};
#endif // API_ENERGYSERVICES_MANAGEMENT_H_
