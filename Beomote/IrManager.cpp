#include "Arduino.h"
#include "IrManager.h"
#include "Beomote.h"
#include "Commands.h"

/*
 * Entry point for handling BeoCommands and logic
 * for mapping Beo to Sony IR commands
 */
void IrManager::handleCommand(BeoCommand cmd) {

  if (isStatusCode(cmd)) {
    handleStatusChange(cmd);
  }
  else {

    if (isTvOn && cmd.command == EXIT) {
      Serial.println("POWER (EXIT)");
      sendSony(SONY_POWER, 12);
      isTvOn = false;
    }
    
    // there are some commands we want to execute if TV or DVD state
    if (isTvOn) {
      handleSony(cmd);
    }
  }
}

/*
 * Handling of commands received via serial input.
 * This function receives an array of chars and 
 * converts it to a BeoCommand structure.
 */
void IrManager::handleCommand(char command[])
{
  Serial.print("IrManager::HandleCommand - ");
  Serial.println(command);
  
  /* NOTE:
   * Moving cmd.link and cmd.address outside of if stmts
   * causes this function to stop working. 
   */
  BeoCommand cmd;

  if (strcmp(command, "VOL.UP") == 0) {
    cmd.link = true;
    cmd.address = SOURCE_VIDEO;
    cmd.command = VOLUME_UP;
    handleCommand(cmd);
  }
  else if (strcmp(command, "VOL.DOWN") == 0) {
    cmd.link = true;
    cmd.address = SOURCE_VIDEO;
    cmd.command = VOLUME_DOWN;
    handleCommand(cmd);
  }
  else if (strcmp(command, "TV.ON") == 0) {
    cmd.link = true;
    cmd.address = SOURCE_VIDEO;
    cmd.command = TV;
    handleCommand(cmd);
  }
  else if (strcmp(command, "TV.OFF") == 0) {
    cmd.link = true;
    cmd.address = SOURCE_VIDEO;
    cmd.command = EXIT;
    handleCommand(cmd);
  }
}

/*
 * Function that handles state transitions. This handles the behaviour
 * of certain IR commands in Sony TV that might have undesired results
 * when pressed twice.
 */
void IrManager::handleStatusChange(BeoCommand cmd) {

  if (!isStatusCode(cmd)) {
    return;  
  }
  else {
    if (cmd.command == TV) {
      Serial.println("SONY TV MODE ENABLED");
      if (currentStatus != TV || !isTvOn) {
        // we don't want to send TV command if we're already in this state
        // because this would exit any Sony related menus and go to Siol TV 
        sendSony(SONY_TV, 12); 
        isTvOn = true;
      }
    }
    currentStatus = cmd.command;
 }
}

/*
 * Function that returns checks whether the input command
 * should change the state of the controller. 
 */
boolean IrManager::isStatusCode(BeoCommand cmd) {
  
  return cmd.command == RADIO || cmd.command == TV || cmd.command == CD || 
         cmd.command == DVD || cmd.command == PHONO;
}

/*
 * Function that triggers an IR led that is attached to the Sony TV 
 */
void IrManager::sendSony(SonyCode code, int numberOfBits = 12) {
    for (int i = 0; i < 4; i++) {
      ir.sendSony(code, numberOfBits);
      delay(40);
  }
}

/*
 * Main logic that translates BeoCommand struct to Sony IR commands. 
 */
void IrManager::handleSony(BeoCommand cmd) {

  if (cmd.command == UP) {
    Serial.println("UP");
    sendSony(SONY_UP, 12);
  }
  else if (cmd.command == VOLUME_UP) {
    Serial.println("VOLUME UP");
    sendSony(SONY_VOLUME_UP, 12);
  }
  else if (cmd.command == VOLUME_DOWN) {
    Serial.println("VOLUME DOWN");
    sendSony(SONY_VOLUME_DOWN, 12);
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
    //openSubtitles();  
  }
  else if (cmd.command == BLUE) {
    Serial.println("SONY ACTION MENU");
    sendSony(SONY_ACTION, 15);
  }
  else if (cmd.command == MENU) {
    Serial.println("SONY HOME MENU");
    sendSony(SONY_HOME, 12);
  }
  else if (cmd.command == STOP) {
    Serial.println("MUTE (STOP)");
    sendSony(SONY_MUTE, 12);
  }
}
