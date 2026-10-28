<div align="center">

# 🏎️ TiltDrive

### Drive any racing game with the tilt of your hand.

An **ESP32-C3 + MPU6050** wearable that turns hand motion into a **virtual Xbox 360 controller**.
Tilt forward to accelerate, tilt back to brake, tilt sideways to steer. Fully **analog**: the more you tilt, the more input you give.

![ESP32-C3](https://img.shields.io/badge/ESP32--C3-SuperMini-blue?logo=espressif)
![Python](https://img.shields.io/badge/Python-3.8%2B-yellow?logo=python&logoColor=white)
![Arduino](https://img.shields.io/badge/Arduino-IDE-00979D?logo=arduino&logoColor=white)
![Platform](https://img.shields.io/badge/Platform-Windows-0078D6?logo=windows&logoColor=white)
![License](https://img.shields.io/badge/License-MIT-green)

</div>

---

## ✨ Features

- 🎚️ **Proportional control**: tilt angle maps smoothly to throttle, brake and steering (not on/off keys)
- ✋ **Flat hand = stopped**: configurable dead zone keeps the car still at rest
- 🧭 **Auto-calibration**: hold your hand flat at power-up and that becomes zero, whatever way the sensor is mounted
- 🧠 **Complementary filter**: gyro + accelerometer fusion for smooth, drift-free angles
- ⚡ **50 Hz** update rate over USB serial
- 🎮 **Virtual Xbox 360 gamepad** via ViGEmBus, so it works in any game with controller support
- 🔧 **Tunable from the command line**: sensitivity, dead zone, smoothing, axis inversion and swap
- 🔌 **Cheap**: around $6 to $8 in parts

## 🎯 How it controls the car

| Hand movement | Game input |
|---|---|
| Flat / resting | Everything 0, car stopped |
| Tilt forward | Right Trigger (throttle) 0 → 100% |
| Tilt back | Left Trigger (brake / reverse) 0 → 100% |
| Tilt left / right | Left stick X (steering) |

## 🧩 How it works

```mermaid
flowchart LR
    A[MPU6050<br/>accel + gyro] -->|I2C| B[ESP32-C3<br/>complementary filter]
    B -->|USB serial<br/>pitch,roll @ 50Hz| C[Python script<br/>deadzone + smoothing]
    C -->|vgamepad| D[ViGEmBus<br/>virtual Xbox 360 pad]
    D --> E[🎮 Your game]
```

## 🔌 Wiring

![Wiring](docs/wiring.svg)

| MPU6050 | ESP32-C3 SuperMini |
|---|---|
| VCC | 3V3 |
| GND | GND |
| SDA | GPIO 8 |
| SCL | GPIO 9 |

> ⚠️ Power the MPU6050 from **3V3**, not 5V. More details in [docs/WIRING.md](docs/WIRING.md).

## 🧰 Hardware

See the full list in [docs/BOM.md](docs/BOM.md). In short: ESP32-C3 SuperMini, MPU6050 (GY-521) module, 4 jumper wires, USB-C cable, and a glove or strap.

## 🚀 Quick start

```bash
git clone https://github.com/YOUR-USERNAME/TiltDrive.git
cd TiltDrive
pip install -r requirements.txt
```

1. Install the **ViGEmBus** driver ([download](https://github.com/nefarius/ViGEmBus/releases)).
2. Wire everything as shown above.
3. Open `firmware/TiltDrive/TiltDrive.ino` in Arduino IDE. Select **ESP32C3 Dev Module**, enable **USB CDC On Boot**, and upload.
4. Find your COM port, then run:

```bash
python software/tiltdrive.py --port COM6
```

5. Keep your hand **flat for the first 2 seconds**, then open your game and pick the Xbox controller.

Full guide: [docs/SETUP.md](docs/SETUP.md)

## 🎛️ Options

```bash
python software/tiltdrive.py --port COM6 --max-angle 20 --deadzone 4 --smooth 0.5
```

| Flag | Default | What it does |
|---|---|---|
| `--port` | `COM6` | ESP32 serial port |
| `--deadzone` | `5.0` | Degrees ignored around flat |
| `--max-angle` | `30.0` | Tilt needed for 100% input (lower = more sensitive) |
| `--smooth` | `0.35` | 0 to 1, higher = snappier, lower = smoother |
| `--invert-pitch` | off | Swap throttle and brake |
| `--invert-roll` | off | Swap left and right |
| `--swap-axes` | off | Swap pitch and roll if the module is rotated 90° |

Tip: send the character `c` over serial to recalibrate without unplugging.

## 🗂️ Project structure

```
TiltDrive/
├── firmware/
│   ├── TiltDrive/TiltDrive.ino     # main ESP32-C3 sketch
│   └── i2c_scanner/i2c_scanner.ino # wiring test
├── software/
│   └── tiltdrive.py                # serial → virtual gamepad
├── docs/
│   ├── wiring.svg
│   ├── WIRING.md
│   ├── SETUP.md
│   ├── BOM.md
│   └── TROUBLESHOOTING.md
├── requirements.txt
└── LICENSE
```

## 🛠️ Troubleshooting

Nothing printing in Serial, crackling audio, wrong direction? See [docs/TROUBLESHOOTING.md](docs/TROUBLESHOOTING.md).

## 🗺️ Roadmap

- [ ] Wireless version (Bluetooth / Wi-Fi) with a LiPo battery
- [ ] Buttons for gear shift, handbrake and nitro
- [ ] Keyboard mode (W/A/S/D pulses) for games without gamepad support
- [ ] Simple GUI for live tuning
- [ ] 3D-printed wrist mount

## 🤝 Contributing

Pull requests and ideas are welcome. Open an issue first for big changes.

## 📄 License

MIT, see [LICENSE](LICENSE).

<div align="center">⭐ If you like this project, give it a star!</div>
