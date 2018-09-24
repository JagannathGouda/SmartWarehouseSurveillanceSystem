// For ARDUINO SubController-Temperature__________________________________
//*******************************************************//
const int total_temp = 4;
int limit = 500;
//*******************************************************//
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CNS, CE
const byte addresses[][6] = {"00001", "00002"};
int activate_pin = 4;
int deactivate_pin = 3;
int alert_pin = 5;

struct package
{
  float temperature = 0;
  float humidity = 0;
};

typedef struct package Package;
Package data;

int j = 0;
int c = 0;
int i = 0;
int y = 0;
float store_temperature[total_temp];
float store_humidity[total_temp];
char check_temp[] = "DHTx";

void setup() {
  Serial.begin(9600);
  radio.begin();
  pinMode(alert_pin, OUTPUT);
  pinMode(activate_pin, INPUT);
  pinMode(deactivate_pin, OUTPUT);
  radio.openWritingPipe(addresses[1]); // 00001
  radio.openReadingPipe(1, addresses[0]); // 00002
  radio.setPALevel(RF24_PA_MIN);
  delay(100);
}

void loop() {
  if (digitalRead(activate_pin) == HIGH)
    c = 1;
  y = 0;
  if (c == 1) {
    j = 0;
    delay(5);
retransmit:
    Serial.println("Transmitting...");                     //Setting to Transmission mode                              //Temporary*********
    radio.stopListening();
    if (i == total_temp) {
      i = 0;
      c = 0;
    }
    if (i == (total_temp - 1))
      y = 1;

    check_temp[3] = (char)(i + 65);
    i++;
    radio.write(&check_temp, sizeof(check_temp));            //Transmitting A,B,C,D...
    Serial.println(check_temp);                                                                                                //Temporary*********

    delay(5);
    Serial.println("Receiving...");                         //Setting to Receiving mode                                        //Temporary*********
    radio.startListening();

    while (!radio.available()) {
      if (j == limit) {
        i--;
        goto  retransmit;
      }
      j++;
    }
    radio.read(&data, sizeof(data));
    Serial.print("Temperature = ");                                                                                            //Temporary*********
    Serial.print(data.temperature);
    store_temperature[i - 1] = data.temperature;
    //Serial.print(char(223));
    Serial.print("*");
    Serial.println("C");
    Serial.print("Humidity = ");
    Serial.print(data.humidity);
    store_humidity[i - 1] = data.humidity;
    Serial.println("%");
    Serial.println("");

    if (y == 1) {
      digitalWrite(deactivate_pin, HIGH);
      Serial.print("Temperature List(*C): ");
      for (int x = 0; x < total_temp; x++) {
        Serial.print(store_temperature[x]);
        Serial.print(" ");
      }
      Serial.println("");

      Serial.print("Humidity     List(%): ");
      for (int x = 0; x < total_temp; x++) {
        Serial.print(store_humidity[x]);
        Serial.print(" ");
      }
      Serial.println("");
      Serial.println("");
      delay(500);
      digitalWrite(deactivate_pin, LOW);
      /* if (check() == 1) {
        digitalWrite(alert_pin, HIGH);
        delay(500);
        digitalWrite(alert_pin, LOW);
        }*/
    }
    delay(1000);                                        //Check-Time delay to give time for this controller to receive low signal
    // call_php();
  }
}
/*
  int check(){

  }
  /*
  void call_php(){

  }*/
