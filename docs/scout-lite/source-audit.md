# 源码审计

核对对象：worktree `scout-lite/baseline-audit`，commit `d3ceb28afeda1196ca27c13adeaa69ada52383f3`（`Merge pull request #1609 from justcallmekoko/develop`）。这是 `justcallmekoko/ESP32Marauder` 的 `master`，也是本 fork 当前 `master`。

这不是 tag `v1.18.0`。上游 tag `v1.18.0` 指向 `b2b9f92382eb60b39b8741be3b567e416cdbe672`，是本 commit 的祖先，中间还有 10 个 commit。源码里的版本字符串两边都是 `v1.18.0`〔源码已核对：`esp32_marauder/configs.h:60`；tag 上同一宏在该 tag 的 `configs.h:52`〕。版本字符串不是 tag 指针。tag 核对命令见 `build-verification.md`。

本仓库没有 `SCOUT`、`PINGEQUA`、`Scout Lite` 字符串。没有 Scout Lite 原理图。`schematics/` 只有 2019 年 ESP32 Marauder 2 文件。下面凡是引脚、分区字节、能否刷机、Flipper / SigRoam 实机，状态都是〔待验证〕。

`platformio.ini` 只有 `[env:native]`。文件里没有 `MARAUDER_C5`。

## 构建入口

C5 固件入口是 Arduino，不是 PlatformIO env。

公开矩阵 `.github/workflows/build_parallel.yml` 用 `arduino-cli`，板 flag 经 `--build-property compiler.cpp.extra_flags='-D<flag>'` 传入〔源码已核对：`.github/workflows/build_parallel.yml:268-275`〕。`configs.h` 里所有板型 `#define` 默认都是注释〔源码已核对：`esp32_marauder/configs.h:15-46`〕。

公开 C5 FQBN（core 版本字符串都是 `3.3.4`）：

| flag | workflow 名称 | FQBN 要点 | 产物名 | 位置 |
| --- | --- | --- | --- | --- |
| `MARAUDER_C5` | ESP32-C5-DevKitC-1 | `esp32c5:FlashSize=8M,PartitionScheme=default_8MB,PSRAM=enabled` | `esp32c5devkitc1` | `build_parallel.yml:46` |
| `XIAO_ESP32_C5` | Seeed XIAO ESP32-C5 | `XIAO_ESP32C5:...FlashSize=8M,PartitionScheme=default_8MB,PSRAM=enabled` | `xiao_esp32c5` | `build_parallel.yml:47` |
| `MARAUDER_T_DONGLE_C5` | LilyGo T-Dongle C5 | `FlashSize=16M,PartitionScheme=custom,PSRAM=enabled` | `t_dongle_c5` | `build_parallel.yml:48` |
| `MARAUDER_PANCAKE` | Marauder Pancake | 与 DevKit 相同 8M / `default_8MB` / PSRAM=enabled | `pancake` | `build_parallel.yml:50` |
| `MARAUDER_POOM` | POOM | 同上 | `poom` | `build_parallel.yml:51` |

`nightly_build.yml:105-109` 有 DevKit、XIAO、T-Dongle、POOM，没有 Pancake。

`MARAUDER_V8`、`MARAUDER_MINI_V3`、`DUAL_MINI_C5` 不在公开编译矩阵。`build_parallel.yml:336-367` 把这三个产物名交给 `justcallmekoko/TFT_eSPI_JCMK` 的 `private-c5-build.yml`。`nightly_build.yml:424-458` 只等待 `_v8.bin` 和 `_mini_v3.bin`。私有仓库本轮没有拉取，二进制是否存在〔待验证〕。

T-Dongle 在编译前把 `installer/partitions/t_dongle_c5.csv` 复制成 sketch 的 `partitions.csv`〔源码已核对：`build_parallel.yml:264-266`〕。DevKit 没有对应的仓库内分区 CSV。`default_8MB` 这个名字只出现在 workflow FQBN 里。分区内容是否等于 Scout Lite〔待验证〕。

native 测试入口是 `.github/workflows/firmware-unit-tests.yml:33`：`.venv/bin/platformio test -e native`。它不编 C5 固件。

## C5 目标宏

`HARDWARE_NAME`：DevKit 为 `"ESP32-C5 DevKit"`〔`configs.h:121-122`〕。

