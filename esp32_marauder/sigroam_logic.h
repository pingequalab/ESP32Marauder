#pragma once

#include <stdint.h>

/* 1 = 2.4 GHz (channel 1..14). 2 = 5 GHz (channel > 14, including DFS).
 * 0 = not a Wi-Fi channel. Same cut the SigRoam FAP uses for ap_24 / ap_5. */
inline int sigroam_band(int channel) {
  if (channel >= 1 && channel <= 14)
    return 1;
  if (channel > 14)
    return 2;
  return 0;
}

/* Exact text "0" turns rotate off. Any other all-digit value must be 4..102400.
 * "00" and "12abc" are rejected; Arduino String::toInt would accept both. */
inline bool sigroam_split_kb_ok(const char* arg, long* out_kb) {
  if (arg == 0 || out_kb == 0 || arg[0] == '\0')
    return false;
  if (arg[0] == '0' && arg[1] == '\0') {
    *out_kb = 0;
    return true;
  }
  long v = 0;
  for (const char* p = arg; *p != '\0'; p++) {
    if (*p < '0' || *p > '9')
      return false;
    v = v * 10 + (*p - '0');
    if (v > 102400)
      return false;
  }
  if (v < 4)
    return false;
  *out_kb = v;
  return true;
}

/* Idle, and now_ms has reached next_ms. The signed delta keeps working
 * when millis() wraps. */
inline bool sigroam_sd_probe_due(bool scanning, uint32_t now_ms, uint32_t next_ms) {
  if (scanning)
    return false;
  return (int32_t)(now_ms - next_ms) >= 0;
}
