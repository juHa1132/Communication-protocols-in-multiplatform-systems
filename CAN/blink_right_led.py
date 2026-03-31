import RPi.GPIO as GPIO
import can
import time

LED_RIGHT = 27

GPIO.setmode(GPIO.BCM)
GPIO.setup(LED_RIGHT, GPIO.OUT)

def control_right_led():
    bus = can.interface.Bus(channel='vcan0', bustype='socketcan')
    try:
        while True:
            message = bus.recv()
            if message.arbitration_id == 0x124 and message.data[0] == 0x01:
                GPIO.output(LED_RIGHT, GPIO.HIGH)
                time.sleep(0.5)
                GPIO.output(LED_RIGHT, GPIO.LOW)
                time.sleep(0.5)
            else:
                GPIO.output(LED_RIGHT, GPIO.LOW)
    except KeyboardInterrupt:
        pass
    finally:
        GPIO.cleanup()

if __name__ == "__main__":
    control_right_led()
