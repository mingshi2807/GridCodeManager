
#ifndef API_SERVICESDB_H_
#define API_SERVICESDB_H_
#include "event_system.h"

#include <optional>
#include <vector>

class EnergyServiceDB
{
public:

  EnergyServiceDB() = default;

  EnergyService
  Create_Service(std::string_view name, EnergyServiceType type)
  {
    EnergyService new_service{name, type};
    m_serivces.push_back(new_service);
    return new_service;
  }

  [[nodiscard]] std::optional<EnergyService>
  FindService(std::string_view name, EnergyServiceType type) const
  {
    for (auto service : m_serivces) {
      if (service.getServiceName() == name &&
          service.getServiceType() == type) {
        return service;
      }
    }
    return std::nullopt;
  }

  std::vector<EnergyService> m_serivces;
};

#endif // API_SERVICESDB_H_