`MARAUDER_C5` 打开：`HAS_BT`、`HAS_NEOPIXEL_LED`、`HAS_GPS`、`HAS_C5_SD`、`HAS_SD`、`USE_SD`、`HAS_DUAL_BAND`、`HAS_NIMBLE_2`、`HAS_IDF_3`、`HAS_DIRECT_UPLOAD`〔`configs.h:533-547`〕。

同一段里保持注释、因此不生效：`HAS_FLIPPER_LED`、`FLIPPER_ZERO_HAT`、`HAS_BATTERY`、`HAS_BUTTONS`、`HAS_PWR_MGMT`、`HAS_SCREEN`、`HAS_PSRAM`、`HAS_TEMP_SENSOR`〔`configs.h:530-544`〕。

`XIAO_ESP32_C5` 若看到 `ARDUINO_XIAO_ESP32C5` 会自动定义〔`configs.h:52-54`〕。该目标有 GPS / BT / 双频 / `HAS_PSRAM`，然后无条件 `#undef HAS_C5_SD`、`HAS_SD`、`USE_SD`〔`configs.h:518-526`、`682-685`〕。

`DUAL_MINI_C5` 只追加 `#define MARAUDER_MINI_V3`〔`configs.h:144-146`〕。没有单独的 Web 服务器源文件。`CommandLine.h:14` 的 `#include "Web.h"` 是注释。

`MARAUDER_POOM` 有 BT、按键、屏、SD、双频、`HAS_PSRAM`、`HAS_DIRECT_UPLOAD`，没有 `HAS_GPS`〔`configs.h:644-658`〕。

## 2.4 / 5 GHz Wi-Fi

双频不是按芯片自动打开，而是 `HAS_DUAL_BAND`。上表里的 C5 目标都定义了它，包括没有 GPS 的 POOM 和没有 SD 的 XIAO。

信道表 `dual_band_channels[51]` 含 1–14 和 32–177〔源码已核对：`esp32_marauder/WiFiScan.h:930`，`DUAL_BAND_CHANNELS` 在 `configs.h:66`〕。Wardrive 在 `HAS_DUAL_BAND` 下使用另一张含 5 字信道号的数组〔`WiFiScan.cpp:6244-6260`〕，否则只有 1–14〔`6262-6268`〕。

扫描调用：双频 `WiFi.scanNetworks(true, true, false, 80, scan_channel)`，否则第四个参数是 `125`〔`WiFiScan.cpp:6418-6421`〕。`HOP_DELAY` 双频为 250，否则 1000〔`configs.h:3328-3331`〕。没有名为 `scan_dwell` 的符号。这些数字是源码字面量，射频上是否等于驻留时间〔待验证〕。

`HAS_PSRAM` 未定义时 `mac_history_len` 为 100，定义时为 500〔`configs.h:2891-2894`〕。因此按当前 `MARAUDER_C5` 宏，历史长度为 100。FQBN 另有 `PSRAM=enabled`，那是板包选项，不是这个宏。本轮装上的 core 3.3.4 里，该选项的编译定义是 `-DBOARD_HAS_PSRAM`，不是 `-DHAS_PSRAM`（安装树 `boards.txt` 的 `esp32c5.menu.PSRAM.enabled.build.defines`）。芯片上有没有 PSRAM〔待验证〕。

Wi-Fi 功能是否在 C5 目标的编译单元里：`WiFiScan.cpp` 随 sketch 编译，双频分支由 `HAS_DUAL_BAND` 保留。实机能否扫到 5 GHz〔待验证〕。

## BLE

`HAS_BT` 在上述 C5 目标（含 POOM、XIAO）都有。NimBLE 头文件包在 `HAS_BT` 里〔`WiFiScan.h:16-17`〕。workflow 为这些板子写的 NimBLE 版本字符串是 `2.3.8`，并且 `HAS_NIMBLE_2` 有定义。

`MARAUDER_M5_NANO_C6` 也有 `HAS_BT`，但没有 `HAS_DUAL_BAND` / GPS / SD〔`configs.h:661-677`〕。它是 C6，不是 C5。

BLE 扫描代码在 `WiFiScan.cpp` 的 `HAS_BT` 段。实机广播与扫描〔待验证〕。

## Wardriving 与日志

命令名 `wardrive`〔`CommandLine.h:89`〕。处理函数要求 `HAS_GPS` 且 `gps_obj.getGpsModuleStatus()`〔`CommandLine.cpp:886-894`〕。`MARAUDER_C5` 有 `HAS_GPS`。模组没被判为存在时，这个分支不打印 `"GPS not supported"`（那句只在未定义 `HAS_GPS` 时编译）。POOM 会走到 `"GPS not supported"`。

