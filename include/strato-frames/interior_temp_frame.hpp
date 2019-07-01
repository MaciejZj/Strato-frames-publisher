#pragma once
#include <cstdint>
#include <string>

namespace frame_topic {
  static const std::string interior_temp = "interior_temp";
}

struct interior_temp_frame_t {
  float temperature;

  interior_temp_frame_t() {}

  interior_temp_frame_t(
      float temperature
  ) 
  : temperature(temperature)
  {}
};
