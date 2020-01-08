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

int beoIrPin = 4;
IRsend irsend;

unsigned char currentMode;

void setup() {
  Serial.begin(9600);
  
  Beo.initialize(beoIrPin);
  Serial.println("I'm alive!");
}

void loop() {
  BeoCommand cmd;
  
  if (Beo.receive(cmd)) {  
    Serial.print(cmd.link, HEX);
    Serial.print(cmd.address, HEX);
    Serial.println(cmd.command, HEX);

  // B&O to SONY translator part
  if (cmd.command == TV && currentMode != TV) {
    currentMode = TV;
    Serial.println("TV MODE ENABLED");
  }
  if (cmd.command == RADIO || cmd.command == CD || cmd.command == PHONO) {
    currentMode = cmd.command;
    Serial.println("CHANGED MODE");
  }

  if (currentMode == TV) {
    if (cmd.command == UP) {
        Serial.println("UP");
        for (int i = 0; i < 4; i++) {
          irsend.sendSony(0x2F0, 12);
          delay(40);
        }
      }
      else if (cmd.command == DOWN) {
        Serial.println("DOWN");
        for (int i = 0; i < 4; i++) {
          irsend.sendSony(0xAF0, 12);
          delay(40);
        }
      }
      else if (cmd.command == LEFT) {
        Serial.println("LEFT");
        for (int i = 0; i < 4; i++) {
          irsend.sendSony(0x2D0, 12);
          delay(40);
        }
      }
      else if (cmd.command == RIGHT) {
        Serial.println("RIGHT");
        for (int i = 0; i < 4; i++) {
          irsend.sendSony(0xCD0, 12);
          delay(40);
        }
      }
      else if (cmd.command == GO) {
        Serial.println("OK (GO)");
        for (int i = 0; i < 4; i++) {
          irsend.sendSony(0xA70, 12);
          delay(40);
        }
      }
      else if (cmd.command == RED) {
        Serial.println("BACK (RED)");
        for (int i = 0; i < 4; i++) {
          irsend.sendSony(0x62E9, 15); // BACK
          delay(40);
        }
      }
      else if (cmd.command == BLUE) {
        Serial.println("HOME (BLUE)");
        for (int i = 0; i < 4; i++) {
          irsend.sendSony(0x70, 12); // HOME
          delay(40);
        }
      }
      else if (cmd.command == GREEN) {
        Serial.println("TV (GREEN)");
        for (int i = 0; i < 4; i++) {
          irsend.sendSony(0x250, 12); // TV
          delay(40);
        }
      }
      else if (cmd.command == EXIT) {
        Serial.println("POWER (EXIT)");
        for (int i = 0; i < 4; i++) {
          irsend.sendSony(0xA90, 12);
          delay(40);
        }
      }
    } // if isTV
  } // if Beo.receiveCmd
}
