# 内部 beta：未修改的 `MARAUDER_C5`

日期：2026-10-16。

这是官方 `MARAUDER_C5` 的一次本地重编。不是 Scout Lite 功能移植。没有改宏，没有新板型。bin 不进 git，不上传 GitHub Release，没有刷机。

## 编译时的 commit

`8b73c1905e810771da7f97fac63e89b2f5d9c1b3`

该 commit 在 `f4bc76727e7f29b7ca64da9c972c9103523c6006` 之上只有文档。`esp32_marauder/` 相对 `f4bc767` 无差异。本文件是编译完成之后补上的记录，不改变固件源码。

## 命令

工具链是这台机器上已经装好的，本轮没有重装 core，也没有重新克隆库。

- `arduino-cli` 1.5.1，路径 `/tmp/bin/arduino-cli`，commit `01f3d4f2b`（2026-06-05）
- core `esp32:esp32@3.3.4`，index `https://github.com/espressif/arduino-esp32/releases/download/3.3.4/package_esp32_dev_index.json`
- 库目录 `/tmp/marauder-libs`，版本与 `.github/workflows/scout-lite-ci.yml` 里写的 ref 一致：ESP32Ping `1.6`，AsyncTCP `v3.4.8`，MicroNMEA `v2.0.6`，ESPAsyncWebServer `v3.8.1`，TFT_eSPI `V2.5.34`，XPT2046_Touchscreen `v1.4`，NimBLE-Arduino `2.3.8`，Adafruit_NeoPixel `1.12.0`，ArduinoJson `v6.18.2`，LinkedList `v1.3.3`，EspSoftwareSerial `8.1.0`，Adafruit_BusIO `1.15.0`，Adafruit_MAX1704X `1.0.2`。`Adafruit_TCA8418` 与仓库 `libraries/Adafruit_TCA8418` 一致。
- 已有的 core 补丁仍在：`platform.txt` 的 `compiler.c.elf.extra_flags=-Wl,-zmuldefs`，C5 的 `cpp_flags` 含 `-fno-exceptions`。这是 CI 同一类修改，本轮没有再改这些文件。
- JPEGDecoder 和 lv_arduino 不在这次的库路径里。无屏的 `MARAUDER_C5` 这次没有用到它们。

```bash
/tmp/bin/arduino-cli compile \
  --fqbn esp32:esp32:esp32c5:FlashSize=8M,PartitionScheme=default_8MB,PSRAM=enabled \
  --libraries /tmp/marauder-libs \
  --build-property compiler.cpp.extra_flags=-DMARAUDER_C5 \
  --warnings none \
  --build-path /tmp/c5-build-feature-gap \
  /tmp/scout-lite-worktrees/feature-gap/esp32_marauder
```

FQBN 与 `.github/workflows/scout-lite-ci.yml` 里的 `esp32:esp32:esp32c5:FlashSize=8M,PartitionScheme=default_8MB,PSRAM=enabled` 相同。flag 是 `-DMARAUDER_C5`。

开始 `2026-10-16T00:31:09Z`，结束 `2026-10-16T00:32:26Z`，退出码 0。

Arduino 输出：Sketch uses 1873710 bytes (56%) of program storage space. Maximum is 3342336 bytes.

## app bin SHA256

文件：`/tmp/c5-build-feature-gap/esp32_marauder.ino.bin`（应用 bin，不是 merged bin）。磁盘长度 1873824 字节。

```
e8c5b57f16745a2bcfbd5e8752fa68301345262cb4f6578556ae1b6316a681f0
```

`build.options.json` 里的 fqbn 与 `compiler.cpp.extra_flags=-DMARAUDER_C5` 与上面的命令一致。sketch 路径是这次 worktree 的 `esp32_marauder`。

## 和 2026-10-15 那次本地 bin 的差别

`d3ceb28` 上记录过的 app bin SHA256 是 `1d572f57bb1f90cf063952672414413e194ee7aef00de1c66218238dc26b60a8`。那份文件仍在这台机器的 `/tmp/c5-build-out/esp32_marauder.ino.bin`。两次报告的 sketch 大小都是 1873710 字节。固件源码目录没有差异。

两份 bin 的应用内容差在编译时刻字符串：`Oct 15 2026` `13:56:54` 与 `Oct 16 2026` `00:32:20`。镜像头部哈希和末尾校验跟着这些字节变。所以跨日的 SHA256 不会相同。这次内部 beta 以 `e8c5b57f16745a2bcfbd5e8752fa68301345262cb4f6578556ae1b6316a681f0` 为准。

## 不是这些东西

- 上面那一节不是 Scout Lite 功能移植
- 不是客户固件
- 没有刷进任何板
- 没有 GitHub Release
- `HAS_PSRAM` 仍然是注释。FQBN 的 `PSRAM=enabled` 只带来 `BOARD_HAS_PSRAM`

## Phase 1 构建

日期：2026-10-16。编译时 HEAD 是 `cf1da663fea6fef21f9c06c8efc8419943c9137e`。这一笔改了 CLI `upload`：sidecar 用 `retry=false`，单文件失败后再试 2 次，只有上传前正在 `WIFI_SCAN_WAR_DRIVE` 且没有失败文件才回到 wardrive。没有改 `executeWarDrive()` 的频道表。没有 Dock、Web UI、SSID 排除、Node/Core、ESP-NOW、SigRoam。

命令与上面相同。开始 `2026-10-16T01:17:35Z`，结束 `2026-10-16T01:17:43Z`，退出码 0。

Arduino 输出：Sketch uses 1873920 bytes (56%) of program storage space. Maximum is 3342336 bytes.

app bin：`/tmp/c5-build-feature-gap/esp32_marauder.ino.bin`，磁盘长度 1874032 字节。

```
37cc863b6112328ff571c56cd1f62e67bf50d4537fa5d95b9beac0e07703be10
```

`build.options.json` 的 fqbn 仍是 `esp32:esp32:esp32c5:FlashSize=8M,PartitionScheme=default_8MB,PSRAM=enabled`，`compiler.cpp.extra_flags=-DMARAUDER_C5`。bin 未提交，未上传 Release，未刷机。上面未改宏的 SHA256 仍然有效，它不是这一笔。

## 无 fix 不占 MAC

日期：2026-10-16。编译时 HEAD 是 `b376eb64e20ec51fe3587205f01cc504a898f489`。`save_mac()` 只在 wardrive 行 `append` 之后调用。路径是 `executeWarDrive()`、`beaconSnifferCallback()`、以及 NimBLE 2 的 wardrive `onResult()`。没有改频道表，没有加 SSID 排除、Dock、删除 CLI 或 `HAS_PSRAM`。

同一条命令。开始 `2026-10-16T01:27:46Z`，结束 `2026-10-16T01:27:57Z`，退出码 0。

Arduino 输出：Sketch uses 1873920 bytes (56%) of program storage space. Maximum is 3342336 bytes.

app bin 仍是 `/tmp/c5-build-feature-gap/esp32_marauder.ino.bin`，磁盘长度 1874032 字节。`sketch/WiFiScan.cpp.o` 在这次编译里重新生成。

```
9005e18a9d563b9cd62c415e0b8f5837d7bab8beb5300e76ce5dab0d746fa4f2
```

bin 未提交，未上传 Release，未刷机。Phase 1 的 SHA256 是上一节，不是这一笔。
