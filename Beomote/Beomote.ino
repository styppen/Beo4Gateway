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

struct cmd {
  beo_command cmd;
  unsigned long received;
  boolean processed;
};


int beoIrPin = 4;
IRsend irsend;
Sony sony;
Siol siol;

unsigned char currentMode;


const int cmdBufferLength = 2;
int bufferIndex = 0;
cmd cmdBuffer[cmdBufferLength];
unsigned long lastCmdReceived = 0;
unsigned long lastEval = 0;

long evaluateBuffer = 1000; //ms
long doublePressThreshold = 300; //ms


const int cmdBufferLength = 2;
int bufferIndex = 0;
cmd cmdBuffer[cmdBufferLength];
unsigned long lastCmdReceived = 0;
unsigned long lastEval = 0;

long evaluateBuffer = 700; //ms
long doublePressThreshold = 400; //ms

void handleStatus() {

  for (int i = 0; i < cmdBufferLength; i++) {
    if (cmdBuffer[i].cmd == TV && currentMode != TV) {
      currentMode = TV;
    }
  
    if (cmdBuffer[i].cmd == DVD && currentMode != DVD){
      currentMode = DVD;
      Serial.println("SIOL BOX MODE ENABLED");
    }
    
    if (cmdBuffer[i].cmd == RADIO || cmdBuffer[i].cmd == CD || cmdBuffer[i].cmd == PHONO) {
      currentMode = cmdBuffer[i].cmd;
      Serial.println("CHANGED MODE");
    }
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
    //printBuffer();
  } // if Beo.receiveCmd

  if (millis() - lastEval > evaluateBuffer)
  {

    if (isBufferReady())
    {

      handleStatus();
      
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

            // some functions are available in TV and DVD mode
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
          } // if cmd not processed
        } // for loop
        
      } // isDoublePress
    } // isBufferReady
    
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

  return isThresholdReached && hasContent && isContentEqual && isProcessed;
}

boolean isBufferReady() {

  boolean isFirstProcessed = cmdBuffer[0].processed;
  boolean hasContent = cmdBuffer[0].received != 0 || cmdBuffer[1].received != 0;
  boolean hasValidCotent = !cmdBuffer[0].processed || !cmdBuffer[1].processed;
  boolean canBeMidPress = (millis() - cmdBuffer[0].received) < doublePressThreshold;

  // buffer could be evaluated between a potential double click ... 
  // we want to prevent that ... wait a bit more
  boolean isMidPress = !isFirstProcessed && canBeMidPress;
  
  return hasContent && hasValidCotent && !isMidPress;
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
