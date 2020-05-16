#include <Arduino.h>
#include "IRremote.h"
#include "Beomote.h"


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
  SIOL_YELLOW   = 0xC1CC12ED
} SiolCode;

class Siol {

    public:

        void sendCommand(SiolCode code);
        void handleCommand(BeoCommand cmd);

    private:
        IRsend ir;


        

};
