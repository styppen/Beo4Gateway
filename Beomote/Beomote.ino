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

struct cmd {
  beo_command cmd;
  unsigned long received;
  boolean processed;
};


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


int beoIrPin = 10;
IRsend irsend;

unsigned char currentMode;


const int cmdBufferLength = 2;
int bufferIndex = 0;
cmd cmdBuffer[cmdBufferLength];
unsigned long lastCmdReceived = 0;
unsigned long lastEval = 0;

long evaluateBuffer = 1000; //ms
long doublePressThreshold = 300; //ms

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
    /*Serial.print(cmd.link, HEX);
    Serial.print(cmd.address, HEX);
    Serial.println(cmd.command, HEX);*/

    addToBuffer(cmd.command);
    printBuffer();
  } // if Beo.receiveCmd

  if (millis() - lastEval > evaluateBuffer)
  {
    //Serial.println("Evaluating buffer");

    if (isBufferReady())
    {
      if (isDoublePress()) {
        Serial.println("Double press detected");
        for (int i = 0; i < cmdBufferLength; i++) {
          if (!cmdBuffer[i].processed) {
            //do some action according to the code
            cmdBuffer[i].processed = true;
          }
        }
      }
      else {
        Serial.println("Single press detected");  

        for (int i = 0; i < cmdBufferLength; i++) {
          if (!cmdBuffer[i].processed) {
            //do some action according to the code
            cmdBuffer[i].processed = true;
          }
        }
        
      }  
    }
    
    lastEval = millis();
  }
}

boolean isDoublePress() {

  unsigned long diff;

  if (cmdBuffer[0].received > cmdBuffer[1].received) {
    diff = cmdBuffer[0].received - cmdBuffer[1].received;
  }
  else if(cmdBuffer[0].received < cmdBuffer[1].received) {
    diff = cmdBuffer[1].received - cmdBuffer[0].received;
  }

  
  boolean isThresholdReached =  diff <= doublePressThreshold;
  boolean hasContent = cmdBuffer[0].cmd != 0 && cmdBuffer[1].cmd != 0;
  boolean isContentEqual = cmdBuffer[0].cmd == cmdBuffer[1].cmd;
  boolean isProcessed = true; //!cmdBuffer[0].processed && !cmdBuffer[1].processed;

  Serial.print("Diff=");
  Serial.println(diff);
  return isThresholdReached && hasContent && isContentEqual && isProcessed;
}

boolean isBufferReady() {

  boolean hasContent = cmdBuffer[0].received != 0 || cmdBuffer[1].received != 0;
  boolean hasValidCotent = !cmdBuffer[0].processed || !cmdBuffer[1].processed;
  
  return hasContent && hasValidCotent;
}

void addToBuffer(beo_command cmd) {
  cmdBuffer[bufferIndex].cmd = cmd;
  cmdBuffer[bufferIndex].received = millis();
  cmdBuffer[bufferIndex].processed = false;
  bufferIndex = (bufferIndex + 1) % cmdBufferLength;
}

void printBuffer() {
  for (int i = 0; i < cmdBufferLength; i++) {
    Serial.print("Command=");
    Serial.print(cmdBuffer[i].cmd, HEX);
    Serial.print(", ");  
    Serial.print(" Received=");
    Serial.print(cmdBuffer[i].received);
    Serial.print(", ");  
    Serial.print(" Processed=");
    Serial.print(cmdBuffer[i].processed);
    Serial.print("; ");
  }  
  Serial.println();
}
