import json
import serial
import paho.mqtt.client as mqtt

USB_DEVICE = '/dev/ttyUSB_CT-UNO'

NETPIE_ID = 'b4682d9c-c282-4bad-8d02-5ad6997b5cb9'
NETPIE_TOKEN = 'tnrutv5Dqv2J9uCJUxKUUp5svd66iBUv'

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
    with serial.Serial('/dev/tty.usbserial-D3071WWY', 9600) as ser:
        while(True) :
            try:
                data = ser.readline().decode('utf-8')
                data = json.loads(data)
            except json.decoder.JSONDecodeError, e:
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
