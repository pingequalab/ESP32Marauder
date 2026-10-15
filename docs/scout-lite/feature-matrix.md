# 功能差异及优先级

基线 commit：`d3ceb28afeda1196ca27c13adeaa69ada52383f3`。比较对象是本仓库里的官方 C5 目标，不是已刷进硬件的二进制。

状态只用三档：源码已核对、待验证、缺失。硬件行全部是待验证。优先级按需求里的功能完整性和「先用官方实现」：高 = 需求第 4 节 A 的主路径；中 = 上传、配置、恢复；低 = 其他板型或本轮明确不扩展的项。

| 功能 | 官方目标 | 源码位置 | 编译开关 | 硬件依赖 | 本仓库现状 | 状态 | 优先级 | 备注 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 模组 ESP32-C5-WROOM-1U-N8R8 | 需求中的 Scout Lite；源码目标名是 ESP32-C5 DevKit | 无 Scout Lite 原理图 | 无 | 模组 | 仓库未记载该模组 | 待验证 | 高 | `schematics/` 不是 Scout Lite |
| Flash 容量 | workflow 写 DevKit/XIAO/Pancake/POOM 为 8M，T-Dongle 为 16M | `.github/workflows/build_parallel.yml:46-51` | FQBN `FlashSize` | Flash | 只看到构建字符串 | 待验证 | 高 | 容量数字是板包选项，不是实测 |
| PSRAM 容量 | FQBN `PSRAM=enabled` | `build_parallel.yml:46` | 板包 `PSRAM` | PSRAM | `MARAUDER_C5` 的 `HAS_PSRAM` 被注释 | 待验证 | 高 | 宏状态见下一行；容量待验证 |
| `HAS_PSRAM` 固件宏 | `MARAUDER_C5` 关闭；XIAO、T-Dongle、v8、Pancake、Mini v3、POOM 打开 | `esp32_marauder/configs.h:523`、`543`、`565`、`587`、`612`、`636`、`655` | `HAS_PSRAM` | 无直接引脚 | DevKit 宏关闭，因此 `mac_history_len` 100、`BUF_SIZE` 3*1024 | 源码已核对 | 高 | core 3.3.4 的 `PSRAM.enabled` 只加 `-DBOARD_HAS_PSRAM`。芯片容量〔待验证〕 |
| 分区与 8MB 布局 | DevKit/Pancake/POOM/XIAO 用 `PartitionScheme=default_8MB`；T-Dongle 用仓库 CSV | `build_parallel.yml:46`；`installer/partitions/t_dongle_c5.csv:1` | FQBN | Flash 分区 | 仓库内没有 DevKit 的分区 CSV | 待验证 | 高 | 能否刷官方 bin〔待验证〕 |
| GPS 模组与引脚 | `MARAUDER_C5` 宏：index 1，`GPS_TX 14`，`GPS_RX 13` | `esp32_marauder/configs.h:2975-2978`；`GpsInterface.cpp:11`、`20` | `HAS_GPS` | L86、UART | 宏存在 | 待验证 | 高 | 有宏也不等于 Scout Lite 接线 |
| microSD 引脚 | `MARAUDER_C5`：CS 10，MISO 2，MOSI 7，SCK 6 | `configs.h:2706-2707`、`3212-3215`；`esp32_marauder.ino:255-256` | `HAS_SD`、`HAS_C5_SD` | microSD、SPI | 初始化代码随这些宏编译 | 待验证 | 高 | 卡槽接线未对照原理图 |
| 外置天线 / USB-C | 需求硬件列表 | 无 | 无 | 射频、USB | 仓库无对应板级描述 | 待验证 | 高 | |
| Flipper UART 引脚与波特率 | 源码只有 `Serial.begin(115200)` | `esp32_marauder.ino:244` | 无 Flipper 宏；`FLIPPER_ZERO_HAT` 在 C5 为注释 | Flipper GPIO | 无 `SERIAL_EN` / `PIN_SERIAL_*`；无 SigRoam 源码 | 待验证 | 高 | 115200 只是源码字面量 |
| 能否刷官方 DevKit 固件 | 研究报告主张可以直接刷 | 无刷机记录 | `MARAUDER_C5` | 整板 | 本轮不刷机 | 待验证 | 高 | |
| 2.4/5 GHz 信道表 | 定义了 `HAS_DUAL_BAND` 的 C5 目标 | `WiFiScan.h:930`；`WiFiScan.cpp:6244-6260` | `HAS_DUAL_BAND` | Wi-Fi 射频 | DevKit、XIAO、T-Dongle、Pancake、POOM、v8、Mini v3 的宏都打开 | 源码已核对 | 高 | 实机 5 GHz〔待验证〕。没有 `scan_dwell` |
| Wardrive 日志 | `MARAUDER_C5` 等有 GPS 的目标 | `CommandLine.cpp:886-894`；`WiFiScan.cpp:6778`；`Buffer.cpp:19-22` | `HAS_GPS`、`HAS_SD` | GPS、SD | 扩展名是 `.log`，带 WiGLE 头；无 fix 不写入 | 源码已核对 | 高 | 文件内容与 FAP 行为〔待验证〕 |
| BLE | 各 C5 目标 | `WiFiScan.h:16-17`；`configs.h:533` | `HAS_BT`、`HAS_NIMBLE_2` | BLE 射频 | 宏打开，NimBLE 包含在 `HAS_BT` 内 | 源码已核对 | 高 | 实机〔待验证〕 |
| GPS 定位状态 | `MARAUDER_C5`、XIAO、T-Dongle、v8、Pancake、Mini v3 | `GpsInterface.cpp:636-645` | `HAS_GPS` | GNSS | `getFixStatusAsString` 为 Yes/No | 源码已核对 | 高 | POOM 缺失，见下。定位〔待验证〕 |
| POOM GPS | `MARAUDER_POOM` | `configs.h:644-658` | 未定义 `HAS_GPS` | GNSS | 该目标不编 GPS；`wardrive` 会编译出 `"GPS not supported"` | 缺失 | 低 | 比较对象，不是 Scout Lite 基线 |
| XIAO SD | `XIAO_ESP32_C5` | `configs.h:682-685` | `#undef HAS_SD` | 无 SD 座的板级假设 | 源码明确去掉 SD | 缺失 | 低 | 「小板没有 SD」仍要等原理图〔待验证〕 |
| SD 文件与失败路径 | 有 `HAS_SD` 的 C5 目标 | `SDInterface.cpp:166-173`；`lang_var.h:169` | `HAS_SD`、`HAS_C5_SD` | microSD | 失败时 `supported=false`，`info` 使用 `"SD Card: Not Connected"` | 源码已核对 | 高 | 无卡会不会挂死〔待验证〕 |
| WiGLE / WDGWars 上传 | 有 `HAS_DIRECT_UPLOAD` 的目标 | `WiFiScan.cpp:11843`、`11901`、`12092`；`CommandLine.h:123` | `HAS_DIRECT_UPLOAD`、`HAS_SD` | SD、可上网的 Wi-Fi | 函数与命令名在；XIAO 特征段没有 `HAS_DIRECT_UPLOAD` | 源码已核对 | 中 | 证书、重试、实网上传〔待验证〕。没有 `UploadWigle.cpp` |
| 上传 sidecar | 同上 | `WiFiScan.cpp:11828` | 同上 | SD | 写 `filePath.wigle` 或 `filePath.wdg` 这种后缀 | 源码已核对 | 中 | 是否等于「文件已标记」的产品语义〔待验证〕 |
| 保存的 Wi-Fi 配置 | 全目标共用 settings | `settings.h:22`、`83-86`；`configs.h:56-58` | 无板级开关 | SPIFFS | 最多 5 组；`join` / `save` / `load` 命令名在 | 源码已核对 | 中 | 掉电后是否仍在〔待验证〕 |
| `info` 状态 | 全目标 | `WiFiScan.cpp:4888-4955`；`CommandLine.h:112` | 无；电池段要 `HAS_BATTERY` | UART | 打印版本、硬件名、分区、IDF、MAC、SD。C5 DevKit 无电池段 | 源码已核对 | 中 | 串口输出〔待验证〕 |
| 内存守卫 | 全目标 | `MemoryGuard.h:12-17`；`WiFiScan.cpp:2220` | 无 | RAM | 主机测试 `test_memory_guard` 覆盖阈值逻辑 | 源码已核对 | 中 | 堆曲线〔待验证〕。`info` 不打印 free heap |
| SPIFFS 备份/恢复 | `HAS_SD` | `CommandLine.h:62-64`；`SDInterface.cpp:228`、`254` | `HAS_SD` | SD | 命令名和 `migrateSPIFFS` 在 | 源码已核对 | 中 | 断电恢复〔待验证〕 |
| CLI 命令名 | 全目标，含无屏的 `MARAUDER_C5` | `CommandLine.h`；`esp32_marauder.ino:244` | 无屏不影响 CLI 编译 | UART | `help`、`wardrive`、`stopscan`、`gpsdata`、`upload`、`info` 等名字在 | 源码已核对 | 高 | 行为与 SigRoam〔待验证〕 |
| SigRoam FAP | 无对应源码 | 无 | 无 | Flipper | 本仓库没有 FAP | 缺失 | 高 | 兼容性〔待验证〕。不能靠加攻击功能来补 |
| WebUI | 研究报告写在 Dual Mini | `CommandLine.h:14` 为注释；无 `Web.cpp` | 无 | 屏或网口 | 本树没有启用的 Web 源文件 | 缺失 | 低 | `DUAL_MINI_C5` 只是 Mini v3 的别名〔`configs.h:144-146`〕 |
| v8 / Mini v3 公开固件 | 源码有宏 | `configs.h:571-641` | `-DMARAUDER_V8` / `-DMARAUDER_MINI_V3` | 屏、按键 | 公开 `build_parallel.yml` 不编，走私有 workflow | 源码已核对 | 低 | 二进制是否随 release 发布〔待验证〕 |
| Pancake 屏和按键 | `MARAUDER_PANCAKE` | `configs.h:595-606` | `HAS_SCREEN`、`HAS_BUTTONS`、`HAS_TOUCH` | 屏、触摸、按键 | 宏是打开的，不是「缺屏缺键」 | 源码已核对 | 低 | 与研究报告相反；实机〔待验证〕 |
| XIAO Flash 4MB | 研究报告表 | `build_parallel.yml:47` | FQBN `FlashSize=8M` | Flash | workflow 写的是 8M | 源码已核对 | 低 | 研究报告的 4MB〔待验证〕，且与 FQBN 不一致 |
| Offensive 功能 | 随 sketch 编译 | 见 `source-audit.md` 同名节 | 无 C5 排除宏；BLE 项要 `HAS_BT` | 射频 | 只记录名称 | 源码已核对 | 低 | 不写步骤。不作为功能数量目标 |
