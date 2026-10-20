#include "SigRoam.h"
#include "CommandLine.h"
#include <esp_heap_caps.h>

const char PROGMEM SIGROAM_CMD[] = "sigroam";

  static uint32_t countLogsMissingWigleSidecar() {
    uint32_t pending = 0;
    if (!sd_obj.supported)
      return 0;
    File dir = SD.open("/");
    if (!dir || !dir.isDirectory()) {
      dir.close();
      return 0;
    }
    File entry = dir.openNextFile();
    while (entry) {
      if (!entry.isDirectory()) {
        String name = entry.name();
        int slash = name.lastIndexOf('/');
        if (slash >= 0)
          name = name.substring(slash + 1);
        if ((name.startsWith("wardrive_") || name.startsWith("wigle-")) &&
            !name.endsWith(".wigle") && !name.endsWith(".wdg") && !name.endsWith(".gpx") &&
            !SD.exists("/" + name + ".wigle"))
          pending++;
      }
      entry.close();
      entry = dir.openNextFile();
    }
    dir.close();
    return pending;
  }

void printSigRoamStatus() {
    const bool scanning = wifi_scan_obj.currentScanMode == WIFI_SCAN_WAR_DRIVE && wifi_scan_obj.sig_session_on;
    const uint8_t st = scanning ? 1 : 0;
    uint32_t session_ms = 0;
    if (scanning) {
      session_ms = millis() - wifi_scan_obj.sig_session_start_ms;
      if (session_ms == 0)
        session_ms = 1;
    }

    uint32_t sats = 0;
    uint32_t gga = 0;
    uint32_t ggafix = 0;
    #ifdef HAS_GPS
      int sat_now = gps_obj.getNumSats();
      if (sat_now > 0)
        sats = (uint32_t)sat_now;
      if (sats > 255)
        sats = 255;
      gga = gps_obj.getGgaCount();
      ggafix = gps_obj.getGgaFixCount();
    #endif
    const uint8_t sd_up = sd_obj.supported ? 1 : 0;
    #ifdef HAS_BT
      const uint8_t ble_ok = 1;
    #else
      const uint8_t ble_ok = 0;
    #endif
    const uint32_t ap = wifi_scan_obj.beacon_frames;
    const uint32_t ble = wifi_scan_obj.bt_frames > 0 ? (uint32_t)wifi_scan_obj.bt_frames : 0;

    String wu = settings_obj.loadSetting<String>("wu");
    String wt = settings_obj.loadSetting<String>("wt");
    const uint8_t key = (wu.length() > 0 && wt.length() > 0) ? 1 : 0;
    wu = "";
    wt = "";
    String ssid = "-";
    uint8_t home = 0;
    String saved_ssid;
    String saved_pass;
    if (settings_obj.loadSavedWifiCredential(0, saved_ssid, saved_pass) && saved_ssid.length() > 0) {
      home = 1;
      saved_ssid.replace("\r", " ");
      saved_ssid.replace("\n", " ");
      ssid = saved_ssid;
    }
    saved_pass = "";

    Serial.print(F("Diag: st="));
    Serial.print(st);
    Serial.print(F(" seal=00 hb="));
    Serial.print(sats);
    Serial.print(F("/1/"));
    Serial.print(st);
    Serial.print(F("/0/"));
    Serial.print(sd_up);
    Serial.print(F("/"));
    Serial.println(ble_ok);

    Serial.print(F("Busy: st="));
    Serial.print(st);
    Serial.println(F(" seal=00"));

    Serial.print(F("Sess: ap="));
    Serial.print(ap);
    Serial.print(F(" ble="));
    Serial.print(ble);
    Serial.print(F(" ms="));
    Serial.print(session_ms);
    Serial.println(F(" ih=0 ihmin=0 ps=0 psmin=0"));

    Serial.print(F("Qual: gga="));
    Serial.print(gga);
    Serial.print(F(" ggafix="));
    Serial.print(ggafix);
    Serial.print(F(" drop=0 net="));
    Serial.print(ap);
    Serial.print(F(" sd="));
    Serial.print(sd_up);
    Serial.print(F(" sats="));
    Serial.print(sats);
    Serial.println(F(" topd=0"));

    Serial.print(F("Radio: wifi=1 ble="));
    Serial.println(ble_ok);

    Serial.print(F("Up: up_q="));
    Serial.print(countLogsMissingWigleSidecar());
    Serial.println(F(" up_last_trans=- up_disc_gps=0 up_reason=-"));

    Serial.println(F("Rank: rank=0 month=0 wifi_gps=0"));

    Serial.print(F("Cfg: key="));
    Serial.print(key);
    Serial.print(F(" home="));
    Serial.print(home);
    Serial.print(F(" ssid="));
    Serial.println(ssid);
  }


