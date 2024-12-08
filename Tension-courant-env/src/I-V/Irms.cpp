#include "Irms.h"
#include <cmath>
#include "Arduino.h"

#define ACTectionRange 20    //Gamme/modèle de notre pince ampèremétrique
#define VREF 5.0
const int ACPin = 36;         //pin de lecture


//------------------------------------------------------------------------------------------------
//Passage aux mesures de courant
//------------------------------------------------------------------------------------------------
float Mesure::readACCurrentValue()
{
  float ACCurrtntValue = 0;
  float peakVoltage = 0;  
  float voltageVirtualValue = 0;  //Vrms
  for (int i = 0; i < 5; i++)
  {
    peakVoltage += analogRead(36);   //read peak voltage
    delay(1);
  }
  peakVoltage = peakVoltage / 5;
  voltageVirtualValue = peakVoltage * 0.707;    //change the peak voltage to the Virtual Value of voltage

  /*The circuit is amplified by 2 times, so it is divided by 2.*/
  voltageVirtualValue = (voltageVirtualValue / 4095 * 5 ) / 2;  

  ACCurrtntValue = voltageVirtualValue * 20;
  return ACCurrtntValue;
}

//---------------------------------------------------------------------------------------------
//Calcul des puissances
//---------------------------------------------------------------------------------------------
float Mesure::calcPuissance( float Vrms, float Irms){
  Papparente = Vrms * Irms;                                // Puissance apparente
  Preactive = 0;
  float Utension = 0.2233;
  float Ucourant = 0.03426;
  incertitude = Utension*Irms + Ucourant*Vrms;
  return Papparente;
  return incertitude;
}

void Mesure::serialprintP(){
  String U = "(" + String(incertitude) + ")";
  Serial.print("Puissance apparente = ");
  Serial.print(Papparente);
  Serial.println(U + "VA");
  Serial.print("Puissance active = ");
  Serial.print(Papparente);
  Serial.println(U + "V");
  Serial.print("Puissance reactive = ");
  Serial.print(Preactive);
  Serial.println(U + "W");
}