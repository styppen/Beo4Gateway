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
  SONY_PLAY  = 0x2CE9
} SonyCode;

typedef enum SiolCode {
  SIOL_BACK  = 0xC1CCA956,
  SIOL_OK    = 0xC1CC827D,
  SIOL_UP    = 0xC1CC42BD,
  SIOL_DOWN  = 0xC1CCC23D,
  SIOL_LEFT  = 0xC1CC22DD,
  SIOL_RIGHT = 0xC1CCA25D,
  SIOL_PLAY  = 0xC1CC59A6,
  SIOL_NUMBER_0 = 0xC1CC06F9,
  SIOL_NUMBER_1 = 0xC1CC8679,
  SIOL_NUMBER_2 = 0xC1CC46B9,
  SIOL_NUMBER_3 = 0xC1CCC639,
  SIOL_NUMBER_4 = 0xC1CC26D9,
  SIOL_NUMBER_5 = 0xC1CCA659,
  SIOL_NUMBER_6 = 0xC1CC6699,
  SIOL_NUMBER_7 = 0xC1CCE619,
  SIOL_NUMBER_8 = 0xC1CC16E9,
  SIOL_NUMBER_9 = 0xC1CC9669,
  SIOL_YELLOW   = 0xC1CC12ED,
  SIOL_BLUE     = 0xC1CC29D6
} SiolCode;


class IrManager {

  public: 
    void handleCommand(BeoCommand cmd);

  private:
    int pin;
    beo_command currentStatus;
    IRsend ir;
    boolean isTvOn = false;
    
    void sendSony(SonyCode code, int numberOfBits);
    boolean isStatusCode(BeoCommand cmd);
    void handleStatusChange(BeoCommand cmd);
    void handleSony(BeoCommand cmd);
    void handleSiol(BeoCommand cmd);

    void openSubtitles();
  
  
};
