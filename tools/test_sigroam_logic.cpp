#include "sigroam_logic.h"

#include <stdio.h>

static int g_fail = 0;

static void check(bool ok, const char* what) {
  if (ok)
    return;
  fprintf(stderr, "FAIL %s\n", what);
  g_fail++;
}

int main() {
  check(sigroam_band(0) == 0, "ch 0");
  check(sigroam_band(-1) == 0, "ch -1");
  check(sigroam_band(1) == 1, "ch 1");
  check(sigroam_band(14) == 1, "ch 14");
  check(sigroam_band(15) == 2, "ch 15");
  check(sigroam_band(36) == 2, "ch 36");
  check(sigroam_band(165) == 2, "ch 165");

  long kb = -1;
  check(!sigroam_split_kb_ok(0, &kb), "null arg");
  check(!sigroam_split_kb_ok("0", 0), "null out");
  check(!sigroam_split_kb_ok("", &kb), "empty");
  check(sigroam_split_kb_ok("0", &kb) && kb == 0, "off");
  check(!sigroam_split_kb_ok("00", &kb), "00");
  check(!sigroam_split_kb_ok("3", &kb), "3");
  check(sigroam_split_kb_ok("4", &kb) && kb == 4, "4");
  check(sigroam_split_kb_ok("1024", &kb) && kb == 1024, "1024");
  check(sigroam_split_kb_ok("01024", &kb) && kb == 1024, "01024");
  check(sigroam_split_kb_ok("102400", &kb) && kb == 102400, "max");
  check(!sigroam_split_kb_ok("102401", &kb), "over");
  check(!sigroam_split_kb_ok("12abc", &kb), "trail");
  check(!sigroam_split_kb_ok("0x10", &kb), "hex");

  check(!sigroam_sd_probe_due(true, 100, 0), "scanning");
  check(!sigroam_sd_probe_due(false, 5, 100), "early");
  check(sigroam_sd_probe_due(false, 100, 100), "exact");
  check(sigroam_sd_probe_due(false, 10, 0xFFFFFFF0u), "wrap due");

  if (g_fail) {
    fprintf(stderr, "%d failed\n", g_fail);
    return 1;
  }
  puts("ALL TESTS PASSED");
  return 0;
}
