import serial

ser=serial.Serial("/dev/ttyUSB0",9600)  #change ACM number as found from ls /dev/tty/ACM*
ser.baudrate=9600

while True:
    read_ser=ser.readline()
    print(read_ser.strip())
