// For ARDUINO IR_Flame_Sensor______________
//******************************************************************//
int irf_number = 2;
int irf_pin = A0;
int threshold = 50;                                   //Check*************************************
//int led_pin = 4;                                         //TEMPORARY--------------------
//******************************************************************//
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CNS, CE
const byte addresses[][6] = {"00001", "00002"};

char call[10] = "";
char ackw[10] = "";
char check_pass[] = "IRFB";
char check_fail[] = "NNN";                               //check****************************
int status = 0;
const int sensorMin = 0;
const int sensorMax = 1024;

void setup() {
  Serial.begin(9600);
  radio.begin();
  pinMode(irf_pin, INPUT);
  // pinMode(led_pin, OUTPUT);                              //TEMPORARY--------------------
  radio.openWritingPipe(addresses[0]); // 00002
  radio.openReadingPipe(1, addresses[1]); // 00001
  radio.setPALevel(RF24_PA_MIN);
}

void loop() {
  int measure = analogRead(irf_pin);
  Serial.println(measure);
  if (measure < threshold) {
    Serial.println("Fire Detected!");
    status = 1;
  }
  //int range = map(measure, sensorMin, sensorMax, 0, 3);

  /*switch (range) {
     case 0:    // A fire closer than 1.5 feet away.
       Serial.println("** Close Fire **");
       status = 1;
       break;
     case 1:    // A fire between 1-3 feet away.
       Serial.println("** Distant Fire **");
       status = 1;
       break;
     case 2:    // No fire detected.
       Serial.println("No Fire");
       break;
    }*/

  while (status == 1) {
    delay(5);
    /*  Serial.println("Receiving...");                   //TEMPORARY--------------------
      radio.startListening();
      while (!radio.available());
      radio.read(&call, sizeof(call));
      Serial.println(call);                             //TEMPORARY--------------------

      if ((call[1] == 'R') && (call[3] == (char)(irf_number + 64)))*/ {
      delay(5);
      Serial.println("Transmitting...");                 //TEMPORARY--------------------
      radio.stopListening();
      radio.write(&check_pass, sizeof(check_pass));
      Serial.println(check_pass);                       //TEMPORARY--------------------

      delay(5);
      Serial.println("Receiving...");                  //TEMPORARY--------------------
      radio.startListening();
      while (!radio.available());
      radio.read(&ackw, sizeof(ackw));
      Serial.println(ackw);                           //TEMPORARY--------------------
      if (ackw[2] == '1') {
        status = 0;
        delay(2000);                                 //Wait for smoke to reduce
        // digitalWrite(led_pin, LOW);
      }
    }
  }
  delay(500);                                            //Check this
}
