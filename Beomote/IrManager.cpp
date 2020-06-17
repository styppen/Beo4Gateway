#include "Arduino.h"
#include "IrManager.h"
#include "Beomote.h"
#include "Commands.h"


void IrManager::handleCommand(BeoCommand cmd) {

  if (isStatusCode(cmd)) {
    handleStatusChange(cmd);
  }
  else {

    // there are some commands we want to execute if TV or DVD state
    if (currentStatus == TV || currentStatus == DVD) {
      if (cmd.command == EXIT) {
        Serial.println("POWER (EXIT)");
        sendSony(SONY_POWER, 12);
        isTvOn = false;
      }
      if (cmd.command == STOP) {
        Serial.println("MUTE (STOP)");
        sendSony(SONY_MUTE, 12);
      }
    }
    
    if (currentStatus == TV) {
      handleSony(cmd);  
    }
    else if (currentStatus == DVD) {
      handleSiol(cmd);  
    }
  }
}

void IrManager::handleStatusChange(BeoCommand cmd) {

  if (!isStatusCode(cmd)) {
    return;  
  }
  else {
    if (cmd.command == TV) {
      Serial.println("SONY TV MODE ENABLED");
      if (currentStatus != TV) {
        // we don't want to send TV command if we're already in this state
        // because this would exit any Sony related menus and go to Siol TV 
        sendSony(SONY_TV, 12); 
      }
    }
    else if(cmd.command == DVD) {
      Serial.println("SIOL TV MODE ENABLED");
      sendSony(SONY_TV, 12);  
      isTvOn = true;
    }
    currentStatus = cmd.command;
 }
    
}

boolean IrManager::isStatusCode(BeoCommand cmd) {
  
  return cmd.command == RADIO || cmd.command == TV || cmd.command == CD || 
         cmd.command == DVD || cmd.command == PHONO;
}

void IrManager::sendSony(SonyCode code, int numberOfBits = 12) {
    for (int i = 0; i < 4; i++) {
      ir.sendSony(code, numberOfBits);
      delay(40);
  }
}

void IrManager::handleSiol(BeoCommand cmd) {
  
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
  if (cmd.command == BLUE) {
    Serial.println("SIOL WINDOW");
    ir.sendNEC(SIOL_BLUE, 32);  
  } 
}

void IrManager::handleSony(BeoCommand cmd) {

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
  else if (cmd.command == GREEN) {
    Serial.println("PLAY/PAUSE");
    sendSony(SONY_PLAY, 15);
  }
  else if(cmd.command == YELLOW) {
    openSubtitles();  
  }
  else if (cmd.command == MENU) {
    Serial.println("SONY HOME MENU");
    sendSony(SONY_HOME, 12);
  }
}

void IrManager::openSubtitles() {
  Serial.println("OPENING SUBTITLES DIALOG ...");
  sendSony(SONY_OK, 12);
  delay(250);  
  for (int i = 0; i < 3; i++) {
    sendSony(SONY_LEFT, 12);  
    delay(250);
  }
  sendSony(SONY_OK, 12);
  Serial.println("SUBTITLES DIALOG OPEN");
}
