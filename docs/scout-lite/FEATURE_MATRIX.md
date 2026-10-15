# 官方 Marauder 与 Scout Lite 差距

基线 commit：`d3ceb28afeda1196ca27c13adeaa69ada52383f3`。

章程枚举对照（只加在表头，不改下面每一行的中文状态）：

- 已验证 → 仅构建或自动化测试（E2）
- 源码支持 → `SOURCE_PRESENT_UNTESTED`（E1）
- 未知 → `UNKNOWN`

`需适配` 和 `硬件受限` 仍是本表的中文标签。本表不新增 `CONFIRMED_WORKING`、`INTEGRATION_BLOCKED`、`NOT_APPLICABLE`。

Scout Lite 还没有板型宏。官方默认对照目标是 `MARAUDER_C5`。「Scout Lite 现状」不是实机已经启用。

状态只允许这五个词：

- 已验证：只有 `d3ceb28` 上记录过的 native 134/134，以及同一次 C5 编译（app bin SHA256 `1d572f57bb1f90cf063952672414413e194ee7aef00de1c66218238dc26b60a8`）。没有硬件功能用这个词。
- 源码支持：宏或代码在这棵树里，没有板级测试。
- 需适配：已经在这棵树里看到的具体源码缺口。不是从别的板型搬代码。
- 硬件受限：这棵树证明官方 C5 目标做不到。
- 未知：其余情况。引脚、Flash/PSRAM 容量、能否刷 DevKit 二进制、SigRoam 都是未知。

本表没有「需适配」行，也没有「硬件受限」行。官方 `MARAUDER_C5` 的宏开着的功能，源码在；树没有证明这块目标做不到某项。因此下一份代码 PR 还没有矩阵依据。

