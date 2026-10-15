# 上游跟踪

事实来自对 `d3ceb28` 的只读核对，详见当时写入、现已并入 `HARDWARE.md`、`FEATURE_MATRIX.md`、`TEST_PLAN.md` 的审计。没有发明 PR 编号。

## 同步点

| 项 | 值 |
| --- | --- |
| 固件同步点 | `d3ceb28afeda1196ca27c13adeaa69ada52383f3` |
| 上游 `master` | 同一 SHA。`git ls-remote https://github.com/justcallmekoko/ESP32Marauder.git refs/heads/master` |
| 说明 | `Merge pull request #1609 from justcallmekoko/develop` |
| 上游 `develop` | `8cdadf98303bb8d55ca9249c48c2f0ed6ee900fe`（核对时，不是本同步点） |

本地固件补丁：无。已向上游打开的 PR：无。

## `v1.18.0`

上游 tag `v1.18.0` 是 lightweight tag，指向 `b2b9f92382eb60b39b8741be3b567e416cdbe672`（2026-10-05，`Fix Remote ID aircraft trail rendering (#1603)`）。

`git rev-list --left-right --count v1.18.0...d3ceb28` 的结果是 `0	10`。同步点比 tag 超前 10 个 commit，tag 是祖先。`v1.18.0` 不是当前基线。

`gh release view v1.18.0 --repo justcallmekoko/ESP32Marauder` 看到非 draft release，`publishedAt` 为 `2026-10-06T17:38:28Z`。

`git fetch origin --tags` 之后，fork `pingequalab/ESP32Marauder` 没有 `v1.18*` tag。`gh release view v1.18.0 --repo pingequalab/ESP32Marauder` 返回 `release not found`。

`MARAUDER_VERSION` 在同步点和该 tag 上都是字符串 `v1.18.0`（同步点 `esp32_marauder/configs.h:60`，tag 上为 `configs.h:52`）。字符串不是 tag 指针。

## 待移植

空。尚未开始。

没有 Scout Lite 板型宏，没有待提交的固件补丁，也没有已编号的移植任务。功能缺口在 `FEATURE_MATRIX.md`，硬件未核实项在 `HARDWARE.md`。
