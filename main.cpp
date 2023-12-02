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
};

// Use case in main function
int
main()
{
  const std::string dataFolder = "./data";

  std::vector<GridCode_LFSM<double>> gridCodeInstancesDouble;

  for (const auto & entry : fs::directory_iterator(dataFolder)) {
    if (IsCSVFile(entry)) {
      auto data = parseCSV<double>(entry.path().string());
      gridCodeInstancesDouble.emplace_back(std::move(data));
    }
  }

  // Example use case: print the contents of the first double instance
  if (!gridCodeInstancesDouble.empty()) {
    const auto & firstInstance = gridCodeInstancesDouble.front();
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
  }

  return 0;
}
