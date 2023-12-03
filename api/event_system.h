#ifndef API_EVENT_SYSTEM_H_
#define API_EVENT_SYSTEM_H_
#include "spdlog/spdlog.h"

#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <string_view>
#include <vector>

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

class EventSystem
{
public:

  void
  Subscribe(
      std::string_view event_type,
      const std::function<void(const EnergyService &)> & function)
  {
    subscribers_[event_type.data()].push_back(function);
  }

  void
  PostEvent(std::string_view event_type, const EnergyService & energyService)
  {
    for (const auto & callable : subscribers_[event_type.data()]) {
      callable(energyService);
    }
  }

  [[maybe_unused]] void
  ListSubscribers()
  {
    for (const auto & [event, fun] : subscribers_) {
      spdlog::info(
          "EventSystem type: " + event + " subscribers " +
          std::to_string(subscribers_[event].size()));
    }
  }

private:

  std::map<std::string, std::vector<std::function<void(const EnergyService &)>>>
      subscribers_;
};
#endif // API_EVENT_SYSTEM_H_


/* int */
/* main() */
/* { */
/*   // Example usage */
/*   EnergyService tsoService{"Transmission Service", EnergyServiceType::TSO};
 */
/*   EnergyService dsoService("Distribution Service", EnergyServiceType::DSO);
 */
/**/
/*   // Print details using the internal function */
/*   std::cout << "TSO Service Details:" << '\n'; */
/*   EnergyService::PrintEnergyService(tsoService); */
/**/
/*   std::cout << "\nDSO Service Details:" << '\n'; */
/*   EnergyService::PrintEnergyService(dsoService); */
/**/
/*   return 0; */
/* } */
