# 官方 C5 目标与 `MARAUDER_C5` 的差异

日期：2026-10-16。源码 commit：`f4bc76727e7f29b7ca64da9c972c9103523c6006`。

只比较本树里这些官方 C5 目标：`MARAUDER_V8`、`MARAUDER_MINI_V3`、`MARAUDER_PANCAKE`、`XIAO_ESP32_C5`、`MARAUDER_T_DONGLE_C5`、`MARAUDER_POOM`。`DUAL_MINI_C5` 只是再定义 `MARAUDER_MINI_V3`（`esp32_marauder/configs.h:144-146`），不单列成一套功能。

一行一个与 `MARAUDER_C5` 不同的官方功能。下面这些宏在列出的六个目标上和 `MARAUDER_C5` 一样，都是打开的，所以没有单独的行：`HAS_BT`（`configs.h:533`）、`HAS_DUAL_BAND`（`542`）、`HAS_NIMBLE_2`（`545`）、`HAS_IDF_3`（`546`）。

动作只有这五个词：`已在 C5`、`可最小打开`、`硬件不具备`、`超出范围`、`未知`。

本表没有 `可最小打开` 行。Scout Lite 已确认的接线已经落在 `MARAUDER_C5` 的 SD/GPS 宏上。其余差异要么要这块板没有的硬件，要么还没有实测，要么超出 2026-10-16 的范围。

`MARAUDER_V8` 和 `MARAUDER_MINI_V3` 的宏在 `configs.h` 里。公开 workflow 没有这两个 flag。

