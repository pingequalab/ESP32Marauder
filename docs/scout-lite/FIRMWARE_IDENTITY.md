# Scout Lite 固件身份

状态：契约。PINGEQUA 版本 `0.1.0` 未发布。实机测试：未测。

本文件记录身份，不改标准 Marauder CLI。`Firmware: Marauder`、标准 `Version:` 行、`HARDWARE_NAME`、WiGLE CSV 头都保持原样。品牌字符串的唯一代码位置是 `esp32_marauder/PingequaIdentity.h`。该头文件目前没有被任何编译单元 include，没有接到 `info`、`gpsdata` 或 SD 更新元数据上。

分支点：`f4bc76727e7f29b7ca64da9c972c9103523c6006`（`origin/master`，2026-10-15，Merge pull request #2）。该树 `MARAUDER_VERSION` 是 `v1.18.0`（`esp32_marauder/configs.h:60`）。

许可仍是仓库根目录 `LICENSE`：MIT，Copyright (c) 2020 Just Call Me Koko。启动横幅 `ESP32 Marauder` / `By: justcallmekoko`（`esp32_marauder/CommandLine.cpp:63-65`）不改。本头文件只加产品标签，不换版权，不加第二份许可证。

## 身份

| 字段 | 值 | 代码 |
| --- | --- | --- |
| Brand | PINGEQUA | `PingequaIdentity::kBrand` |
| Product | Scout Lite | `kProduct` |
| Firmware Display Name | PINGEQUA Scout Lite Firmware | `kFirmwareDisplayName` |
| Edition | Marauder Edition | `kEdition` |
| Board ID | pingequa_scout_lite_c5 | `kBoardId` |
| Hardware Display Name | Scout Lite (PINGEQUA) | `kHardwareDisplayName` |
| Firmware Family | Marauder | `kFirmwareFamily` |
| PINGEQUA Version | 0.1.0 | `kVersion`，`kVersionReleased == false` |
| Marauder Base Version | v1.18.0 | 现有宏 `MARAUDER_VERSION`，不复制进身份头 |
| 分支点 | f4bc76727e7f29b7ca64da9c972c9103523c6006 | 本契约所依据的树 |

`kFirmwareFamily` 的值是 `Marauder`，只供以后的 `PQ Family:` 行使用。它不是 `lang_var.h` 里的 `Firmware: Marauder`，也不许写回去替换那一行。

## 命令前缀

选定前缀：`pq`。子命令：`pq info`、`pq caps`。本 PR 不注册这两条命令，也不把它们写进 `help`。

`CommandLine::runCommand` 用首词整词相等分发（`esp32_marauder/CommandLine.cpp:272` 切开参数，`276` 行起比较），不是前缀匹配。`CommandLine.h:53-226` 的首词里没有 `pq`。以 `p` 开头、但不相等的首词是 `packetcount`、`pingscan`、`portscan`、`protocolinfo`。`info` 和 `gpsdata` 仍是独立首词。`pq info` 的首词是 `pq`，不会进入 `INFO_CMD`。

Flipper Marauder companion 默认分支 `feature_wifi_marauder_app`，commit `d6e8c4d926b56b3bb8a2ee200a54baea57b77fea`（2026-10-05）。`wifi_marauder_app.h:7` 的版本宏是 `v0.7.6`。GitHub Releases latest 在 2026-10-16 返回 404，这里以该 commit 的源码为准。`scenes/wifi_marauder_scene_start.c` 的菜单会发 `info`、`gpsdata`、`help`、`protocolinfo`，不会发 `pq`、`pq info` 或 `pq caps`。因此不实现菜单已经在发的那些命令，只保留一个尚未接线的新首词。

SigRoam `pingequalab/sigroam-wardriving` tag `v0.6.1` 等于 `69ecfd7bbdc81d55ea7c76c142bb4f204b1c502d`（2026-10-08）。`application.fam:34` 为 `fap_version="0.6.1"`。它发出的命令是 `wardrive`、`wardrive -serial`、`stopscan`、`gpsdata`、`wardrivepoi`、`info`（`src/sr_parse_marauder.c:38-50`）。没有 `pq`。

回显规则：固件对每条命令先打 `#` 加原文（`CommandLine.cpp:270`）。SigRoam 把「`#` 紧贴命令名」当成回显（`ends_with_echo`，同文件 1002-1014 行）。`#info` 会认成 info 回显；`#pq info` 里 `info` 前面是空格，不是 `#`，不会认成 info 回显。以后接线时必须保持这个空格，禁止收成 `#pqinfo` 或改掉现有的 `#` 回显。

