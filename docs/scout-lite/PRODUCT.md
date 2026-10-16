# Scout Lite 产品目标

来源是 `requirements-v1.0.md`，并以 2026-10-16 的范围更正为准。这里写目标，不写「已经在 Scout Lite 上工作」。

Scout Lite 固件以 JustCallMeKoko 官方 ESP32 Marauder 为唯一基础。不重写 Marauder 核心，不用其他第三方固件替代。

Fork 停在官方 `MARAUDER_C5` / ESP32-C5 DevKit。目标是：在 Scout Lite 硬件上，跑它能够运行的最全官方 Marauder 功能。只从本树里其他官方目标做最小移植。候选必须已经在这棵树里，由宏或很小的板级文件挡住，并且不需要屏幕、按键或这块板没有的硬件。不发明功能。不因为另一块板有某段代码就整段复制。

SigRoam 优化、FAP 合约测试、SigRoam 兼容超出范围。历史句子可以留在旧文档里，不作为目标。

## 硬件目标

设计目标是现有 PINGEQUA Scout Lite：ESP32-C5-WROOM-1U-N8R8、Quectel L86-M33、microSD、2.4/5 GHz Wi-Fi、外置天线、Flipper Zero GPIO/UART、USB-C。第一阶段不重做 PCB。

已经关掉、不要重开的结论见 `HARDWARE.md`：

- SD IO2/IO6/IO7/IO10，GPS 固件脚 GPIO14 RX（`GPS_TX`）/ GPIO13 TX（`GPS_RX`），与 DevKit 一致。原理图确认，实机未测。
- 板上只有 BOOT 和 EN。Flipper 不能进入下载模式。
- GPS 在 Flipper pin 15/16 上超出范围，不为此改固件。
- USB 自动切换不声称与 DevKit 相同，不调查。
- `HAS_PSRAM` 仍是注释。丝印 N8R8 不是测到的启用。

## 技术目标

- 上游：`justcallmekoko/ESP32Marauder`。
- 默认官方目标：`MARAUDER_C5` / ESP32-C5-DevKitC-1。
- `v1.18.0` 只是候选钉扎。同步点以 `UPSTREAM_TRACKING.md` 为准。
- 其他官方 C5 目标只做功能比较，不混用它们的二进制，也不整板复制它们的引脚和屏驱。

## 功能目标

对照表在 `FEATURE_MATRIX.md`。以后的代码顺序在 `MINIMAL_CHANGE_PLAN.md`。本文件不授权实现那些项。

已经在 `MARAUDER_C5` 里、并且硬件接线与 DevKit 对齐的官方功能，保持官方宏，不为 Scout Lite 再写一层。需要屏幕、按键、T-Dongle 显示、别的板的 SD/GPS 脚，或 Flipper 下载控制的功能，不移植。

`HAS_PSRAM` 在实测之前保持注释。

## 维护目标

用薄的 board support 表达差异。现在不需要新的板型宏。保持官方 CLI。每个产品补丁要有用途和测试证据。通用修复上游接受后，从本地补丁里去掉。

后续希望用 GitHub Actions 检查上游变化、编官方 C5 固件、跑测试，并在人工批准后才发布。AI 不能自行部署生产固件。内部构建记录不是客户发布。

## 验收目标

最终要在 Scout Lite 实机上验证过，保留 Marauder 官方行为，差异清楚，并且能继续接官方更新。这一条现在还不是完成状态。没有客户稳定固件。
