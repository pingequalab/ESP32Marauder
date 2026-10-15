# Scout Lite 固件项目 — 当前状态

> 最后更新：2026-10-15  
> 当前里程碑：M0 完成，M1 部分完成  
> 状态维护者：Cursor Project Coordinator（负责收集证据；不得自行宣布正式发布）

没有客户稳定固件。本文件不把编译通过写成实机可用。

通道 A 只读下面这一行，当作已经处理过的上游稳定 tag。稳定 tag 指 `justcallmekoko/ESP32Marauder` 最新的非 draft、非 prerelease Release。人把这一行改成新 tag 之后，下一次检查才把它当成已处理。

scout-lite-upstream-stable-tag: v1.18.0

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
| Flash/PSRAM/分区 | FQBN 写 8M、`default_8MB`、`PSRAM=enabled`。`HAS_PSRAM` 在 `esp32_marauder/configs.h:543` 被注释。FQBN `PSRAM=enabled` 只增加 `BOARD_HAS_PSRAM`。丝印 `ESP32-C5-WROOM-1U-N8R8` 只是料号，不是运行时测量。不把 PSRAM 标成已启用。芯片容量 UNKNOWN | `configs.h:543`；`HARDWARE.md` | 芯片容量 UNKNOWN |
| SD / GPS 固件脚 | Colin 2026-10-15：重要接线按 ESP32-C5 DevKit 的 `MARAUDER_C5`。SD 是 IO2/IO6/IO7/IO10。GPS 固件脚是 GPIO14=`GPS_TX`（RX），GPIO13=`GPS_RX`（TX） | `HARDWARE.md` | 原理图确认。实机未测。不是 `CONFIRMED_WORKING` |
| USB 自动切换 | 不声称与 DevKit 相同 | `HARDWARE.md` | 不再调查，除非以后的任务需要 |
| 下载模式 | 实板不能由 Flipper 进入下载模式。只有板上 BOOT 和 EN。`PB2`/`PC3` 不是下载路径 | `HARDWARE.md` | Colin 2026-10-15。不是功能项的 `CONFIRMED_WORKING` |
| 当前可复现构建 | `b242f00` 上 PR CI 为 SUCCESS：workflow `Firmware Unit Tests` 的 `Native Unity tests`，workflow `Scout Lite CI` 的 `Native Unity tests`，以及 `Compile MARAUDER_C5`。`d3ceb28` 本地 app bin SHA256 `1d572f57bb1f90cf063952672414413e194ee7aef00de1c66218238dc26b60a8`。未发布，未刷机 | [PR #1](https://github.com/pingequalab/ESP32Marauder/pull/1) | E2。未刷机 |
| 当前客户可用稳定版 | 无 | 无发布记录 | 未发布。不是客户稳定固件 |

GPS 接到 Flipper pin 15 和 16，不是官方 GPIO13/14。已记录，不是任务，不计划改固件。

## 2. 当前正在进行的 Cursor 任务

| 任务 | Agent / 分支 / PR | 目标 | 阻塞/状态 | 下一动作 |
| --- | --- | --- | --- | --- |
| 源码与功能审计 | [PR #1](https://github.com/pingequalab/ESP32Marauder/pull/1)，head `b242f00`，merge `dc263eb` | 文档基线与功能矩阵 | MERGED | 不重复审计 |
| 章程与状态入库 | 分支 `docs/scout-lite-charter` | 保存章程、合并 `AGENTS.md`、填写本状态、加入通道 A | 草案。未合并 | Colin 已于 2026-10-15 批准：required CI 为绿后可合入 `master`。Agent 不合并。不是固件发布，不刷机 |

## 3. P0 功能证据矩阵

没有 `CONFIRMED_WORKING` 行。双频、BLE、GPS、SD、wardrive、WiGLE/WDG、CLI 在 `MARAUDER_C5` 上源码存在，Scout Lite 上未测。

| 功能 | 上游源码/宏 | Scout Lite 状态 | 证据等级 E0–E4 | 风险 | 下一步 |
| --- | --- | --- | --- | --- | --- |
| 启动/构建/分区 | `-DMARAUDER_C5`；FQBN 见第 1 节 | 编译证据见第 1 节。分区是否等于 Scout Lite：UNKNOWN。未刷机 | 编译 E2；分区 UNKNOWN | 构建绿灯不等于可刷 | 不把这次 bin 当客户固件 |
| GPS/No-Fix | `GPS_TX 14` 是固件 RX（GPIO14）。`GPS_RX 13` 是固件 TX（GPIO13）。`Serial2.begin(..., GPS_TX, GPS_RX)` | 原理图与 `MARAUDER_C5` 相同。功能仍是 `SOURCE_PRESENT_UNTESTED`。不是 `CONFIRMED_WORKING` | 原理图确认；源码 E1；实机未到 E3 | 实机未测 | 不为这两根固件脚改宏 |
| microSD/日志 | `SD_MISO 2`，`SD_SCK 6`，`SD_MOSI 7`，`SD_CS 10` | 原理图与 `MARAUDER_C5` 相同。功能仍是 `SOURCE_PRESENT_UNTESTED`。不是 `CONFIRMED_WORKING` | 原理图确认；源码 E1；实机未到 E3 | 实机未测 | 不为这四根线改宏 |
| Wardriving/双频 | `MARAUDER_C5` 上 wardrive 与双频源码在 | `SOURCE_PRESENT_UNTESTED` | E1 | 实机未测 | 板级证据之前不改固件 |
| WiGLE 上传 | `MARAUDER_C5` 上 WiGLE/WDG 源码在 | `SOURCE_PRESENT_UNTESTED` | E1 | 实网上传未测 | 不假设上传可用 |
| Marauder CLI | `MARAUDER_C5` 上 CLI 源码在 | `SOURCE_PRESENT_UNTESTED` | E1 | 无 FAP 命令清单 | 见第 5 节任务 2 |
| SigRoam FAP | 本仓库没有 SigRoam FAP | UNKNOWN | 无本仓库源码 | 兼容性未知 | 有真实命令清单或抓包之前不做合约测试 |

## 4. 活跃 PR、待上游贡献补丁与冲突

| ID | PR | 类型（upstream / board / extension） | 当前状态 | 下一步 |
| --- | --- | --- | --- | --- |
| 审计 | [PR #1](https://github.com/pingequalab/ESP32Marauder/pull/1) | 文档 | MERGED，`dc263eb` | 不重开 |
| 章程 | 分支 `docs/scout-lite-charter` | 文档 | 草案，未合并 | Colin 已于 2026-10-15 批准：required CI 为绿后可合入 `master`。Agent 不执行合并 |
| 上游补丁 | 无 | upstream | 无给上游的 PR | 没有已证实的通用缺陷可送上游 |

## 5. 当前三个最高价值任务

1. SD 与 GPS 固件脚已在原理图层对齐 `MARAUDER_C5`，仍不是实机 `CONFIRMED_WORKING`，不为这些宏改固件。非关键走线已关闭，不再追查。依赖：实机日志。不适合上游 PR。
2. 只有已经存在真实 FAP 命令清单或抓包时，才做 CLI/SigRoam 合约测试。在证明真实不兼容之前，只做测试，不改协议。依赖：任务所需的 FAP 命令清单或抓包；本仓库没有这份材料。在那之前不适合上游 PR。
3. 实机证据之前不开始 M2 固件。通道 A 已于 2026-10-15 批准，workflow 是 `.github/workflows/scout-lite-upstream-tag.yml`，合入 `master` 之后才按日运行。依赖：任务 1 的实机日志。不适合上游 PR。不打开固件 PR。

## 6. Decision Pending

不改写章程里的愿景。下面各项与已落地事实不一致，留在这里：

- 章程仍写审计进行中。审计 PR 已经合并：[PR #1](https://github.com/pingequalab/ESP32Marauder/pull/1)，merge `dc263eb`，head `b242f00`。
- 章程写稳定分支 `main`。这个 fork 用 `master`。
- 章程枚举和矩阵的五个中文标签不一致。`FEATURE_MATRIX.md` 表头已加对照：已验证 → 仅构建或自动化测试（E2）；源码支持 → `SOURCE_PRESENT_UNTESTED`（E1）；未知 → `UNKNOWN`。没有改每一行的标签，也没有新增 `CONFIRMED_WORKING`、`INTEGRATION_BLOCKED`、`NOT_APPLICABLE`。
- 「可以直接刷 DevKit 固件」保持 UNKNOWN。SD/GPS 同线不是刷机许可。
- PSRAM 容量仍是 UNKNOWN。`HAS_PSRAM` 保持注释，丝印 N8R8 不是已测到的启用。

## 7. 最近已完成工作与证据

| 日期 | 完成事项 | 证据（Commit / PR / CI / 实机日志） | 剩余问题 |
| --- | --- | --- | --- |
| 2026-10-15 | 审计文档并入 `master` | [PR #1](https://github.com/pingequalab/ESP32Marauder/pull/1)，merge `dc263ebe968770f2ec4720524a1e5fc322105ffa`，head `b242f00`。该 head 上 workflow `Firmware Unit Tests` 的 `Native Unity tests`、workflow `Scout Lite CI` 的 `Native Unity tests`、以及 `Compile MARAUDER_C5` 为 SUCCESS。`d3ceb28` 本地 app bin SHA256 `1d572f57bb1f90cf063952672414413e194ee7aef00de1c66218238dc26b60a8`，未发布，未刷机 | 原理图、芯片容量、实机、客户稳定版都没有 |
| 2026-10-15 | 保存章程并填写本状态 | 分支 `docs/scout-lite-charter`。章程文件与上传原文一致 | Decision Pending 未改章程原文 |
| 2026-10-15 | Colin 批准文档 PR 合入条件，并批准通道 A | 批准范围只有这两件。workflow：`.github/workflows/scout-lite-upstream-tag.yml`。已处理 tag 行是 `scout-lite-upstream-stable-tag: v1.18.0`。未合并，未发布固件，未刷机 | schedule 要等该文件在 `master` 上才运行。原理图未确认 |
| 2026-10-15 | Colin 确认 SD/GPS 接线，USB 开关除外 | `HARDWARE.md`。原理图确认，实机未测，不是 `CONFIRMED_WORKING`。未改固件，未合并，未刷机 | 当天留下的非关键走线问题已在下一条关闭 |
| 2026-10-15 | Colin 收窄硬件范围 | 重要接线仍是 DevKit 的 SD 与 GPS 固件脚。下载模式只有板上 BOOT 和 EN。非关键走线不再追查。USB 自动切换仍不声称与 DevKit 相同。未改固件，未合并，未刷机 | 实机未测。PSRAM 容量 UNKNOWN。不是 `CONFIRMED_WORKING` |

## 8. 最近一次同步摘要

- 上游版本：源码版本字符串 `v1.18.0`。tag `v1.18.0`（`b2b9f92382eb60b39b8741be3b567e416cdbe672`）比 `d3ceb28afeda1196ca27c13adeaa69ada52383f3` 落后 10 commits。
- 新增功能及受影响模块：无固件改动。另加一个每日 tag workflow。该 workflow 不编译固件。
- 自动测试：PR #1 在 `b242f00` 上 `Firmware Unit Tests` / `Native Unity tests`、`Scout Lite CI` / `Native Unity tests`、`Compile MARAUDER_C5` 为 SUCCESS。通道 A 不在 pull request 上跑。GitHub 的 schedule 只从默认分支 `master` 触发。
- 实机验证：无。未刷机。
- 是否允许稳定版发布：否。没有客户稳定固件。Colin 批准的是这份文档 PR 在 required CI 为绿后可合入 `master`，不是固件发布。Agent 不合并，不刷机。