`pq` 回复只在收到 `pq` 时出现。标签已放在身份头里，接线时从那里取，禁止在 `.cpp` 里再写品牌句子。计划中的行：

```text
PQ Brand: PINGEQUA
PQ Product: Scout Lite
PQ Display: PINGEQUA Scout Lite Firmware
PQ Edition: Marauder Edition
PQ Board: pingequa_scout_lite_c5
PQ Hardware: Scout Lite (PINGEQUA)
PQ Family: Marauder
PQ Version: 0.1.0
PQ Released: no
PQ Marauder: <MARAUDER_VERSION>
PQ Caps-Begin
PQ Caps-End
```

`PQ Marauder:` 后面打印现有 `MARAUDER_VERSION`，不在身份头里存第二份版本字面量。`pq caps` 在没有额外能力时只有 `PQ Caps-Begin` 和 `PQ Caps-End`。未知子命令只打 `PQ Error: unknown`，不落入 `info`。这些行都不含 `[BUF/BEGIN]` 或 `[BUF/CLOSE]`。

SigRoam 认设备靠 `Firmware: ` 的值恰好等于 `Marauder` 八个字符（`sr_parse_marauder.c:836-867`）。`Version: ` 只被拷进 `out->version`（869-872 行），不作为能力位。额外能力必须来自以后的 `pq caps`，禁止解析 `Version:`、`PQ Version:` 或 `v1.18.0` 来打开功能。旧 SigRoam 不发 `pq` 时，额外能力视为空，标准命令照旧。

## 兼容矩阵

| 对方 | 钉住的版本 | 它依赖的字节 | 本 PR |
| --- | --- | --- | --- |
| Flipper Marauder app | `d6e8c4d`，源码版本宏 `v0.7.6` | 只把 11 字节 `[BUF/BEGIN]` / `[BUF/CLOSE]` 当协议（`wifi_marauder_uart.c:51-68`）。菜单发 `info`，不发 `pq` | 不改这两段标记，不改菜单已有命令的回包 |
| SigRoam FAP | tag `v0.6.1` = `69ecfd7` | `Firmware: Marauder` 才标成 Marauder；`Version:` 和 `Hardware:` 只拷贝 | 不改这三行。能力不进版本字符串 |
| `info` 串口 | 本树 `RunInfo` | `Firmware: Marauder`，然后 `Version: ` + `MARAUDER_VERSION`，然后 `Hardware: ` + `HARDWARE_NAME` | 不改 |
| `gpsdata` | `RunGPSInfo` | `==== GPS Data ====` 以及 `Fix` / `Sats` / `Acc` / `Lat` / `Lon` / `Alt` / `D/T` | 不改 |
| `protocolinfo --machine` | `CommandLine.cpp:559-572` | JSON 里 `firmware` 字段是 `version_number`（即 `MARAUDER_VERSION`） | 不改 |
| 启动横幅 | `CommandLine.cpp:63-65` | `ESP32 Marauder`、`version_number`、`By: justcallmekoko` | 不改 |
| SD 更新 | `Metadata.hardware` = `HARDWARE_NAME` | `MARAUDER_C5` 为 `ESP32-C5 DevKit`，芯片 `esp32c5` | 不改 |
| WiGLE CSV | `WiFiScan.h:928` | 见下方黄金头 | 不改 |

## 影响

| 面 | 现在的行为 | 本 PR | 以后若改错的后果 |
| --- | --- | --- | --- |
| `Firmware: Marauder` | `lang_var.h:159` `text4_20`，`info` 打印 `text_table4[20]` | 不动 | SigRoam 不再把设备认成 Marauder |
| `Version:` | `lang_var.h:160` `text4_21` = `Version: `，后接 `MARAUDER_VERSION` | 不动 | 版本行不再是标准行；SigRoam 拷走的也不是上游版本 |
| `HARDWARE_NAME` | `MARAUDER_C5` 为 `ESP32-C5 DevKit`（`configs.h:121-122`） | 不动 | SD 更新身份改变，见下一节 |
| 显示名 | 尚无串口输出 | 只存在于头文件 | 若写进 `info`，会改变 SigRoam 已拷贝的 `Hardware:` |
| `pq` | 未注册 | 只定义常量 | 若做成 `info` 的别名，会撞上 Flipper 菜单 |
| `[BUF/BEGIN]` `[BUF/CLOSE]` | `Buffer.cpp:205-207` | 不动 | Flipper 把 PCAP 流切错 |
| WiGLE 头 | `brand=JustCallMeKoko` | 不动 | 未做导入对照前不能改 |
| 许可与署名 | MIT，Just Call Me Koko | 不动 | 违反上游许可证的保留版权要求 |

