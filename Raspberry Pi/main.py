import datetime
import json
import serial
import paho.mqtt.client as mqtt

USB_DEVICE = '/dev/ttyUSB_CT-UNO'

NETPIE_ID = '6cd6b2b5-a74b-4bdd-953e-18fc4b65b908'
NETPIE_TOKEN = '3HGgFKN5NNn96ndP5y1XMH77EdHUvtp7'

TOPICS_SHADOW = {
    "update": "@shadow/data/update"
}

TOPICS_MSG = {
    "feeds": "@msg/landslide/feeds/"
}

client = mqtt.Client(client_id=NETPIE_ID)


def main():
#    client.on_connect = on_connect
#    client.on_message = on_message
    client.username_pw_set(NETPIE_TOKEN)
    client.connect("mqtt.netpie.io", 1883, 60)
    client.loop_start()
    with serial.Serial(USB_DEVICE, 9600) as ser:
        while(True) :
            try:
                data = ser.readline().decode('utf-8')
                data = json.loads(data)
                data['timestamp'] = str(datetime.datetime.now())
            except json.decoder.JSONDecodeError as  e:
                print(e)
                continue
            payload = {"data": data}
            payload = json.dumps(payload)
            client.publish(TOPICS_MSG["feeds"], payload=payload)
            client.publish(TOPICS_SHADOW['update'], payload=payload, qos=0, retain=False)
            print(payload)
        #while
    #with
#def main

if __name__ == "__main__":
    main()
