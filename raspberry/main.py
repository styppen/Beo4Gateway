import serial
import Codes
import logging
from Player import Player

logging.basicConfig(format='%(asctime)s %(levelname)-6s %(message)s', level=logging.INFO, datefmt='%Y-%m-%d %H:%M:%S')

logging.info('Logger initalised')

ser = serial.Serial('/dev/ttyUSB0', 9600)  #change ACM number as found from ls /dev/tty*
ser.baudrate = 9600

current_state = "0000"
player = Player('http://localhost:3000')
modes = [Codes.TV, Codes.LIGHT, Codes.RADIO, Codes.SAT, Codes.DVD, Codes.CD, Codes.V_TAPE, Codes.RECORD, Codes.A_TAPE, Codes.PHONO]

print 'Current mode: ' + current_state

while True:
    read_ser = ser.readline()
    read_ser = read_ser.strip()
    logging.info('Received code = ' + read_ser)
    if read_ser in modes:
        if current_state == Codes.CD and read_ser != Codes.CD:
            logging.info('Pausing music because mode was switched')
            player.pause()

            # turn of the random led indicator so that it doesn't bother us
            player.random_led.off()
        current_state = read_ser
        logging.info('Current mode = ' + current_state)

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
