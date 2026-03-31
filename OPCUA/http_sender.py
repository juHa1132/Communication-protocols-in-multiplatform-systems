import requests
import time
from opcua import Client

url = "opc.tcp://localhost:4840"
client = Client(url)
client.connect()

while True:
    dist = client.get_node("ns=2;i=2").get_value()
    response = requests.post("http://localhost:5000/update_distance", json={"distance": dist})
    print("wysłano")
    time.sleep(1)

client.disconnect()
