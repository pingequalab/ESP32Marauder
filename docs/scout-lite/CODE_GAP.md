# 代码级差距

日期：2026-10-16。Marauder 树：`f4bc76727e7f29b7ca64da9c972c9103523c6006` 的 `esp32_marauder/`，版本字符串 `MARAUDER_VERSION` 是 `v1.18.0`（`esp32_marauder/configs.h:60`）。官方稳定 release 是 `v1.18.0`，2026-10-06：https://github.com/justcallmekoko/ESP32Marauder/releases/tag/v1.18.0 。同日之后的 `d3ceb28_nightly` 是 pre-release，不是稳定版。

Dual Band Wardriver 只读对照 tag `v2.3.2`，commit `35b3c1b360e95980d0a904f7541ea766dd960986`，2026-08-19，仍是该仓库 Latest：https://github.com/justcallmekoko/ESP32DualBandWardriver/releases/tag/v2.3.2 。源码没有放进本仓库。下面 Wardriver 行号都指这个 tag 的 `src/`。

截图和 `deep-research-report-2` 是主张，不是事实。结论只用这四个词：`已有`、`部分`、`缺失`、`超出范围`。SigRoam 优化、FAP 合约测试、SigRoam 兼容超出范围。本轮不改固件。

`MARAUDER_C5` 打开 `HAS_GPS`、`HAS_SD`、`HAS_C5_SD`、`HAS_BT`、`HAS_DUAL_BAND`、`HAS_NIMBLE_2`、`HAS_DIRECT_UPLOAD`（`configs.h:533-547`）。下面的行为按这个目标会编进去。没有实机测试。

## 截图里的 Marauder 能力

