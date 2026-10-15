# 架构

当前树就是上游 Marauder。没有 Scout Lite BSP，本文也不添加。

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

CLI 命令名留在 `CommandLine.h`。SigRoam 若要兼容，走这些名字，不另做一套栈。仓库里没有 FAP。

## 相对上游的实际差异

同步点 `d3ceb28afeda1196ca27c13adeaa69ada52383f3` 等于 `justcallmekoko/ESP32Marauder` 的 `master`。

在该点之上，这个分支只有文档和 `scout-lite-ci.yml`。`esp32_marauder/` 与其他固件源码的 diff 是空的。没有本地固件补丁，没有假的 BSP。
