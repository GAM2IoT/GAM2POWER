#include "Arduino.h"

#define ADC_BITS    10

#define ADC_COUNTS  (1<<ADC_BITS)

class Mesure
{
  public:
    void voltage(unsigned int _inPinV, double _VCAL);
    void voltageTX(double _VCAL);
    float calcV(unsigned int timeout);
    void serialprintV();
    float serialprintC();
    float calcPuissance(float Vrms, float Irms);
    void serialprintP();
    void calcPhaseShift(unsigned int DureeMesure);
    void serialprintPhase();
    double filteredV1();
    //Variables de tout le programme
    double filteredV,
           offsetV,
           V_RATIO,
           sqV, sumV,
           calculateIrms(),
           periode,
           Pastemps,
           Carreesomme;;
    
    int Tensionalim,
        VCAL,
        nbrpassage,
        nbrechantillons,
        analogPin,
        numMeasurements,
        Papparente,
        Pactive,
        Preactive;

    float Vmesure,
          readACCurrentValue(),
          ACCurrtntValue,
          peakVoltage,
          incertitude,
          voltageVirtualValue;

    long peakTimeVoltage,
         peakTimeCurrent,
         startTime,
         deltaTimeMicroseconds,
         endTime;
};