| 功能 | 在哪里打开 | C5 DevKit | Scout Lite 硬件 | 动作 |
| --- | --- | --- | --- | --- |
| GPS `HAS_GPS` | XIAO `518-522`，T-Dongle `560`，V8 `582`，Pancake `607`，Mini v3 `631`。POOM `644-659` 没有这个宏 | `538` 已定义。脚是 `GPS_TX 14`、`GPS_RX 13`（`2975-2978`） | 原理图与 DevKit 相同：GPIO14 是固件 RX，GPIO13 是固件 TX。实机未测 | 已在 C5 |
| GPS 引脚 | T-Dongle `2967-2970` 为 12/11。XIAO `2971-2974` 为 12/11，注释写 GPIO13/14 留给 USB。V8 `2979-2982`、Pancake `2983-2986`、Mini v3 `2987-2990` 与 C5 同为 14/13。POOM 无 `HAS_GPS` | `2975-2978` | 与 C5 相同。Flipper pin 15/16 见下行 | 已在 C5 |
| Flipper pin 15/16 上的 GPS | 无官方宏 | 无 | 已记录。不是任务 | 超出范围 |
| microSD `HAS_C5_SD` / `HAS_SD` / `USE_SD` | T-Dongle `561-563`，V8 `583-585`，Pancake `608-610`，Mini v3 `632-634`，POOM `651-653`。XIAO `682-685` 把这三项 `#undef` | `539-541` 已定义 | SD 是 IO2/IO6/IO7/IO10。原理图确认，实机未测 | 已在 C5 |
| microSD 引脚 | C5：`SD_CS 10`（`2706-2707`），`SD_MISO 2` / `SD_MOSI 7` / `SD_SCK 6`（`3212-3215`）。T-Dongle：`SD_CS 23`（`2708-2709`），MISO 7 / MOSI 2 / SCK 6（`3216-3219`）。V8：`SD_CS 10`（`2712-2713`），总线用 TFT 脚（`3222-3225`）。Pancake：`SD_CS 7`（`2716-2717`），总线用 TFT 脚（`3228-3231`）。Mini v3 屏段在 `HAS_SCREEN` 内写 `TFT_*` 和 `SD_CS 4`（`2211-2223`），后面又 `#define SD_CS 10`（`2720-2721`），MISO/MOSI/SCK 跟 TFT（`3270-3273`）。POOM：`SD_CS 5`（`2724-2725`），MISO 8 / MOSI 4 / SCK 6（`3276-3279`） | 见左，C5 四根脚已写出 | 与 C5 的四根脚一致。不要换成上列其他脚 | 已在 C5 |
| 直接上传 `HAS_DIRECT_UPLOAD` | T-Dongle `568`，V8 `592`，Pancake `616`，Mini v3 `641`，POOM `658`。XIAO 的 `518-527` 没有这个宏。调用在 `WiFiScan.cpp:11750` | `547` 已定义 | 不另要屏或按键。实网上传未测 | 已在 C5 |
| NeoPixel `HAS_NEOPIXEL_LED` | C5 `535`，Pancake `603`，POOM `648`。V8 `578` 与 Mini v3 `626` 是注释。XIAO、T-Dongle 不定义它。C5 的数据脚 `PIN 27`（`2860-2861`） | 宏开着，`PIN 27` | 原理图确认项里没有这颗灯。不把 DevKit 的 GPIO27 灯写成 Scout Lite 已有。不改这个宏 | 已在 C5 |
| PSRAM `HAS_PSRAM` | XIAO `523`，T-Dongle `565`，V8 `587`，Pancake `612`，Mini v3 `636`，POOM `655`。打开后走 `BUF_SIZE` / `SNAP_LEN`（`3167-3170`）、`MAX_HTML_SIZE`（`2881-2882`）、`mac_history_len`（`2891-2892`），并调用 `psramInit()`（`esp32_marauder.ino:297-300`） | `543` 是注释。因此 `BUF_SIZE` 是 `3 * 1024`（`3173-3175`），`MAX_HTML_SIZE` 是 11400（`2883-2884`），`mac_history_len` 是 100（`2893-2894`）。FQBN 的 `PSRAM=enabled` 只加 `BOARD_HAS_PSRAM`，不是这个宏 | 丝印 N8R8 不是测到的启用。容量未知 | 未知 |
| 屏幕、全屏菜单、迷你屏、ILI9341 图 | V8：`HAS_SCREEN` `580`、`HAS_FULL_SCREEN` `581`、`HAS_ILI9341` `1426`。Pancake：`595-606` 内的屏宏，`HAS_ILI9341` `1500`。Mini v3：`HAS_SCREEN` `629`、`HAS_MINI_SCREEN` `630`，屏段 `2211-2223`。POOM：`HAS_SCREEN` `649`、`HAS_MINI_SCREEN` `650`，几何 `1023-1038`。Packet monitor 图在 `WiFiScan.cpp:5003-5007`，以及 `10626-10628`（`#ifdef HAS_SCREEN` 里面） | `537` `HAS_SCREEN` 是注释。无屏分支在 `2755` | 没有屏幕。Mini v3 的 `TFT_CS 27` 与 C5 NeoPixel 的 `PIN 27` 是同一个脚号 | 硬件不具备 |
| T-Dongle 屏 `HAS_T_DONGLE_DISPLAY` | 仅 T-Dongle `552`，脚 `556-559`。源文件 `TDongleDisplay.cpp` | 无 | 没有这块屏。SPI 脚与 Scout Lite 的 SD 脚冲突 | 硬件不具备 |
| 按键 `HAS_BUTTONS` | Pancake `602`，脚在 `871-876`（只有 `C_BTN 28`）。Mini v3 `625`，脚在 `971-984`。POOM `646`，脚在 `999-1017`。V8 `577` 是注释 | `534` 是注释 | 板上只有 BOOT 和 EN。Flipper 不能进入下载模式 | 硬件不具备 |
| 小键盘 `HAS_MINI_KB` | Mini v3 `628`，POOM `647` | 无 | 没有键盘 | 硬件不具备 |
| 触摸 `HAS_TOUCH` / `HAS_CAP_TOUCH` | V8 `572`。Pancake `596-597`，电容触摸复位 `CTP_RST 8`（`3072`） | 无 | 没有触摸屏 | 硬件不具备 |
| 电池 `HAS_BATTERY` | V8 `575`，I2C `SCL 4` / `SDA 5`（`3056-3058`）。Pancake `600`，I2C `SDA 9` / `SCL 10`（`3067-3070`），与触摸共用总线。Mini v3 `623` 是注释。XIAO、T-Dongle、POOM 不定义 | `532` 是注释 | 原理图确认项里没有电量计。不能借用 V8 或 Pancake 的 I2C 脚 | 未知 |
| 活动灯 `HAS_ACT_LED` | 仅 V8 `591`。`ACT_LED_PIN 28`（`3337-3338`） | 无 | 原理图确认项里没有 GPIO28 灯 | 未知 |
| T-Dongle LED `HAS_T_DONGLE_LED` | 仅 T-Dongle `553-555`。数据脚 2，时钟脚 6 | 无 | 脚 2 和脚 6 是 Scout Lite 的 SD MISO 和 SCK | 硬件不具备 |
| XIAO 单色灯 `XIAO_LED_PIN` | 仅 XIAO `526`，值为 27。`esp32_marauder.ino:380-381` 在这个目标上走 `xiaoLED`，不走 NeoPixel | C5 不定义它。C5 的 GPIO27 已是 NeoPixel `PIN` | Scout Lite 不是 XIAO 模块。不要用另一套灯驱动占 GPIO27 | 硬件不具备 |
| T-Dongle 16MB 分区 | workflow FQBN `FlashSize=16M,PartitionScheme=custom`（`.github/workflows/build_parallel.yml:48`）。表在 `installer/partitions/t_dongle_c5.csv:1` | DevKit FQBN 是 `FlashSize=8M,PartitionScheme=default_8MB`（`build_parallel.yml:46`，`scout-lite-ci.yml` 同字符串）。仓库里没有这份 8MB CSV | Flash 容量没有实测。丝印不是容量测量 | 未知 |
| WebUI | 本树没有启用的 `Web.cpp`。`CommandLine.h:14` 的 `#include "Web.h"` 是注释。`DUAL_MINI_C5` 不增加网页源文件 | 无 | 不靠不存在的源码造功能 | 超出范围 |
| SigRoam 优化 | 无本仓库实现。2026-10-15 文档曾把它当成方向 | 无 | 无 | 超出范围 |
| FAP 合约测试 | 本仓库没有 FAP。2026-10-15 状态曾把它列为任务，证据是「没有命令清单」 | 无 | 无 | 超出范围 |
| SigRoam 兼容 | 无 FAP 源码，无抓包。旧矩阵把这一行标成未知 | 官方 CLI 在 `CommandLine.h`，这是 Marauder 命令名，不是 SigRoam 合约 | 无 | 超出范围 |
| USB 自动切换 | 无比较宏 | 不声称与 DevKit 相同 | 不调查 | 超出范围 |
| Flipper 进入下载模式 | 无官方功能宏 | 无 | 只有板上 BOOT 和 EN。`PB2`/`PC3` 不是下载路径 | 硬件不具备 |

