#include <SoftwareSerial.h>

SoftwareSerial mySerial(9, 10);
int dht_pin = 3;
int irf_pin = 4;
int mqx_pin = 5;
int sw420_pin = 6;
int buzzer_pin = 11;
int s = 0;

void setup() {
  mySerial.begin(9600);
  Serial.begin(9600);
  pinMode(dht_pin, INPUT);
  pinMode(irf_pin, INPUT);
  pinMode(mqx_pin, INPUT);
  pinMode(sw420_pin, INPUT);
  pinMode(buzzer_pin, OUTPUT);
}

void loop() {
  s = 0;
  if (digitalRead(dht_pin) == HIGH) {
    s = 1;
    //Serial.println(
    digitalWrite(buzzer_pin, HIGH);
  }
  else if (digitalRead(irf_pin) == HIGH) {
    s = 2;
    Serial.println("Spark/Flame Detected!");
    digitalWrite(buzzer_pin, HIGH);
  }
  else if (digitalRead(mqx_pin) == HIGH) {
    s = 3;
    Serial.println("Smoke Detected!");
    digitalWrite(buzzer_pin, HIGH);
  }
  /*else if (digitalRead(sw420_pin) == HIGH) {
    s = 4;
    Serial.println("Earthquake Detected!");
    digitalWrite(buzzer_pin, HIGH);
  }*/
  else {
    s = 0;
  }
  
  if (s != 0)
    alert();
}

void alert() {
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+918763920915\"\r"); // Replace with owner's mobile number
  delay(1000);
  digitalWrite(buzzer_pin, LOW);
  switch (s) {
    case 0:
      break;
    case 1: mySerial.println("Alert! Critical TEMPERATURE or HUMIDITY REACHED.");
      break;
    case 2: mySerial.println("Alert! FIRE/SPARKS DETECTED.");
      break;
    case 3: mySerial.println("Alert! SMOKE/FIRE DETECTED.");
      break;
    case 4: mySerial.println("Alert! EARTHQUAKE DETECTED.");
      break;
    default:
      break;
  }

  s = 0;
  delay(100);
  mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}
