import RPi.GPIO as GPIO
import can
import time

LED_LEFT = 21

GPIO.setmode(GPIO.BCM)
GPIO.setup(LED_LEFT, GPIO.OUT)

def blink_left_led():
    bus = can.interface.Bus(channel='vcan0', bustype='socketcan')
    try:
        while True:
            message = bus.recv()
            if message.arbitration_id == 0x123 and message.data[0] == 0x01:
                GPIO.output(LED_LEFT, GPIO.HIGH)
                time.sleep(0.5)
                GPIO.output(LED_LEFT, GPIO.LOW)
                time.sleep(0.5)
    except KeyboardInterrupt:
        pass
    finally:
        GPIO.cleanup()

if __name__ == "__main__":
    blink_left_led()
