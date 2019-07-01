#pragma once
#include <cstdint>
#include <string>

namespace frame_topic {
  static const std::string ahrs = "ahrs";
}

struct ahrs_frame_t {
  int16_t acc_x;
  int16_t acc_y;
  int16_t acc_z;
  int16_t gyro_x;
  int16_t gyro_y;
  int16_t gyro_z;
  int16_t mag_x;
  int16_t mag_y;
  int16_t mag_z;
  uint32_t pressure;
  bool falling;

  ahrs_frame_t () {}

  ahrs_frame_t (
    int16_t acc_x,
    int16_t acc_y,
    int16_t acc_z,
    int16_t gyro_x,
    int16_t gyro_y,
    int16_t gyro_z,
    int16_t mag_x,
    int16_t mag_y,
    int16_t mag_z,
    uint32_t pressure,
    bool falling
  ) 
  : acc_x(acc_x)
  , acc_y(acc_y)
  , acc_z(acc_z)
  , gyro_x(gyro_x)
  , gyro_y(gyro_y)
  , gyro_z(gyro_z)
  , mag_x(mag_x)
  , mag_y(mag_y)
  , mag_z(mag_z)
  , pressure(pressure)
  , falling(falling)
  {}
};
