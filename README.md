# VESCXPRESS2X3Bridge

`vesc_express` firmware, built for the VESC2X3Bridge PCB
(ESP32-S3-WROOM-1-N8R2) specifically — a fork of
[vedderb/vesc_express](https://github.com/vedderb/vesc_express), narrowed
to this one board rather than the full multi-board upstream project.

## What the board does

The VESC2X3Bridge PCB bridges a Ninebot X3-series dashboard (G3/ZT3/GT3)
to a VESC motor controller, sitting on the same shared CAN bus as both.
This firmware is the hardware base for that bridge — stock `vesc_express`
plus board-specific config (CAN pins, flash/partition layout, WiFi/BLE),
no custom C application logic. The actual bridge behavior (dashboard
frame emulation, lights, profile switching, battery reporting) lives
entirely in a LispBM script (`main.lisp`) and a QML settings page,
uploaded separately after flashing this firmware — see the
[VESCXPRESS2G3Bridge](../VESCXPRESS2G3Bridge) repo for those scripts and
the full flashing/upload procedure.

## Hardware config (`main/hwconf/hw_vescxpress2g3bridge.h`)

- **CAN**: TX=GPIO48, RX=GPIO45 (confirmed for this PCB).
- **Flash/partitions**: 8MB (`sdkconfig.defaults.esp32s3_n8r2`,
  `partition_ota_8mb.csv`) — matches the WROOM-1-N8R2 module's flash size,
  and includes the `lisp`/`qml` partitions the script/settings page need.
- **WiFi**: disabled by default (`CONF_WIFI_MODE 0`) — this bridge only
  needs CAN + VESC Tool over BLE/USB.
- **BLE**: advertises as `"X3Bridge"` instead of the default `"Express"`.

This fork is scoped to this one board: the other boards' `sdkconfig.defaults.*`
and unused `partition_*.csv` files that shipped in upstream `vesc_express`
have been removed. Their `main/hwconf/hw_*.h`/`.c` files are still present
(only the sdkconfig/partition side was trimmed so far), which means
`build_all.py` (upstream's build-every-target script, which discovers
boards by globbing `hw_*.h` files) will still *find* those other boards
and then fail partway through each one when it can't locate their
`sdkconfig.defaults.<target>` — don't run it here. Just build the one
target below.

## Toolchain

ESP-IDF **v5.5.4** (other versions are likely to cause compatibility
issues). Setup instructions:
[https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/get-started/linux-macos-setup.html](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/get-started/linux-macos-setup.html)

```bash
git clone -b v5.5.4 --recursive https://github.com/espressif/esp-idf.git esp-idf-v5.5.4
cd esp-idf-v5.5.4/
./install.sh esp32s3
```

Activate it in each new shell before building:

```bash
source /path/to/esp-idf-v5.5.4/export.sh   # bash/zsh
# or
source /path/to/esp-idf-v5.5.4/export.fish # fish
```

## Building

No `idf.py set-target` step needed — passing `HW_NAME` is enough, since
`CMakeLists.txt` looks up `hw_vescxpress2g3bridge.h`, reads its
`HW_TARGET` (`esp32s3_n8r2`), and derives `IDF_TARGET=esp32s3`
automatically before the ESP-IDF project machinery even runs:

```bash
idf.py build -DHW_NAME="VESCXPRESS2G3Bridge"
```

This produces:
- `build/vesc_express.bin` — the application
- `build/bootloader/bootloader.bin`
- `build/partition_table/partition-table.bin`

All three are needed for a first-time flash on a blank chip (via VESC
Tool's manual/custom-files flash option); updating an already-flashed
board only needs `vesc_express.bin` through VESC Tool's normal firmware
update flow. See VESCXPRESS2G3Bridge's `docs/flashing-and-upload-guide.md`
for the full procedure, including uploading `main.lisp`/
`main-settings.qml` afterward.

**Note:** if you change environment variables or hardware config after an
existing build, run `idf.py reconfigure` (or `idf.py fullclean` for a
harder reset) before rebuilding — CMake caches the target and won't pick
up the change automatically.

## License

GPLv3, inherited from upstream `vesc_express` (see `LICENSE`). This is a
published modified version per GPLv3 §5 — the hardware-specific changes
are isolated to `main/hwconf/hw_vescxpress2g3bridge.{h,c}` and
`sdkconfig.defaults.esp32s3_n8r2`; no other upstream logic is modified.
