# Phantom Board

<p align="center">
  <img src="docs/icons/icon_black.png" alt="Phantom Board icon" width="200">
</p>

## Quick Start

```bash
cd resources
bash daemon_setup.sh
bash daemon_build.sh
bash ui_build.sh
sudo chmod 666 /dev/input/eventX
sudo chmod 666 /dev/uinput
bash daemon_run.sh
bash ui_run.sh
```

## Test

```bash
sudo hexdump /dev/input/eventX
```

## Commit Message 

I used the following prefixes for commit messages:

- `chore: ` maintenance
- `docs: ` documentation only changes
- `feat: ` new feature
- `fix: ` bug fix
- `refactor: ` code changes that don't change behavior
- `style: ` code format only changes

Example: 

`feat: add IBus IME support`
