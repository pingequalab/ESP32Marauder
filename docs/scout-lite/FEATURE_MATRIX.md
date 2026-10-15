# 官方 Marauder 与 Scout Lite 差距

基线 commit：`d3ceb28afeda1196ca27c13adeaa69ada52383f3`。

Scout Lite 还没有板型宏。官方默认对照目标是 `MARAUDER_C5`。本列「Scout Lite」只表示这个 fork 相对该目标还缺什么，不是实机已经启用。

状态：源码已核对、待验证、缺失。硬件行全部待验证。优先级沿用需求：高 = 主路径，中 = 上传/配置/恢复，低 = 其他板或明确不扩展的项。

| 功能 | 官方目标 | 源码位置 | 编译开关 | 硬件依赖 | Scout Lite 现状 | 状态 | 优先级 | 备注 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 模组、天线、USB-C | 需求中的设计目标 | 无 Scout Lite 原理图 | 无 | 整板 | 无板型宏，仓库未记载该模组 | 待验证 | 高 | |
| Flash 容量 | workflow 写 DevKit 8M | `build_parallel.yml:46` | FQBN `FlashSize` | Flash | 只有构建字符串 | 待验证 | 高 | |
| PSRAM 容量 | FQBN `PSRAM=enabled` | `build_parallel.yml:46` | 板包 `PSRAM` | PSRAM | 固件 `HAS_PSRAM` 在 DevKit 关闭 | 待验证 | 高 | 见 `HARDWARE.md` |
| 分区 | `PartitionScheme=default_8MB` | `build_parallel.yml:46` | FQBN | Flash | 仓库内无 DevKit 分区 CSV | 待验证 | 高 | 能否刷官方 bin 待验证 |
| GPS 引脚 | `GPS_TX 14` / `GPS_RX 13` | `configs.h:2975-2978` | `HAS_GPS` | GNSS、UART | 宏在官方目标里，Scout Lite 未对照 | 待验证 | 高 | |
| microSD 引脚 | CS 10，MISO 2，MOSI 7，SCK 6 | `configs.h:2706-2707`、`3212-3215` | `HAS_SD`、`HAS_C5_SD` | microSD | 初始化代码在官方目标里 | 待验证 | 高 | |
| Flipper UART | `Serial.begin(115200)` | `esp32_marauder.ino:244` | `FLIPPER_ZERO_HAT` 未开 | Flipper | 无 SigRoam 源码，无 Scout Lite 引脚 | 待验证 | 高 | |
| 2.4/5 GHz | `HAS_DUAL_BAND` 的 C5 目标 | `WiFiScan.h:930`；`WiFiScan.cpp:6244-6260` | `HAS_DUAL_BAND` | 射频 | 官方 DevKit 宏打开；Scout Lite 未单独配置 | 源码已核对 | 高 | 实机 5 GHz 待验证。无 `scan_dwell` |
| Wardrive 日志 | 有 `HAS_GPS` 的目标 | `CommandLine.cpp:886-894`；`WiFiScan.cpp:6778`；`Buffer.cpp:19-22` | `HAS_GPS`、`HAS_SD` | GPS、SD | 官方路径是 `/wardrive_<n>.log` | 源码已核对 | 高 | FAP 行为待验证 |
| BLE | 各 C5 目标 | `WiFiScan.h:16-17`；`configs.h:533` | `HAS_BT`、`HAS_NIMBLE_2` | 射频 | 官方 DevKit 宏打开 | 源码已核对 | 高 | 实机待验证 |
| GPS 状态 | DevKit、XIAO、T-Dongle、v8、Pancake、Mini v3 | `GpsInterface.cpp:636-645` | `HAS_GPS` | GNSS | 官方有 Yes/No 字符串 | 源码已核对 | 高 | 定位待验证 |
| SD 失败路径 | 有 `HAS_SD` 的 C5 目标 | `SDInterface.cpp:166-173`；`lang_var.h:169` | `HAS_SD` | microSD | 失败时 `supported=false` | 源码已核对 | 高 | 无卡会不会挂死待验证 |
| WiGLE / WDGWars | 有 `HAS_DIRECT_UPLOAD` 的目标 | `WiFiScan.cpp:11843`、`11901`、`12092` | `HAS_DIRECT_UPLOAD`、`HAS_SD` | SD、网络 | 命令名 `upload` 在；无 `UploadWigle.cpp` | 源码已核对 | 中 | 实网上传待验证 |
| Wi-Fi 配置 | 共用 settings | `settings.h:22` | 无板级开关 | SPIFFS | 最多 5 组 | 源码已核对 | 中 | 掉电保持待验证 |
| `info` / 内存 / SPIFFS 恢复 | 共用；电池段要 `HAS_BATTERY` | `WiFiScan.cpp:4888`；`MemoryGuard.h:12`；`SDInterface.cpp:228` | DevKit 无 `HAS_BATTERY` | UART、SD | 官方代码在；Scout Lite 无单独恢复层 | 源码已核对 | 中 | 实机待验证 |
| CLI | 含无屏的 `MARAUDER_C5` | `CommandLine.h` | 无 | UART | 命令名在；无 Scout Lite 扩展 | 源码已核对 | 高 | SigRoam 待验证 |
| SigRoam FAP | 无 | 无 | 无 | Flipper | 本仓库没有 | 缺失 | 高 | |
| Scout Lite board profile | 无 | 无 `SCOUT` 宏 | 无 | 板级 | 尚未开始 | 缺失 | 高 | 不要先造 BSP |
| WebUI | 研究报告写在 Dual Mini | `CommandLine.h:14` 为注释 | 无 | 无 | 本树没有 `Web.cpp` | 缺失 | 低 | `DUAL_MINI_C5` 只是再定义 `MARAUDER_MINI_V3`（`configs.h:144-146`） |
| POOM GPS | `MARAUDER_POOM` | `configs.h:644-658` | 未定义 `HAS_GPS` | GNSS | 该官方目标缺失 GPS | 缺失 | 低 | 比较对象 |
| XIAO SD | `XIAO_ESP32_C5` | `configs.h:682-685` | `#undef HAS_SD` | SD | 该官方目标去掉 SD | 缺失 | 低 | |
| v8 / Mini v3 公开 CI | 源码有宏 | `configs.h:571-641` | `-DMARAUDER_V8` / `-DMARAUDER_MINI_V3` | 屏 | 公开 `build_parallel.yml` 不编 | 源码已核对 | 低 | 私有 workflow 是否发布二进制待验证 |
| Pancake 屏和按键 | `MARAUDER_PANCAKE` | `configs.h:595-606` | `HAS_SCREEN`、`HAS_BUTTONS` | 屏、按键 | 宏是打开的 | 源码已核对 | 低 | 研究报告「缺屏缺键」与宏不一致。实机待验证 |

## Offensive

只记名称、源文件、编译开关。

- deauth：`esp32_marauder/WiFiScan.cpp`，`esp32_marauder/CommandLine.h`（`ATTACK_TYPE_DEAUTH`）。无 C5 排除宏。
- beacon / probe / funny / rickroll / badmsg / sleep / sae / csa / quiet：`CommandLine.h` 的 `ATTACK_TYPE_*`，`WiFiScan.cpp`。无 C5 排除宏。
- evil portal：`esp32_marauder/EvilPortal.cpp`，`CommandLine.h`（`EVIL_PORTAL_CMD`）。无 C5 排除宏。`HAS_PSRAM` 只影响 `MAX_HTML_SIZE`。
- 凭据收集：`esp32_marauder/EvilPortal.cpp`。无单独板级宏。
- karma：`CommandLine.h`（`KARMA_CMD`），`CommandLine.cpp`。无 C5 排除宏。
- blespam / spoofat：`CommandLine.h`，`WiFiScan.cpp`。开关 `HAS_BT`；`MARAUDER_C5` 定义了它。
