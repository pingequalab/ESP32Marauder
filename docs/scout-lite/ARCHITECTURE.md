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

## 以后的薄板型层

Scout Lite 若要有自己的目标，应是 `configs.h` 里一个新的板型宏，加上 workflow 里的一条 FQBN。GPS、SD、UART 若与 `MARAUDER_C5` 不同，只放这个宏下面。

不要复制 Mini v3、v8 或别的机型的大段实现。不要加框架。CLI 命令名保持 Marauder 原样。SigRoam 仍走现有 UART CLI。

这一步还没开始。仓库里没有 `SCOUT` 宏。

## 相对上游的实际差异

同步点 `d3ceb28afeda1196ca27c13adeaa69ada52383f3` 等于 `justcallmekoko/ESP32Marauder` 的 `master`。

在该点之上，这个分支只加文档和 `scout-lite-ci.yml`。`esp32_marauder/` 与其他固件源码的 diff 是空的。没有本地固件补丁。