| 能力 | Marauder 实际行为 | 证据 | 相对 Dual Band Wardriver v2.3.2 | 结论 |
| --- | --- | --- | --- | --- |
| 2.4/5 GHz 频道走动 | `main` 里 `WIFI_SCAN_WAR_DRIVE` 每隔 `channel_hop_delay * HOP_DELAY` 调用 `executeWarDrive()`。`channel_hop_delay` 初值 1，双频时 `HOP_DELAY` 是 250。`executeWarDrive()` 用静态表按顺序取一个频道，调用 `WiFi.scanNetworks(true, true, false, 80, scan_channel)`。表里先重复 5 GHz 的 36–48 和 149–161，再 1/6/11，然后一段 5 GHz，最后 14 到 1。扫描还在跑就直接 return。这不是名为 `scan_dwell` 的变量。 | 源码已核对 `esp32_marauder/WiFiScan.cpp:12818-12824`，`6225-6260`，`6409-6419`；`WiFiScan.h:456`；`configs.h:3328-3331` | Wardriver 的 `runWardrive()` 在扫描结束后对全频道再调 `WiFi.scanNetworks(true, true, false, CHANNEL_TIMER)`，`CHANNEL_TIMER` 是 80。不是同一张加权表。 | 已有 |
| SD 上的 WiGLE 行 | `RunBeaconScan()` 在 GPS 模块状态为真时 `startLog("wardrive")`，再 `buffer_obj.append(header_line)`。文件名是 `Buffer::createFile()` 生成的 `/wardrive_<n>.log`，不是 `.csv`。表头是 `WigleWifi-1.4`，列是 `MAC,SSID,AuthMode,FirstSeen,Channel,RSSI,CurrentLatitude,CurrentLongitude,AltitudeMeters,AccuracyMeters,Type`。Wi-Fi 行有两条路径。`executeWarDrive()` 拼扫描结果：逗号换成下划线，`seen_mac()` 为真则跳过，`getFixStatus()` 为假则只 `Serial.print`，不 `append`，也不 `save_mac()`。`beaconSnifferCallback()` 在同一模式里另拼一行（`WiFiScan.cpp:9233`），同样只在有 fix 且未暂停时 `append` 并 `save_mac()`。没有国家码列，也没有把经纬度写成 `No Fix` 再写入 SD。 | 源码已核对 `WiFiScan.cpp:6772-6779`，`6303-6345`，`8895`，`9231-9250`；`WiFiScan.h:928`；`Buffer.cpp:10-22`，`85-86`，`131-135`；`GpsInterface.cpp:574-588`，`604-606`，`636-637`。`lat` 初值是空串（`GpsInterface.h:96`） | `processWardrive()` 同样只在 `gps.getFixStatus()` 时 `buffer.append`。显示串可以拼 `GPS: No Fix`，该行不进 SD。 | 已有 |
| BLE wardrive，是否和 Wi-Fi 一起跑 | `MARAUDER_C5` 走 NimBLE 2 的 `bluetoothScanAllCallback::onResult()`。模式是 `WIFI_SCAN_WAR_DRIVE` 时拼 BLE 行，只在有 fix 且不在 geofence 暂停时 `append`，并且只在这次 `append` 之后 `save_mac()`。`executeWarDrive()` 若 `pBLEScan->isScanning()` 为真就 return，不开始下一次 Wi-Fi 扫描。BLE 只在 `wardrive_channel_index % 4 == 0` 时 `pBLEScan->start(500, false, false)`。两者交替，不是同时扫。 | 源码已核对 `WiFiScan.cpp:1076-1079`，`1467-1498`，`6275-6284`，`6386-6405` | `runWardrive()` 在 `processWardrive()` 之后 `scanBLE()`，并用 `while (pBLEScan->isScanning())` 等完。也是交替。 | 已有 |
| WiGLE 上传 | `WiFiScan::wigleUpload()` 用 `WiFiClientSecure`，`setInsecure()`，连接 `api.wigle.net:443`，POST multipart。没有证书校验。没有 `UploadWigle.cpp`。失败返回 false，没有第二轮循环。 | 源码已核对 `WiFiScan.cpp:12092`，`12163-12176` | `WiFiOps` 里同样有连 `api.wigle.net` 的上传。Dock 会批量调用。 | 已有 |
| WDGWars 上传 | `WiFiScan::wdgwarsUpload()` 连接 `wdgwars.pl:443`，头是 `X-API-Key`，路径 `/api/v2/upload-csv`。成功看响应里有没有 `202 Accepted` 或 `"ok":true`。密钥来自设置 `wdg_key`，或启动时读 SD 上的 `/wdg_key.txt` 再删掉该文件。 | 源码已核对 `WiFiScan.cpp:11901-11972`，`12038-12040`，`1994-2011`；`settings.h:20` | Wardriver 的 `WiFiOps::wdgwarsUpload()` 是同一类请求。 | 已有 |
| 上传目标选择 | CLI `upload -d wigle\|wdg\|both` 分别传 `WIGLE_UPLOAD`、`WDG_UPLOAD`、`BOTH_UPLOAD`。先 `joinSavedWiFi(false)`。没有 `-d` 或值不对则不上传。屏幕菜单里的三项目标在 `HAS_SCREEN` 下，`MARAUDER_C5` 不编菜单。 | 源码已核对 `CommandLine.cpp:678-711`；`WiFiScan.h:239`；`MenuFunctions.cpp:3188`，`3234`，`3280` | Web 页用 `svc=wigle\|wdg\|both`。另外 `uploadAllPending()` 按缺哪个 sidecar 决定。 | 已有 |
| Geofence | `geofence set/clear` 写入 SPIFFS，最多 5 个，半径 0.1–1.0 英里。`executeWarDrive()` 开头 `updateGeofenceState()` 为真就停掉当前扫描并 return，这一轮不写行。进入和离开会打 `Wardrive paused inside geofence` / `Wardrive resumed outside geofence`。 | 源码已核对 `CommandLine.cpp:843-883`；`settings.cpp:780-785`；`WiFiScan.cpp:6228-6235`，`6434-6467` | `process` 路径外还有 geofence 暂停，以及暂停期间仍可找 Dock SSID。Marauder 没有后一半。 | 已有 |
| `wardrive` / `stopscan` / `gpsdata` | `wardrive` 在 `getGpsModuleStatus()` 为真时 `startScanFromCLI(WIFI_SCAN_WAR_DRIVE)`。注释掉的 `-s` 没有生效。`stopscan` 调用 `StartScan(WIFI_SCAN_OFF)`。`gpsdata` 进入 `WIFI_SCAN_GPS_DATA`。三个命令名都在 `CommandLine.h`。 | 源码已核对 `CommandLine.cpp:371-381`，`421-430`，`886-894`；`CommandLine.h:66`，`88-89` | Wardriver 没有这套 CLI 名字。它用自己的 `runWardrive()` 循环。 | 已有 |
| Flipper 串口模式 | `setup()` 是 `Serial.begin(115200)`，这是官方 CLI。`FLIPPER_ZERO_HAT` 在 `MARAUDER_C5` 段是注释。`protocolinfo --machine` 只报告 SPIFFS 备份能力，不是 wardrive 协议。`BT_SCAN_FLIPPER` 是在空中看 Flipper 广播，不是串口模式。本仓库没有 SigRoam FAP。 | 源码已核对 `esp32_marauder.ino:244`；`configs.h:531`；`CommandLine.cpp:551-564`。SigRoam 超出范围 | Wardriver 不用这套 CLI。 | 部分 |
| `MARAUDER_C5` 的 GPS 和 SD | `GpsInterface::begin()` 用 `Serial2.begin(9600, SERIAL_8N1, GPS_TX, GPS_RX)`，脚是 14 和 13。`setup()` 在 `HAS_C5_SD` 下 `sharedSPI.begin(SD_SCK, SD_MISO, SD_MOSI)`，再 `sd_obj.initSD()`，C5 分支是 `SD.begin(SD_CS, *_spi)`。失败则 `supported=false` 并打印 `SD Card NOT Supported`。模块没起来时 `RunBeaconScan()` 直接 return，不建日志。原理图与这四根 SD 脚、两根 GPS 脚一致。实机未测。不是刷机许可。 | 源码已核对 `GpsInterface.cpp:17-20`；`configs.h:2975-2978`，`3212-3215`，`2706-2707`；`esp32_marauder.ino:255-256`，`353-356`，`388-389`；`SDInterface.cpp:126-173` | Wardriver 是另一套固件，不代替这段初始化。 | 已有 |

