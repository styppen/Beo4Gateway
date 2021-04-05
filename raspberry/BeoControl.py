import Codes
import logging
from Player import Player
import paho.mqtt.client as mqtt
import threading
import time

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
    command = command.strip()
    logger.info('Received on topic [ ' + message.topic + ' ], RAW message  [ ' + command + ' ]')

def on_serial(client, userdata, message):
    global current_state, input_index, dimmer_hold, previous_state
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
        input_index = 0
        player.pause()

    # handling of dimmer functionality
    if command == Codes.LIGHT and current_state != Codes.LIGHT:
        # we have to remember the previous state because the LIGHT option has timeout on the Beo4
        previous_state = current_state
        current_state = Codes.LIGHT

    # after LIGHT timeout, Beo4 reverts to previous state
    if command == Codes.LIST and current_state == Codes.LIGHT:
        logger.info('Restoring previous state')
        current_state = previous_state

    if current_state == Codes.LIGHT and command == Codes.GO:
        client.publish('beo/dimmer', 'DIMMER.TOGGLE')

    if current_state == Codes.LIGHT and command == Codes.GREEN:
        if not dimmer_hold:
            client.publish('beo/dimmer', 'DIMMER.HOLD')
            dimmer_hold = not dimmer_hold
        elif dimmer_hold:
            client.publish('beo/dimmer', 'DIMMER.RELEASE')
            dimmer_hold = not dimmer_hold



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
    global current_state, input_loop_list, input_index
    command = str(message.payload.decode("utf-8"))
    command = command.strip()
    logger.info('Received on topic [ ' + message.topic + ' ], RAW message  [ ' + command + ' ]')

    # handle messages from BeoEye
    if command == 'PLAY.SHORT' and current_state == Codes.CD:
        player.toggle_play()
    elif command == 'PLAY.DOUBLE':
        if input_index == 0:
            client.publish(beo_serial_in, 'RADIO;')
            current_state = Codes.RADIO
        elif input_index == 1:
            client.publish(beo_serial_in, 'CD;')
            current_state = Codes.CD
        elif input_index == 2:
            client.publish(beo_serial_in, 'AUX;')
            current_state = Codes.DVD

        input_index = input_index + 1
        input_index = input_index % 3

    elif command == 'PLAY.LONG':
        client.publish(beo_serial_in, 'OFF;')
    elif command == 'UP.SHORT':
        client.publish(beo_serial_in, 'VOL.UP;')
    elif command == 'UP.DOUBLE':
        if current_state == Codes.RADIO:
            client.publish(beo_serial_in, 'NEXT;')
        elif current_state == Codes.CD:
            player.next()
    elif command == 'DOWN.SHORT':
        client.publish(beo_serial_in, 'VOL.DOWN;')
    elif command == 'DOWN.DOUBLE':
        if current_state == Codes.RADIO:
            client.publish(beo_serial_in, 'PREV;')
        elif current_state == Codes.CD:
            player.previous()
    elif command == 'TIMER.SHORT':
        client.publish(beo_serial_in, 'TV.ON;')
    elif command == 'TIMER.LONG':
        client.publish(beo_serial_in, 'TV.OFF;')

broker_address = '192.168.1.2'
main_topic = "beo/#"
beo_serial_in = 'beo/serial/in'
beo_serial_out = 'beo/serial/out'
beo_eye = 'beo/eye'

airplay_enabled = False
dimmer_hold = False

logger = init_logger()
client = connect_mqtt("BeoControl2", broker_address, main_topic)
client.message_callback_add('beo/eye', on_beo_eye)
client.message_callback_add('beo/serial/out', on_serial)

current_state = '0000'
previous_state = '0000'
player = Player('http://localhost:3000')

modes = [Codes.TV, Codes.RADIO, Codes.SAT, Codes.DVD,
         Codes.CD, Codes.V_TAPE, Codes.RECORD, Codes.A_TAPE, Codes.PHONO]

input_index = 0
logger.info('Current mode: ' + current_state)

airplay = threading.Thread(airplay_check(), args=(1,))
airplay.start()

while True:
    continue
