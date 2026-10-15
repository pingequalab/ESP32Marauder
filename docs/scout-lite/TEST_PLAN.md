# 测试标准

编译、协议、硬件是三道验收线。下面只有编译线有已经采集的证据。协议和硬件是标准，不是通过记录。证据对应的固件树是 `d3ceb28afeda1196ca27c13adeaa69ada52383f3`。之后的文档 commit 没有改固件。

## 编译

标准：`platformio test -e native` 通过，并且 `MARAUDER_C5` 能按官方 FQBN 编完。不刷机。

已采集，退出码 0：

```text
PLATFORMIO_SETTING_ENABLE_TELEMETRY=No /tmp/pio-venv/bin/platformio test -e native
```

PlatformIO 6.1.19。结果：`134 test cases: 134 succeeded`。这不覆盖射频或 C5。

已采集，`COMPILE_EXIT:0`：

```text
arduino-cli compile \
  --fqbn "esp32:esp32:esp32c5:FlashSize=8M,PartitionScheme=default_8MB,PSRAM=enabled" \
  --libraries /tmp/marauder-libs \
  --build-property "compiler.cpp.extra_flags=-DMARAUDER_C5" \
  --warnings none \
  --build-path /tmp/c5-build-out \
  /tmp/scout-lite-worktrees/baseline-audit/esp32_marauder
```

`arduino-cli` 1.5.1，core `esp32:esp32@3.3.4`。日志：`Sketch uses 1873710 bytes (56%) of program storage space. Maximum is 3342336 bytes.`

`/tmp/c5-build-out/esp32_marauder.ino.bin` SHA256：`1d572f57bb1f90cf063952672414413e194ee7aef00de1c66218238dc26b60a8`。

这次是本机直接调用 `arduino-cli`。库 ref 与 `build_parallel.yml` 相同，并打了该 workflow 里 3.3.4 的 `platform.txt` / `cpp_flags` 补丁。它不是 GitHub 上的 `ArminJo/arduino-test-compile` 运行记录。

`.github/workflows/scout-lite-ci.yml` 把 native 测试和一次 C5 编译收成 workflow。C5 步使用与 `build_parallel.yml:268-275` 相同的 action、FQBN、`-DMARAUDER_C5` 和 core 3.3.4，库 checkout 与该文件的 `Custom*` 步骤相同。没有 upload、release、刷机或 secrets。T-Dongle 分区、APA102、`esptool` 和 artifact 上传没有抄进来，因为它们不属于这次 `MARAUDER_C5` 编译（`tft: false`；`esptool` 在上游 workflow 里安装后没有被编译步调用）。

这个 workflow 没有在 GitHub 上跑过。没有绿色勾。

## 协议

标准，不是通过：

- CLI 命令名与上游 Marauder 一致，包括 `help`、`info`、`wardrive`、`stopscan`、`gpsdata`、`upload`。
- 响应格式不被 Scout Lite 私自改掉。
- SigRoam FAP 仍能驱动这些命令。

源码里命令名存在，见 `FEATURE_MATRIX.md`。没有 SigRoam 源码，没有 Flipper 对测。SigRoam / Flipper 待验证。本项没有 PASS。

## 硬件

标准，不是通过。每一条都要 Scout Lite 原理图或实机日志，当前都没有：

- 上电后 `info` 能打出版本和 `ESP32-C5 DevKit` 硬件名。
- GNSS 有 fix，坐标写入 wardrive 日志；无 fix 时不把空定位写成已定位。
- microSD 可挂载；无卡时失败路径不挂死。
- 2.4 与 5 GHz 都能进日志。
- WiGLE 或 WDGWars 上传在授权网络上留下可核对的结果。
- 断电后 SD 与配置可恢复。
- Flipper UART 上的命令与本机串口一致。
- 分区与回滚在这块板上成立。

引脚、Flash、PSRAM、能否刷官方 bin 仍待验证，见 `HARDWARE.md`。本项没有 PASS。
