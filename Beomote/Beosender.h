/*
  Beosender.h - Library for sending special IR codes.
  Created by styppen.
*/

#include "Arduino.h"

class Beosender
{
  public:
    Beosender(int pin);
    void sendIR(byte address, byte command);
    void pulse(int usec);
    void data(int usec);

    void handleCommand(char command[]);

    void addToBuffer(char c);
    boolean isCommand();
    
  private:
    char serialCmd;
    int index = 0;
    char serialBuffer[10];
};
