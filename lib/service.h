#ifndef _LIB_SERVICE_H_
#define _LIB_SERVICE_H_

#include <iostream>

namespace gridcode {

// Enum definition
enum class EnergyServiceType
{
  TSO,
  DSO
};

class EnergyService
{
public:

  explicit EnergyService(std::string_view name, EnergyServiceType type)
      : m_name{name}
      , m_serviceType{type}
  {}

  [[nodiscard]] auto
  getServiceType() const
  {
    return m_serviceType;
  }

  [[nodiscard]] auto
  getServiceName() const
  {
    return m_name;
  }

  void
  setServiceName(std::string_view name)
  {
    m_name = name;
  }

  void
  setServiceType(EnergyServiceType type)
  {
    m_serviceType = type;
  }

  static void
  PrintEnergyService(const EnergyService & service)
  {
    std::cout << "Service Name : " << service.getServiceName() << '\n';
    std::cout << "Service Type : " << static_cast<int>(service.getServiceType())
              << '\n';
  }

private:

  std::string_view m_name; // FCR, aFRR, FFR, RST, RSCT
  EnergyServiceType m_serviceType;
};
} // namespace gridcode
#endif //_LIB_SERVICE_H_
