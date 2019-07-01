#pragma once
#include <cstdint>
#include <string>

namespace frame_topic {
  static const std::string pressure = "pressure";
}

struct pressure_frame_t {
  float pressure;

  pressure_frame_t() {}
  pressure_frame_t(
    float pressure
  )
  : pressure(pressure)
  {}
};