`WIFI_SCAN_WAR_DRIVE` 在 GPS 模组状态为真时 `startLog("wardrive")` 并写入 WiGLE 头〔`WiFiScan.cpp:6775-6779`，头字符串 `WiFiScan.h:928`〕。

文件名由 `Buffer::createFile` 生成：非 pcap、非 gpx 时为 `<prefix><name>_<n>.log`〔`Buffer.cpp:19-22`〕。默认 prefix 是 `"/"`〔`Buffer.cpp:12`〕。所以源码路径形态是 `/wardrive_<n>.log`，不是 `.csv`。

已见 BSSID 跳过〔`WiFiScan.cpp:6321-6324`〕。`getFixStatus()` 为假时不 `append`〔`6343-6345`〕，串口仍 `print` 该行〔`6341`〕。`getLat()` / `getLon()` 返回成员字符串，默认空〔`GpsInterface.h:96`，`GpsInterface.cpp:604-609`〕。字面量 `"No Fix"` 在 BLE 显示串〔`WiFiScan.cpp:709`〕，不是经纬度字段的固定文本。无 fix 时日志里是否出现空坐标或 0〔待验证〕，因为没跑实机，也没把 NMEA 解析器的空值行为当成已测。

SSID 中的逗号换成下划线〔`WiFiScan.cpp:6327`〕。

`wardrivepoi` 命令存在〔`CommandLine.h:134`〕。POI 文件前缀 `wardrive_poi_`、扩展名 `.gpx`〔`WiFiScan.cpp:6515-6517`，`Buffer.cpp:26-28`〕。

## GPS

`MARAUDER_C5`、v8、Pancake、Mini v3 的宏相同：`GPS_SERIAL_INDEX 1`，`GPS_TX 14`，`GPS_RX 13`〔`configs.h:2975-2990`〕。`GpsInterface.cpp:11` 用 `HardwareSerial Serial2(GPS_SERIAL_INDEX)`。`begin` 把 `GPS_TX`、`GPS_RX` 传给 `Serial2.begin`〔`GpsInterface.cpp:20`〕。

源码里的探测波特率字面量是 9600、38400、115200，并有一段把 9600 改到 115200 的发送〔`GpsInterface.cpp:13-24`、`100-121`〕。这些是源码字面量。Scout Lite 上的 L86 波特率、接线、有没有定位〔待验证〕。宏名字和 DevKit 原理图是否一致，本仓库没有 Scout Lite 原理图，不能标成已核对硬件。

`getFixStatus()` 返回 `good_fix`〔`GpsInterface.cpp:636-638`〕。`getFixStatusAsString()` 返回 `"Yes"` / `"No"`〔`640-645`〕。

XIAO 的 GPS 宏是 `GPS_TX 12`、`GPS_RX 11`〔`configs.h:2971-2974`〕。T-Dongle 是 `GPS_TX 12`、`GPS_RX 11`〔`2967-2970`〕。POOM 没有 GPS 宏。

## SD

`MARAUDER_C5` 的宏：`SD_CS 10`，`SD_MISO 2`，`SD_MOSI 7`，`SD_SCK 6`〔`configs.h:2706-2707`、`3212-3215`〕。`esp32_marauder.ino:231-233` 在 `HAS_C5_SD` 下构造 `SDInterface(&sharedSPI, SD_CS)`，`setup` 里 `sharedSPI.begin(SD_SCK, SD_MISO, SD_MOSI)`〔`255-257`〕。挂载失败打印 `"Failed to mount SD Card"`〔`SDInterface.cpp:166-171`〕。`info` 在未挂载时打印 `text_table4[30]`，该表项是 `"SD Card: Not Connected"`〔`WiFiScan.cpp:4948-4953`，`lang_var.h:169`、`195`〕。

其他目标的 CS 宏不同：T-Dongle `SD_CS 23`〔`configs.h:2708-2709`〕，Pancake `SD_CS 7`〔`2716-2717`〕，POOM `SD_CS 5`〔`2724-2725`〕，v8 与 Mini v3 为 `SD_CS 10` 且 SPI 脚走 `TFT_*`〔`2712-2713`、`3222-3226`、`3270-3273`〕。XIAO 去掉 SD。

引脚是否接到 Scout Lite 卡槽〔待验证〕。无卡时会不会挂死〔待验证〕。源码这条失败路径会把 `supported` 设为 false 并返回。

