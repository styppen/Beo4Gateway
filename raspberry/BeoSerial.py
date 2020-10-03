import serial
import paho.mqtt.client as mqtt
import logging

def connect_mqtt(client_name, broker, subscribe_topic):
    logger.info("Creating new MQTT instance")
    client = mqtt.Client(client_name)

    logger.info("Connecting to raspberry broker")
    client.connect(broker)

    client.on_message = on_message
    client.loop_start()

    logger.info("Subscribing to MQTT topic " + subscribe_topic)
    client.subscribe(subscribe_topic)

    return client

def init_logger():
    logger = logging.getLogger('main')
    logger.setLevel(logging.INFO)
    log_handler = logging.StreamHandler()
    log_formatter = logging.Formatter('%(asctime)s %(levelname)-6s %(message)s')
    log_handler.setFormatter(log_formatter)
    logger.addHandler(log_handler)
    logger.info('Logger initalised')
    return logger


def on_message(client, userdata, message):
    command = str(message.payload.decode("utf-8"))
    logger.info(command)
    ser.write(command)

def init_serial(device):
    ser = serial.Serial(device, 115200)
    ser.baudrate = 115200
    logger.info("Serial reader initialised")
    return ser

serial_device = '/dev/ttyUSB0'
broker_address = "192.168.1.2"
beo_serial_in = "beo/serial/in"
beo_serial_out = "beo/serial/out"

logger = init_logger()
ser = init_serial(serial_device)
client = connect_mqtt("BeoSerial", broker_address, beo_serial_in)

while True:
    read_ser = ser.readline()
    read_ser = read_ser.strip()
    logger.info('Received from serial: [ ' + read_ser + ' ]')
    client.publish(beo_serial_out, read_ser)


