# 最小改动计划

日期：2026-10-16。对照源码：`f4bc76727e7f29b7ca64da9c972c9103523c6006`。

本轮不改固件，不实现下面任何一项，不刷机，不发版。

结论：板级宏表里仍然没有「硬件已经证实、改一行宏就能打开」的项。函数级对照在 `CODE_GAP.md`。下面第 1b 节只列已经在源码里对上的缺口。本轮不改这些函数。SigRoam 仍超出范围。不把研究报告里的「可以直接刷」或未核对 GPIO 写进章程。

## 1. 继续使用官方 `MARAUDER_C5`

- 宏或文件：构建 flag `-DMARAUDER_C5`。不新增 `SCOUT` 板型宏。不改 `esp32_marauder/configs.h` 的 `529-548`。
- 硬件：SD IO2/IO6/IO7/IO10 与 `SD_CS 10`、`SD_MISO 2`、`SD_MOSI 7`、`SD_SCK 6`（`configs.h:2706-2707`、`3212-3215`）一致。GPS GPIO14 RX / GPIO13 TX 与 `GPS_TX 14`、`GPS_RX 13`（`2975-2978`）一致。原理图确认，实机未测。
- 不要复制：任何其他 C5 目标的整段 feature block、屏驱、`User_Setup_*.h`、分区 CSV。
- 怎么测：用 `.github/workflows/scout-lite-ci.yml` 的同一 FQBN 重编未修改的 `MARAUDER_C5`。记录在 `INTERNAL_BETA.md`。不刷机。

## 1b. 阶段

证据在 `CODE_GAP.md`。不改 `executeWarDrive()` 的频道表。

Phase 1，本提交：

- 无屏 CLI `upload` 调用 `uploadFile(..., false, ...)`，已有 `.wigle` / `.wdg` 就跳过。没有强制重传开关，不加新命令。`MenuFunctions.cpp` 仍传 `true`，`HAS_SCREEN` 包住，`MARAUDER_C5` 不编。
- `joinSavedWiFi()` 之前记住 `currentScanMode`。每个文件都成功，且之前是 `WIFI_SCAN_WAR_DRIVE`，才 `startScanFromCLI(WIFI_SCAN_WAR_DRIVE)`。否则 `StartScan(WIFI_SCAN_OFF)`。有文件失败就不恢复。
- 同一个文件失败后再试 2 次，一共 3 次，然后下一个文件。这不是 `retry` 参数。

Phase 2，现在不做：SSID 排除表；无屏删除日志的 CLI；无 fix 时已 `save_mac()`（`WiFiScan.cpp:6324`，旁路 `9232`），之后有 fix 也不会再记该 MAC。

Phase 3，现在不做：有限次的 Dock 触发连接。不搬整段 `runDockMode()`。

不做：重写扫描器、Node/Core、ESP-NOW、SigRoam、屏幕 UI。

## 2. `HAS_PSRAM`：现在不打开

这是其他 C5 目标相对 DevKit 唯一「看起来像一行宏」的功能差。动作仍是 `未知`，不是 `可最小打开`。

- 宏或文件：`configs.h:543` 的 `//#define HAS_PSRAM`。相关分支已经在树里：`esp32_marauder.ino:297-300` 的 `psramInit()`，`configs.h:2881-2884` 的 `MAX_HTML_SIZE`，`2891-2894` 的 `mac_history_len`，`3167-3175` 的 `BUF_SIZE` / `SNAP_LEN`，`utils.h:47-61` 的 `getPSRAMUsagePercent()`。
- 硬件：丝印 `ESP32-C5-WROOM-1U-N8R8` 不是运行时测量。FQBN `PSRAM=enabled` 只增加 `BOARD_HAS_PSRAM`，不会定义 `HAS_PSRAM`。
- 不要复制：XIAO、T-Dongle、V8、Pancake、Mini v3、POOM 的 feature block。不要把它们的 `BUF_SIZE` 数字再抄一份。现有 `#ifdef HAS_PSRAM` 已经分叉。
- 怎么测：先在实机读到 PSRAM 容量，再考虑只取消这一行注释。取消前后各编一次同一 FQBN，不刷客户机。没有这次读数之前，不改这一行。

## 3. 屏幕：不移植

- 宏或文件：`HAS_SCREEN`、`HAS_FULL_SCREEN`、`HAS_MINI_SCREEN`、`HAS_ILI9341`、`HAS_T_DONGLE_DISPLAY`。文件包括 `MenuFunctions`、`Display`、`TDongleDisplay.cpp`、`User_Setup_marauder_pancake.h`、`User_Setup_marauder_t_dongle_c5.h`，以及 `WiFiScan.cpp` 里 packet monitor 图（`5003-5007`、`10626-10628`）。Mini v3 屏段是 `configs.h:2211-2223`。
- 硬件：Scout Lite 没有屏幕，也没有 T-Dongle 屏。
- 不要复制：整份 TFT 初始化、菜单、背光、`TFT_CS 27`（与 C5 NeoPixel 的 `PIN 27` 相同）、T-Dongle 的 SPI 脚（与 SD 的 IO2/IO6/IO7/IO10 冲突）。
- 怎么测：不移植，所以没有屏的测试项。

