
#ifndef _LIB_SERVICESDB_H_
#define _LIB_SERVICESDB_H_
#include "service.h"

#include <optional>
#include <vector>

class EnergyServiceDB
{
public:

  EnergyServiceDB() = default;

  gridcode::EnergyService
  Create_Service(
      const std::string_view name, const gridcode::EnergyServiceType & type)
  {
    gridcode::EnergyService new_service{name, type};
    m_serivces.push_back(new_service);
    return new_service;
  }

  [[nodiscard]] std::optional<gridcode::EnergyService>
  FindService(std::string_view name, gridcode::EnergyServiceType type) const
  {
    for (auto service : m_serivces) {
      if (service.getServiceName() == name &&
          service.getServiceType() == type) {
        return service;
      }
    }
    return std::nullopt;
  }

private:

  std::vector<gridcode::EnergyService> m_serivces;
};

#endif // _LIB_SERVICESDB_H_
