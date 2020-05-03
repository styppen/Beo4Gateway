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


int beoIrPin = 10;
IRsend irsend;

unsigned char currentMode;

const int commandBufferLength = 10;
beo_command commandBuffer[commandBufferLength];
int bufferIndex = 0;
const long evaluateBufferAfter = 800; // miliseconds
const long clearBufferAfter = 3000; // miliseconds
unsigned long lastEval = 0;
unsigned long lastClear = 0;

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

void addToBuffer(beo_command cmd) {
  commandBuffer[bufferIndex] = cmd;
  bufferIndex++;
  bufferIndex = bufferIndex%commandBufferLength;
}

void printBuffer() {
  for (int i = 0; i < commandBufferLength; i++) {
    Serial.print(commandBuffer[i], HEX);
    Serial.print(" ");
  }  

  Serial.println();
}

void clearBuffer() {
  for (int i = 0; i < commandBufferLength; i++) {
    commandBuffer[i] = 0;  
  }
  lastClear = millis();
  bufferIndex = 0;
  //Serial.println("Buffer cleared");
}

void clearRepeatCodes(beo_command cmd)
{
  for (int i = 0; i < commandBufferLength; i++)
  {
    if (commandBuffer[i] == cmd)
    {
      commandBuffer[i] = 0;  
    }
  }
  
}

void evaluateBuffer() {

  beo_command cmd;
  for (int i = 0; i < commandBufferLength; i++) {

    if (commandBuffer[i] == RED) {
      //Serial.println("Found RED code in the buffer");
      if (commandBuffer[(i + 1) % commandBufferLength] == RED_REPEAT) {
        Serial.println("Send RED_REPEAT code");
        clearRepeatCodes(RED_REPEAT);
      }
      else {
        Serial.println("Send RED code");
      }
      commandBuffer[i] = 0;
      break;
    }
    else if(commandBuffer[i] == GREEN) {
      //Serial.println("Found GREEN code in the buffer");
      if (commandBuffer[(i + 1) % commandBufferLength] == GREEN_REPEAT) {
        Serial.println("Send GREEN_REPEAT code");
      }
      else {
        Serial.println("Send GREEN code");
      }
      commandBuffer[i] = 0;
      break;
    }
    else if(commandBuffer[i] == YELLOW) {
      //Serial.println("Found YELLOW code in the buffer");
      if (commandBuffer[(i + 1) % commandBufferLength] == YELLOW_REPEAT) {
        Serial.println("Send YELLOW_REPEAT code");
      }
      else {
        Serial.println("Send YELLOW code");
      }
      commandBuffer[i] = 0;
      break;
    }
    else if(commandBuffer[i] == BLUE) {
      //Serial.println("Found BLUE code in the buffer");
      if (commandBuffer[(i + 1) % commandBufferLength] == BLUE_REPEAT) {
        Serial.println("Send BLUE_REPEAT");
      }
      else {
        Serial.println("Send BLUE code");
      }
      commandBuffer[i] = 0;
      break;
    }
  }
  lastEval = millis();
}

void loop() {
  BeoCommand cmd;

  // 1. receive the commands from the sensor
  // 2. store the received commands in buffer
  // 3. when threshold is reached, evaluate the buffer
  
  if (Beo.receive(cmd)) {  
    Serial.print(cmd.link, HEX);
    Serial.print(cmd.address, HEX);
    Serial.println(cmd.command, HEX); 
    addToBuffer(cmd.command);
    printBuffer();
  } // if Beo.receiveCmd

  if ((millis() - lastEval) > evaluateBufferAfter) {
    //Serial.println("Evaluating the buffer");
    evaluateBuffer();
    //printBuffer();
  }
  

  // clear the buffer every X seconds
  if ((millis() - lastClear) > clearBufferAfter) {
    //printBuffer();
    lastClear = millis();
  }
  
}