## HARDWARE_NAME 与 SD 更新

`HARDWARE_NAME` 的使用点：

| 位置 | 作用 |
| --- | --- |
| `esp32_marauder/configs.h:77-138` | 按板型宏定义。`MARAUDER_C5` 取 `ESP32-C5 DevKit` |
| `esp32_marauder/WiFiScan.cpp:4905` | 有屏时 `info` 画 `Hardware: ` + `HARDWARE_NAME`。`MARAUDER_C5` 的 `HAS_SCREEN` 在本树是注释 |
| `esp32_marauder/WiFiScan.cpp:4912` | `info` 串口同一行。SigRoam 按 `Hardware: ` 前缀整行拷贝 |
| `esp32_marauder/FirmwareMetadata.cpp:4` | 仅 `PIO_UNIT_TESTING` 时定义成 `Native test target` |
| `esp32_marauder/FirmwareMetadata.cpp:34-40` | 写进 `Metadata.hardware`（48 字节，`FirmwareMetadata.h:10`） |
| `esp32_marauder/SDInterface.cpp:508-528` | `metadataMatches` 要求硬件字符串和芯片字符串都相同，否则拒绝 SD 更新 |

迁移风险：`metadataMatches`（`FirmwareMetadata.cpp:104-107`）是硬件字段和芯片字段的字符串相等。`MARAUDER_C5` 的当前键是 `ESP32-C5 DevKit` + `esp32c5`。若把 `HARDWARE_NAME` 改成 `Scout Lite (PINGEQUA)`，旧固件会拒绝带新字符串的 `update.bin`，新固件也会拒绝仍嵌入 `ESP32-C5 DevKit` 的镜像。两个方向的 SD 更新都会在 `SDInterface.cpp:520` 打印 `Rejected SD update`。`ALLOW_UNVERIFIED_SD_UPDATE` 是开发开关（`configs.h:13`），不能当作产品迁移手段。显示名长度 20，小于 48，放得进字段，但这不能成为改字段的理由。

安全的后续步骤：继续用 `HARDWARE_NAME` 做更新身份。显示名只从 `pq info` 的 `PQ Hardware:` 出来。若将来更新包必须带 Board ID，新增字段并提高 `METADATA_SCHEMA_VERSION`，过渡期同时接受 schema 1 的旧硬件字符串。不要就地改写现有 48 字节硬件字段。

## WiGLE CSV

本树的头在 `esp32_marauder/WiFiScan.h:928`，两行是：

```text
WigleWifi-1.4,appRelease=v1.18.0,model=ESP32 Marauder,release=v1.18.0,device=ESP32 Marauder,display=SPI TFT,board=ESP32 Marauder,brand=JustCallMeKoko
MAC,SSID,AuthMode,FirstSeen,Channel,RSSI,CurrentLatitude,CurrentLongitude,AltitudeMeters,AccuracyMeters,Type
```

`appRelease` 和 `release` 来自 `MARAUDER_VERSION`。列头与 WiGLE CSV 1.4 的固定列一致。查阅 2026-10-16：https://api.wigle.net/csvFormat-1_4.html 。该页规定格式记号 `WigleWifi-1.4`，预头键顺序为 `appRelease,model,release,device,display,board,brand`，列头为上面第二行。同日 https://api.wigle.net/csvFormat.html 描述的是 1.6，列头在 `Type` 前多了别的列。本树不是 1.6，禁止为了加品牌改成 1.6。

任何品牌字节写进这个头之前，要先有下面的对照，而不是直接改：

1. 把上面两行做成黄金样本。样本里的版本必须等于当时的 `MARAUDER_VERSION`，现在是 `v1.18.0`。
2. 从同一份 wardrive 日志取未改头的文件，确认前两行与黄金样本逐字节相同，且观察行仍能按 1.4 列解析。
3. 用固件现有的 `upload -d wigle` 上传这份未改文件，记下接受或拒绝。这一步是实机/账号操作，本 PR 没做。
4. 只改拟议中的品牌字节，观察行不动，再上传一次。接受或拒绝必须与第 3 步相同。
5. 预头值里若出现逗号，按该页写的 UTF-8 RFC4180 加引号。未加引号的逗号判失败。
6. 格式记号必须仍是 `WigleWifi-1.4`。列名和顺序必须仍是 1.4 那 11 列。