## 4. 按键、小键盘、触摸、Flipper 下载：不移植

- 宏或文件：`HAS_BUTTONS`（Pancake `602`，Mini v3 `625`，POOM `646`）、`HAS_MINI_KB`（Mini v3 `628`，POOM `647`）、`HAS_TOUCH` / `HAS_CAP_TOUCH`（V8 `572`，Pancake `596-597`）。按键脚在 `871-876`、`971-984`、`999-1017`。
- 硬件：板上只有 BOOT 和 EN。Flipper 不能进入下载模式。没有键盘，没有触摸。
- 不要复制：五向键、POOM 的 `B_BTN`、Pancake 的 `C_BTN 28`、电容触摸的 `CTP_RST`。不要写让 Flipper 拉 BOOT/EN 或进入下载模式的固件。
- 怎么测：不移植，所以没有按键或 Flipper 下载的测试项。

## 5. 灯：保持 C5 的 NeoPixel 宏，不改成别的板

- 宏或文件：C5 已有 `HAS_NEOPIXEL_LED`（`535`）和 `PIN 27`（`2860-2861`）。不要改。不要加上 `XIAO_LED_PIN`（`526`）、`HAS_T_DONGLE_LED`（`553-555`）、`HAS_ACT_LED`（`591`，`ACT_LED_PIN 28` 在 `3337-3338`）。
- 硬件：NeoPixel 是否在 Scout Lite 上，原理图确认项没写，所以是未知；这不构成改宏的理由。XIAO 灯是另一块模块。T-Dongle 的灯脚 2 和 6 就是 SD 的 MISO 和 SCK。GPIO28 活动灯没有确认。
- 不要复制：`xiaoLED.cpp`、T-Dongle LED 初始化、V8 的 `ACT_LED_PIN`。
- 怎么测：不改宏。若以后原理图写明 GPIO27 被别的功能占用，再单独立项，本计划不预做。

## 6. 电池：不移植

- 宏或文件：`HAS_BATTERY`。V8 的 I2C 在 `3056-3058`。Pancake 的 I2C 在 `3067-3070`，并和触摸共用。`BatteryInterface` 只在这个宏下编译（`esp32_marauder.ino:369-371`）。
- 硬件：没有已确认的电量计。动作是未知，不是可以打开。
- 不要复制：V8 或 Pancake 的 I2C 脚、`HAS_MAX1704X`、触摸共用总线。
- 怎么测：没有接线证据之前不编带 `HAS_BATTERY` 的固件。

## 7. SD 与 GPS 脚：不改

- 宏或文件：保持第 1 节的 C5 脚。不要改成 T-Dongle 的 GPS 12/11（`2967-2970`）或 SD CS 23，不要改成 XIAO 的 GPS 12/11（`2971-2974`），不要改成 Pancake 的 `SD_CS 7`，不要改成 POOM 的 SD 脚，不要引入 Mini v3 屏段里的 `SD_CS 4`。
- 硬件：原理图已经与 C5 对齐。
- 不要复制：上面任一板的引脚块。Flipper pin 15/16 超出范围，不写固件改动。
- 怎么测：不改宏。实机 GPS/SD 仍是未测，测试要等有日志的任务，不在本计划里改代码。

## 8. 分区：不换成 T-Dongle

- 宏或文件：保持 `esp32:esp32:esp32c5:FlashSize=8M,PartitionScheme=default_8MB,PSRAM=enabled`。不要改用 `installer/partitions/t_dongle_c5.csv` 或 `FlashSize=16M,PartitionScheme=custom`（`build_parallel.yml:48`）。
- 硬件：Flash 容量未知。构建字符串不是容量测量。
- 不要复制：T-Dongle 的 16MB 表，也不要为 Scout Lite 新写分区 CSV。
- 怎么测：不改分区。内部 beta 只用上面的官方 DevKit FQBN。

## 9. 明确超出范围，没有代码项

- SigRoam 优化、FAP 合约测试、SigRoam 兼容。不添加测试桩，不改 CLI 去迁就 FAP。
- WebUI。`CommandLine.h:14` 仍是注释。不从树外找网页源码。
- USB 自动切换。不调查，不改 USB 相关脚。

## 本轮验证

只重编未修改的 `MARAUDER_C5`。命令、commit、app bin SHA256 写在 `INTERNAL_BETA.md`。该 bin 不是功能移植，不提交进 git，不上传 Release，不刷机。