| 功能 | 官方目标 | 源码位置 | 编译开关 | 硬件依赖 | Scout Lite 现状 | 状态 | 优先级 | 备注 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| native 单元测试 | `env:native` | `test/`，`platformio.ini` | `platformio test -e native` | 无 | `d3ceb28` 上 134/134，退出码 0 | 已验证 | 高 | 不覆盖射频或整机 |
| `MARAUDER_C5` 编译 | ESP32-C5-DevKitC-1 | `.github/workflows/build_parallel.yml:46` | `-DMARAUDER_C5` | 无 | 同 commit 上 `arduino-cli compile` 退出码 0 | 已验证 | 高 | SHA256 见上。未刷机。`scout-lite-ci.yml` 未在 GitHub 跑过 |
| 模组、天线、USB-C | 需求中的设计目标 | 无 Scout Lite 原理图 | 无 | 整板 | 无板型宏 | 未知 | 高 | |
| Flash 容量 | workflow 写 DevKit `FlashSize=8M` | `build_parallel.yml:46` | FQBN `FlashSize` | Flash | 只有构建字符串 | 未知 | 高 | |
| PSRAM 容量 | FQBN `PSRAM=enabled` | `build_parallel.yml:46` | 板包 `PSRAM` | PSRAM | 固件宏 `HAS_PSRAM` 在 DevKit 段被注释 | 未知 | 高 | 容量未测。见 `HARDWARE.md` |
| 分区 / 刷官方 bin | `PartitionScheme=default_8MB` | `build_parallel.yml:46` | FQBN | Flash | 仓库内无 DevKit 分区 CSV，未刷机 | 未知 | 高 | |
| GPS 引脚 | `GPS_TX 14` / `GPS_RX 13` | `configs.h:2975-2978` | `HAS_GPS` | GNSS、UART | 宏在官方目标里 | 未知 | 高 | 引脚不是已核实接线 |
| microSD 引脚 | CS 10，MISO 2，MOSI 7，SCK 6 | `configs.h:2706-2707`、`3212-3215` | `HAS_SD`、`HAS_C5_SD` | microSD | 初始化调用在 `esp32_marauder.ino:255-256` | 未知 | 高 | |
| Flipper UART | `Serial.begin(115200)` | `esp32_marauder.ino:244` | `FLIPPER_ZERO_HAT` 未开 | Flipper | 无 Scout Lite 引脚 | 未知 | 高 | 115200 只是字面量 |
| SigRoam FAP | 无 FAP 源码 | 无 | 无 | Flipper | 本仓库没有 FAP | 未知 | 高 | 兼容性未测 |
| 2.4/5 GHz | 定义了 `HAS_DUAL_BAND` 的 C5 目标 | `WiFiScan.h:930`；`WiFiScan.cpp:6244-6260` | `HAS_DUAL_BAND` | 射频 | `MARAUDER_C5` 打开该宏 | 源码支持 | 高 | 实机能否扫到 5 GHz 是未知。无 `scan_dwell` |
| Wardrive 日志 | 有 `HAS_GPS` 的目标 | `CommandLine.cpp:886-894`；`WiFiScan.cpp:6778`；`Buffer.cpp:19-22` | `HAS_GPS`、`HAS_SD` | GPS、SD | 路径形态是 `/wardrive_<n>.log` | 源码支持 | 高 | |
| BLE | 各 C5 目标 | `WiFiScan.h:16-17`；`configs.h:533` | `HAS_BT`、`HAS_NIMBLE_2` | 射频 | `MARAUDER_C5` 打开 `HAS_BT` | 源码支持 | 高 | |
| GPS 状态字符串 | DevKit 以及同样定义 `HAS_GPS` 的 C5 目标 | `GpsInterface.cpp:636-645` | `HAS_GPS` | GNSS | Yes/No 字符串在 | 源码支持 | 高 | 有没有 fix 是未知 |
| SD 失败路径 | 有 `HAS_SD` 的 C5 目标 | `SDInterface.cpp:166-173`；`lang_var.h:169` | `HAS_SD` | microSD | 失败时 `supported=false` | 源码支持 | 高 | 无卡会不会挂死是未知 |
| WiGLE / WDGWars | 有 `HAS_DIRECT_UPLOAD` 的目标 | `WiFiScan.cpp:11843`、`11901`、`12092` | `HAS_DIRECT_UPLOAD`、`HAS_SD` | SD、网络 | 命令名 `upload` 在。没有 `UploadWigle.cpp` | 源码支持 | 中 | 实网上传是未知 |
| Wi-Fi 配置 | 共用 settings | `settings.h:22` | 无板级开关 | SPIFFS | 最多 5 组 | 源码支持 | 中 | 掉电后还在不在是未知 |
| `info` / 内存 / SPIFFS 恢复 | 共用。电池段要 `HAS_BATTERY` | `WiFiScan.cpp:4888`；`MemoryGuard.h:12`；`SDInterface.cpp:228` | DevKit 未定义 `HAS_BATTERY` | UART、SD | 官方代码在，没有 Scout Lite 单独层 | 源码支持 | 中 | |
| CLI 命令名 | 含无屏的 `MARAUDER_C5` | `CommandLine.h` | 无 | UART | `help`、`info`、`wardrive`、`stopscan`、`gpsdata`、`upload` 在 | 源码支持 | 高 | 与 SigRoam 是否一致是未知 |
| Scout Lite 板型宏 | 无 | `configs.h` 无 `SCOUT` | 无 | 板级 | 尚未添加 | 未知 | 高 | 引脚未知，不能靠这份矩阵开代码 PR |
| WebUI | 研究报告写在 Dual Mini | `CommandLine.h:14` 为注释 | 无 | 无 | 本树没有 `Web.cpp` | 未知 | 低 | 不是已证实的 C5 缺口。`DUAL_MINI_C5` 只是再定义 `MARAUDER_MINI_V3`（`configs.h:144-146`） |
| POOM 的 GPS | `MARAUDER_POOM` | `configs.h:644-658` | 该目标未定义 `HAS_GPS` | GNSS | 只说明这个比较目标 | 未知 | 低 | 不能当成官方 C5 做不到 GPS |
| XIAO 的 SD | `XIAO_ESP32_C5` | `configs.h:682-685` | `#undef HAS_SD` | SD | 只说明这个比较目标 | 未知 | 低 | `MARAUDER_C5` 仍有 `HAS_SD` |
| v8 / Mini v3 宏 | 源码有宏 | `configs.h:571-641` | `-DMARAUDER_V8` / `-DMARAUDER_MINI_V3` | 屏 | 公开 `build_parallel.yml` 不编这两个 flag | 源码支持 | 低 | 私有构建是否发布二进制是未知 |
| Pancake 屏和按键宏 | `MARAUDER_PANCAKE` | `configs.h:595-606` | `HAS_SCREEN`、`HAS_BUTTONS` | 屏、按键 | 宏是打开的 | 源码支持 | 低 | 研究报告「缺屏缺键」与宏不一致。实机是未知 |

## Offensive

只记名称、源文件、编译开关。状态都是源码支持。没有步骤和参数。

- deauth：`esp32_marauder/WiFiScan.cpp`，`esp32_marauder/CommandLine.h`（`ATTACK_TYPE_DEAUTH`）。无 C5 排除宏。
- beacon / probe / funny / rickroll / badmsg / sleep / sae / csa / quiet：`CommandLine.h` 的 `ATTACK_TYPE_*`，`WiFiScan.cpp`。无 C5 排除宏。
- evil portal：`esp32_marauder/EvilPortal.cpp`，`CommandLine.h`（`EVIL_PORTAL_CMD`）。无 C5 排除宏。`HAS_PSRAM` 只影响 `MAX_HTML_SIZE`。
- 凭据收集：`esp32_marauder/EvilPortal.cpp`。无单独板级宏。
- karma：`CommandLine.h`（`KARMA_CMD`），`CommandLine.cpp`。无 C5 排除宏。
- blespam / spoofat：`CommandLine.h`，`WiFiScan.cpp`。开关 `HAS_BT`；`MARAUDER_C5` 定义了它。
