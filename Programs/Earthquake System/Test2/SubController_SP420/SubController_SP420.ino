// For ARDUINO SubController-Vibration-SP 420__________________________________
//*******************************************************//
const int total_sp420 = 3;
int limit = 500;
//Edit line numbers: 66,88,91-107,119,121
//*******************************************************//
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
//#include <SoftwareSerial.h>

RF24 radio(7, 8); // CNS, CE
//SoftwareSerial mySerial(9, 10);
const byte addresses[][6] = {"00001", "00002"};
int alert_pin = 5;
int activate_pin = 4;
int deactivate_pin = 3;

//int z = 0;
int x = 0;
//int i = 0;
byte c = 0;
char check_sp420[] = "SP420x";
char response[10] = "";
char check_yes[] = "111";
char check_no[] = "000";
byte flag[total_sp420];

void setup() {
  //  mySerial.begin(9600);   // Setting the baud rate of GSM Module
  Serial.begin(9600);
  radio.begin();
  pinMode(deactivate_pin, OUTPUT);                                                         //Temporary*********
  pinMode(activate_pin, INPUT);
  pinMode(alert_pin, OUTPUT);
  radio.openWritingPipe(addresses[1]); // 00001
  radio.openReadingPipe(1, addresses[0]); // 00002
  radio.setPALevel(RF24_PA_MIN);
  delay(100);
}

void loop() {
  if (digitalRead(activate_pin) == HIGH) {
    int i = 0;
    for (x = 1; x <= total_sp420; x++) {

retransmit:
      int j = 0;
      delay(5);
      Serial.println("Transmitting...");                                                    //Temporary*********
      radio.stopListening();
      if (i == total_sp420)
        i = 0;
      if (i == total_sp420 - 1)
        c = 1;

      check_sp420[5] = (char)(i + 65);
      i++;
      radio.write(&check_sp420, sizeof(check_sp420));
      Serial.println(check_sp420);                                                            //Temporary*********

      delay(5);
      Serial.println("Receiving...");                                                     //Temporary*********
      radio.startListening();

      while (!radio.available()) {
        if (j == limit) {
          i--;
          goto retransmit;
        }
        j++;
      }

      radio.read(&response, sizeof(response));
      Serial.println(response);                                                          //Temporary*********

      if (response[2] == 'Y') {
        delay(5);
        Serial.println("Transmitting...");                                               //Temporary*********
        radio.stopListening();
        radio.write(&check_yes, sizeof(check_yes));
        Serial.println(check_yes);                                                       //Temporary*********
        flag[i - 1] = 1;
      }
      /* else {
         if (response[2] != 'N')

         }*/
      if (c == 1) {
        //z++;
        if (check() == 1) {
          digitalWrite(alert_pin, HIGH);
          delay(1000);
          digitalWrite(alert_pin, LOW);
        }
        digitalWrite(deactivate_pin, HIGH);
        delay(500);
        digitalWrite(deactivate_pin, LOW);
        c = 0;
      }
    }
    //  if (z == 2) {
    for (int x = 0; x < total_sp420; x++)
      flag[x] = 0;
    //z = 0;
    // }
  }
  delay(500);                          //time delay for subcontroller to deactivate
}

int check() {
  byte f = 1;
  for (int x = 0; x < total_sp420; x++)
    if (flag[x] == 0) {
      f = 0;
      break;
    }

  if (f == 1)
    return 1;
  else
    return 0;
}
