# Troubleshooting

| Problem | Fix |
|---------|-----|
| Serial Monitor shows nothing | Set **USB CDC On Boot = Enabled**, re-upload, press RESET. Check baud is 115200. |
| Still nothing | Flash `i2c_scanner`. If it says "No I2C device found", recheck wiring and solder joints. |
| Scanner shows `0x69` | Change `MPU_ADDR` to `0x69` in the firmware. |
| `could not open port` | Close Serial Monitor/Plotter. Check the COM number in Device Manager. |
| Board keeps rebooting when Python starts | Use the repo script (DTR/RTS are disabled). |
| Crackling or broken audio when ESP32 is plugged in | USB ground noise into the 3.5mm audio jack. Use another USB port (back of PC), a better cable, HDMI/Bluetooth/USB audio, or a powered USB hub. |
| Throttle and brake are swapped | Add `--invert-pitch`. |
| Left and right are swapped | Add `--invert-roll`. |
| Tilt forward steers instead | Module is rotated 90°. Add `--swap-axes`. |
| Car creeps when hand is resting | Raise `--deadzone` (for example 7) or recalibrate by sending `c`. |
| Too sensitive / not sensitive enough | Lower or raise `--max-angle`. |
| `vgamepad` import or driver error | Install ViGEmBus, then restart the PC. |
| Game ignores the controller | Make sure the game supports Xbox controllers and the script is running before the game starts. |
| Upload fails | Hold BOOT, tap RESET, release BOOT, upload again. Use a data-capable USB cable. |
