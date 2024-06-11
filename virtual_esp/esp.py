import time
import paho.mqtt.client as mqtt

from actuators.buzzer import Buzzer
from sensors.motion import MotionSensor
from sensors.ultrasonic import UltrasonicSensor

MQTT_BROKER = "192.168.223.218"
PORT = 1883
KEEPALIVE = 60

TOPIC_ULTRASONIC = "home/front-door/sensor/ultrasonic"
TOPIC_MOTION = "home/front-door/sensor/motion"
TOPIC_BUZZER = "home/front-door/actuator/buzzer"

SENSE_INTERVAL = 5

def create_mqtt_client():
    client = mqtt.Client()
    client.connect(MQTT_BROKER, PORT, KEEPALIVE)
    return client

def on_message(client, userdata, msg):
    payload = msg.payload.decode()
    print(f"Message received on {msg.topic}: {payload}")
    if msg.topic == TOPIC_BUZZER and payload == "ON":
        buzzer = userdata['buzzer']
        buzzer.activate()
        time.sleep(3)
        buzzer.stop()


def main():
    client = create_mqtt_client()

    ultrasonic = UltrasonicSensor()
    motion = MotionSensor()
    buzzer = Buzzer()

    client.user_data_set({'buzzer': buzzer})
    client.on_message = on_message

    client.subscribe(TOPIC_BUZZER)

    client.loop_start()
    while True:
        motion_detected = motion.sense_motion()
        if motion_detected:
            state = "Motion detected"
            print(state)
            client.publish(TOPIC_MOTION, state)

            distance = ultrasonic.measure_distance()
            print(f"Distance: {distance:.2f} cm")
            client.publish(TOPIC_ULTRASONIC, f"{distance:.2f}")
        
        time.sleep(SENSE_INTERVAL)

if __name__ == "__main__":
    main()