`ls`、`update`、`backupspiffs`、`backupstatus`、`restorespiffs` 的帮助字符串包在 `HAS_SD` 里〔`CommandLine.cpp:287-291`〕。SPIFFS 备份/恢复实现在 `SDInterface::migrateSPIFFS`〔`SDInterface.cpp:228`〕，回滚路径字面量 `/spiffs.restore-rollback`〔`SDInterface.cpp:254`〕。断电恢复是否可靠〔待验证〕。

## WiGLE / WDGWars

`HAS_DIRECT_UPLOAD` 在 DevKit、T-Dongle、v8、Pancake、Mini v3、POOM 都有。XIAO 的特征段没有这行〔`configs.h:518-526`〕。

命令名 `upload`〔`CommandLine.h:123`〕。帮助字符串是 `upload -d <wdg/wigle/both>`〔`CommandLine.h:209`〕。命令实际行为〔待验证〕。

实现不在 `UploadTasks.cpp` / `UploadWigle.cpp`（本树没有这两个文件）。函数是 `WiFiScan::uploadFile`、`wigleUpload`、`wdgwarsUpload`〔`WiFiScan.cpp:11843`、`12092`、`11901`〕。主机字面量 `api.wigle.net` 与 `wdgwars.pl`，端口字面量 443〔`12166`、`11955`〕。成功后写 sidecar：`filePath + "." + service`〔`11828`〕。token 从 settings 读，不是 `credentials.h`（本树没有该文件）。WDG key 的保存调用在 `WiFiScan.cpp:2009`。

`retry` 是函数参数〔`WiFiScan.h:1111`〕。菜单和 CLI 调用处传入 `true`〔`CommandLine.cpp:701`，`MenuFunctions.cpp:3188`〕。这不等于已经核对「失败后自动重试直到成功」。重试策略〔待验证〕。没有 `stopScan()` 这个符号；停止扫描的命令是 `stopscan`，内部 `StartScan(WIFI_SCAN_OFF)`〔`CommandLine.cpp:371-381`〕。

native 测试 `test_wdg_response`、`test_upload_stream_buffer` 覆盖的是可在主机编译的解析/缓冲代码，不是上网上传。

## Wi-Fi 配置

最多 5 组保存的配置〔`settings.h:22`〕。`JSON_SETTING_SIZE` 为 6144，注释写明容纳 5 组且不常驻密码〔`configs.h:56-58`〕。读写在 `settings.cpp` 的 `loadSavedWifiCredential` / `saveWifiCredential`〔`settings.h:83-86`〕。CLI：`join`、`save`、`load`〔`CommandLine.h:115-117`〕。`join -s` 在有保存项时调用 `joinSavedWiFi`〔`CommandLine.cpp:1774-1776`〕。配置存在哪、掉电后还在不在〔待验证〕。源码使用 SPIFFS/settings，本轮没有挂文件系统跑。

## 状态、内存、恢复

`info` 打印版本、`HARDWARE_NAME`、运行分区 label、IDF 版本、STA/AP MAC；有 SD 时打印卡状态〔`WiFiScan.cpp:4888-4955`〕。`MARAUDER_C5` 没有 `HAS_BATTERY`，电池段不编译。

`MEM_LOWER_LIM` 对 `MARAUDER_C5` 为 10000〔`configs.h:2830-2831`〕。`HAS_PSRAM` 未定义时 pcap `BUF_SIZE` 为 `3 * 1024`，`SNAP_LEN` 2324；定义时为 `8 * 1024` 和 `4096`〔`configs.h:3167-3175`〕。Evil portal 的 `MAX_HTML_SIZE` 同样看 `HAS_PSRAM`：30000 或 11400〔`configs.h:2881-2885`〕。按 DevKit 宏走后者。

`MemoryGuard` 阈值在 `MemoryGuard.h:12-17`。`WiFiScan::checkMem` 调用它〔`WiFiScan.cpp:2220-2221`〕。屏幕状态栏里的 `esp_get_free_heap_size`〔`MenuFunctions.cpp:1303`〕在显示更新路径；`MARAUDER_C5` 没有 `HAS_SCREEN`。`info` 这段源码不打印 free heap。

`reboot`、`update` 命令名存在〔`CommandLine.h:55-56`〕。分区回滚和 OTA 是否在 Scout Lite 上可用〔待验证〕。

