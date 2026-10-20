#pragma once

#include <Arduino.h>
#include <LinkedList.h>

extern const char SIGROAM_CMD[];

void sigroamRunCommand(LinkedList<String>& cmd_args);
bool collectWardriveLogs();
