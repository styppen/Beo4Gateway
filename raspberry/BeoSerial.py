import serial
import paho.mqtt.client as mqtt
import logging

def init_logger(log_file):
    logger = logging.getLogger('main')
    logger.setLevel(logging.INFO)
    log_handlers = [logging.StreamHandler(), logging.FileHandler(log_file)]
    log_formatter = logging.Formatter('%(asctime)s %(levelname)-6s %(message)s')
    for handler in log_handlers:
        handler.setFormatter(log_formatter)
        logger.addHandler(handler)

    logger.info('Logger initalised')
    return logger


def on_message(client, userdata, message):
    command = str(message.payload.decode("utf-8"))
    logger.info(command)
    ser.write(command)

def on_connect(client, userdata, flags, rc):
    logger.info("MQTT client connected!")

def on_disconnect(client, userdata, rc):
    if rc != 0:
        logger.error("Unexpected MQTT disconnection [error-code=%d]. Will auto-reconnect", rc)

def init_serial(device):
    ser = serial.Serial(device, 115200)
    ser.baudrate = 115200
    logger.info("Serial reader initialised")
    return ser

serial_device = '/dev/ttyUSB0'
broker_address = '192.168.1.30'
beo_serial_in = "beo/serial/in"
beo_serial_out = "beo/serial/out"
log_file = '/home/volumio/beoserial.log'

logger = init_logger(log_file)
ser = init_serial(serial_device)

logger.info("Creating new MQTT client")
client = mqtt.Client('BeoSerial')

logger.info("Connecting to MQTT broker ... ")
client.connect(broker_address)

client.message_callback_add(beo_serial_in, on_message)
client.on_connect = on_connect
client.on_disconnect = on_disconnect

logger.info("Subscribing to MQTT topic " + beo_serial_in)
client.subscribe(beo_serial_in)
client.loop_start()

while True:
    read_ser = ser.readline()
    read_ser = read_ser.strip()
    logger.info('Received from serial: [ {0} ]'.format(read_ser.decode("utf-8")))
    client.publish(beo_serial_out, read_ser)