`protocolinfo` 命令名存在〔`CommandLine.h:61`〕。机器可读输出格式的实机兼容〔待验证〕。

## CLI / UART / Flipper

CLI 实现是 `CommandLine.cpp`，由 `esp32_marauder.ino` 无条件包含。`Serial.begin(115200)`〔`esp32_marauder.ino:244`〕。这是源码字面量。Scout Lite 与 Flipper 的引脚、电平、波特率〔待验证〕。

`MARAUDER_C5` 没有打开 `FLIPPER_ZERO_HAT`。本仓库没有 SigRoam FAP。标准命令名还包含 `help`、`settings`、`gps`、`gpsdata`、`nmea`、`gpspoi`、`gpstracker`、`geofence`、`recon`、`stopscan`、`channel`、`list`、`select`、`led`、`brightness`、`ls`。`sniffbt` 等 BLE 命令包在 `HAS_BT`〔`CommandLine.cpp:358-364`〕。

`recon wifi|ble|status|stop` 的帮助字符串〔`CommandLine.h:154`〕。实现在 `ReconMission.cpp`，双频信道索引走 `HAS_DUAL_BAND`〔`ReconMission.cpp:378`〕。

没有 `SERIAL_EN`、`PIN_SERIAL_RX`、`PIN_SERIAL_TX`。

## 其他现有 C5 目标

| 目标 | 相对 DevKit 的源码差异 | 状态 |
| --- | --- | --- |
| XIAO ESP32-C5 | 有 GPS 宏和 `HAS_PSRAM`，SD 被 undef，FQBN 带 `CDCOnBoot=cdc` 且 FlashSize=8M | 源码已核对；引脚与 8MB 是否属实〔待验证〕 |
| T-Dongle C5 | 有屏宏、16MB 自定义分区 CSV、GPS 宏 12/11、SD_CS 23 | 源码已核对；硬件〔待验证〕 |
| Pancake | 有屏、触摸、按键、电池宏、`HAS_PSRAM`、SD_CS 7 | 源码已核对。研究报告写的「缺屏幕和按键」与这些宏不一致 |
| POOM | 有屏和 SD，无 `HAS_GPS` | GPS 在该目标为缺失 |
| v8 | 有屏、电池、`HAS_PSRAM`、`HAS_ACT_LED`、SD 走 TFT SPI | 源码已核对；公开 workflow 不编它 |
| Mini v3 / Dual Mini | 有屏、键盘、按键、`HAS_PSRAM`；Dual 只是再定义 Mini v3 | 源码已核对；公开 workflow 不编；WebUI 在本树缺失 |

Remote ID、geofence、foxhunt、recon 的主机侧逻辑有 native 测试（见 `test/`）。它们不是 C5 实机结果。

## Offensive 功能

只记名称、源文件、编译开关。没有步骤、参数、payload 或报文格式。

- deauth：`esp32_marauder/WiFiScan.cpp`，`esp32_marauder/CommandLine.h`（`ATTACK_TYPE_DEAUTH`）。无单独板级宏，`MARAUDER_C5` 不排除。
- beacon / probe / funny / rickroll / badmsg / sleep / sae / csa / quiet：`esp32_marauder/CommandLine.h` 的 `ATTACK_TYPE_*`，实现在 `esp32_marauder/WiFiScan.cpp`。无单独板级宏。
- evil portal：`esp32_marauder/EvilPortal.cpp`，`esp32_marauder/CommandLine.h`（`EVIL_PORTAL_CMD`）。无单独板级宏。`HAS_PSRAM` 只影响 `MAX_HTML_SIZE`。
- 凭据收集：与 evil portal 同一源文件 `esp32_marauder/EvilPortal.cpp`。无单独板级宏。
- karma：`esp32_marauder/CommandLine.h`（`KARMA_CMD`），`esp32_marauder/CommandLine.cpp`。无单独板级宏。
- blespam / spoofat：`esp32_marauder/CommandLine.h`，`esp32_marauder/WiFiScan.cpp`。开关 `HAS_BT`；`MARAUDER_C5` 定义了 `HAS_BT`。

## 本轮没有当成事实的项

研究报告里的「可以直接刷入」、GPIO 对照、SigRoam 实机、`scan_dwell` 5–10 ms、`.csv` 文件名、`0x3005`、`--ci-test`、`UploadWigle.cpp`，都没有在本仓库被证成。细节写在 `deep-research-report-2.md` 的标注里。
