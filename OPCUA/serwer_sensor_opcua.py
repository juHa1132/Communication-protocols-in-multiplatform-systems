from opcua import Server
import time
import RPi.GPIO as GPIO

# Setup GPIO
GPIO.setmode(GPIO.BCM)
TRIG = 23
ECHO = 24
GPIO.setup(TRIG, GPIO.OUT)
GPIO.setup(ECHO, GPIO.IN)

def get_distance():
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

server = Server()
url = "opc.tcp://0.0.0.0:4840"
server.set_endpoint(url)

name = "DistanceOPCUAServer"
addspace = server.register_namespace(name)

node = server.get_objects_node()
param = node.add_object(addspace, "Parameters")

distance = param.add_variable(addspace, "Distance", 0.0)
distance.set_writable()

server.start()
print("Serwer OPC UA uruchomiony na {}".format(url))

try:
    while True:
        dist = get_distance()
        print(f"Odległość: {dist} cm")
        distance.set_value(dist)
        time.sleep(1)
except KeyboardInterrupt:
    server.stop()
    GPIO.cleanup()-9
