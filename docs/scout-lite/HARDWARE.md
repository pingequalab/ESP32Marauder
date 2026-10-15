# Scout Lite 硬件

## 已确认的范围

2026-10-15，Colin 确认：重要接线按 ESP32-C5 DevKit 的官方 Marauder 目标。非关键走线不再追查。

这是原理图确认。没有实机测试，没有刷机。不是 `CONFIRMED_WORKING`。

本仓库 `schematics/` 只有 2019 年 ESP32 Marauder 2 文件。下面的原理图结论来自 Colin 的确认。本轮没有量过 Flash、PSRAM 或波特率。

## 原理图已确认，实机未测

| 项 | 原理图（Colin，2026-10-15） | `MARAUDER_C5` 源码 | 实机 |
| --- | --- | --- | --- |
| microSD | IO2 MISO，IO6 SCK，IO7 MOSI，IO10 CS | `SD_MISO 2`，`SD_SCK 6`，`SD_MOSI 7`，`SD_CS 10`（`esp32_marauder/configs.h:2707`、`3213-3215`） | 未测。不是 `CONFIRMED_WORKING` |
| GPS 固件脚 | GPIO14 是固件 RX。GPIO13 是固件 TX | `GPS_TX 14`，`GPS_RX 13`（`configs.h:2977-2978`）。`GpsInterface.cpp:20` 与 `:63` 是 `Serial2.begin(..., GPS_TX, GPS_RX)` | 未测。不是 `CONFIRMED_WORKING` |
| 模组丝印 | `ESP32-C5-WROOM-1U-N8R8` 是图纸上的料号 | `HAS_PSRAM` 在 `configs.h:543` 仍是注释 | 不是运行时 PSRAM 测量。不把 PSRAM 标成已启用。容量 UNKNOWN |

`GPS_TX` 是固件 RX，接 GPIO14。`GPS_RX` 是固件 TX，接 GPIO13。`Serial2.begin` 把 `GPS_TX`、`GPS_RX` 按这个顺序放进 RX、TX 参数。

GPS 接到 Flipper pin 15 和 16，不是官方 GPIO13/14。已记录，不是任务，不计划改固件。

## 下载模式

实板不能让 Flipper 进入下载模式。只有板上 BOOT 和 EN 可以。`PB2`/`PC3` 不是下载模式路径。

## USB 自动切换

USB 自动切换不声称与 DevKit 相同。除非以后的任务需要，不再调查。

## 源码已核对，这次确认没有覆盖

这些仍是 `d3ceb28` 上官方 `MARAUDER_C5` 的宏或 workflow 字符串，不是 Colin 这次确认的板级事实。

| 项 | 源码 | 实机 |
| --- | --- | --- |
| 板名 | `HARDWARE_NAME` 为 `"ESP32-C5 DevKit"`，`esp32_marauder/configs.h:121-122` | 待验证 |
| 编译入口 | `.github/workflows/build_parallel.yml:46`：`esp32:esp32:esp32c5:FlashSize=8M,PartitionScheme=default_8MB,PSRAM=enabled`，`-DMARAUDER_C5` | 待验证。仓库里没有这份 8MB 分区 CSV |
| `HAS_PSRAM` | `configs.h:543` 被注释。`mac_history_len` 因此按 100，`BUF_SIZE` 按 `3 * 1024`（`configs.h:2891-2894`、`3167-3175`） | 待验证。core 3.3.4 的 `PSRAM=enabled` 只加 `-DBOARD_HAS_PSRAM`，不是这个宏。丝印 N8R8 不改变这一点 |
| UART | `Serial.begin(115200)`（`esp32_marauder.ino:244`）。没有 `SERIAL_EN` 或 `PIN_SERIAL_RX` / `PIN_SERIAL_TX`。`FLIPPER_ZERO_HAT` 在该目标是注释（`configs.h:530-531`） | 实机未测。SigRoam 未测 |
| 屏 / 电池 / 按键 | `HAS_SCREEN`、`HAS_BATTERY`、`HAS_BUTTONS` 在 `MARAUDER_C5` 段是注释（`configs.h:530-537`） | 待验证 |

其他 C5 目标的引脚宏不同，也不能拿来当 Scout Lite 的已核实配置。比较见 `FEATURE_MATRIX.md`。