每 4 个频道索引，`executeWarDrive()` 会离开 STA 扫描，调用 `setWiFiMode()` 和 `broadcastCustomBeacon()`（`WiFiScan.cpp:6352-6382`）。这里不写帧内容。

## 相对 Dual Band Wardriver 的行为

| 能力 | Marauder 实际行为 | 证据 | 相对 Dual Band Wardriver v2.3.2 | 结论 |
| --- | --- | --- | --- | --- |
| 上传成功是否记住 | `writeSidecar()` 在成功后写 `<文件>.wigle` 或 `<文件>.wdg`，内容一行 `uploaded=` 加 GPS 时间。`uploadFile()` 只有 `retry==false` 才看这个文件。无屏 CLI `upload` 现在传 `false`。没有单独的强制重传开关，所以不新加命令。屏幕菜单仍传 `true`，包在 `HAS_SCREEN` 里，`MARAUDER_C5` 不编。 | 源码已核对 `WiFiScan.cpp:11823-11854`；`CommandLine.cpp` 的 `uploadFile(..., false, ...)`；`MenuFunctions.cpp:3188` 仍是 `true`；`WiFiScan.h:1111` | `uploadAllPending()` 用 sidecar 是否存在决定还传不传（`WiFiOps.cpp:2281-2317`）。Web 页把已有 sidecar 显示成完成（`2477-2489`）。 | 已有 |
| 上传后回到 wardrive | CLI 在 `joinSavedWiFi()` 之前记住 `currentScanMode`。每个待传文件都成功，或因 sidecar 被跳过，且之前的模式是 `WIFI_SCAN_WAR_DRIVE` 时，断开上传用的 Wi-Fi，再 `startScanFromCLI(WIFI_SCAN_WAR_DRIVE)`。这会走现有的 `StartScan`，不改频道表。任一文件 3 次都失败，或者之前不是 wardrive，仍 `StartScan(WIFI_SCAN_OFF)`。`joinSavedWiFi()` 失败则直接 return，不改模式。 | 源码已核对 `CommandLine.cpp` 的 `mode_before_upload` 与 `upload_failed` | `departDock()` 拆掉 Dock 再回到扫描（`WiFiOps.cpp:3220-3246`）。这里不搬那段状态机。 | 已有 |
| 失败重试 | `wigleUpload()` / `wdgwarsUpload()` 失败仍 `return false`，函数内没有循环。CLI 对同一个文件最多再试 2 次，一共 3 次，然后换下一个文件。这不是 `uploadFile()` 的 `retry` 参数。 | 源码已核对 `WiFiScan.cpp:11955-11964`，`12163-12176`；`CommandLine.cpp` 的 `attempt < 3` | Dock 的连接次数和 Node 找 Core 的退避都不搬。 | 已有 |
| sidecar 状态文件 | 写文件的函数在。无屏 CLI 现在用它跳过。无屏的 C5 没有菜单去显示这些文件。 | 源码已核对 `WiFiScan.cpp:11823-11840` | 同一命名。Dock 和网页靠它跳过。 | 已有 |
| Dock SSID 触发 | 无 `TRIGGER_SSID`，无 `runDockMode()`。`joinSavedWiFi()` 只为已经保存的上传用 Wi-Fi 做连接，不在 wardrive 扫描结果里找一个触发名再自动上传。 | 源码已核对 `WiFiScan.cpp:2490-2504`。全树没有 `runDockMode` | `runWardrive()` 在处理结果前比较 `TRIGGER_SSID_NAME`（`settings.h:31`，`WiFiOps.cpp:1268-1279`）。离开覆盖后 `departDock()`。 | 缺失 |
| Web UI | `CommandLine.h:14` 的 `#include "Web.h"` 是注释。本树没有 wardrive 管理页。`EvilPortal.cpp` 里的 `AsyncWebServer` 不是日志管理。不写它的请求内容。 | 源码已核对 `CommandLine.h:14` | `WebServer server(80)`（`WiFiOps.cpp:84`）。`WiFiOps::serveConfigPage()`（`2351`）提供设置、文件列表、`/download`、`/upload`（`2468-2723`）。 | 缺失 |
| 日志列表、删除、下载 | CLI `ls` 调用 `sd_obj.listDir()`。没有删除日志的 CLI。`SDInterface::removeFile()` 存在。删除 wardrive 日志的调用在 `MenuFunctions.cpp`，包在屏幕菜单里，`MARAUDER_C5` 不编 `HAS_SCREEN`。没有 HTTP 下载。 | 源码已核对 `CommandLine.cpp:544-548`；`SDInterface.cpp:220`；`MenuFunctions.cpp:3305-3317` | 网页列文件并 `/download`（`2468-2718`）。删除在屏菜单 `ui.cpp:107-110` 和 `208-212`。网页路径里没有 delete 路由。 | 部分 |
| Wi-Fi 凭据 | `Settings::saveWifiCredential()` 写入 SPIFFS，最多 `MAX_SAVED_WIFI_PROFILES` 5 组。上传前 `joinSavedWiFi()` 按这些组去连。 | 源码已核对 `settings.h:22`；`settings.cpp:679-705`；`WiFiScan.cpp:2490-2493` | 设置里是 Dock 的 `t_ssid` / 密码、管理密码、WiGLE/WDG 密钥（`settings.cpp:505-531`），不是这 5 组上传热点。 | 已有 |
| 无 Fix 是否写行 | 有模块但 `good_fix==false` 时不 `append`，也不 `save_mac()`。串口仍打印拼好的行。保存下来的行里没有字面量 `No Fix`。之后有 fix 时，这个 MAC 还没被占住，可以写入。三条路径都是这样：`executeWarDrive()`、`beaconSnifferCallback()`、NimBLE 2 的 wardrive `onResult()`。 | 源码已核对 `WiFiScan.cpp:6342-6344`，`9246-9249`，`1495-1497`；`GpsInterface.cpp:636-644` | `processWardrive()` `1575-1610` 同一规则：无 fix 则 `do_save` 保持 false。截图说两边策略不同，这份 tag 里写 SD 的条件相同。 | 已有 |
| SSID 排除 | 只把逗号换成下划线。没有排除表。 | 源码已核对 `WiFiScan.cpp:6326-6327` | `sx_0` 到 `sx_9`，`MAX_SSID_EXCLUSIONS` 10，`isSSIDExcluded()`（`configs.h:155`；`WiFiOps.cpp:1511-1563`；`settings.cpp:533-536`）。 | 缺失 |
| 日志换新文件 | 每次 `startLog("wardrive")` 找下一个不存在的 `/wardrive_<n>.log`。一次扫描过程中不按大小切开。 | 源码已核对 `Buffer.cpp:19-22`；`WiFiScan.cpp:6778` | `Buffer::createFile()` 同样是下一个序号（`Buffer.cpp:16-28`）。`departDock()` 再 `startLog()` 开新文件。没有按大小旋转的函数。 | 部分 |
| Solo | 没有 `SOLO_MODE`。wardrive 只有 `executeWarDrive()` 这一条路径。 | 本树无此符号 | 未定义 `CORE`/`NODE` 时 `run_mode = SOLO_MODE`（`WiFiOps.h:193-199`；`configs.h:48-50`）。Release 说明列了 PR #64「adaptive SOLO channel dwell」。tag `35b3c1b` 的 `src/` 没有 `ChannelDwell.cpp`。`gh` 比较该 PR 的合并提交与这个 tag 的结果是 `diverged`。不把 dwell 算法写成这个 tag 里的行为。 | 缺失 |
| Node/Core 与 ESP-NOW | 本树没有 `esp_now_init`。不设计。 | 源码已核对：本树无此符号 | tag 里有。`WiFiOps.cpp:74` 的 `node_table`，`1097` 的 `esp_now_init()`，`657` 的 `sendCoreRequest()`，`752` 的 `sendEncryptedStringToCore()`。截图写「无」，和这个 tag 相反。默认未定义 `CORE`/`NODE` 时走 Solo，但这些函数在同一文件里。 | 超出范围 |

