#pragma once
#include <cstdint>
#include <string>

namespace frame_topic {
  static const std::string gps = "gps";
}

struct gps_frame_t {
  uint32_t      time;         // UTC time, format: HHMMSS
  double        lat;          // -90.0 - 90.0
  char          lat_dir;      // 'N' / 'S' 
  double        lon;          // -180.0 - 180.0
  char          lon_dir;      // 'E' / 'W'
  uint8_t       fix_quality;  // 0 - no fix, 1 - fix
  uint8_t       satelites;    // visible satilites 0 - 24  
  double        altitude;     // altitude in [m]

  gps_frame_t() {}

  gps_frame_t(
    uint32_t time,
    double  lat,
    char    lat_dir,
    double  lon,
    char    lon_dir,
    uint8_t fix_quality,
    uint8_t satelites,
    double  altitude
  )
  : time(time)
  , lat(lat)
  , lat_dir(lat_dir)
  , lon(lon)
  , lon_dir(lon_dir)
  , fix_quality(fix_quality)
  , satelites(satelites)
  , altitude(altitude)
  {}
};
