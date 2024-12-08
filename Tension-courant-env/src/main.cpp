/*!
* @brief Lire la puissance d'un système et les donnés environnementales
* @n Fonctions: Lit les données toutes les 5 secondes et les envoie en MQTT
* @copyright Copyright (c) 2024 GAM2IOT
* @version V0.1
* @date 2024-4-12
* @url https://github.com/DFRobot/DFRobot_CCS811
*/


#include <Arduino.h>
#include "I-V/Irms.h"                   // Ajouter le fichier avec toutes les fonctions
#include "BME280/Env.h"
#include "WiFi.h"
#include "esp_wpa2.h"
#include "time.h"
#include "PubSubClient.h"
#include "time.h"
#include "MCMVoltSense.h"             // Include MCM Volt Sense Library

Mesure Valeurs;                      
Environnement bme280;
WiFiClient espClient; 
PubSubClient client(espClient); 
MCMmeter meter;                       // Create an instance

const char *mqtt_broker = "XXX.XX.X.XXX"; // Identifiant du broker (Adresse IP)
const char *topic = "Pap"; // Nom du topic sur lequel les données seront envoyés. 
const char *mqtt_username = ""; // Identifiant dans le cas d'une liaison sécurisée
const char *mqtt_password = ""; // Mdp dans le cas d'une liaison sécurisée
const int mqtt_port = 1883; // Port : 1883 dans le cas d'une liaison non sécurisée et 8883 dans le cas d'une liaison cryptée

// Paramètres Wifi (à adapter sur un wifi plus simple en commentaire)
#define EAP_IDENTITY ""
#define EAP_PASSWORD "" 
#define EAP_USERNAME "" 
const char* ssid = ""; // Wifi SSID

//const char* ssid = ""; // Wifi SSID
//#define EAP_PASSWORD ""

//constante d'horodatage
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600 * 1;
const int   daylightOffset_sec = 3600 * 0;

// Fonction réception du message MQTT 
void callback(char *topic, byte *payload, unsigned int length) { 
  Serial.print("Le message a été envoyé sur le topic : "); 
  Serial.println(topic); 
  Serial.print("Message:"); 
  for (int i = 0; i < length; i++) { 
    Serial.print((char) payload[i]); 
  } 
  Serial.println(); 
  Serial.println("-----------------------"); 
}

void setup() 
{
  Serial.begin(115200);
  meter.VoltageStp(34, 38, 1.7);  // Voltage: input pin, calibration, phase_shift
  pinMode(33, INPUT);
  bme280.setupBME();

  // Connexion au réseau Wifi 
  WiFi.disconnect(true);
  WiFi.begin(ssid, WPA2_AUTH_PEAP, EAP_IDENTITY, EAP_USERNAME, EAP_PASSWORD); 
  //Avec un réseau plus simple :
  //Wifi.begin(ssid,EAP_PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(F("Tentative de connexion au WIFI"));
  }

  Serial.println("");
  Serial.println(F("L'ESP32 est connecté au WiFi !"));
  
// Connexion au broker MQTT  
  client.setServer(mqtt_broker, mqtt_port); 
  client.setCallback(callback); 

  while (!client.connected()) { 
    String client_id = "esp32-client-"; 
    client_id += String(WiFi.macAddress()); 
    Serial.printf("La chaîne de mesure %s se connecte au broker MQTT", client_id.c_str()); 
 
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) { 
      Serial.println("La chaîne de mesure est connectée au broker."); 
    } else { 
      Serial.print("La chaîne de mesure n'a pas réussi à se connecter ... "); 
      Serial.print(client.state()); 
      delay(2000); 
    } 
  } 

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void loop()
{
  float Irms = Valeurs.readACCurrentValue();
  float Irms1 = -0.0235*Irms*Irms+1.0166*Irms;
  Serial.println(Irms1);
  meter.analogVoltage(40,2000);  // Measure the AC voltage. Arguments = (# of AC cycles, timeout)
  float Vrms = meter.Vrms;       // Save the RMS Voltage into a variable.
  float Vrms1 = 0.0925*Vrms*Vrms-27.873*Vrms+2252.5;
  Serial.println(Vrms1);
  Valeurs.calcPuissance(Vrms1, Irms1);
  Valeurs.serialprintP();

  bme280.serialprintEnv();
  float Pression = bme280.calcPression();
  float Temp = bme280.calctemp();
  float Hum = bme280.calcHum();
  float Pap = Vrms1 * Irms1;

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
  }

  char timeMinutes[10];
  strftime(timeMinutes,sizeof(timeMinutes), "%H:%M:%S", &timeinfo);
  Serial.println(timeMinutes);

  String Data = "Time" + String(timeMinutes) + " Pap" + String(Pap) + " Vrms" + String(Vrms1) + " Irms" + String(Irms1) + " Hum" + String(Hum) + " Temp" + String(Temp) + " Pression" + String(Pression);
  
  client.publish(topic, Data.c_str()); // Publication de la température sur le topic (envoi d'une chaîne de caractères)
  client.subscribe(topic); // S'abonne au topic pour recevoir des messages
  client.loop(); // Gère les messages MQTT (pour lire la valeur de la température sur le moniteur série de platformIO)
  delay(5000); // Pause de 5 secondes entre chaque envoi
}
