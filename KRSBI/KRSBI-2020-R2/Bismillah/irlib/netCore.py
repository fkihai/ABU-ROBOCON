import paho.mqtt.client as client
import json as json

mqtt = None
robotID = ""
data = {
    'bt': None,
    'r1': None,
    'r2': None,
    'r3': None
}

def __init(broker, ID):
    global robotID
    global mqtt

    robotID = ID
    mqtt = client.Client()
    mqtt.on_message = on_message
    mqtt.connect(broker)
    mqtt.loop_start()

def on_message(client, userdata, message):
    global data

    #print message.payload
    data[message.topic] = json.loads(message.payload)
    #data[message.topic] = message.payload

def subscribe(topic):
    mqtt.subscribe(topic, 1)

def publish(data):
    mqtt.publish(robotID, json.dumps(data), 1)

def clearData(topic=None, key=None):
    global data

    if topic == None:
        data = {
            'bt': None,
            'r1': None,
            'r2': None,
            'r3': None
        }
    else:
        if key == None:
            data[topic] = None
        else:
            data[topic][key] = None

def readData(topic=None):
    if topic == None:
        return data

    return data[topic]
