import serial
import requests

radio_config = {}

def load_config():
    config_file = open('config.ini')
    i = 0
    for line in config_file:
        tmp = line.strip().split('=')
        radio_config[tmp[0]] = tmp[1]
        i += 1
    print radio_config


load_config() 

ser = serial.Serial('/dev/ttyUSB0',9600)  #change ACM number as found from ls /dev/tty*
ser.baudrate=9600

current_state = '0000'
random_enabled = False

cd_mode = '0192'
go_button = '0135'
next_button = '0134'
previous_button = '0132'
random_button = '01C1'

red_button = 'D9'
green_button = 'D5'
yellow_button = 'D4'
blue_button = 'D8'
# GO 0135
# >> 0134
# << 0132
# TV 0080
# LIGHT 01B9B
# RADIO 0181
# SAT 008A
# DVD 0086
# CD 0192
# V.TAPE 0585
# RECORD 0537
# A.TAPE 0191
# PHONO 0193
modes = ['0080', '01B9B', '0181', '008A', '0086', '0192', '0585', '0537', '0191', '0193']

print 'Current mode: ' + current_state

while True:
    read_ser = ser.readline()
    read_ser = read_ser.strip()
    print 'RECEIVED CODE:' + read_ser
    if read_ser in modes:
        if current_state == cd_mode and read_ser != cd_mode:
            print 'Pausing music because mode was switched'
            resp = requests.get('http://localhost:3000/api/v1/commands/?cmd=pause')
        current_state = read_ser
        print 'Current mode: ' + current_state

    if current_state == cd_mode:
        if read_ser == go_button:
            resp = requests.get('http://localhost:3000/api/v1/commands/?cmd=toggle')
            print 'PLAY/PAUSE'
        elif read_ser == next_button:
            resp = requests.get('http://localhost:3000/api/v1/commands/?cmd=next')
            print 'NEXT ACTION'
        elif read_ser == previous_button:
            resp = requests.get('http://localhost:3000/api/v1/commands/?cmd=prev')
            print 'PREVIOUS ACTION'
        elif read_ser == random_button:
            resp = requests.get('http://localhost:3000/api/v1/commands/?cmd=random')
            random_enabled = not random_enabled
            if random_enabled:
                print 'RANDOM ENABLED'
            else:
                print 'RANDOM DISABLED'
        elif read_ser[2:] == red_button:
            print 'RED button pressed'
            resp = requests.get('http://localhost:3000/api/v1/commands/?cmd=clearQueue')
            resp = requests.get('http://localhost:3000/api/v1/commands/?cmd=playplaylist&name=Rock')
        elif read_ser[2:] == green_button:
            print 'GREEN button pressed'
        elif read_ser[2:] == yellow_button:
            print 'YELLOW button pressed'
            resp = requests.get('http://localhost:3000/api/v1/commands/?cmd=clearQueue')
            resp = requests.get('http://localhost:3000/api/v1/commands/?cmd=playplaylist&name=Classical')
        elif read_ser[2:] == blue_button:
            print 'BLUE button pressed'
            resp = requests.get('http://localhost:3000/api/v1/commands/?cmd=clearQueue')
            resp = requests.get('http://localhost:3000/api/v1/commands/?cmd=playplaylist&name=Jazz')
        elif read_ser in radio_config.keys():
            radio_name = radio_config[read_ser]
            resp = requests.get('http://localhost:3000/api/v1/commands/?cmd=playplaylist&name=' + radio_name)
            print 'Playing Webradio: ' + radio_name
