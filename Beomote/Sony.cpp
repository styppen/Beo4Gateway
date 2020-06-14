#include "Sony.h"
#include "IRremote.h"
#include "Beomote.h"

void Sony::sendCommand(SonyCode code, int numberOfBits = 12) {
    for (int i = 0; i < 4; i++) {
      ir.sendSony(code, numberOfBits);
      delay(40);
  }
}


void Sony::handleCommand(BeoCommand cmd) {

  if (cmd.command == UP) {
      Serial.println("UP");
      sendCommand(SONY_UP, 12);
  }
  else if (cmd.command == DOWN) {
    Serial.println("DOWN");
    sendCommand(SONY_DOWN, 12);
  }
  else if (cmd.command == LEFT) {
    Serial.println("LEFT");
    sendCommand(SONY_LEFT, 12);
  }
  else if (cmd.command == RIGHT) {
    Serial.println("RIGHT");
    sendCommand(SONY_RIGHT, 12);
  }
  else if (cmd.command == GO) {
    Serial.println("OK (GO)");
    sendCommand(SONY_OK, 12);
  }
  else if (cmd.command == RED) {
    Serial.println("BACK (RED)");
    sendCommand(SONY_BACK, 15);
  }
  else if (cmd.command == BLUE) {
    Serial.println("HOME (BLUE)");
    sendCommand(SONY_HOME, 12);
  }
  else if (cmd.command == GREEN) {
    Serial.println("PLAY/PAUSE");
    sendCommand(SONY_PLAY, 15);
  }
  else if (cmd.command == MENU) {
    Serial.println("GOTO TV");
    sendCommand(SONY_TV, 12);
  }
    
}
