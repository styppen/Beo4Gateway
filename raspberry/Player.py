import requests
from gpiozero import LED


def load_config():
    config_file = open('config.ini')
    i = 0
    radio_config = {}
    for line in config_file:
        tmp = line.strip().split('=')
        radio_config[tmp[0]] = tmp[1]
        i += 1
    return radio_config


class Player:

    def __init__(self, hostname):
        self.hostname = hostname
        self.random_enabled = self.get_random()
        self.radio_config = load_config()
        self.random_led = self.init_random_led()
        print {'hostname': self.hostname, 'random': self.random_enabled}

    def init_random_led(self, gpio_pin=17):
        led = LED(gpio_pin)
        if self.random_enabled:
            led.on()
        else:
            led.off()
        return led

    def get_random(self):
        resp = self.get_status()
        return resp.json()['random']

    def get_status(self):
        return requests.get(self.hostname + '/api/v1/getState')

    def play(self):
        self.do_command('play')
        print 'PLAY'

    def pause(self):
        self.do_command('pause')
        print 'PAUSE'

    def toggle_play(self):
        self.do_command('toggle')
        print 'PLAY/PAUSE'

    def next(self):
        self.do_command('next')
        print 'NEXT'

    def previous(self):
        self.do_command('prev')
        print 'PREVIOUS'

    def random(self):
        self.do_command('random')
        self.random_led.toggle()
        print 'RANDOM'

    def play_playlist(self, playlist_name):
        requests.get(self.hostname + '/api/v1/commands/?cmd=playplaylist&name=' + playlist_name)
        print 'PLAY PLAYLIST: ' + playlist_name

    def clear_queue(self):
        self.do_command('clearQueue')
        print 'CLEAR QUEUE'

    def do_command(self, command):
        requests.get(self.hostname + '/api/v1/commands/?cmd=' + command)
