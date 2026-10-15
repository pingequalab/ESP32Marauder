# 构建核对

工作目录是 `/tmp/scout-lite-worktrees/baseline-audit`，commit `d3ceb28afeda1196ca27c13adeaa69ada52383f3`。没有刷机。没有 `--upload`。产物在 `/tmp`，没有放进 git。

## 入口

`platformio.ini` 只有 `[env:native]`。C5 固件入口是 `.github/workflows/build_parallel.yml` 的 Arduino CLI：FQBN `esp32:esp32:esp32c5:FlashSize=8M,PartitionScheme=default_8MB,PSRAM=enabled`，extra flag `-DMARAUDER_C5`，core `esp32:esp32@3.3.4`。本轮按这个入口编，没有新写 PlatformIO C5 env。

## v1.18.0 tag

本地在 fetch 之前没有 tag。

```text
git -C /tmp/scout-lite-worktrees/baseline-audit tag | wc -l
```

输出 `0`。

```text
git -C /tmp/scout-lite-worktrees/baseline-audit fetch origin --tags --prune
```

退出码 0。之后：

```text
git -C /tmp/scout-lite-worktrees/baseline-audit tag -l 'v1.18*'
```

无输出。`git tag | wc -l` 仍是 0。

```text
git ls-remote --tags origin 'refs/tags/v1.18*'
```

无输出。fork `pingequalab/ESP32Marauder` 上没有这个 tag。

```text
git ls-remote --tags https://github.com/justcallmekoko/ESP32Marauder.git 'refs/tags/v1.18*'
```

退出码 0。输出：

```text
b2b9f92382eb60b39b8741be3b567e416cdbe672	refs/tags/v1.18.0
```

```text
gh release view v1.18.0 --repo justcallmekoko/ESP32Marauder --json tagName,publishedAt,targetCommitish,isDraft,name
```

退出码 0。`publishedAt` 为 `2026-10-06T17:38:28Z`，`targetCommitish` 为 `b2b9f92382eb60b39b8741be3b567e416cdbe672`，`isDraft` 为 false。

```text
gh release view v1.18.0 --repo pingequalab/ESP32Marauder --json tagName
```

输出 `release not found`。

```text
git fetch https://github.com/justcallmekoko/ESP32Marauder.git tag v1.18.0
```

退出码 0。这次把上游 tag 取进了本地 git 目录（与 `/workspace` 共用对象库）。没有 merge，没有 push。

```text
git rev-parse v1.18.0^{}
git log -1 --format='%H %ci %s' v1.18.0
git rev-list --left-right --count v1.18.0...HEAD
git merge-base --is-ancestor v1.18.0 HEAD
```

结果：tag 是 lightweight tag，指向 commit `b2b9f92382eb60b39b8741be3b567e416cdbe672`，时间 `2026-10-05 11:07:34 -0400`，说明 `Fix Remote ID aircraft trail rendering (#1603)`。`rev-list` 输出 `0	10`。ancestor 检查退出码 0。HEAD 比该 tag 多 10 个 commit，tag 不是当前基线。

`git ls-remote https://github.com/justcallmekoko/ESP32Marauder.git refs/heads/master` 的 SHA 与 HEAD 相同：`d3ceb28afeda1196ca27c13adeaa69ada52383f3`。

两边的 `MARAUDER_VERSION` 字符串都是 `v1.18.0`。字符串不是 tag。

## native 测试

第一次 `python3 -m venv /tmp/pio-venv` 失败，退出码 1，原因是没有 `python3.12-venv`。

```text
sudo apt-get update -qq && sudo apt-get install -y python3.12-venv python3-pip curl ca-certificates
```

退出码 0。

```text
python3 -m venv /tmp/pio-venv
/tmp/pio-venv/bin/python -m pip install --disable-pip-version-check platformio==6.1.19
```

pip 退出码 0。`/tmp/pio-venv/bin/platformio --version` 输出 `PlatformIO Core, version 6.1.19`。

在 worktree 根目录执行：

```text
PLATFORMIO_SETTING_ENABLE_TELEMETRY=No /tmp/pio-venv/bin/platformio test -e native
```

退出码 0。摘要：

```text
native         test_boot_splash           PASSED    00:00:06.335
native         test_geofence_math         PASSED    00:00:00.563
native         test_target_list_sort      PASSED    00:00:00.609
native         test_pcap_header           PASSED    00:00:00.489
native         test_memory_guard          PASSED    00:00:00.511
native         test_mac_address           PASSED    00:00:00.593
native         test_wdg_response          PASSED    00:00:00.506
native         test_recon_ui              PASSED    00:00:00.507
native         test_owned_list_lifecycle  PASSED    00:00:00.498
native         test_upload_stream_buffer  PASSED    00:00:00.536
native         test_display_line          PASSED    00:00:00.562
native         test_gps_tracker_stats     PASSED    00:00:00.627
native         test_remote_id_model       PASSED    00:00:00.580
native         test_recon_mission_state   PASSED    00:00:00.552
native         test_fox_hunt_target       PASSED    00:00:00.587
native         test_menu_input_repeat     PASSED    00:00:00.609
native         test_t_dongle_stats        PASSED    00:00:00.662
native         test_t_dongle_bus          PASSED    00:00:00.573
native         test_firmware_metadata     PASSED    00:00:00.788
native         test_remote_id_decoder     PASSED    00:00:00.822
native         test_switches              PASSED    00:00:00.540
native         test_beacon_frame          PASSED    00:00:00.607
native         test_ipv4_range            PASSED    00:00:00.588
native         test_recon_probe_queue     PASSED    00:00:00.556
native         test_ibeacon               PASSED    00:00:00.586
native         test_menu_marquee          PASSED    00:00:00.643
================ 134 test cases: 134 succeeded in 00:00:21.030 ================
```

