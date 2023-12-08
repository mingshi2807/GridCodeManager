#include "EnergyServiceManagement.h"
#include "GridCode.h"
#include "event_system.h"
#include "logListener.h"
#include "measurement.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

namespace fs = std::filesystem;

static constexpr char CSV_DELIM         = ';';
static const char * const CSV_EXTENSION = ".csv";

static constexpr double Freq_Measure =
    50.5105217; // Simulate Siemens Q200 input / or Moving average (EMA) sampled
                // in ROCOF window of 500ms

// Type trait for checking if a type is arithmetic
template <typename T>
struct is_arithmetic_type : std::is_arithmetic<T>
{};

[[nodiscard]] bool
IsCSVFile(const fs::path & filespath)
{
  return fs::is_regular_file(filespath) &&
         filespath.extension() == CSV_EXTENSION;
}

// CSV parsing
template <typename T>
[[nodiscard]] auto
parseCSV(const std::string_view & filePath)
{
  static_assert(
      is_arithmetic_type<T>::value, "Only arithmetic types are allowed.");

  std::vector<std::tuple<std::optional<T>, std::optional<T>>> data;

  std::ifstream file(filePath.data());
  if (!file.is_open()) {
    std::cerr << "Error opening file: " << filePath << '\n';
    return data;
  }

  std::string line;
  while (std::getline(file, line)) {
    std::istringstream tmpss(line);
    std::string freqStr;
    std::string powerStr;

    if (std::getline(tmpss, freqStr, CSV_DELIM) &&
        std::getline(tmpss, powerStr, CSV_DELIM)) {
      std::optional<T> freq =
          freqStr.empty() ? std::nullopt : std::optional<T>(std::stod(freqStr));
      std::optional<T> power = powerStr.empty()
                                   ? std::nullopt
                                   : std::optional<T>(std::stod(powerStr));
      data.emplace_back(freq, power);
    }
  }

  return data;
};

// Measurement interface test
std::optional<double>
MeasurementInterfaceTest(const MeasurementInterface & metering)
{
  return metering.getMeasurement();
}

// Use case in main function
// Just for test before cmake :clang++-14 -std=c++17 -Wall -pedantic -o main
// main.cpp -lfmt
int
main()
{
  const std::string dataFolder = "./data";

  std::vector<GridCode_LFSM<double>> gridCodeInstancesLFSM_O;

  for (const auto & entry : fs::directory_iterator(dataFolder)) {
    if (IsCSVFile(entry)) {
      auto data = parseCSV<double>(entry.path().string());
      gridCodeInstancesLFSM_O.emplace_back(std::move(data));
    }
  }

  // Example use case: print the contents of the first double instance
  if (!gridCodeInstancesLFSM_O.empty()) {
    const auto & firstInstance = gridCodeInstancesLFSM_O.front();
    const auto & data          = firstInstance.getData();

    for (const auto & entry : data) {
      std::cout << "Frequency: "
                << (std::get<0>(entry).has_value()
                        ? std::to_string(*std::get<0>(entry))
                        : "N/A")
                << ", Power: "
                << (std::get<1>(entry).has_value()
                        ? std::to_string(*std::get<1>(entry))
                        : "N/A")
                << '\n';
    }

    // Example use case : interpolate a power value from freq input with high
    // sig figs

    auto interpolatedPower = firstInstance.interpolate(Freq_Measure);

    if (interpolatedPower.has_value()) {
      std::cout << "Interpolated Power Value at frequency = Freq_Measure: "
                << (*interpolatedPower) * 100 << " % of Pmax" << '\n';
    }
    else {
      std::cout << "Interpolation failed." << '\n';
    }
  }

  EventSystem events_system{};
  SetupLogEventHandlers(events_system);

  EnergyServiceDB services_db{};
  EnergyServiceManagement services_management{services_db, events_system};

  services_management.RegisterNewService(
      "FCR", gridcode::EnergyServiceType::TSO);
  /* services_management.ResetServiceTypeToDefault( */
  /*     "aFRR", EnergyServiceType::DSO); // suppose FCR and TSO are default
   * config */

  // Case 1: Instantiate with dnp3
  DataAcquisition metering1{dnp3Measurement(Freq_Measure)};

  // Case 2: Instantiate with modbus
  DataAcquisition metering2{modbusMeasurement(Freq_Measure)};

  std::optional<double> testdnpvalue = MeasurementInterfaceTest(metering1);
  std::cout << "Test Value (Case dnp): "
            << (testdnpvalue ? std::to_string(*testdnpvalue)
                             : "Unsupported type")
            << '\n';


  std::optional<double> testmodbusvalue = MeasurementInterfaceTest(metering2);
  std::cout << "Test Value (Case modbus): "
            << (testmodbusvalue ? std::to_string(*testmodbusvalue)
                                : "Unsupported type")
            << '\n';
  return 0;
}
