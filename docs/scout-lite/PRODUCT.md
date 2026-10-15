# Scout Lite 产品目标

来源是 `requirements-v1.0.md`。这里写目标，不写「已经在 Scout Lite 上工作」。

Scout Lite 固件以 JustCallMeKoko 官方 ESP32 Marauder 为唯一基础。不重写 Marauder 核心，不用其他第三方固件替代。

目标是：在现有 Scout Lite 硬件允许的范围内，尽量补齐官方功能，同时让相对上游的长期差异最小。

## 硬件目标

设计目标是现有 PINGEQUA Scout Lite：ESP32-C5-WROOM-1U-N8R8、Quectel L86-M33、microSD、2.4/5 GHz Wi-Fi、外置天线、Flipper Zero GPIO/UART、USB-C。第一阶段不重做 PCB。这些是目标，不是已核实配置。核实标准见 `HARDWARE.md`。

## 技术目标

- 上游：`justcallmekoko/ESP32Marauder`。
- 默认官方目标：`MARAUDER_C5` / ESP32-C5-DevKitC-1。
- `v1.18.0` 只是候选钉扎。同步点以 `UPSTREAM_TRACKING.md` 为准。
- 其他官方 C5 目标只做功能比较，不混用它们的二进制。

## 功能目标

在官方实现里逐项核对，缺了再按这个顺序补：直接启用已有功能，修正板型配置，修 C5 通用 bug，复用其他官方目标里已经公开的实现，最后才写 Scout Lite 独有代码。

目标范围：

- 2.4/5 GHz Wi-Fi
- BLE
- Wardriving 和日志
- GPS 状态和轨迹
- SD 与文件
- WiGLE / WDGWars 上传
- Wi-Fi 配置
- 状态、内存、错误恢复
- CLI / UART / Flipper
- 现有 C5 目标里、现有硬件可能支持的其他官方功能

## 维护目标

用薄的 board support 和独立模块表达差异。保持标准 CLI 和现有 SigRoam FAP。每个产品补丁要有用途和测试证据。通用修复上游接受后，从本地补丁里去掉。

后续希望用 GitHub Actions 检查上游变化、编 Scout Lite 固件、跑测试，并在人工批准后才发布。AI 不能自行部署生产固件。

## 验收目标

最终要在 Scout Lite 实机上验证过，保留 Marauder 标准兼容，差异清楚，并且能继续接官方更新。这一条现在还不是完成状态。
