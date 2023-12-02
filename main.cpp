#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <vector>

namespace fs = std::filesystem;

static constexpr char CSV_DELIM         = ';';
static const char * const CSV_EXTENSION = ".csv";

static constexpr double Freq_Measure =
    50.5105217; // Simulate Siemens Q200 input

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

// CSV parsing using value semantics lambda function
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

// Linear Interpolation Utility Class
template <typename T>
class LinearInterpolator
{
private:

  const std::vector<T> m_freq;
  const std::vector<T> m_power;

public:

  LinearInterpolator(
      const std::vector<T> & xpoint, const std::vector<T> & ypoint)
      : m_freq(xpoint)
      , m_power(ypoint)
  {}

  [[nodiscard]] constexpr std::optional<T>
  operator()(const T & f_measure) const
  {
    if (m_freq.size() != m_power.size() || m_power.empty()) {
      // Invalid input data, return nullopt
      return std::nullopt;
    }

    auto it = std::lower_bound(m_freq.begin(), m_freq.end(), f_measure);

    if (it == m_freq.begin()) {
      // f_measure is before the first point, return m_power[0]
      return m_power[0];
    }

    if (it == m_freq.end()) {
      // f_measure is after the last point, return m_power[n-1]
      return m_power[m_power.size() - 1];
    }

    // Linear interpolation between it-1 and it
    auto idx = static_cast<size_t>(std::distance(m_freq.begin(), it));
    T x0     = m_freq[idx - 1];
    T x1     = m_freq[idx];
    T y0     = m_power[idx - 1];
    T y1     = m_power[idx];

    return y0 + (f_measure - x0) * (y1 - y0) / (x1 - x0);
  }
};

// GridCode_LFSM class
template <typename T>
class GridCode_LFSM
{
private:

  std::vector<std::tuple<std::optional<T>, std::optional<T>>> data;

public:

  explicit GridCode_LFSM(
      std::vector<std::tuple<std::optional<T>, std::optional<T>>> && initData)
      : data(std::move(initData))
  {}

  // Getter functions
  [[nodiscard]] const auto &
  getData() const
  {
    return data;
  }

  // Linear interpolation function
  [[nodiscard]] std::optional<T>
  interpolate(const T & x) const
  {
    // Extract x and y values for interpolation
    std::vector<T> xValues;
    std::vector<T> yValues;

    for (const auto & entry : data) {
      if (std::get<0>(entry).has_value() && std::get<1>(entry).has_value()) {
        xValues.push_back(*std::get<0>(entry));
        yValues.push_back(*std::get<1>(entry));
      }
    }

    LinearInterpolator<T> interpolator(xValues, yValues);
    return interpolator(x);
  }
};

// Use case in main function
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

  return 0;
}
