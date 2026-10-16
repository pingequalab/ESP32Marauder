# 架构

日期：2026-10-16。一块固件，两个 Flipper 应用。没有 Scout Lite BSP，本文也不添加。

## 标准 Marauder 兼容优先

标准 Flipper Marauder 应用的 CLI 是兼容接口。它已经发出的命令、回复和交互保持原样。`[BUF/BEGIN]` 和 `[BUF/CLOSE]` 仍是该应用唯一的结构化标记。新句子如果出现在标准命令的回复里，就改变了这条接口，所以不往 `wardrive`、`stopscan`、`gpsdata`、`info`、`upload -d`、`ls`、`settings` 的回复里加 SigRoam 字段。

目录里用户安装到的应用版本，以及默认分支尚未发布的菜单，写在 `FLIPPER_MARAUDER_COMPATIBILITY_MATRIX.md`。这里不重开 Marauder 源码审计。

## 一块固件，两个应用

同一份 Scout Lite 固件同时被两个应用打开。换应用不重新刷机，也不换分区。

标准应用是 `0xchocolate/flipperzero-wifi-marauder`。SigRoam 应用是 `pingequalab/sigroam-wardriving`。SigRoam 只做 wardriving 体验：GPS 状态、双频统计、记录质量、会话管理、上传状态、错误恢复。不把 Marauder 的扫描、攻击、门户菜单做进 SigRoam，也不在固件里为那些菜单新增第二套实现。

Wi-Fi、GPS、SD、wardrive 仍走现有模块：`WiFiScan.cpp`、`GpsInterface.cpp`、`SDInterface.cpp`、`Buffer.cpp`。SigRoam 命名空间只做命令入口，调用这些函数。不新建扫描循环，不复制 `executeWarDrive()`。

## SigRoam 命名空间边界

标准动词保持唯一含义。SigRoam 已经会发、而且必须继续按原义处理的，是 `wardrive`、`wardrive -serial`、`stopscan`、`gpsdata`、`wardrivepoi`、`info`。`wardrive -serial` 用的是现有的 `-serial` 开关（`CommandLine.cpp` 在扫描命令前读取），不另作解释。

SigRoam 应用另外发送 `upload`（没有 `-d`）和 `uploadstatus`，并解析 `Radio:`、`Qual:`、`Sess:`、`Busy:`、`Diag:`、`Up:`、`Rank:`、`Cfg:`。这些不是标准菜单命令，也不能塞进 `upload -d` 或 `info` 的回复。`v0.6.1` 不发送请求这些行的前缀，而是把它们当成 `#info` 的后半。本固件不追加到 `info`。状态命令是 `sigroam`，标准菜单没有这个词，只打印这八行。不把裸 `upload` 或 `uploadstatus` 接上 `upload -d`。

`info` 在标准固件里会进入 `SHOW_INFO`。SigRoam 应用自己对非 SigRoam 版本补发 `stopscan`。不改 `info` 去迁就这一步。

## 现在的分层

板型选择在 `esp32_marauder/configs.h`。默认的板型 `#define` 全是注释。CI 用编译参数 `-DMARAUDER_C5` 之类打开一个目标（`.github/workflows/build_parallel.yml:273`）。

`esp32_marauder/esp32_marauder.ino` 做 `setup`：串口、按宏初始化 SD SPI 和各子系统。

功能落在这些现成模块上，而不是另一套框架：

- Wi-Fi / BLE / wardrive / 上传：`WiFiScan.cpp`
- CLI：`CommandLine.cpp`
- GNSS：`GpsInterface.cpp`
- SD 与 SPIFFS 备份：`SDInterface.cpp`
- 配置：`settings.cpp`
- 日志文件名：`Buffer.cpp`
- 内存门槛：`MemoryGuard.cpp`
- 版本字符串：`configs.h` 的 `MARAUDER_VERSION`

`platformio.ini` 只有 `[env:native]`，用来跑 `test/` 里的主机测试。它不产生 C5 固件。

## 边界

三层分开。现在没有第四层，也没有 BSP 目录。

板型配置只放选择宏和引脚。`esp32_marauder/configs.h` 的 `MARAUDER_C5` 段（约 529–547 行，以及 GPS/SD 宏）和 `esp32_marauder/esp32_marauder.ino` 里 `HAS_C5_SD` 的 SPI 初始化就是这一层。官方编译入口是 `.github/workflows/build_parallel.yml` 的 FQBN 和 `-DMARAUDER_C5`。以后若要 Scout Lite 目标，也只加同类宏和一条 FQBN。引脚还是未知，所以这一层还没写。

核心是与板名无关的行为：`WiFiScan.cpp`、`CommandLine.cpp`、`GpsInterface.cpp`、`SDInterface.cpp`、`settings.cpp`、`Buffer.cpp`、`MemoryGuard.cpp`。不把 Scout Lite 品牌或协议写进这些文件。

产品附加物是本 fork 的说明和检查，不是固件：`AGENTS.md`、`docs/scout-lite/`、`.github/workflows/scout-lite-ci.yml`。`scout-lite-ci.yml` 只复用现有 native 测试和一次 C5 编译，不增加轮询任务。

CLI 命令名留在 `CommandLine.h`。标准应用只走这些名字。SigRoam 的额外状态走上一节的命名空间，不另做一套 Wi-Fi、GPS、SD 或 wardrive 栈。本仓库没有 FAP 源码。FAP 在 `pingequalab/sigroam-wardriving`，只读，不放进本仓库。

## 相对上游的实际差异

同步点 `d3ceb28afeda1196ca27c13adeaa69ada52383f3` 等于 `justcallmekoko/ESP32Marauder` 的 `master`。

`scout-lite/feature-gap` 在该点之上还有文档、`scout-lite-ci.yml`，以及很小的 CLI 上传补丁：无屏 `upload -d` 收集 `wardrive_` / `wigle-`，sidecar 按 `retry=false` 跳过，上传前若是 wardrive 且每个文件都成功才走原来的 `startScanFromCLI`。触发 SSID 已删。没有第二套引擎，没有 BSP。双应用命名空间还没写进固件。
