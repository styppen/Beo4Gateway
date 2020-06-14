/*
  Beosender.cpp - Library for sending special IR codes with the Arduino platform.
*/

#include "Arduino.h"
#include "Beosender.h"
#include "Commands.h"
#include <avr/interrupt.h>


// SETUP OF TIMER 1
#define ENABLE_PULSE     (TCCR1A |= _BV(COM1A1))
#define DISABLE_PULSE    (TCCR1A &= ~(_BV(COM1A1)))
#define ENABLE_INTR_T1    (TIMSK1 = _BV(OCIE1A))
#define DISABLE_INTR_T1   (TIMSK1 = 0)

#define PULSE_SET_FREQ(khzVal) ({ \
  const uint16_t pwm = clk / 2000 / (khzVal); \
  TCCR1A = _BV(WGM11); \
  TCCR1B = _BV(WGM13) | _BV(CS10); \
  ICR1 = pwm; \
  OCR1A = pwm / 3; \
})

#define PULSE_SET_NORMAL() ({ \
  TCCR1A = 0; \
  TCCR1B = _BV(WGM12) | _BV(CS10); \
  OCR1A = clk * USECPERTICK / 1000000; \
  TCNT1 = 0; \
})

int pulseFreqKhz = 444;
int lastByte = 0;
long clk = 16000000;


Beosender::Beosender(int pin)
{
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);

    
}

void Beosender::sendIR(byte address, byte command)
{
    DISABLE_INTR_T1;
    PULSE_SET_FREQ(pulseFreqKhz);

    lastByte = 0;

    pulse(200); data(3125);  // start bit
    pulse(200); data(3125);  // start bit
    pulse(200); data(15625); // start bit
    pulse(200); data(3125);  // start bit


    for (int i = 7; i >- 1; i--) {
        pulse(200);
        byte state = bitRead(address, i);
        if(lastByte == state) {
            data(6250); //send Repeat
        }
        else if (state == 1){
            data(9357); //send "1"
        }
        else {
            data(3125); //send "0"
        }
        lastByte=state;
    }

    for (int i = 7; i >- 1; i--) {
        pulse(200);
        byte state = bitRead(command, i);
        if(lastByte == state){
            data(6250); //send Repeat
        }
        else if (state == 1){
            data(9357); //send "1"
        }
        else {
            data(3125); //send "0"
        }
 
        lastByte=state;
    }



    pulse(200); data(12500); // trailer bit
    pulse(200); data(1);    // stop bit
    delay(50);
}

void Beosender::addToBuffer(char c) {
  serialBuffer[index] = c;  
}

boolean Beosender::isCommand() {
  return serialBuffer[index] == ';';
}

void Beosender::handleCommand(char command[]) {
  if (strcmp(command, "VOL.UP") == 0) {
    sendIR(SOURCE_AUDIO, VOLUME_UP);
  }
  else if (strcmp(command, "VOL.DOWN") == 0) {
    sendIR(SOURCE_AUDIO, VOLUME_DOWN);
  }
  else if (strcmp(command, "NEXT") == 0) {
    sendIR(SOURCE_AUDIO, UP);
  }
  else if (strcmp(command, "PREV") == 0) {
    sendIR(SOURCE_AUDIO, DOWN);
  }
  else if (strcmp(command, "LOUD") == 0) {
    sendIR(SOURCE_AUDIO, LOUDNESS);
  }
  else if (strcmp(command, "RADIO") == 0) {
    sendIR(SOURCE_AUDIO, RADIO);
  }
  else if (strcmp(command, "CD") == 0) {
    sendIR(SOURCE_AUDIO, CD);
  }
  else if (strcmp(command, "OFF") == 0) {
    sendIR(SOURCE_AUDIO, STANDBY);
  }
}


void Beosender::pulse(int duration) {

  ENABLE_PULSE; //start PWM on PIN 9
  delayMicroseconds(duration); //duration of PWM   
}



void Beosender::data(int duration) {

  DISABLE_PULSE; //turn off PWM on PIN 
  delayMicroseconds(duration); //duration of gap
}
