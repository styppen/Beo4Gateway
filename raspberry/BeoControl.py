import Codes
import logging
from Player import Player
import paho.mqtt.client as mqtt
import threading
import time

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
    command = command.strip()
    logger.info('Received on topic [ ' + message.topic + ' ], RAW message  [ ' + command + ' ]')

def on_connect(client, userdata, flags, rc):
    logger.info("MQTT client connected!")

def on_disconnect(client, userdata, rc):
    if rc != 0:
        logger.error("Unexpected MQTT disconnection. Will auto-reconnect")

def on_serial(client, userdata, message):
    global current_state
    command = str(message.payload.decode("utf-8"))
    command = command.strip()

    logger.info('Received on topic [ ' + message.topic + ' ], RAW message  [ ' + command + ' ]')
    # we have to check if there is a meaningful state transition to be done
    if command in modes:
        if current_state == Codes.CD and command != Codes.CD:
            logger.info('Pausing music because mode was switched')
            player.pause()

            # turn of the random led indicator so that it doesn't bother us
            player.random_led.off()
        current_state = command
        logger.info('Current mode = ' + current_state)

    # handle any action that might be relevant for Volumio
    if current_state == Codes.CD:
        if player.random_enabled:
            player.random_led.on()
        else:
            player.random_led.off()

        if command == Codes.GO:
            player.toggle_play()
        elif command == Codes.NEXT:
            player.next()
        elif command == Codes.PREVIOUS:
            player.previous()
        elif command == Codes.RANDOM:
            player.random()
        elif command == Codes.RED:
            player.clear_queue()
            player.play_playlist('Rock')
        elif command == Codes.YELLOW:
            player.clear_queue()
            player.play_playlist('Classical')
        elif command == Codes.BLUE:
            player.clear_queue()
            player.play_playlist('Jazz')
        elif command in player.radio_config.keys():
            radio_name = player.radio_config[command]
            player.clear_queue()
            player.play_playlist(radio_name)

    # see if we need to update the BeoEye LEDs
    if command in (Codes.DVD, Codes.TV):
        client.publish('beo/eye', 'TIMER.LED.ON')
        client.publish('beo/eye', 'PLAY.LED.ON')
    if command == Codes.EXIT:
        client.publish('beo/eye', 'TIMER.LED.OFF')
    elif command in (Codes.CD, Codes.PHONO, Codes.RADIO):
        client.publish('beo/eye', 'PLAY.LED.ON')
    elif command == Codes.STANDBY:
        client.publish('beo/eye', 'PLAY.LED.OFF')
        player.pause()

def airplay_check():
    global airplay_enabled, current_state
    while True:
        is_airplay = player.is_airplay()
        if is_airplay and not airplay_enabled:
            logger.info("Enabling AirPlay. Switching to CD mode.")
            client.publish(beo_serial_in, "CD;")
            client.publish(beo_eye, 'PLAY.LED.ON')
            current_state = Codes.CD
        elif not is_airplay and airplay_enabled:
            logger.info("AirPlay was disabled.")
        airplay_enabled = is_airplay
        time.sleep(2)

def on_beo_eye(client, userdata, message):
    global current_state
    command = str(message.payload.decode("utf-8"))
    command = command.strip()
    logger.info('Received on topic [ ' + message.topic + ' ], RAW message  [ ' + command + ' ]')

    # handle any messages from BeoEye
    if command == 'PLAY.SHORT':
        client.publish(beo_serial_in, 'RADIO;')
    elif command == 'PLAY.LONG':
        client.publish(beo_serial_in, 'OFF;')
    elif command == 'UP.SHORT':
        client.publish(beo_serial_in, 'VOL.UP;')
    elif command == 'DOWN.SHORT':
        client.publish(beo_serial_in, 'VOL.DOWN;')
    elif command == 'UP.LONG':
        client.publish(beo_serial_in, 'NEXT;')
    elif command == 'DOWN.LONG':
        client.publish(beo_serial_in, 'PREV;')
    elif command == 'TIMER.SHORT':
        client.publish(beo_serial_in, 'TV.ON;')
    elif command == 'TIMER.LONG':
        client.publish(beo_serial_in, 'TV.OFF;')

broker_address = '192.168.1.30'
main_topic = "beo/#"
beo_serial_in = 'beo/serial/in'
beo_serial_out = 'beo/serial/out'
beo_eye = 'beo/eye'
log_file = '/home/volumio/beocontrol.log'

airplay_enabled = False

logger = init_logger(log_file)
logger.info("Creating new MQTT instance")

client = mqtt.Client("BeoControl", clean_session=False)
logger.info("Connecting to MQTT broker ... ")
client.connect(broker_address)
client.on_message = on_message
client.on_connect = on_connect
client.on_disconnect = on_disconnect

logger.info("Subscribing to MQTT topic " + main_topic)
client.subscribe(main_topic)
client.message_callback_add('beo/eye', on_beo_eye)
client.message_callback_add('beo/serial/out', on_serial)
client.loop_start()

current_state = "0000"
player = Player('http://localhost:3000')

modes = [Codes.TV, Codes.LIGHT, Codes.RADIO, Codes.SAT, Codes.DVD,
         Codes.CD, Codes.V_TAPE, Codes.RECORD, Codes.A_TAPE, Codes.PHONO]

logger.info('Current mode: ' + current_state)

airplay = threading.Thread(airplay_check(), args=(1,))
airplay.start()


