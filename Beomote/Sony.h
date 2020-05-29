#include <Arduino.h>
#include "IRremote.h"
#include "Beomote.h"

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
  SONY_TV    = 0x250
} SonyCode;


class Sony {

    public:
        void sendCommand(SonyCode code, int numberOfBits);

        void handleCommand(BeoCommand cmd);

    private:
        IRsend ir;
};
