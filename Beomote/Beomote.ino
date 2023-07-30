/*
 * This source code is a modified Beomote code from christianlykke9
 * https://github.com/christianlykke9/Beomote
 * 
 * The B&O IR Eye is connected to pin 4
 * The IR LED is connected to pin 3 
 * 
 * The code is responsible to receive commands from Beo4 remote.
 * When a code is received, it is output via Serial.
 * Additionally, when IR B&O code for TV is received, the controller 
 * enters mode to control the Sony TV.
 * 
 * Besides the conversion of Beo4 IR commands to Sony TV, the controller
 * also waits for commands via serial input. The serial commands should
 * end with a ';' char. Once the serial command is received it is passed
 * to the IrManager to handle the command.
 * 
 * Then of course, follows the mapping from B&O to Sony TV. 
 * 
 * When compiling and uploading use the following settings:
 *      Board: Arduino Nano
 *      Processor: ATMega328P (Old bootloader)
 *      Port: Whatever appears in the menu
 *      Library: IRremote.h 2.8.1
 */

#include "Beomote.h"
#include "IRremote.h"
#include "Beosender.h"
#include "IrManager.h"

IrManager manager;
int beoIrPin = 4;
char serialCmd;
const int bufferSize = 10;
int index = 0;
static char serialBuffer[bufferSize];

void setup() {
  Serial.begin(115200);
  
  Beo.initialize(beoIrPin);
  Serial.println("I'm alive!");
}

void loop() {
  BeoCommand cmd;

  while(Serial.available() > 0) {
    
    serialCmd = Serial.read();
    serialBuffer[index] = serialCmd;

    if (serialBuffer[index] == ';') {
      char command[index - 1];
      for(int i = 0; i < index; i++) {
        command[i] = serialBuffer[i];
      }
      command[index] = '\0';
      
      Serial.print("Received serial command: ");  
      Serial.println(command);
      manager.handleCommand(command);
      
      index = 0;
      memset(serialBuffer, 0, sizeof(serialBuffer));
    }
    else {
      index++;  
    } 
  }
  
  if (Beo.receive(cmd)) {  
    Serial.println(cmd.command, HEX); 
    manager.handleCommand(cmd);
  }
}
