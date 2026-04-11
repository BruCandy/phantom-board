# Phantom Board

<p align="center">
  <img src="data/icons/icon_black.png" alt="Phantom Board icon" width="200">
</p>

## Quick Start

```bash
cd resources
bash daemon_setup.sh
bash daemon_build.sh
sudo chmod 666 /dev/input/eventX
sudo chmod 666 /dev/uinput
bash daemon_run.sh
```

## Test

```bash
sudo hexdump /dev/input/eventX
```