## Offensive

这些名称在列出的 C5 目标上没有排除宏，因此不是差异行。`MARAUDER_C5` 也没有排除宏。只记名称和宏。`HAS_PSRAM` 只改变 `MAX_HTML_SIZE`（`configs.h:2881-2884`），不单列成另一项。

- deauth：`ATTACK_TYPE_DEAUTH`
- beacon：`ATTACK_TYPE_BEACON`
- probe：`ATTACK_TYPE_PROBE`
- funny：`ATTACK_TYPE_FUNNY`
- rickroll：`ATTACK_TYPE_RR`
- badmsg：`ATTACK_TYPE_BM`
- sleep：`ATTACK_TYPE_S`
- sae：`ATTACK_TYPE_SAE`
- csa：`ATTACK_TYPE_CSA`
- quiet：`ATTACK_TYPE_QUIET`
- evilportal：`EVIL_PORTAL_CMD`
- karma：`KARMA_CMD`
- blespam：`BT_SPAM_CMD`，还要 `HAS_BT`
- spoofat：`BT_SPOOFAT_CMD`，还要 `HAS_BT`

宏在 `esp32_marauder/CommandLine.h:74-75`、`99-108`、`126`、`129`。

## 历史证据

不删除。2026-10-15 的矩阵用过另一套中文状态，基线当时写成 `d3ceb28afeda1196ca27c13adeaa69ada52383f3`。同一次记录：native 测试 134/134，以及该 commit 上未刷机的 app bin SHA256 `1d572f57bb1f90cf063952672414413e194ee7aef00de1c66218238dc26b60a8`。那些数字只证明当时的构建，不证明实机功能。

当时 SigRoam 行是「未知」，依据是本仓库没有 FAP。2026-10-16 起这三行是 `超出范围`。没有 FAP 这个事实还在。
