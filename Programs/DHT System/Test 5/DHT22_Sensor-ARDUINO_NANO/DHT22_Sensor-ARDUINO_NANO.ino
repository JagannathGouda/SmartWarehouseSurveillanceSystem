// For DHT22 Sensor-ARDUINO NANO______________
//******************************************************************//
int dht_number = 4;

//******************************************************************//
#include "DHT.h"
#include <dht.h>

#define DHTPIN 2
#define DHTTYPE DHT22

float temp = 0;
float humidity = 0;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  dht.begin();
  Serial.begin(9600);
}

void loop() {
  temp = dht.readTemperature();
  humidity = dht.readHumidity();
  Serial.println(temp);                         //TEMPORARY--------------------
  Serial.println(humidity);
}
