#include "Arduino.h"

class Environnement
{
  public:
    void setupBME();
    int calctemp();
    int calcHum();
    int calcPression();
    void serialprintEnv();

    uint8_t adresseI2C = 0x77;

    int Temp,
        Hum,
        Pression;
};