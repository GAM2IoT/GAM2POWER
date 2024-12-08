#include "Env.h"
#include "Arduino.h"

#include <Adafruit_BME280.h>

Adafruit_BME280 bme;

void Environnement::setupBME() {
    if (!bme.begin(adresseI2C)) {
        Serial.println(F("--> ÉCHEC…"));
        while (1) {  // Arrêt du programme en cas d'échec
        }
    } else {
        Serial.println(F("--> RÉUSSIE !"));
    }
    Serial.println();
}


int Environnement::calctemp() {
    Temp = bme.readTemperature();
    return Temp;
}

int Environnement::calcHum(){
    Hum = bme.readHumidity();
    return Hum;
}

int Environnement::calcPression(){
    Pression = bme.readPressure() / 100.0F;
    return Pression;
}

void Environnement::serialprintEnv(){
    Serial.print(F("Température = "));
    Serial.print(Temp);
    Serial.println(F(" °C"));

    Serial.print(F("Humidité = "));
    Serial.print(Hum);
    Serial.println(F(" %"));
  
    Serial.print(F("Pression atmosphérique = "));
    Serial.print(Pression);
    Serial.println(F(" hPa"));
}