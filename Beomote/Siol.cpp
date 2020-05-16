#include "Siol.h"
#include "IRremote.h"

void Siol::sendCommand(SiolCode code) {
  ir.sendNEC(code, 32);  
}

void Siol::handleCommand(BeoCommand cmd) {
  if (cmd.command == UP) {
    Serial.println("SIOL UP");
    ir.sendNEC(SIOL_UP, 32);
  }
  if (cmd.command == DOWN) {
    Serial.println("SIOL DOWN");
    ir.sendNEC(SIOL_DOWN, 32);
  }
  if (cmd.command == LEFT) {
    Serial.println("SIOL LEFT");
    ir.sendNEC(SIOL_LEFT, 32);
  }
  if (cmd.command == RIGHT) {
    Serial.println("SIOL RIGHT");
    ir.sendNEC(SIOL_RIGHT, 32);
  }
  if (cmd.command == GO) {
    Serial.println("SIOL OK");
    ir.sendNEC(SIOL_OK, 32);
  }
  if (cmd.command == YELLOW) {
    Serial.println("SIOL YELLOW");
    ir.sendNEC(SIOL_YELLOW, 32);
  }
  if (cmd.command == RED) {
    Serial.println("SIOL BACK");
    ir.sendNEC(SIOL_BACK, 32);
  }
  if (cmd.command == NUMBER_0) {
    Serial.println("SIOL 0");
    ir.sendNEC(SIOL_NUMBER_0, 32);
  }
  if (cmd.command == NUMBER_1) {
    Serial.println("SIOL 1");
    ir.sendNEC(SIOL_NUMBER_1, 32);
  }
  if (cmd.command == NUMBER_2) {
    Serial.println("SIOL 2");
    ir.sendNEC(SIOL_NUMBER_2, 32);
  }
  if (cmd.command == NUMBER_3) {
    Serial.println("SIOL 3");
    ir.sendNEC(SIOL_NUMBER_3, 32);
  }
  if (cmd.command == NUMBER_4) {
    Serial.println("SIOL 4");
    ir.sendNEC(SIOL_NUMBER_4, 32);
  }
  if (cmd.command == NUMBER_5) {
    Serial.println("SIOL 5");
    ir.sendNEC(SIOL_NUMBER_5, 32);
  }
  if (cmd.command == NUMBER_6) {
    Serial.println("SIOL 6");
    ir.sendNEC(SIOL_NUMBER_6, 32);
  }
  if (cmd.command == NUMBER_7) {
    Serial.println("SIOL 7");
    ir.sendNEC(SIOL_NUMBER_7, 32);
  }
  if (cmd.command == NUMBER_8) {
    Serial.println("SIOL 8");
    ir.sendNEC(SIOL_NUMBER_8, 32);
  }
  if (cmd.command == NUMBER_9) {
    Serial.println("SIOL 9");
    ir.sendNEC(SIOL_NUMBER_9, 32);
  }
  if (cmd.command == GREEN) {
    Serial.println("SIOL PLAY");
    ir.sendNEC(SIOL_PLAY    , 32);
  }
}
