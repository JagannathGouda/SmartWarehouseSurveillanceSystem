// For DHT22 Sensor-ARDUINO NANO______________
//******************************************************************//
int dht_number = 4;
//int led_pin = 4;                                      //TEMPORARY--------------------
//******************************************************************//
#include "DHT.h"
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <dht.h>

#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

RF24 radio(7, 8); // CNS, CE
const byte addresses[][6] = {"00001", "00002"};

char call[10] = "";

struct package
{
  float temperature = 0;
  float humidity = 0;
};

typedef struct package Package;
Package data;

void setup() {
  dht.begin();
  Serial.begin(9600);
  radio.begin();
  //pinMode(led_pin, OUTPUT);                              //TEMPORARY--------------------
  radio.openWritingPipe(addresses[0]); // 00002
  radio.openReadingPipe(1, addresses[1]); // 00001
  radio.setPALevel(RF24_PA_MIN);
}

void loop() {
  delay(5);
  Serial.println("Receiving...");                   //TEMPORARY--------------------
  radio.startListening();
  while (!radio.available());
  radio.read(&call, sizeof(call));
  Serial.println(call);                             //TEMPORARY--------------------

  // if ((call[1] == 'H') && (call[3] == (char)(dht_number + 64))) {
  if (call[3] == (char)(dht_number + 64)) {
    data.temperature = dht.readTemperature();
    data.humidity = dht.readHumidity();
    delay(5);
    Serial.println("Transmitting...");                 //TEMPORARY--------------------
    radio.stopListening();
    radio.write(&data, sizeof(data));
    Serial.println(data.temperature);                         //TEMPORARY--------------------
    Serial.println(data.humidity);
  }
  //delay(2000);
}
