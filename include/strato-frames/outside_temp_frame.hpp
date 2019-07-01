#pragma once
#include <cstdint>
#include <string>

namespace frame_topic {
  static const std::string outside_temp = "outside_temp";
}

struct outside_temp_frame_t {
  float temperature;

  outside_temp_frame_t() {}

  outside_temp_frame_t(
    float temperature
  ) 
  : temperature(temperature)
  {}
};