void printSigRoamMem() {
  Serial.print(F("Mem: int_free="));
  Serial.print(heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
  Serial.print(F(" int_min="));
  Serial.print(heap_caps_get_minimum_free_size(MALLOC_CAP_INTERNAL));
  Serial.print(F(" int_blk="));
  Serial.print(heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL));
  Serial.print(F(" ps_total="));
  Serial.print(heap_caps_get_total_size(MALLOC_CAP_SPIRAM));
  Serial.print(F(" ps_free="));
  Serial.print(heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
  Serial.print(F(" ps_min="));
  Serial.println(heap_caps_get_minimum_free_size(MALLOC_CAP_SPIRAM));
}

static bool isWardriveLogName(const String& name) {
  return (name.startsWith("wardrive_") || name.startsWith("wigle-")) &&
         name.indexOf('/') < 0 && name.indexOf('\\') < 0 && name.indexOf("..") < 0;
}

// Upload markers must go with the log: new logs reuse the lowest free index,
// and a stale <name>.wigle would make the new file look already uploaded.
static bool removeWardriveLog(const String& name) {
  if (!sd_obj.supported || !isWardriveLogName(name))
    return false;
  if (!sd_obj.removeFile("/" + name))
    return false;
  sd_obj.removeFile("/" + name + ".wdg");
  sd_obj.removeFile("/" + name + ".wigle");
  return true;
}

static int removeAllWardriveLogs() {
  if (!sd_obj.supported)
    return -1;
  LinkedList<String> names;
  sd_obj.listDirToLinkedList(&names);
  int removed = 0;
  for (int i = 0; i < names.size(); i++) {
    String name = names.get(i);
    int slash = name.lastIndexOf('/');
    if (slash >= 0)
      name = name.substring(slash + 1);
    if (!isWardriveLogName(name))
      continue;
    if (sd_obj.removeFile("/" + name)) {
      Serial.println("Removed file: " + name);
      removed++;
    }
    else {
      Serial.println("Could not remove file: " + name);
    }
  }
  return removed;
}


bool collectWardriveLogs() {
  if (!sd_obj.supported)
    return false;
  if (sd_obj.sd_files == nullptr)
    sd_obj.sd_files = new LinkedList<String>();
  sd_obj.sd_files->clear();

  File dir = SD.open("/");
  if (!dir || !dir.isDirectory()) {
    dir.close();
    return true;
  }

  File entry = dir.openNextFile();
  while (entry) {
    if (!entry.isDirectory()) {
      String name = entry.name();
      if ((name.startsWith("wardrive_") || name.startsWith("wigle-")) &&
          !name.endsWith(".wigle") && !name.endsWith(".wdg") && !name.endsWith(".gpx"))
        sd_obj.sd_files->add(name);
    }
    entry.close();
    entry = dir.openNextFile();
  }
  dir.close();
  return true;
}


void sigroamRunCommand(LinkedList<String>& cmd_args) {
    String sub = cmd_args.size() > 1 ? cmd_args.get(1) : "";
    if (sub == "mem") {
      printSigRoamMem();
    }
    else if (sub == "ignore") {
      String act = cmd_args.size() > 2 ? cmd_args.get(2) : "list";
      if (act == "add") {
        if (cmd_args.size() > 3 && settings_obj.addIgnoreSsid(cmd_args.get(3)))
          Serial.println("Ignore added: " + cmd_args.get(3));
        else
          Serial.println(F("Ignore add failed (dup/full/invalid; max 10, 1-32 chars)"));
      }
      else if (act == "rm") {
        int idx = cmd_args.size() > 3 ? cmd_args.get(3).toInt() : 0;
        if (idx >= 1 && settings_obj.removeIgnoreSsid(idx - 1))
          Serial.printf("Ignore %d removed\n", idx);
        else
          Serial.println(F("Invalid index"));
      }
      else if (act == "clear") {
        settings_obj.clearIgnoreSsids();
        Serial.println(F("Ignore list cleared"));
      }
      else {
        uint8_t n = settings_obj.getIgnoreSsidCount();
        if (n == 0) Serial.println(F("Ignore list empty"));
        else for (uint8_t i = 0; i < n; i++) {
          String s; settings_obj.loadIgnoreSsid(i, s);
          Serial.printf("%u: %s\n", i + 1, s.c_str());
        }
      }
    }
    else if (sub == "rm") {
      #ifdef HAS_SD
        String target = cmd_args.size() > 2 ? cmd_args.get(2) : "";
        if (wifi_scan_obj.currentScanMode != WIFI_SCAN_OFF) {
          Serial.println(F("Busy: stopscan first"));
        }
        else if (target == "all") {
          int n = removeAllWardriveLogs();
          if (n < 0) Serial.println(F("SD not available"));
          else Serial.printf("Removed %d files\n", n);
        }
        else if (removeWardriveLog(target)) {
          Serial.println("Removed file: " + target);
        }
        else {
          Serial.println(F("Remove failed (usage: sigroam rm <wardrive_*|wigle-*>|all, SD root only)"));
        }
      #endif
    }
    else if (sub == "split") {
      if (cmd_args.size() > 2) {
        String arg = cmd_args.get(2);
        long kb = arg.toInt();
        if (arg == "0" || (kb >= 4 && kb <= 102400))
          wifi_scan_obj.wardrive_split_kb = kb;
        else
          Serial.println(F("Invalid size (0=off, 4-102400 KB)"));
      }
      Serial.printf("Split: %u KB (next wardrive)\n", (unsigned)wifi_scan_obj.wardrive_split_kb);
    }
    else {
      printSigRoamStatus();
    }
    return;
}
