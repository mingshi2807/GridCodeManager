#include <optional>
#include <tuple>
#include <vector>

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