在第 4 步有记录之前，不改 `WiFiScan.h:928`。

## 源码位置

| 对象 | 位置 | 本 PR |
| --- | --- | --- |
| 品牌字符串 | `esp32_marauder/PingequaIdentity.h` | 新增，未 include |
| `MARAUDER_VERSION` | `esp32_marauder/configs.h:60` | 不改，值为 `v1.18.0` |
| `version_number` | `esp32_marauder/esp32_marauder.ino:118`；屏上另有 `Display.h:106` | 不改 |
| `Firmware: Marauder` | `esp32_marauder/lang_var.h:159` | 不改 |
| `Version: ` | `esp32_marauder/lang_var.h:160` | 不改 |
| `info` 串口/屏幕 | `esp32_marauder/WiFiScan.cpp:4888-4914` | 不改 |
| 启动横幅 | `esp32_marauder/CommandLine.cpp:57-66` | 不改 |
| `help` 版本行 | 同上，`version_number` | 不改 |
| 命令表 | `esp32_marauder/CommandLine.h:53-226` | 不改 |
| 分发 | `esp32_marauder/CommandLine.cpp:263-276`，回显在 `270` | 不改 |
| `gpsdata` 入口 | `esp32_marauder/CommandLine.cpp:421-432` | 不改 |
| `gpsdata` 串口块 | `esp32_marauder/WiFiScan.cpp:4654-4667` | 不改 |
| `protocolinfo` | `esp32_marauder/CommandLine.cpp:551-574` | 不改 |
| PCAP 标记 | `esp32_marauder/Buffer.cpp:202-207` | 不改 |
| `HARDWARE_NAME` | `configs.h:77-138`，`WiFiScan.cpp:4905` 与 `4912`，`FirmwareMetadata.cpp:4` 与 `38` | 不改 |
| SD 更新比对 | `esp32_marauder/SDInterface.cpp:478-536`；规则在 `FirmwareMetadata.cpp:94-111` | 不改 |
| WiGLE 头 | `esp32_marauder/WiFiScan.h:928` | 不改 |
| 屏启动版本 | `esp32_marauder/Display.cpp:298`、`342` | 不改。`MARAUDER_C5` 无屏，这条不在 C5 串口上 |
| 章程入口 | `docs/scout-lite/PROJECT_CHARTER.md` 第 6.5 节 | 只加这一节 |

## 测试

实机测试：未测。本 PR 没有新的串口字节，下面前两行是本树源码核对，其余是接线或改头之前的门禁。

| 项 | 做法 | 结果 |
| --- | --- | --- |
| 首词冲突 | 对照 `CommandLine.h:53-226`，确认没有首词 `pq` | 源码核对通过。`packetcount` / `pingscan` / `portscan` / `protocolinfo` 只是同字母，不是同一词 |
| Flipper 菜单 | 读 `d6e8c4d` 的 `wifi_marauder_scene_start.c` | 菜单不发 `pq`。它发的 `info`、`gpsdata` 本 PR 没有新实现 |
| SigRoam 命令 | 读 `v0.6.1` `sr_parse_marauder.c:38-50` | 不发 `pq`。认设备仍要 `Firmware: Marauder` |
| 回显边界 | 以后接线时检查 `#pq info`：`info` 前一字节是 `0x20`，不是 `#` | 未接线，未测 |
| `pq info` / `pq caps` 回包 | 接线后逐行对照上一节的计划行，并确认 `info` 与 `gpsdata` 字节不变 | 未接线，未测 |
| 能力来源 | SigRoam 只解析 `PQ Cap:`，不解析 `Version:` 或 `PQ Version:` | 未改 SigRoam，未测 |
| `[BUF/BEGIN]` `[BUF/CLOSE]` | 标记仍是 `Buffer.cpp:205-207` 的 11 字节 | 源码未改。实机未测 |
| `HARDWARE_NAME` | `MARAUDER_C5` 仍为 `ESP32-C5 DevKit`；元数据比对函数未改 | 源码未改。未做 SD 更新 |
| WiGLE 黄金头 | 改品牌字节前按上一节 6 步做导入对照 | 未改头。上传未测 |
| 实机 | Scout Lite 串口跑 `info`、`gpsdata`、以后的 `pq info` | 未测 |
