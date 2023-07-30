#pragma once

#include "Arduino.h"
#include "Beomote.h"
#include "IRRemote.h"

typedef enum SonyCode {
  SONY_POWER = 0xA90,
  SONY_MUTE  = 0x290,
  SONY_UP    = 0x2F0,
  SONY_DOWN  = 0xAF0,
  SONY_LEFT  = 0x2D0,
  SONY_RIGHT = 0xCD0,
  SONY_HOME  = 0x70,
  SONY_BACK  = 0x62E9,
  SONY_OK    = 0xA70,
  SONY_TV    = 0x250,
  SONY_PLAY  = 0x2CE9,
  SONY_ACTION = 0x6923,
  SONY_VOLUME_UP = 0x490,
  SONY_VOLUME_DOWN = 0xC90
} SonyCode;

class IrManager {

  public: 
    void handleCommand(BeoCommand cmd);
    void handleCommand(char command[]);

  private:
    int pin;
    beo_command currentStatus;
    IRsend ir;
    boolean isTvOn = false;
    
    void sendSony(SonyCode code, int numberOfBits);
    boolean isStatusCode(BeoCommand cmd);
    void handleStatusChange(BeoCommand cmd);
    void handleSony(BeoCommand cmd);  
  
};
