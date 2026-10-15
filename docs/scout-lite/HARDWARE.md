# Scout Lite 硬件

## 已确认的范围

2026-10-15，Colin 确认：除 USB 自动切换电路以外，Scout Lite 的 SD 与 GPS 接线和 ESP32-C5 DevKit、官方 Marauder `MARAUDER_C5` 相同。

这是原理图确认。没有实机测试，没有刷机。不是 `CONFIRMED_WORKING`。

USB 自动切换电路不要当成和 DevKit 相同。

本仓库 `schematics/` 只有 2019 年 ESP32 Marauder 2 文件。下面的原理图结论来自 Colin 的确认。本轮没有量过 Flash、PSRAM 或波特率。

## 原理图已确认，实机未测

| 项 | 原理图（Colin，2026-10-15） | `MARAUDER_C5` 源码 | 实机 |
| --- | --- | --- | --- |
| microSD | IO2 MISO，IO6 SCK，IO7 MOSI，IO10 CS | `SD_MISO 2`，`SD_SCK 6`，`SD_MOSI 7`，`SD_CS 10`（`esp32_marauder/configs.h:2707`、`3213-3215`） | 未测。不是 `CONFIRMED_WORKING` |
| GPS | 模组 pin 14 / GPIO14 是固件 RX。模组 pin 13 / GPIO13 是固件 TX | `GPS_TX 14`，`GPS_RX 13`（`configs.h:2977-2978`）。`GpsInterface.cpp:20` 与 `:63` 是 `Serial2.begin(..., GPS_TX, GPS_RX)` | 未测。不是 `CONFIRMED_WORKING` |
| 模组丝印 | `ESP32-C5-WROOM-1U-N8R8` 是图纸上的料号 | `HAS_PSRAM` 在 `configs.h:543` 仍是注释 | 不是运行时 PSRAM 测量。不把 PSRAM 标成已启用。容量 UNKNOWN |

`GPS_TX` 是固件 RX，接 GPIO14。`GPS_RX` 是固件 TX，接 GPIO13。`Serial2.begin` 把 `GPS_TX`、`GPS_RX` 按这个顺序放进 RX、TX 参数。

## USB 自动切换

`TS3USB221` 在 GPIO13/14。相对上面「SD/GPS 与 DevKit 同线」这一句，这条是 Scout Lite 特有的。不要把它当成 DevKit 的同等电路。

OE#、选择阈值、极性都还没有核完。

## 明确未知

- H1 上的 `MOSI` / `MISO` / `CS` / `SCK` 是 Flipper SPI 名称，没有证明它们就是 `SD_*` 网络。
- H1 上的 `PB2` / `PC3` 没有证明就是 BSS138 的 `EN` / `IO28` 网络。
- D2 `SS14` 在 `+5V` 和 H1 pin 1 之间的方向未知。
- H2 `GPS_TXD` 上的 100Ω 未知。
- Flipper UART 在 GPIO11/12 不在 Colin 这次确认里。

## 源码已核对，这次确认没有覆盖

这些仍是 `d3ceb28` 上官方 `MARAUDER_C5` 的宏或 workflow 字符串，不是 Colin 这次确认的板级事实。

| 项 | 源码 | 实机 |
| --- | --- | --- |
| 板名 | `HARDWARE_NAME` 为 `"ESP32-C5 DevKit"`，`esp32_marauder/configs.h:121-122` | 待验证 |
| 编译入口 | `.github/workflows/build_parallel.yml:46`：`esp32:esp32:esp32c5:FlashSize=8M,PartitionScheme=default_8MB,PSRAM=enabled`，`-DMARAUDER_C5` | 待验证。仓库里没有这份 8MB 分区 CSV |
| `HAS_PSRAM` | `configs.h:543` 被注释。`mac_history_len` 因此按 100，`BUF_SIZE` 按 `3 * 1024`（`configs.h:2891-2894`、`3167-3175`） | 待验证。core 3.3.4 的 `PSRAM=enabled` 只加 `-DBOARD_HAS_PSRAM`，不是这个宏。丝印 N8R8 不改变这一点 |
| UART | `Serial.begin(115200)`（`esp32_marauder.ino:244`）。没有 `SERIAL_EN` 或 `PIN_SERIAL_RX` / `PIN_SERIAL_TX`。`FLIPPER_ZERO_HAT` 在该目标是注释（`configs.h:530-531`） | 待验证。GPIO11/12 未确认。SigRoam 未测 |
| 屏 / 电池 / 按键 | `HAS_SCREEN`、`HAS_BATTERY`、`HAS_BUTTONS` 在 `MARAUDER_C5` 段是注释（`configs.h:530-537`） | 待验证 |

其他 C5 目标的引脚宏不同，也不能拿来当 Scout Lite 的已核实配置。比较见 `FEATURE_MATRIX.md`。
