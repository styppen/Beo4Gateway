import serial
import Codes
from gpiozero import LED
from Player import Player

ser = serial.Serial('/dev/ttyUSB0', 9600)  #change ACM number as found from ls /dev/tty*
ser.baudrate = 9600

current_state = "0000"
random_enabled = False;
random_led = LED(17)
player = Player('http://volumio')

modes = [Codes.TV, Codes.LIGHT, Codes.RADIO, Codes.SAT, Codes.DVD, Codes.CD, Codes.V_TAPE, Codes.RECORD, Codes.A_TAPE, Codes.PHONO]

print 'Current mode: ' + current_state

while True:
    read_ser = ser.readline()
    read_ser = read_ser.strip()
    print 'RECEIVED CODE:' + read_ser
    if read_ser in modes:
        if current_state == Codes.CD and read_ser != Codes.CD:
            print 'Pausing music because mode was switched'
            player.pause()
            # turn of the random led indicator so that it doesn't bother us
            random_led.off()
        current_state = read_ser
        print 'Current mode: ' + current_state

    if current_state == Codes.CD:
        
        # check if we have to turn on the random led
        if player.random_enabled:
            random_led.on()
        else:
            random_led.off()

        if read_ser == Codes.GO:
            player.toggle_play()
        elif read_ser == Codes.NEXT:
            player.next()
        elif read_ser == Codes.PREVIOUS:
            player.previous()
        elif read_ser == Codes.RANDOM:
            player.random()
            random_enabled = not random_enabled
            if random_enabled:
                print "RANDOM ENABLED"
                random_led.on()
            else:
                print "RANDOM DISABLED"
                random_led.off()
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
