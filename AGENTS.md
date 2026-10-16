# Scout Lite 代理规则

维护基于官方 ESP32 Marauder `MARAUDER_C5` / ESP32-C5 DevKit 的 Scout Lite 固件。长期与上游的差异保持最小。

## 先读

1. 先读 `docs/scout-lite/PROJECT_CHARTER.md`，包括 2026-10-16 范围更正。那里是项目意图和约束。
2. 动手前读 `docs/scout-lite/PROJECT_STATUS.md`、`docs/scout-lite/FEATURE_MATRIX.md`、`docs/scout-lite/MINIMAL_CHANGE_PLAN.md`。接着已有审计和 PR，不重复启动同一项审计。
3. Scout Lite 原理图和可复现的实机日志，优先于研究报告和假设。
4. 依赖某项上游功能之前，写明源码路径、相关符号、版本或 tag 或 commit、构建开关、验证状态。
5. `docs/scout-lite/deep-research-report-2.md` 是研究输入，不是已验证事实。研究报告里的推断保持「待验证」，不是事实。

## 范围

产品目标是：Scout Lite 硬件能跑的最全官方 Marauder 功能。只从本树已有的官方目标做最小打开。候选必须已经在这棵树里，由宏或很小的板级文件挡住，并且不需要屏幕、按键或这块板没有的硬件。不发明功能。不因为另一块板有某功能就整段复制。

Fork 停在官方 `MARAUDER_C5`。不要新建 Scout Lite 板型宏来搬别的板的功能。

SigRoam 优化、FAP 合约测试、SigRoam 兼容超出范围。历史文档保留，只标超出范围，不删证据。

已确认、不要重开的硬件：

- SD 是 IO2/IO6/IO7/IO10。GPS 固件脚是 GPIO14 RX（`GPS_TX`）和 GPIO13 TX（`GPS_RX`）。与 DevKit 一致。原理图确认，实机未测。
- 板上只有 BOOT 和 EN。Flipper 不能进入下载模式。
- GPS 接到 Flipper pin 15 和 16 超出范围。不为它改固件。
- USB 自动切换不声称与 DevKit 相同。不调查。
- `HAS_PSRAM` 保持注释。丝印 N8R8 不是测到的启用。
- 没有客户发布。不刷机。

## 必须遵守

1. 优先用上游已有实现。缺功能先核对 `FEATURE_MATRIX.md`。改动以官方 `MARAUDER_C5` 为底。需要屏、按键或 Flipper 下载控制的功能不移植。
2. 不重写 Marauder 核心。补丁小，能单独回退，并且能单独测。没有具体必要和回归计划，不重构上游的扫描、Wi-Fi、SD、GPS、CLI、分区或启动逻辑。
3. 保持官方 Marauder CLI。运行时只使用 `CommandLine` / `WiFiScan` 里已经有的命令和串口原文，不新增命令，也不另做一套回复；Scout Lite 必须和官方 Marauder 应用保持兼容。不为 SigRoam 改命令或做合约测试。改公开命令必须有官方 CLI 的兼容测试，并且先有矩阵依据。
4. 研究报告里的推断保持「待验证」，不是事实。
5. `HARDWARE.md` 里已经写明的原理图确认项不要重开。其余硬件假设一律未核实。引脚宏不等于接线已确认。硬件宏不是板级事实。
6. 合适的通用修复提交上游。维护本地补丁记录；上游吸收等价修复后删掉本地重复补丁。未核实的上游变更不自动合并。遵守上游贡献规范和第三方许可。
7. 不得在没有 Colin 明确批准的情况下，自动合并稳定分支、向客户交付固件，或给生产硬件刷机。CI 为红则不合。`docs/scout-lite/TEST_PLAN.md` 里要求的测试实际通过之前，不发布固件，也不刷生产设备。文档 PR 不是固件发布。内部 beta 只是未修改的 `MARAUDER_C5` 构建记录，不是 Scout Lite 功能移植，也不是发布。
8. 不写攻击步骤、参数、payload 或报文格式。功能数量不能靠扩大 offensive 行为来凑。矩阵里 offensive 项只写名称和宏。
9. 自动化由事件驱动。除通道 A 外，不要再排重复轮询的定时任务，也不安排无条件的全仓库复查。Colin 在 2026-10-15 只批准通道 A：`.github/workflows/scout-lite-upstream-tag.yml` 每天一次，对照仓库里已处理的稳定 tag。tag 没变就安静结束，不启动 AI。tag 变了就只开一个 GitHub issue 然后停止。同一 tag 不开第二个。这个 workflow 不打开固件 PR，不合并，不编译固件，不发布，不刷机。
10. 区分源码存在、构建已打开、自动化测试通过、Scout Lite 实机已核实。在有证据改掉之前，沿用官方构建流程和工具链。不要假设不同 C5 硬件目标的二进制可以互相刷写。
11. 只做合法、已授权的无线研究或测试。不提交密钥、WiGLE API key、生产凭据或原始私人 wardriving 数据。保留构建产物和发布溯源（SHA、工具链、分区、测试结果）。不要把 bin 提交进 git，也不要上传 GitHub Release。

## 每次改动

- 标出上游源文件和 commit。
- 说明根因或功能缺口，给出最小改动。对照 `MINIMAL_CHANGE_PLAN.md`。计划里写「不移植」的项不要做。
- 编 C5 目标，并跑已有测试。记下证据和剩余风险。
- 更新 `docs/scout-lite/FEATURE_MATRIX.md`。项目状态有实质变化时，更新 `docs/scout-lite/PROJECT_STATUS.md`。
- 开 PR。一个问题一个小 PR。稳定分支合并、客户固件交付、生产硬件刷机，都要 Colin 明确批准。CI 为红则不合。

每次任务至少写明：问题与证据；上游现状；最小方案；改动文件；跑过的测试；结果与限制；是否适合上游 PR；回退方式；下一步。完成项要链到 commit、PR、构建日志或实机测试。

## 并行与询问

- 不重开已经在做的审计。
- 起初最多两个并行任务，能串行就串行。
- 只在架构、安全、硬件、兼容、成本或发布上有实质取舍时，才请人做决定。稳定分支合并、客户固件和刷生产硬件不在此列，始终要 Colin 明确批准。

优先级：上游兼容 > 可靠 > 可维护 > 功能数量。
