# Scout Lite 固件项目 — 当前状态

> 最后更新：2026-10-16  
> 当前里程碑：M0 完成，M1 部分完成。范围已按 Colin 2026-10-16 收窄  
> 状态维护者：Cursor Project Coordinator（负责收集证据；不得自行宣布正式发布）

没有客户稳定固件。本文件不把编译通过写成实机可用。

2026-10-16：一块 `MARAUDER_C5` 固件同时服务标准 Flipper Marauder 应用和 SigRoam。标准 CLI 不改。SigRoam 是可选命名空间，还没写进固件。当天早些时候「SigRoam 超出范围」的句子留在章程里，以较晚的双应用节为准。差异表是 `FEATURE_MATRIX.md`。应用命令矩阵是 `FLIPPER_MARAUDER_COMPATIBILITY_MATRIX.md`。以后的代码顺序是 `MINIMAL_CHANGE_PLAN.md` 第 0 节。本轮只写文档。

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
- `docs/scout-lite/PROJECT_CHARTER.md`（含 2026-10-16 双应用节和更早的范围更正，原文未整章重写）
- `docs/scout-lite/FLIPPER_MARAUDER_COMPATIBILITY_MATRIX.md`
- `docs/scout-lite/MINIMAL_CHANGE_PLAN.md`
- `docs/scout-lite/CODE_GAP.md`
- `docs/scout-lite/INTERNAL_BETA.md`

## 1. 构建与版本基线

