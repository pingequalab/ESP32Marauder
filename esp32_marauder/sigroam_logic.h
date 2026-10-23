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

/* IDF 5.5.5 heap_caps_get_minimum_free_size() ignores heaps registered from
 * app_main onward (esp_heap_caps.h). Arduino initArduino() adds PSRAM there
 * when CONFIG_SPIRAM_BOOT_INIT is off, so the IDF call stays 0 while free
 * size is real. idf_min == 0 means "not tracked", not "heap empty". */
inline uint32_t sigroam_psram_low(uint32_t free_now, uint32_t idf_min, uint32_t* seen_min) {
  uint32_t low = free_now;
  if (idf_min != 0 && idf_min < low)
    low = idf_min;
  if (*seen_min < low)
    low = *seen_min;
  *seen_min = low;
  return low;
}
