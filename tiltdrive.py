"""
TiltDrive - turn hand tilt into a virtual Xbox 360 controller.

Reads "pitch,roll" lines from the ESP32-C3 over USB serial and maps them
to analog triggers (throttle / brake) and the left stick (steering).

Usage:
    python tiltdrive.py --port COM6
    python tiltdrive.py --port COM6 --max-angle 20 --deadzone 4
"""
import argparse
import time

import serial
import vgamepad as vg


def parse_args():
    p = argparse.ArgumentParser(description="Tilt-controlled virtual gamepad")
    p.add_argument("--port", default="COM6", help="ESP32 serial port (COM6, /dev/ttyACM0...)")
    p.add_argument("--baud", type=int, default=115200)
    p.add_argument("--deadzone", type=float, default=5.0, help="degrees ignored around flat")
    p.add_argument("--max-angle", type=float, default=30.0, help="degrees for 100%% input")
    p.add_argument("--smooth", type=float, default=0.35, help="0..1 (higher = snappier)")
    p.add_argument("--invert-pitch", action="store_true", help="swap throttle <-> brake")
    p.add_argument("--invert-roll", action="store_true", help="swap left <-> right")
    p.add_argument("--swap-axes", action="store_true", help="swap pitch and roll (rotated module)")
    return p.parse_args()


def make_axis(deadzone, max_angle):
    def axis(angle):
        """Angle in degrees -> -1..1 with deadzone."""
        if abs(angle) < deadzone:
            return 0.0
        sign = 1 if angle > 0 else -1
        val = (abs(angle) - deadzone) / (max_angle - deadzone)
        return sign * min(val, 1.0)
    return axis


def main():
    a = parse_args()
    axis = make_axis(a.deadzone, a.max_angle)

    pad = vg.VX360Gamepad()

    # DTR/RTS off so opening the port does not reset the ESP32-C3
    ser = serial.Serial()
    ser.port, ser.baudrate, ser.timeout = a.port, a.baud, 1
    ser.dtr = False
    ser.rts = False
    ser.open()
    time.sleep(2)

    print("TiltDrive running. Keep your hand flat at startup. Ctrl+C to quit.")
    thr = brk = steer = 0.0
    try:
        while True:
            # always use the freshest sample, drop any backlog
            if ser.in_waiting > 200:
                ser.reset_input_buffer()
                continue

            line = ser.readline().decode(errors="ignore").strip()
            try:
                pitch, roll = map(float, line.split(","))
            except ValueError:
                continue

            if a.swap_axes:
                pitch, roll = roll, pitch
            if a.invert_pitch:
                pitch = -pitch
            if a.invert_roll:
                roll = -roll

            p, r = axis(pitch), axis(roll)

            thr += (max(p, 0.0) - thr) * a.smooth
            brk += (max(-p, 0.0) - brk) * a.smooth
            steer += (r - steer) * a.smooth

            pad.right_trigger_float(value_float=thr)   # RT = accelerate
            pad.left_trigger_float(value_float=brk)    # LT = brake / reverse
            pad.left_joystick_float(x_value_float=steer, y_value_float=0.0)
            pad.update()
    except KeyboardInterrupt:
        pass
    finally:
        pad.reset()
        pad.update()
        ser.close()
        print("\nStopped.")


if __name__ == "__main__":
    main()
