// For ARDUINO SubController-IR-Flame-Detector__________________________________
//*******************************************************//
const int total_ir_flame = 3;
int limit = 500;
//Edit line numbers: 66,88,91-107,119,121
//*******************************************************//
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CNS, CE

const byte addresses[][6] = {"00001", "00002"};
int alert_pin = 5;
int activate_pin = 4;
int deactivate_pin = 3;

int i = 0;
byte c = 0;
char check_irf[] = "IRFx";
char response[10] = "";
char check_yes[] = "111";
char check_no[] = "000";

void setup() {
  Serial.begin(9600);
  radio.begin();
  pinMode(deactivate_pin, OUTPUT);                                                 //Temporary*********
  pinMode(activate_pin, INPUT);
  pinMode(alert_pin, OUTPUT);
  radio.openWritingPipe(addresses[1]); // 00001
  radio.openReadingPipe(1, addresses[0]); // 00002
  radio.setPALevel(RF24_PA_MIN);
  delay(100);

  digitalWrite(alert_pin, LOW);
  digitalWrite(deactivate_pin, LOW);
}

void loop() {
  if (digitalRead(activate_pin) == HIGH) {
    for (int x = 1; x <= total_ir_flame; x++) {
      int j = 0;
      delay(5);
    //  Serial.println("Transmitting...");                                                    //Temporary*********
     // radio.stopListening();
      if (i == total_ir_flame)
        i = 0;
      if (i == total_ir_flame - 1)
        c = 1;

      check_irf[3] = (char)(i + 65);
      i++;
      //radio.write(&check_irf, sizeof(check_irf));
      // Serial.println(check_irf);                                                            //Temporary*********

      delay(5);
      Serial.println("Receiving...");                                                       //Temporary*********
      radio.startListening();

      while (!radio.available()) {
        //Serial.println("...");
        if (j == limit)
          break;
        j++;
      }
      Serial.println("Waiting...");
      radio.read(&response, sizeof(response));
      Serial.println(response);                                                          //Temporary*********
     // Serial.println("im here");

      if ((response[1] == 'R') && (response[2] == 'F')) {
        switch (response[3]) {
          case 'A': Serial.println("Node A detected Fire!");
            break;
          case 'B': Serial.println("Node B detected Fire!");
            break;
          case 'C': Serial.println("Node C detected Fire!");
            break;
          default:
            break;
        }
        delay(5);
        Serial.println("Transmitting...");                                               //Temporary*********
        radio.stopListening();
        radio.write(&check_yes, sizeof(check_yes));
        Serial.println(check_yes);                                                       //Temporary*********

        digitalWrite(alert_pin, HIGH);
        delay(1000);
        digitalWrite(alert_pin, LOW);
      }

      if (c == 1) {
        digitalWrite(deactivate_pin, HIGH);
        delay(500);
        digitalWrite(deactivate_pin, LOW);
      }
    }
  }
  delay(500);                                      //time delay for succontroller to deactivate
}