完整日志在 `/tmp/native-test.log`，不在仓库里。这套测试不编 C5，也不碰射频。

## C5 固件构建

`arduino-cli` 1.5.1（Commit `01f3d4f2b`，Date `2026-06-05T10:22:17Z`）装到 `/tmp/bin`。安装脚本退出码 0。

库按 workflow 的 ref 克隆到 `/tmp/marauder-libs`：ESP32Ping `1.6`，AsyncTCP `v3.4.8`，MicroNMEA `v2.0.6`，ESPAsyncWebServer `v3.8.1`，TFT_eSPI `V2.5.34`，XPT2046_Touchscreen `v1.4`，NimBLE-Arduino `2.3.8`，Adafruit_NeoPixel `1.12.0`，ArduinoJson `v6.18.2`，LinkedList `v1.3.3`，EspSoftwareSerial `8.1.0`，Adafruit_BusIO `1.15.0`，Adafruit_MAX1704X `1.0.2`，再加上仓库自带的 `libraries/Adafruit_TCA8418`。

```text
arduino-cli core install esp32:esp32@3.3.4
```

日志里 `CORE_INSTALL_EXIT:0`。`arduino-cli core list` 显示已装 `esp32:esp32 3.3.4`，索引里的 Latest 是 `3.3.12`。用的是 workflow 指定的 3.3.4，不是 Latest。

编译前按 workflow 对 core 3.3.4 的 `platform.txt` 做了 `-Wl,-zmuldefs` 替换，并把 `cpp_flags` 里的 `-fexceptions` 换成 `-fno-exceptions`。其中包括 `esp32c5/flags/cpp_flags`。

编译命令：

```text
arduino-cli compile \
  --fqbn "esp32:esp32:esp32c5:FlashSize=8M,PartitionScheme=default_8MB,PSRAM=enabled" \
  --libraries /tmp/marauder-libs \
  --build-property "compiler.cpp.extra_flags=-DMARAUDER_C5" \
  --warnings none \
  --build-path /tmp/c5-build-out \
  /tmp/scout-lite-worktrees/baseline-audit/esp32_marauder
```

`COMPILE_EXIT:0`。关键输出：

```text
Sketch uses 1873710 bytes (56%) of program storage space. Maximum is 3342336 bytes.
Global variables use 84176 bytes (25%) of dynamic memory, leaving 243504 bytes for local variables. Maximum is 327680 bytes.
```

`Maximum is 3342336` 与这次装上的 core 里 `esp32c5.menu.PartitionScheme.default_8MB.upload.maximum_size=3342336` 一致。`Global variables` 那行的上限是 327680 字节。这是链接器打出来的内部 RAM 数字，不是 PSRAM 容量测量。

同一份 core 的 `boards.txt` 把 `PSRAM.enabled` 定义成 `-DBOARD_HAS_PSRAM`。固件自己的 `HAS_PSRAM` 仍由 `configs.h` 决定，`MARAUDER_C5` 那里是注释。

`default_8MB.csv`（安装树 `hardware/esp32/3.3.4/tools/partitions/default_8MB.csv`，不是本仓库文件）：

```text
nvs,      data, nvs,     0x9000,  0x5000,
otadata,  data, ota,     0xe000,  0x2000,
app0,     app,  ota_0,   0x10000, 0x330000,
app1,     app,  ota_1,   0x340000,0x330000,
spiffs,   data, spiffs,  0x670000,0x180000,
coredump, data, coredump,0x7F0000,0x10000,
```

这只说明本次 FQBN 用了这份表。它是不是 Scout Lite 上的分区〔待验证〕。

SHA256（`sha256sum`，路径 `/tmp/c5-build-out`）：

| 文件 | 字节 | SHA256 |
| --- | --- | --- |
| `esp32_marauder.ino.bin` | 1873824 | `1d572f57bb1f90cf063952672414413e194ee7aef00de1c66218238dc26b60a8` |
| `esp32_marauder.ino.merged.bin` | 8388608 | `cd6736a53d5669f764d7655d680c4267a0a534fc7d41c6c8056e6e4323ec4954` |
| `esp32_marauder.ino.bootloader.bin` | 20784 | `f0cc3970dbf6001eb398e589244047f0731acbc0ded1ccfa952cd7559c083b13` |
| `esp32_marauder.ino.partitions.bin` | 3072 | `1d9cca96de0fe07ad7fc0648b9878ddecd9ce565e38b589ad20fea698ed4c80c` |
| `esp32_marauder.ino.elf` | 25312040 | `c0185b11b0363f803f350a8b57e696b1a4c1480a000773b96a6e884e5a38f465` |

`merged.bin` 的 8388608 字节是 8MiB 镜像长度，不是读出来的芯片容量。

未跑：烧录、串口、GPS、SD、Wi-Fi、BLE、Flipper、SigRoam。v8 / Mini v3 / Dual Mini 的私有 workflow 没有在本机跑。
