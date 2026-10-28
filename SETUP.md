# Setup Guide

## 1. Install the driver
Download and run **ViGEmBus** from <https://github.com/nefarius/ViGEmBus/releases>. Restart if asked.
Check Device Manager → System devices for *Nefarius Virtual Gamepad Emulation Bus*.

## 2. Install Python packages
```bash
pip install -r requirements.txt
```

## 3. Wire the hardware
Follow [WIRING.md](WIRING.md).

## 4. Test the wiring (recommended)
Flash `firmware/i2c_scanner/i2c_scanner.ino` and open Serial Monitor at **115200**.
You should see `I2C device found at 0x68`.

## 5. Flash the main firmware
Arduino IDE settings:

| Setting | Value |
|---------|-------|
| Board | ESP32C3 Dev Module |
| USB CDC On Boot | **Enabled** |
| Upload speed | 921600 (or 115200 if unstable) |

If upload fails: hold **BOOT**, tap **RESET**, release BOOT, then upload again.

Open Serial Monitor (115200) and you should see lines like `1.2,-0.4`. **Close the Serial Monitor** afterwards, because only one program can use the port.

## 6. Find your COM port
Windows: Device Manager → Ports (COM & LPT).

## 7. Run
```bash
python software/tiltdrive.py --port COM6
```
Keep your hand flat for the first 2 seconds (calibration).

## 8. Test
Press **Win + R**, type `joy.cpl`, open *Xbox 360 Controller* → Properties. Tilt your hand and watch the triggers and stick move.

## 9. Play
Start a racing game and select the controller. Tune with `--max-angle`, `--deadzone`, `--smooth`.
