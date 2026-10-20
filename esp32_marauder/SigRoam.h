#pragma once

#include <Arduino.h>
#include <LinkedList.h>

extern const char SIGROAM_CMD[];

struct SigRoamState {
  uint32_t wardrive_split_kb = 1024;
  bool sig_session_on = false;
  uint32_t sig_session_start_ms = 0;
};

extern SigRoamState sigroam_state;

void sigroamRunCommand(LinkedList<String>& cmd_args);
bool collectWardriveLogs();
void sigroamOnStartScan(uint8_t scan_mode);
void sigroamArmWardriveRotate(const String& header_line);