| 项目 | 当前值 | 证据链接 | 状态 |
| --- | --- | --- | --- |
| 官方稳定 Tag | `v1.18.0` = `b2b9f92382eb60b39b8741be3b567e416cdbe672`。该 tag 比下面的 SHA 落后 10 commits | 上游 tag `v1.18.0` | 已记录。下一次升级仍要重新检查最新 stable tag |
| 官方源 SHA | 文档前 fork 与上游对齐点 `d3ceb28afeda1196ca27c13adeaa69ada52383f3`。源码版本字符串 `MARAUDER_VERSION` 为 `v1.18.0`（`esp32_marauder/configs.h`） | 该 SHA | 不是 tag `v1.18.0` |
| PINGEQUA Fork SHA | 分支是 `master`，不是 `main`。2026-10-16 的 `origin/master` 是 `f4bc76727e7f29b7ca64da9c972c9103523c6006`（PR #2 的合并）。审计 head 仍是 `b242f00`。更早的审计合并是 `dc263ebe968770f2ec4720524a1e5fc322105ffa` | [PR #1](https://github.com/pingequalab/ESP32Marauder/pull/1)，[PR #2](https://github.com/pingequalab/ESP32Marauder/pull/2) | 已合并的是文档，不是客户固件 |
| C5 构建 flag | Arduino CLI，`-DMARAUDER_C5`。FQBN `esp32:esp32:esp32c5:FlashSize=8M,PartitionScheme=default_8MB,PSRAM=enabled`。core `esp32:esp32@3.3.4` | `.github/workflows/build_parallel.yml`，`.github/workflows/scout-lite-ci.yml` | 构建字符串已记录。板级是否匹配 UNKNOWN |
| Flash/PSRAM/分区 | FQBN 写 8M、`default_8MB`、`PSRAM=enabled`。`HAS_PSRAM` 在 `esp32_marauder/configs.h:543` 被注释。FQBN `PSRAM=enabled` 只增加 `BOARD_HAS_PSRAM`。丝印 `ESP32-C5-WROOM-1U-N8R8` 只是料号，不是运行时测量。不把 PSRAM 标成已启用。芯片容量 UNKNOWN | `configs.h:543`；`HARDWARE.md` | 芯片容量 UNKNOWN |
| SD / GPS 固件脚 | Colin 2026-10-15：重要接线按 ESP32-C5 DevKit 的 `MARAUDER_C5`。SD 是 IO2/IO6/IO7/IO10。GPS 固件脚是 GPIO14=`GPS_TX`（RX），GPIO13=`GPS_RX`（TX） | `HARDWARE.md` | 原理图确认。实机未测。不是 `CONFIRMED_WORKING` |
| USB 自动切换 | 不声称与 DevKit 相同 | `HARDWARE.md` | 不再调查，除非以后的任务需要 |
| 下载模式 | 实板不能由 Flipper 进入下载模式。只有板上 BOOT 和 EN。`PB2`/`PC3` 不是下载路径 | `HARDWARE.md` | Colin 2026-10-15。不是功能项的 `CONFIRMED_WORKING` |
| 当前可复现构建 | 2026-10-16 在 commit `8b73c1905e810771da7f97fac63e89b2f5d9c1b3` 重编未修改的 `MARAUDER_C5`。退出码 0。app bin SHA256 `e8c5b57f16745a2bcfbd5e8752fa68301345262cb4f6578556ae1b6316a681f0`。更早的 `d3ceb28` 本地 bin 是 `1d572f57bb1f90cf063952672414413e194ee7aef00de1c66218238dc26b60a8`；两次 sketch 大小都是 1873710 字节，差异是编译日期字符串。`b242f00` 上 PR CI 的 `Compile MARAUDER_C5` 仍是 SUCCESS。未发布，未刷机 | `docs/scout-lite/INTERNAL_BETA.md`；[PR #1](https://github.com/pingequalab/ESP32Marauder/pull/1) | E2。未修改的官方目标。不是功能移植。未刷机 |
| 当前客户可用稳定版 | 无 | 无发布记录 | 未发布。不是客户稳定固件 |

GPS 接到 Flipper pin 15 和 16，不是官方 GPIO13/14。已记录。2026-10-16 起超出范围，不计划改固件。

## 2. 当前正在进行的 Cursor 任务

| 任务 | Agent / 分支 / PR | 目标 | 阻塞/状态 | 下一动作 |
| --- | --- | --- | --- | --- |
| 源码与功能审计 | [PR #1](https://github.com/pingequalab/ESP32Marauder/pull/1)，head `b242f00`，merge `dc263eb` | 文档基线与功能矩阵 | MERGED | 不重复审计 |
| 章程与状态入库 | [PR #2](https://github.com/pingequalab/ESP32Marauder/pull/2)，合并 `f4bc767` | 保存章程、合并 `AGENTS.md`、填写本状态、加入通道 A | 已合并到 `master`。当时状态文件仍写「未合并」，以这个合并提交为准 | 不重开。2026-10-16 的范围更正是另一份文档 PR |
| 功能差距与双应用文档 | 分支 `scout-lite/feature-gap` | 标准 CLI 不改。SigRoam 记为同一固件上的可选命名空间。写架构、命令矩阵和最小顺序 | 进行中。本轮不改固件 | 不合并，不发版，不刷机 |

## 3. P0 功能证据矩阵

没有 `CONFIRMED_WORKING` 行。双频、BLE、GPS、SD、wardrive、WiGLE/WDG、CLI 在 `MARAUDER_C5` 上源码存在，Scout Lite 上未测。

| 功能 | 上游源码/宏 | Scout Lite 状态 | 证据等级 E0–E4 | 风险 | 下一步 |
| --- | --- | --- | --- | --- | --- |
| 启动/构建/分区 | `-DMARAUDER_C5`；FQBN 见第 1 节 | 编译证据见第 1 节。分区是否等于 Scout Lite：UNKNOWN。未刷机 | 编译 E2；分区 UNKNOWN | 构建绿灯不等于可刷 | 不把这次 bin 当客户固件 |
| GPS/No-Fix | `GPS_TX 14` 是固件 RX（GPIO14）。`GPS_RX 13` 是固件 TX（GPIO13）。`Serial2.begin(..., GPS_TX, GPS_RX)` | 原理图与 `MARAUDER_C5` 相同。功能仍是 `SOURCE_PRESENT_UNTESTED`。不是 `CONFIRMED_WORKING` | 原理图确认；源码 E1；实机未到 E3 | 实机未测 | 不为这两根固件脚改宏 |
| microSD/日志 | `SD_MISO 2`，`SD_SCK 6`，`SD_MOSI 7`，`SD_CS 10` | 原理图与 `MARAUDER_C5` 相同。功能仍是 `SOURCE_PRESENT_UNTESTED`。不是 `CONFIRMED_WORKING` | 原理图确认；源码 E1；实机未到 E3 | 实机未测 | 不为这四根线改宏 |
| Wardriving/双频 | `MARAUDER_C5` 上 wardrive 与双频源码在 | `SOURCE_PRESENT_UNTESTED` | E1 | 实机未测 | 板级证据之前不改固件 |
| WiGLE 上传 | `MARAUDER_C5` 上 WiGLE/WDG 源码在 | `SOURCE_PRESENT_UNTESTED` | E1 | 实网上传未测 | 不假设上传可用 |
| Marauder CLI | `MARAUDER_C5` 上 CLI 源码在 | `SOURCE_PRESENT_UNTESTED` | E1 | 实机未测 | 保持标准命令和回复。SigRoam 只用单独命名空间 |
| SigRoam FAP | 本仓库没有 FAP 源码。应用在 `pingequalab/sigroam-wardriving` tag `v0.6.1`（`69ecfd7`） | 同一固件上的可选命名空间，固件入口未写 | 应用源码已读；固件未实现 | 不改标准命令 | 按 `MINIMAL_CHANGE_PLAN.md` 第 0 节。不刷机 |

## 4. 活跃 PR、待上游贡献补丁与冲突

| ID | PR | 类型（upstream / board / extension） | 当前状态 | 下一步 |
| --- | --- | --- | --- | --- |
| 审计 | [PR #1](https://github.com/pingequalab/ESP32Marauder/pull/1) | 文档 | MERGED，`dc263eb` | 不重开 |
| 章程 | [PR #2](https://github.com/pingequalab/ESP32Marauder/pull/2) | 文档 | 已合并，`f4bc767`。2026-10-15 的批准记录保留 | 不重开 |
| 功能差距 | 分支 `scout-lite/feature-gap` | 文档，加一次未改宏的 C5 构建记录 | 草案 | Agent 不合并。不是固件发布 |
| 上游补丁 | 无 | upstream | 无给上游的 PR | 没有已证实的通用缺陷可送上游 |

## 5. 当前三个最高价值任务

2026-10-15 的任务 2 是「有 FAP 清单再做 SigRoam 合约测试」。2026-10-16 改为：SigRoam 是同一固件上的可选命名空间，不另做合约测试框架，也不改标准命令。本仓库仍然没有 FAP 源码。

1. 不为已对齐的 SD/GPS 宏改固件。实机仍不是 `CONFIRMED_WORKING`。依赖：实机日志。不适合上游 PR。不打开功能移植 PR。
2. Phase 1 已进 CLI `upload`：sidecar 跳过、仅当上传前正在 wardrive 且没有失败文件时回到 wardrive、单文件再试 2 次。无 fix 不占 MAC 也已做。无屏 `upload` 会从 SD 根填 `wardrive_` / `wigle-` 日志，空指针不解引用。触发 SSID 已删：没有 `t_ssid` / `t_pass`，也没有新的串口句子。回复仍是原来的 `Upload OK`、`WiGLE failed`、`Upload complete`、`StartingWardrive. Stop with stopscan` 和已有 `[UPLOAD]` 行。标准命令停在 Flipper 应用 `flipperzero-wifi-marauder` 已经发送的内容，以及 `[BUF/BEGIN]`、`[BUF/CLOSE]`。SigRoam 命名空间还没写。Phase 2 不做：SSID 排除、无屏删日志。不做扫描器重写、Node/Core、ESP-NOW、屏幕 UI、PSRAM、Dock 触发。依赖：`MINIMAL_CHANGE_PLAN.md` 第 0 节和第 1b 节。不适合整段上游 PR。
3. `HAS_PSRAM` 保持注释，直到有运行时容量。需要屏、按键或 Flipper 下载控制的代码不移植。Phase 1 的 C5 构建记录见 `INTERNAL_BETA.md`。不提交 bin，不上传 Release，不刷机。

## 6. Decision Pending

不改写章程里的愿景。下面各项与已落地事实不一致，留在这里：

- 章程仍写审计进行中。审计 PR 已经合并：[PR #1](https://github.com/pingequalab/ESP32Marauder/pull/1)，merge `dc263eb`，head `b242f00`。
- 章程写稳定分支 `main`。这个 fork 用 `master`。
- 章程第 5 节仍用 `CONFIRMED_WORKING` 那套枚举。2026-10-16 的 `FEATURE_MATRIX.md` 改成动作列：`已在 C5` / `可最小打开` / `硬件不具备` / `超出范围` / `未知`。旧矩阵的构建证据留在该文件末尾。不为此重写章程枚举。
- 「可以直接刷 DevKit 固件」保持 UNKNOWN。SD/GPS 同线不是刷机许可。
- PSRAM 容量仍是 UNKNOWN。`HAS_PSRAM` 保持注释，丝印 N8R8 不是已测到的启用。
- 章程正文里的 SigRoam 目标句保留。2026-10-16 早些时候的更正节曾写超出范围。同日较晚的双应用节盖过那一句，不以改写全文来删历史。

## 7. 最近已完成工作与证据

| 日期 | 完成事项 | 证据（Commit / PR / CI / 实机日志） | 剩余问题 |
| --- | --- | --- | --- |
| 2026-10-15 | 审计文档并入 `master` | [PR #1](https://github.com/pingequalab/ESP32Marauder/pull/1)，merge `dc263ebe968770f2ec4720524a1e5fc322105ffa`，head `b242f00`。该 head 上 workflow `Firmware Unit Tests` 的 `Native Unity tests`、workflow `Scout Lite CI` 的 `Native Unity tests`、以及 `Compile MARAUDER_C5` 为 SUCCESS。`d3ceb28` 本地 app bin SHA256 `1d572f57bb1f90cf063952672414413e194ee7aef00de1c66218238dc26b60a8`，未发布，未刷机 | 原理图、芯片容量、实机、客户稳定版都没有 |
| 2026-10-15 | 保存章程并填写本状态 | 分支 `docs/scout-lite-charter`。章程文件与上传原文一致 | Decision Pending 未改章程原文 |
| 2026-10-15 | Colin 批准文档 PR 合入条件，并批准通道 A | 批准范围只有这两件。workflow：`.github/workflows/scout-lite-upstream-tag.yml`。已处理 tag 行是 `scout-lite-upstream-stable-tag: v1.18.0`。未合并，未发布固件，未刷机 | schedule 要等该文件在 `master` 上才运行。原理图未确认 |
| 2026-10-15 | Colin 确认 SD/GPS 接线，USB 开关除外 | `HARDWARE.md`。原理图确认，实机未测，不是 `CONFIRMED_WORKING`。未改固件，未合并，未刷机 | 当天留下的非关键走线问题已在下一条关闭 |
| 2026-10-15 | Colin 收窄硬件范围 | 重要接线仍是 DevKit 的 SD 与 GPS 固件脚。下载模式只有板上 BOOT 和 EN。非关键走线不再追查。USB 自动切换仍不声称与 DevKit 相同。未改固件，未合并，未刷机 | 实机未测。PSRAM 容量 UNKNOWN。不是 `CONFIRMED_WORKING` |
| 2026-10-16 | 章程 PR 已在 `master` | [PR #2](https://github.com/pingequalab/ESP32Marauder/pull/2)，合并 `f4bc76727e7f29b7ca64da9c972c9103523c6006`。固件相对 `d3ceb28` 只多了文档和 CI yml，没有功能宏改动 | 该合并里的状态文件当时仍写章程未合并。本节更正这一点 |
| 2026-10-16 | 范围改为最全官方功能的最小移植，SigRoam 超出范围 | `FEATURE_MATRIX.md`、`MINIMAL_CHANGE_PLAN.md`、章程更正节。没有 `可最小打开` 行。未改固件 | 不发版，不刷机 |
| 2026-10-16 | 内部 beta：重编未修改的 `MARAUDER_C5` | 编译 commit `8b73c1905e810771da7f97fac63e89b2f5d9c1b3`。退出码 0。app bin SHA256 `e8c5b57f16745a2bcfbd5e8752fa68301345262cb4f6578556ae1b6316a681f0`。命令在 `INTERNAL_BETA.md`。bin 未提交，未上传 Release，未刷机 | 不是 Scout Lite 功能移植。不是客户固件 |
| 2026-10-16 | 函数级差距，只收核对过的点 | `CODE_GAP.md`。Wardriver 对照 tag `v2.3.2` `35b3c1b`，未拷进仓库。研究报告里的「可以直接刷」、`scan_dwell`、`UploadWigle.cpp`、无 Fix 写入 SD，都没有写成事实。章程未加入这些句子 | 随后只做 Phase 1 |
| 2026-10-16 | Phase 1：sidecar、有条件恢复 wardrive、单文件再试 2 次 | 编译 commit `cf1da663fea6fef21f9c06c8efc8419943c9137e`。退出码 0。app bin SHA256 `37cc863b6112328ff571c56cd1f62e67bf50d4537fa5d95b9beac0e07703be10`。命令在 `INTERNAL_BETA.md`。不改频道表。屏幕菜单仍传 `retry=true`。Phase 2/3 未做 | bin 未提交。未刷机。不合并 |
| 2026-10-16 | 无 fix 不占 MAC | 编译 commit `b376eb64e20ec51fe3587205f01cc504a898f489`。退出码 0。app bin SHA256 `9005e18a9d563b9cd62c415e0b8f5837d7bab8beb5300e76ce5dab0d746fa4f2`。三条写入路径都是 `append` 之后才 `save_mac()` | bin 未提交。未刷机。不合并。SSID 排除、删除 CLI、PSRAM 没做 |
| 2026-10-16 | 无屏日志列表和单 SSID 触发上传 | 编译 commit `1e6a5572a6295695f30b6026d4a5f45255c821dc`。退出码 0。app bin SHA256 `f17d12826d2f2e676e9fc356199bf7b5b56ebaa4fe1328b5401f813f705ab041`。设置键 `t_ssid` / `t_pass`。回复仍是原有串口句子 | 下一行已删掉触发 SSID。bin 未提交。未刷机。不合并 |
| 2026-10-16 | 删掉触发 SSID，只留 Flipper 已有 `upload` | 编译 commit `5b75787bcc96ff8e2bb1f1e49b11fc8707291b12`。退出码 0。app bin SHA256 `45e6c5976a8b034344cac5eb867e5853f1808d8899934bf29d6ca608325241c9`。命令在 `INTERNAL_BETA.md`。无屏仍收集 `wardrive_` / `wigle-`。回复仍是原有串口句子 | bin 未提交。未刷机。不合并 |
| 2026-10-16 | 双应用架构只写文档 | `ARCHITECTURE.md`、`FLIPPER_MARAUDER_COMPATIBILITY_MATRIX.md`、章程双应用节。未改固件 | 命名空间未实现。实机未测。不合并。不刷机 |

## 8. 最近一次同步摘要

- 上游版本：源码版本字符串 `v1.18.0`。tag `v1.18.0`（`b2b9f92382eb60b39b8741be3b567e416cdbe672`）比 `d3ceb28afeda1196ca27c13adeaa69ada52383f3` 落后 10 commits。
- 新增功能及受影响模块：`5b75787` 删掉触发 SSID。无屏 `upload -d` 仍收集 `wardrive_` / `wigle-`。每日 tag workflow 不编译固件。
- 自动测试：PR #1 在 `b242f00` 上 `Firmware Unit Tests` / `Native Unity tests`、`Scout Lite CI` / `Native Unity tests`、`Compile MARAUDER_C5` 为 SUCCESS。通道 A 不在 pull request 上跑。GitHub 的 schedule 只从默认分支 `master` 触发。
- 实机验证：无。未刷机。
- 是否允许稳定版发布：否。没有客户稳定固件。Colin 在 2026-10-15 批准的是当时的文档 PR 在 required CI 为绿后可合入 `master`。该章程 PR 已合并为 `f4bc767`。这次范围更正不是固件发布。Agent 不合并，不刷机。
