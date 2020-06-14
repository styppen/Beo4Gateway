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
#include "Sony.h"
#include "Siol.h"
#include "Beosender.h"

int beoIrPin = 4;
int beoSendPin = 9;
IRsend irsend;
unsigned char currentMode;

Sony sony;
Siol siol;
Beosender beo(beoSendPin);

char serialCmd;
const int bufferSize = 10;
int index = 0;
char serialBuffer[bufferSize];

void setup() {
  Serial.begin(9600);
  
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
      beo.handleCommand(command);
      
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
    Serial.print(cmd.link, HEX);
    Serial.print(cmd.address, HEX);
    Serial.println(cmd.command, HEX);

    // B&O to SONY translator part
    if (cmd.command == TV && currentMode != TV) {
      currentMode = TV;
      Serial.println("TV MODE ENABLED");
    }
  
    if (cmd.command == DVD && currentMode != DVD){
      currentMode = DVD;
      Serial.println("SIOL BOX MODE ENABLED");
    }
    
    if (cmd.command == RADIO || cmd.command == CD || cmd.command == PHONO) {
      currentMode = cmd.command;
      Serial.println("CHANGED MODE");
    }

    if (currentMode == TV || currentMode == DVD) {
      if (cmd.command == STOP) {
        Serial.println("MUTE (STOP)");
        sony.sendCommand(SONY_MUTE, 12);
      }
      if (cmd.command == EXIT) {
        Serial.println("POWER (EXIT)");
        sony.sendCommand(SONY_POWER, 12);
      }
    }
    
    if (currentMode == TV) {
      sony.handleCommand(cmd);
    }
    else if (currentMode == DVD) {
      siol.handleCommand(cmd);
    }
  } // if Beo.receiveCmd
}
