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
 * Then of course, follows the mapping from B&O to Sony TV. 
 */

#include "Beomote.h"
#include "IRremote.h"
#include "Beosender.h"
#include "IrManager.h"

int beoIrPin = 4;
int beoSendPin = 9;
IRsend irsend;
unsigned char currentMode;

IrManager manager;
Beosender beo(beoSendPin);

char serialCmd;
const int bufferSize = 10;
int index = 0;
char serialBuffer[bufferSize];

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
      beo.handleCommand(manager, command);
      
      Beo.setInitialised(false);
      index = 0;
    }
    else {
      index++;  
    }
    
  }

  if (Serial.available() == 0 && !Beo.isInitialised()) {
    Beo.initialize(beoIrPin);
  }
  
  if (Beo.receive(cmd)) {  
    Serial.println(cmd.command, HEX); 
    
    manager.handleCommand(cmd);
  } // if Beo.receiveCmd
}
