#ifndef _LIB_MEASUREMENT_H_
#define _LIB_MEASUREMENT_H_

#include <functional>
#include <optional>
#include <stdexcept>

// Measurement Interface
class MeasurementInterface
{
public:

  virtual ~MeasurementInterface() = default;
  [[nodiscard]] virtual std::optional<double>
  getMeasurement() const = 0;
};

// dnp3
class dnp3Measurement : public MeasurementInterface
{
private:

  double m_dnp3Data;

public:

  explicit dnp3Measurement(double data)
      : m_dnp3Data(data)
  {}

  // Copy constructor
  dnp3Measurement(const dnp3Measurement & other)
      : m_dnp3Data(other.m_dnp3Data)
  {}

  // Copy assignment operator
  dnp3Measurement &
  operator=(const dnp3Measurement & other)
  {
    if (this != &other) {
      m_dnp3Data = other.m_dnp3Data;
    }
    return *this;
  }

  // Move constructor
  dnp3Measurement(dnp3Measurement && other) noexcept
      : m_dnp3Data(other.m_dnp3Data)
  {}

  // Move assignment operator
  dnp3Measurement &
  operator=(dnp3Measurement && other) noexcept
  {
    if (this != &other) {
      m_dnp3Data = other.m_dnp3Data;
    }
    return *this;
  }

  [[nodiscard]] std::optional<double>
  getMeasurement() const override
  {
    return m_dnp3Data;
  }
};

// modbus
class modbusMeasurement : public MeasurementInterface
{
private:

  double m_modbusData;

public:

  explicit modbusMeasurement(double data)
      : m_modbusData(data)
  {}

  [[nodiscard]] std::optional<double>
  getMeasurement() const override
  {
    return m_modbusData;
  }
};

// Data Acquisition by type erased template
class DataAcquisition final : public MeasurementInterface
{
private:

  std::function<std::optional<double>()> getMeasurementCallable;

public:

  // Constructor templated
  template <typename T>
  explicit DataAcquisition(const T & measure)
      : getMeasurementCallable(
            [measure]() -> std::optional<double>
            {
              if constexpr (std::is_base_of_v<MeasurementInterface, T>) {
                return measure.getMeasurement();
              }
              else {
                throw std::runtime_error("Unsupported type");
              }
            })
  {}

  ~DataAcquisition() final = default;

  [[nodiscard]] std::optional<double>
  getMeasurement() const override
  {
    return getMeasurementCallable();
  }
};
#endif //_LIB_MEASUREMENT_H_
