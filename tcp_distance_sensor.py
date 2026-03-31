import socket
import time
import RPi.GPIO as GPIO

def error(msg):
    print(msg)
    exit(1)

def measure_distance():
    TRIG = 23
    ECHO = 24

    GPIO.setmode(GPIO.BCM)
    GPIO.setup(TRIG, GPIO.OUT)
    GPIO.setup(ECHO, GPIO.IN)

    GPIO.output(TRIG, False)
    time.sleep(2)

    GPIO.output(TRIG, True)
    time.sleep(0.00001)
    GPIO.output(TRIG, False)

    while GPIO.input(ECHO) == 0:
        pulse_start = time.time()

    while GPIO.input(ECHO) == 1:
        pulse_end = time.time()

    pulse_duration = pulse_end - pulse_start
    distance = pulse_duration * 17150
    distance = round(distance, 2)

    return distance

def main():
    import sys
    if len(sys.argv) < 2:
        print("ERROR, no port provided")
        exit(1)

    portno = int(sys.argv[1])
    sockfd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        sockfd.bind(('', portno))
    except socket.error as msg:
        error(f"ERROR on binding: {msg}")

    sockfd.listen(5)
    print(f"Serwer nasłuchuje na porcie {portno}")

    try:
        newsockfd, addr = sockfd.accept()
        print(f"Połączono z {addr}")

        while True:
            distance = measure_distance()
            message = f"Distance: {distance} cm\n"
            newsockfd.sendall(message.encode('utf-8'))
            time.sleep(1)  # Wysyłaj dane co sekundę
    except KeyboardInterrupt:
        print("Cleaning up!")
    finally:
        newsockfd.close()
        sockfd.close()
        GPIO.cleanup()

if __name__ == "__main__":
    main()