## 研究报告里抽出的点

报告路径是上传的 `deep-research-report-2_0a35.md`。下面只记核对结果。未核对的句子不写进 `PROJECT_CHARTER.md`。

| 报告里的说法 | 核对 |
| --- | --- |
| 官方稳定版可以拿 C5 DevKit 构建当起点 | 上游文档已核对：https://github.com/justcallmekoko/ESP32Marauder/releases/tag/v1.18.0 （2026-10-06）。本树 FQBN 与 `build_parallel.yml:46` 一致。 |
| 可以直接刷进 Scout Lite，不必改 PCB | 待验证。源码没有刷机记录。SD/GPS 同脚不是刷机许可。 |
| PSRAM 和 8MB 分区已经与 Scout Lite 一致 | 待验证。`HAS_PSRAM` 仍是 `configs.h:543` 的注释。丝印不是测量。 |
| Mini v3 有可选 Flipper UART 模式 | 源码已核对 `configs.h:621-622`：`FLIPPER_ZERO_HAT` 是注释。 |
| Dual C5 Mini 含 WebUI 和分布式模式 | 源码已核对 `configs.h:144-146`：`DUAL_MINI_C5` 只再定义 `MARAUDER_MINI_V3`。`CommandLine.h:14` 的 Web 头文件是注释。 |
| T-Dongle 构建是 16MB | 源码已核对 `build_parallel.yml:48`；`installer/partitions/t_dongle_c5.csv:1`。Scout Lite 的 Flash 容量仍待验证，不能由此推出「板子是 8MB 所以不匹配」。 |
| Pancake 无屏、无键，可直接替换 | 源码已核对 `configs.h:595-606`：`HAS_SCREEN`、`HAS_BUTTONS`、`HAS_TOUCH` 是打开的。 |
| XIAO 是 4MB、无 GPS、无 SD、无 CLI | 源码已核对 `build_parallel.yml:47` 的 FQBN 是 `FlashSize=8M`。`configs.h:518-522` 有 `HAS_GPS`。`682-685` 关掉 SD。CLI 没有按该板关掉。 |
| 频道驻留由 `scan_dwell` 控制，大约 5–10 毫秒 | 缺失。本树没有 `scan_dwell`。双频 wardrive 的扫描参数是 80，外层间隔是 `HOP_DELAY` 250。见上表。 |
| 无 Fix 时把经纬度写成 `No Fix` 再保存 | 源码已核对 `WiFiScan.cpp:6305-6345`：无 fix 不 `append`。 |
| CSV 含国家码，扩展名是 `wardrive_*.csv`，由 `WardriveTasks` 写 | 缺失这些名字。实际是 `header_line` 和 `/wardrive_<n>.log`。 |
| 手动和自动上传；上传时 `stopScan()` 再恢复；失败会重试；`UploadTasks.cpp` / `UploadWigle.cpp`；token 在 `credentials.h`；做证书校验 | 手动 `upload` 已核对。没有这些文件名，没有 `stopScan()` 之后恢复 wardrive，没有失败重试循环。`wigleUpload()` 调用 `setInsecure()`（`WiFiScan.cpp:12163`）。密钥在 SPIFFS 设置和 SD 文本，不是 `credentials.h`。 |
| `0x3005`、`--ci-test` | 缺失。本树没有这两处字符串。 |
| `#define SERIAL_EN 1` 或改 `PIN_SERIAL_RX/TX` 才能接 Flipper | 缺失这些宏。`MARAUDER_C5` 使用 `Serial.begin(115200)`（`esp32_marauder.ino:244`）。 |
| SigRoam FAP 已经兼容，应加协议补丁 | 超出范围。本仓库没有 FAP。不把兼容写成事实。 |
| Dual Band Wardriver 当前正式版是 v2.3.x，本仓库抓到的最新正式版是 v2.3.2 | 上游文档已核对：https://github.com/justcallmekoko/ESP32DualBandWardriver/releases/tag/v2.3.2 （2026-08-19）。`gh release list` 里它仍是 Latest。 |
| 该版本的 Node/Core 不存在 | 与 tag 相反。见上表。不设计。 |
