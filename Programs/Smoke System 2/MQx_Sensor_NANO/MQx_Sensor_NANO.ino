// For ARDUINO Sensor-MQx______________
//******************************************************************//
int mq_number = 1;
float mq_pin = A0;
long threshold = 140;                                   //Check*************************************
int led_pin = 3;
int limit = 500;
//******************************************************************//
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CNS, CE
const byte addresses[][6] = {"00001", "00002"};

char call[10] = "";
char ackw[10] = "";
char check_pass[] = "MQXA";
char check_fail[] = "NNN";                    //check****************************
int status = 0;

void setup() {
  Serial.begin(9600);
  radio.begin();
  pinMode(mq_pin, INPUT);
  pinMode(led_pin, OUTPUT);                              //TEMPORARY--------------------
  radio.openWritingPipe(addresses[0]); // 00002
  radio.openReadingPipe(1, addresses[1]); // 00001
  radio.setPALevel(RF24_PA_MIN);
}

void loop() {
  float measure = analogRead(mq_pin);
  Serial.println(measure);
  if (measure > threshold) {
    status = 1;
    digitalWrite(led_pin, HIGH);                        //TEMPORARY--------------------
  }
  while (status == 1) {
    int j = 0;
    /* delay(5);
      Serial.println("Receiving...");                   //TEMPORARY--------------------
      radio.startListening();
      while (!radio.available());
      radio.read(&call, sizeof(call));
      Serial.println(call);                             //TEMPORARY--------------------

      if ((call[1] == 'Q') && (call[2] == 'x')) {
    */
    delay(5);
    Serial.println("Transmitting...");                 //TEMPORARY--------------------
    radio.stopListening();
    for (int n = 0; n <= 5; n++) {
      radio.write(&check_pass, sizeof(check_pass));
      Serial.println(check_pass);
    }

    delay(5);
    Serial.println("Receiving...");                  //TEMPORARY--------------------
    radio.startListening();
    while (!radio.available()) {
      if (j == limit)
        break;
      j++;
    }
    radio.read(&ackw, sizeof(ackw));
    Serial.println(ackw);                           //TEMPORARY--------------------
    if (ackw[2] == '1') {
      status = 0;
      delay(10000);                                 //Wait for smoke to reduce
      digitalWrite(led_pin, LOW);
    }
  }
  delay(500);
}
