// For ARDUINO Sensor-SP-420______________
//******************************************************************//
int sp420_number = 1;
int sp420_pin = 3;
long threshold = 2000;
//int led_pin = 4;                                      //TEMPORARY--------------------
//******************************************************************//
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CNS, CE
const byte addresses[][6] = {"00001", "00002"};

char call[10] = "";
char ackw[10] = "";
char check_pass[] = "YYY";
char check_fail[] = "NNN";                    //check****************************
int status = 0;

void setup() {
  Serial.begin(9600);
  radio.begin();
  pinMode(sp420_pin, INPUT);
  // pinMode(led_pin, OUTPUT);                              //TEMPORARY--------------------
  radio.openWritingPipe(addresses[0]); // 00002
  radio.openReadingPipe(1, addresses[1]); // 00001
  radio.setPALevel(RF24_PA_MIN);
}

void loop() {
  long measure = check();
  if (measure > threshold) {
    Serial.println("Vibration Detected");
    status = 1;
    // digitalWrite(led_pin, HIGH);                        //TEMPORARY--------------------
  }
  while (status == 1) {
    delay(5);
    Serial.println("Receiving...");                   //TEMPORARY--------------------
    radio.startListening();
    while (!radio.available());
    radio.read(&call, sizeof(call));
    Serial.println(call);                             //TEMPORARY--------------------

    if (call[1] == 'P' && (call[5] == (char)(sp420_number + 64))) {
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
        delay(3000);                                           //Time for vibration sensor to settle
        //digitalWrite(led_pin, LOW);
      }
    }
  }
}

long check() {
  delay(10);
  long measure = pulseIn(sp420_pin, HIGH);
  return measure;
}
