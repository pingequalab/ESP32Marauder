# Scout Lite 硬件

## 已核实

没有已核实配置。

本仓库没有 Scout Lite 原理图。`schematics/` 只有 2019 年 ESP32 Marauder 2 文件。本轮没有硬件测试，没有刷机，没有量过 Flash、PSRAM、GPIO 或波特率。

因此下面没有一行可以写成 Scout Lite 的板级事实。研究报告里的引脚表不是已核实配置。

## 源码已核对 / 实机待验证

这些是 `d3ceb28` 上官方 `MARAUDER_C5` 的宏和 workflow 字符串。它们不是 Scout Lite 接线。

| 项 | 源码 | 实机 |
| --- | --- | --- |
| 板名 | `HARDWARE_NAME` 为 `"ESP32-C5 DevKit"`，`esp32_marauder/configs.h:121-122` | 待验证 |
| 编译入口 | `.github/workflows/build_parallel.yml:46`：`esp32:esp32:esp32c5:FlashSize=8M,PartitionScheme=default_8MB,PSRAM=enabled`，`-DMARAUDER_C5` | 待验证。仓库里没有这份 8MB 分区 CSV |
| `HAS_PSRAM` | `configs.h:543` 被注释。`mac_history_len` 因此按 100，`BUF_SIZE` 按 `3 * 1024`（`configs.h:2891-2894`、`3167-3175`） | 待验证。core 3.3.4 的 `PSRAM.enabled` 只加 `-DBOARD_HAS_PSRAM`，不是这个宏 |
| GNSS | `HAS_GPS`。`GPS_SERIAL_INDEX 1`，`GPS_TX 14`，`GPS_RX 13`（`configs.h:2975-2978`）。`GpsInterface.cpp:11` 用 `HardwareSerial Serial2(GPS_SERIAL_INDEX)` | 待验证。不能当成 L86 已接通 |
| microSD | `HAS_SD`、`HAS_C5_SD`。`SD_CS 10`，`SD_MISO 2`，`SD_MOSI 7`，`SD_SCK 6`（`configs.h:2706-2707`、`3212-3215`）。`esp32_marauder.ino:255-256` 调用 `sharedSPI.begin` | 待验证 |
| UART | `Serial.begin(115200)`（`esp32_marauder.ino:244`）。没有 `SERIAL_EN` 或 `PIN_SERIAL_RX` / `PIN_SERIAL_TX`。`FLIPPER_ZERO_HAT` 在该目标是注释（`configs.h:530-531`） | 待验证。Flipper 引脚和 SigRoam 未测 |
| 屏 / 电池 / 按键 | `HAS_SCREEN`、`HAS_BATTERY`、`HAS_BUTTONS` 在 `MARAUDER_C5` 段是注释（`configs.h:530-537`） | 待验证 |
| 模组、天线、USB-C | 需求里的设计目标。源码没有 Scout Lite 模组名 | 待验证 |

其他 C5 目标的引脚宏不同，也不能拿来当 Scout Lite 的已核实配置。比较见 `FEATURE_MATRIX.md`。
