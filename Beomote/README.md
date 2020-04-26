# Beo4/arduino
This is the Arduino part.

Bang & Olufsen IR eye is connected to Arduino Nano. 
The data received from the IR eye is then sent via serial port to raspberry pi.

There are also two IR leds connected to Arduino. 
When TV button is pressed on the Beo4 remote, the code switches to mode where B&O codes are translated to Sony TV codes and transmitted via the IR leds. 
That way, we are able to control Sony TV with Beo4 remote.

Additionally, when DVD button is pressed on Beo4 remote, the code swithces to mode where B&O codes are translated to IPTV setop box for IPTV and transmitted via the IR leds.
Analogue to the TV button, we are able to control the setop box with one Beo4 remote.


