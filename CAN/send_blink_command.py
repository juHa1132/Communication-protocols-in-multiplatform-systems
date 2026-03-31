import can
import time

def send_blink_command():
    bus = can.interface.Bus(channel='vcan0', bustype='socketcan')
    msg = can.Message(arbitration_id=0x123, data=[0x01], is_extended_id=False)
    try:
        while True:
            bus.send(msg)
            time.sleep(1)
    except KeyboardInterrupt:
        pass

if __name__ == "__main__":
    send_blink_command()
