from opcua import Server
from random import randint
import datetime
import time

server = Server()

url = "opc.tcp://192.168.100.45:4840"
server.set_endpoint(url)

name = "OPCUA_SIMULATION_SERVER"
addspace = server.register_namespace(name)

node = server.get_objects_node()

Param = node.add_object(addspace, "Parameters")

Temp = Param.add_variable(addspace, "Temperature", 0)
Press = Param.add_variable(addspace, "Preassure", 0)
Time = Param.add_variable(addspace, "Time", 0)

Temp.set_writable()
Press.set_writable()
Time.set_writable()

server.start()
print("Server started at {}".format(url))

while True:

    Temperature = randint(10, 50)
    Preassure = randint(200, 999)
    TIME = datetime.datetime.now()

    print(Temperature, Preassure, TIME)

    Temp.set_value(Temperature)
    Press.set_value(Preassure)
    Time.set_value(TIME)

    time.sleep(2)