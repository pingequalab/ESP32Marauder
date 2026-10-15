# Scout Lite 固件项目 — 当前状态

> 最后更新：2026-10-15  
> 当前里程碑：M0 完成，M1 部分完成  
> 状态维护者：Cursor Project Coordinator（负责收集证据；不得自行宣布正式发布）

没有客户稳定固件。本文件不把编译通过写成实机可用。

已有文档，只链接，不在这里复述：

- `docs/scout-lite/PRODUCT.md`
- `docs/scout-lite/HARDWARE.md`
- `docs/scout-lite/FEATURE_MATRIX.md`
- `docs/scout-lite/ARCHITECTURE.md`
- `docs/scout-lite/UPSTREAM_TRACKING.md`
- `docs/scout-lite/TEST_PLAN.md`
- `docs/scout-lite/requirements-v1.0.md`
- `docs/scout-lite/deep-research-report-2.md`
- `docs/scout-lite/PROJECT_CHARTER.md`

## 1. 构建与版本基线

| 项目 | 当前值 | 证据链接 | 状态 |
| --- | --- | --- | --- |
| 官方稳定 Tag | `v1.18.0` = `b2b9f92382eb60b39b8741be3b567e416cdbe672`。该 tag 比下面的 SHA 落后 10 commits | 上游 tag `v1.18.0` | 已记录。下一次升级仍要重新检查最新 stable tag |
| 官方源 SHA | 文档前 fork 与上游对齐点 `d3ceb28afeda1196ca27c13adeaa69ada52383f3`。源码版本字符串 `MARAUDER_VERSION` 为 `v1.18.0`（`esp32_marauder/configs.h`） | 该 SHA | 不是 tag `v1.18.0` |
| PINGEQUA Fork SHA | 分支是 `master`，不是 `main`。当前 `master` 为合并提交 `dc263ebe968770f2ec4720524a1e5fc322105ffa`。审计 head `b242f00`。文档前 SHA 同上 | [PR #1](https://github.com/pingequalab/ESP32Marauder/pull/1) | 已合并的是文档审计，不是客户固件 |
| C5 构建 flag | Arduino CLI，`-DMARAUDER_C5`。FQBN `esp32:esp32:esp32c5:FlashSize=8M,PartitionScheme=default_8MB,PSRAM=enabled`。core `esp32:esp32@3.3.4` | `.github/workflows/build_parallel.yml`，`.github/workflows/scout-lite-ci.yml` | 构建字符串已记录。板级是否匹配 UNKNOWN |
| Flash/PSRAM/分区 | FQBN 写 8M、`default_8MB`、`PSRAM=enabled`。`HAS_PSRAM` 在 `esp32_marauder/configs.h:543` 被注释。FQBN `PSRAM=enabled` 只增加 `BOARD_HAS_PSRAM`。芯片容量 UNKNOWN | `configs.h:543`；上述 workflow | 芯片容量 UNKNOWN |
| Flipper UART 配置 | 无 Scout Lite 原理图。GPS 宏 `GPS_TX 14` / `GPS_RX 13`、SD 宏 `SD_CS 10`、`SD_MISO 2`、`SD_MOSI 7`、`SD_SCK 6` 只在源码里 | `FEATURE_MATRIX.md`，`HARDWARE.md` | 硬件 UNKNOWN |
| 当前可复现构建 | `b242f00` 上 PR CI 为 SUCCESS：workflow `Firmware Unit Tests` 的 `Native Unity tests`，workflow `Scout Lite CI` 的 `Native Unity tests`，以及 `Compile MARAUDER_C5`。`d3ceb28` 本地 app bin SHA256 `1d572f57bb1f90cf063952672414413e194ee7aef00de1c66218238dc26b60a8`。未发布，未刷机 | [PR #1](https://github.com/pingequalab/ESP32Marauder/pull/1) | E2。未刷机 |
| 当前客户可用稳定版 | 无 | 无发布记录 | 未发布。不是客户稳定固件 |

## 2. 当前正在进行的 Cursor 任务

| 任务 | Agent / 分支 / PR | 目标 | 阻塞/状态 | 下一动作 |
| --- | --- | --- | --- | --- |
| 源码与功能审计 | [PR #1](https://github.com/pingequalab/ESP32Marauder/pull/1)，head `b242f00`，merge `dc263eb` | 文档基线与功能矩阵 | MERGED | 不重复审计 |
| 章程与状态入库 | 分支 `docs/scout-lite-charter` | 保存章程、合并 `AGENTS.md`、填写本状态 | 草案。未合并 | 合并需要 Colin 明确批准 |

## 3. P0 功能证据矩阵

没有 `CONFIRMED_WORKING` 行。双频、BLE、GPS、SD、wardrive、WiGLE/WDG、CLI 在 `MARAUDER_C5` 上源码存在，Scout Lite 上未测。

| 功能 | 上游源码/宏 | Scout Lite 状态 | 证据等级 E0–E4 | 风险 | 下一步 |
| --- | --- | --- | --- | --- | --- |
| 启动/构建/分区 | `-DMARAUDER_C5`；FQBN 见第 1 节 | 编译证据见第 1 节。分区是否等于 Scout Lite：UNKNOWN。未刷机 | 编译 E2；分区 UNKNOWN | 构建绿灯不等于可刷 | 不把这次 bin 当客户固件 |
| GPS/No-Fix | `MARAUDER_C5` 上 GPS 源码在。宏 `GPS_TX 14` / `GPS_RX 13` 只是源码 | `SOURCE_PRESENT_UNTESTED`。硬件 UNKNOWN | E1 | 引脚未核实 | 先拿原理图或实测日志 |
| microSD/日志 | `MARAUDER_C5` 上 SD 源码在。宏 `SD_CS 10`、`SD_MISO 2`、`SD_MOSI 7`、`SD_SCK 6` 只是源码 | `SOURCE_PRESENT_UNTESTED`。硬件 UNKNOWN | E1 | 引脚未核实 | 先拿原理图或实测日志 |
| Wardriving/双频 | `MARAUDER_C5` 上 wardrive 与双频源码在 | `SOURCE_PRESENT_UNTESTED` | E1 | 实机未测 | 板级证据之前不改固件 |
| WiGLE 上传 | `MARAUDER_C5` 上 WiGLE/WDG 源码在 | `SOURCE_PRESENT_UNTESTED` | E1 | 实网上传未测 | 不假设上传可用 |
| Marauder CLI | `MARAUDER_C5` 上 CLI 源码在 | `SOURCE_PRESENT_UNTESTED` | E1 | 无 FAP 命令清单 | 见第 5 节任务 2 |
| SigRoam FAP | 本仓库没有 SigRoam FAP | UNKNOWN | 无本仓库源码 | 兼容性未知 | 有真实命令清单或抓包之前不做合约测试 |

## 4. 活跃 PR、待上游贡献补丁与冲突

| ID | PR | 类型（upstream / board / extension） | 当前状态 | 下一步 |
| --- | --- | --- | --- | --- |
| 审计 | [PR #1](https://github.com/pingequalab/ESP32Marauder/pull/1) | 文档 | MERGED，`dc263eb` | 不重开 |
| 章程 | 分支 `docs/scout-lite-charter` | 文档 | 草案，未合并 | 等 Colin 批准后才合并 |
| 上游补丁 | 无 | upstream | 无给上游的 PR | 没有已证实的通用缺陷可送上游 |

## 5. 当前三个最高价值任务

1. 任何板级补丁之前，先拿到 Scout Lite 原理图，或一份实测的引脚 / Flash / PSRAM 日志。依赖：现在没有这份硬件证据。不适合上游 PR。
2. 只有已经存在真实 FAP 命令清单或抓包时，才做 CLI/SigRoam 合约测试。在证明真实不兼容之前，只做测试，不改协议。依赖：任务所需的 FAP 命令清单或抓包；本仓库没有这份材料。在那之前不适合上游 PR。
3. 第 1 项到手之前不开始 M2 固件。通道 A 自动化等 Colin 批准，本变更不实现它。依赖：任务 1 的硬件证据；通道 A 另需 Colin 批准。不适合上游 PR。

## 6. Decision Pending

不改写章程里的愿景。下面各项与已落地事实不一致，留在这里：

- 章程仍写审计进行中。审计 PR 已经合并：[PR #1](https://github.com/pingequalab/ESP32Marauder/pull/1)，merge `dc263eb`，head `b242f00`。
- 章程写稳定分支 `main`。这个 fork 用 `master`。
- 章程枚举和矩阵的五个中文标签不一致。`FEATURE_MATRIX.md` 表头已加对照：已验证 → 仅构建或自动化测试（E2）；源码支持 → `SOURCE_PRESENT_UNTESTED`（E1）；未知 → `UNKNOWN`。没有改每一行的标签，也没有新增 `CONFIRMED_WORKING`、`INTEGRATION_BLOCKED`、`NOT_APPLICABLE`。
- 「可以直接刷 DevKit 固件」保持 UNKNOWN。
- 本变更不添加每日 GitHub Action，也不添加 Cursor subscription。通道 A 只作为等待批准的提案：每天检查一次 tag；没有变化就安静结束；稳定 tag 移动时只开一个 issue。安静的那一天不跑 AI。`AGENTS.md` 仍要求自动化由事件驱动。批准之前不把这条定时检查写进 workflow。

## 7. 最近已完成工作与证据

| 日期 | 完成事项 | 证据（Commit / PR / CI / 实机日志） | 剩余问题 |
| --- | --- | --- | --- |
| 2026-10-15 | 审计文档并入 `master` | [PR #1](https://github.com/pingequalab/ESP32Marauder/pull/1)，merge `dc263ebe968770f2ec4720524a1e5fc322105ffa`，head `b242f00`。该 head 上 workflow `Firmware Unit Tests` 的 `Native Unity tests`、workflow `Scout Lite CI` 的 `Native Unity tests`、以及 `Compile MARAUDER_C5` 为 SUCCESS。`d3ceb28` 本地 app bin SHA256 `1d572f57bb1f90cf063952672414413e194ee7aef00de1c66218238dc26b60a8`，未发布，未刷机 | 原理图、芯片容量、实机、客户稳定版都没有 |
| 2026-10-15 | 保存章程并填写本状态 | 分支 `docs/scout-lite-charter`。章程文件与上传原文一致 | Decision Pending 未改章程原文 |

## 8. 最近一次同步摘要

- 上游版本：源码版本字符串 `v1.18.0`。tag `v1.18.0`（`b2b9f92382eb60b39b8741be3b567e416cdbe672`）比 `d3ceb28afeda1196ca27c13adeaa69ada52383f3` 落后 10 commits。
- 新增功能及受影响模块：无固件改动。本状态对应的变更只动章程、状态、`AGENTS.md` 和矩阵表头对照。
- 自动测试：PR #1 在 `b242f00` 上 `Firmware Unit Tests` / `Native Unity tests`、`Scout Lite CI` / `Native Unity tests`、`Compile MARAUDER_C5` 为 SUCCESS。本变更不新增 workflow，也不重跑固件构建。
- 实机验证：无。未刷机。
- 是否允许稳定版发布：否。没有客户稳定固件。合并稳定分支需要 Colin 明确批准。
