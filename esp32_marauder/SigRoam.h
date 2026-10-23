#pragma once

#include <Arduino.h>
#include <LinkedList.h>

#ifdef MARAUDER_C5
#define SIGROAM_HARDWARE_DISPLAY "PINGEQUA Scout Lite (Dual-band Wardriver)"
#else
#define SIGROAM_HARDWARE_DISPLAY HARDWARE_NAME
#endif

extern const char SIGROAM_CMD[];

struct SigRoamState {
  uint32_t wardrive_split_kb = 1024;
  bool sig_session_on = false;
  uint32_t sig_session_start_ms = 0;
  uint32_t wifi_24 = 0;
  uint32_t wifi_5 = 0;
  uint32_t sd_drops = 0;
};

extern SigRoamState sigroam_state;

void sigroamRunCommand(LinkedList<String>& cmd_args);
bool collectWardriveLogs();
void sigroamLoadPersisted();
void sigroamNoteSdBoot();
void sigroamNoteWifiChannel(int channel);
void sigroamNoteSdDrop();
void sigroamOnStartScan(uint8_t scan_mode);
void sigroamArmWardriveRotate(const String& header_line);
void sigroamPrintBootBanner();
