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

// SONY REMOTE CODES
const unsigned long SONY_POWER = 0xA90;
const unsigned long SONY_MUTE  = 0x290;
const unsigned long SONY_UP    = 0x2F0;
const unsigned long SONY_DOWN  = 0xAF0;
const unsigned long SONY_LEFT  = 0x2D0;
const unsigned long SONY_RIGHT = 0xCD0;
const unsigned long SONY_HOME  = 0x70;
const unsigned long SONY_BACK  = 0x62E9;
const unsigned long SONY_OK    = 0xA70;
const unsigned long SONY_TV    = 0x250;

// SIOL BOX REMOTE CODES
const unsigned long SIOL_BACK  = 0xC1CCA956;
const unsigned long SIOL_OK    = 0xC1CC827D;
const unsigned long SIOL_UP    = 0xC1CC42BD;
const unsigned long SIOL_DOWN  = 0xC1CCC23D;
const unsigned long SIOL_LEFT  = 0xC1CC22DD;
const unsigned long SIOL_RIGHT = 0xC1CCA25D;
const unsigned long SIOL_PLAY  = 0xC1CC59A6;
const unsigned long SIOL_NUMBER_0 = 0xC1CC06F9;
const unsigned long SIOL_NUMBER_1 = 0xC1CC8679;
const unsigned long SIOL_NUMBER_2 = 0xC1CC46B9;
const unsigned long SIOL_NUMBER_3 = 0xC1CCC639;
const unsigned long SIOL_NUMBER_4 = 0xC1CC26D9;
const unsigned long SIOL_NUMBER_5 = 0xC1CCA659;
const unsigned long SIOL_NUMBER_6 = 0xC1CC6699;
const unsigned long SIOL_NUMBER_7 = 0xC1CCE619;
const unsigned long SIOL_NUMBER_8 = 0xC1CC16E9;
const unsigned long SIOL_NUMBER_9 = 0xC1CC9669;
const unsigned long SIOL_YELLOW   = 0xC1CC12ED;


int beoIrPin = 4;
IRsend irsend;

unsigned char currentMode;

void sendSony(unsigned long sonyCode, int numberOfBits) {
  for (int i = 0; i < 4; i++) {
    irsend.sendSony(sonyCode, numberOfBits);
    delay(40);
  }
}

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
        sendSony(SONY_MUTE, 12);
      }
      if (cmd.command == EXIT) {
        Serial.println("POWER (EXIT)");
        sendSony(SONY_POWER, 12);
      }
    }
    
    if (currentMode == TV) {
      if (cmd.command == UP) {
          Serial.println("UP");
          sendSony(SONY_UP, 12);
      }
      else if (cmd.command == DOWN) {
        Serial.println("DOWN");
        sendSony(SONY_DOWN, 12);
      }
      else if (cmd.command == LEFT) {
        Serial.println("LEFT");
        sendSony(SONY_LEFT, 12);
      }
      else if (cmd.command == RIGHT) {
        Serial.println("RIGHT");
        sendSony(SONY_RIGHT, 12);
      }
      else if (cmd.command == GO) {
        Serial.println("OK (GO)");
        sendSony(SONY_OK, 12);
      }
      else if (cmd.command == RED) {
        Serial.println("BACK (RED)");
        sendSony(SONY_BACK, 15);
      }
      else if (cmd.command == BLUE) {
        Serial.println("HOME (BLUE)");
        sendSony(SONY_HOME, 12);
      }
      else if (cmd.command == GREEN) {
        Serial.println("TV (GREEN)");
        sendSony(SONY_TV, 12);
      }
    } // if isTV
    else if (currentMode == DVD) {
      if (cmd.command == UP) {
        Serial.println("SIOL UP");
        irsend.sendNEC(SIOL_UP, 32);
      }
      if (cmd.command == DOWN) {
        Serial.println("SIOL DOWN");
        irsend.sendNEC(SIOL_DOWN, 32);
      }
      if (cmd.command == LEFT) {
        Serial.println("SIOL LEFT");
        irsend.sendNEC(SIOL_LEFT, 32);
      }
      if (cmd.command == RIGHT) {
        Serial.println("SIOL RIGHT");
        irsend.sendNEC(SIOL_RIGHT, 32);
      }
      if (cmd.command == GO) {
        Serial.println("SIOL OK");
        irsend.sendNEC(SIOL_OK, 32);
      }
      if (cmd.command == YELLOW) {
        Serial.println("SIOL YELLOW");
        irsend.sendNEC(SIOL_YELLOW, 32);
      }
      if (cmd.command == RED) {
        Serial.println("SIOL BACK");
        irsend.sendNEC(SIOL_BACK, 32);
      }
      if (cmd.command == NUMBER_0) {
        Serial.println("SIOL 0");
        irsend.sendNEC(SIOL_NUMBER_0, 32);
      }
      if (cmd.command == NUMBER_1) {
        Serial.println("SIOL 1");
        irsend.sendNEC(SIOL_NUMBER_1, 32);
      }
      if (cmd.command == NUMBER_2) {
        Serial.println("SIOL 2");
        irsend.sendNEC(SIOL_NUMBER_2, 32);
      }
      if (cmd.command == NUMBER_3) {
        Serial.println("SIOL 3");
        irsend.sendNEC(SIOL_NUMBER_3, 32);
      }
      if (cmd.command == NUMBER_4) {
        Serial.println("SIOL 4");
        irsend.sendNEC(SIOL_NUMBER_4, 32);
      }
      if (cmd.command == NUMBER_5) {
        Serial.println("SIOL 5");
        irsend.sendNEC(SIOL_NUMBER_5, 32);
      }
      if (cmd.command == NUMBER_6) {
        Serial.println("SIOL 6");
        irsend.sendNEC(SIOL_NUMBER_6, 32);
      }
      if (cmd.command == NUMBER_7) {
        Serial.println("SIOL 7");
        irsend.sendNEC(SIOL_NUMBER_7, 32);
      }
      if (cmd.command == NUMBER_8) {
        Serial.println("SIOL 8");
        irsend.sendNEC(SIOL_NUMBER_8, 32);
      }
      if (cmd.command == NUMBER_9) {
        Serial.println("SIOL 9");
        irsend.sendNEC(SIOL_NUMBER_9, 32);
      }
      if (cmd.command == GREEN) {
        Serial.println("SIOL PLAY");
        irsend.sendNEC(SIOL_PLAY    , 32);
      }
    } // if isDVD
  } // if Beo.receiveCmd
}
