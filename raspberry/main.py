import serial
import Codes
import logging
from Player import Player
import paho.mqtt.client as mqtt

logger = logging.getLogger('main')
logger.setLevel(logging.INFO)
log_handler = logging.StreamHandler()
log_formatter = logging.Formatter('%(asctime)s %(levelname)-6s %(message)s')
log_handler.setFormatter(log_formatter)
logger.addHandler(log_handler)
logger.info('Logger initalised')

ser = serial.Serial('/dev/ttyUSB0', 9600)  #change ACM number as found from ls /dev/tty*
ser.baudrate = 9600

current_state = "0000"
player = Player('http://localhost:3000')
modes = [Codes.TV, Codes.LIGHT, Codes.RADIO, Codes.SAT, Codes.DVD, Codes.CD, Codes.V_TAPE, Codes.RECORD, Codes.A_TAPE, Codes.PHONO]

logger.info('Current mode: ' + current_state)

def on_message(client, userdata, message):
    command = str(message.payload.decode("utf-8"))
    logger.info("MQTT message received: \"" + command + "\"")

    if command == 'PLAY.SHORT':
        ser.write('RADIO;')
    elif command == 'PLAY.LONG':
        ser.write('OFF;')
    elif command == 'UP.SHORT':
        ser.write('VOL.UP;')
    elif command == 'DOWN.SHORT':
        ser.write('VOL.DOWN;')
    elif command == 'UP.LONG':
        ser.write('NEXT;')
    elif command == 'DOWN.LONG':
        ser.write('PREV;')    


broker_address = "192.168.1.2"

logger.info("Creating new MQTT instance")
client = mqtt.Client("P1")

logger.info("Connecting to raspberry broker")
client.connect(broker_address)

client.on_message = on_message
client.loop_start()

logger.info("Subscribing to MQTT topic beo/eye")
client.subscribe("beo/eye")

logger.info("Publishing init message to topic beo/eye")
client.publish("beo/eye", "MQTT is alive!")

ser.write("ARDUINO TEST");

while True:
    read_ser = ser.readline()
    read_ser = read_ser.strip()
    logger.info('Received code = ' + read_ser)
    if read_ser in modes:
        if current_state == Codes.CD and read_ser != Codes.CD:
            logger.info('Pausing music because mode was switched')
            player.pause()

            # turn of the random led indicator so that it doesn't bother us
            player.random_led.off()
        current_state = read_ser
        logger.info('Current mode = ' + current_state)

    if current_state == Codes.CD:

        if player.random_enabled:
            player.random_led.on()
        else:
            player.random_led.off()

        if read_ser == Codes.GO:
            player.toggle_play()
        elif read_ser == Codes.NEXT:
            player.next()
        elif read_ser == Codes.PREVIOUS:
            player.previous()
        elif read_ser == Codes.RANDOM:
            player.random()
        elif read_ser[2:] == Codes.RED:
            player.clear_queue()
            player.play_playlist('Rock')
        elif read_ser[2:] == Codes.GREEN:
            continue
        elif read_ser[2:] == Codes.YELLOW:
            player.clear_queue()
            player.play_playlist('Classical')
        elif read_ser[2:] == Codes.BLUE:
            player.clear_queue()
            player.play_playlist('Jazz')
        elif read_ser in player.radio_config.keys():
            radio_name = player.radio_config[read_ser]
            player.clear_queue()
            player.play_playlist(radio_name)
