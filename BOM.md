# Bill of Materials

| # | Part | Qty | Notes | Approx. cost |
|---|------|-----|-------|--------------|
| 1 | ESP32-C3 SuperMini | 1 | Native USB, tiny, perfect for a wearable | $3 to $4 |
| 2 | MPU6050 (GY-521) module | 1 | 6-axis accelerometer + gyroscope, I2C | $1 to $2 |
| 3 | Jumper wires (female-female) or thin wire | 4 | Solder directly for a cleaner build | < $1 |
| 4 | USB-C data cable | 1 | Must be a **data** cable, not charge-only | $2 |
| 5 | Glove, wrist strap or velcro band | 1 | To mount the sensor on your hand | $1 to $3 |
| 6 | Double-sided tape or hot glue | 1 | Keep the module flat and fixed | — |

**Optional**

| Part | Why |
|------|-----|
| Header pins + soldering iron | Solid connections (loose pins are the #1 cause of "no sensor found") |
| 3.7V LiPo + charger module | Needed for a future wireless version |
| 3D-printed case | Protects the board on your hand |

## Software

| Tool | Purpose |
|------|---------|
| Arduino IDE 2.x + ESP32 board package | Flash the firmware |
| Python 3.8+ | Run the bridge script |
| `pyserial` | Read the ESP32 serial stream |
| `vgamepad` | Create the virtual Xbox 360 controller |
| ViGEmBus driver | Windows driver behind the virtual gamepad |
