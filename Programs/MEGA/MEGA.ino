// For ARDUINO MEGA__________________________________
//*******************************************************//
const int total_temp = 1;
const int total_ir_flame = 1;
const int total_mq = 1;
//const int total_sp420 = 1;
int limit = 500;
//long vib_threshold = 2000;
//*******************************************************//
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(9, 10);
RF24 radio(7, 8); // CNS, CE
const byte addresses[][6] = {"00001", "00002"};
//int activate_pin = 4;
//int deactivate_pin = 3;
int dht_alert_pin = 24;
int irf_alert_pin = 26;
int mqx_alert_pin = 28;
//int vib_alert_pin = 30;
//long vib_data = 0;

struct package
{
  float temperature = 0;
  float humidity = 0;
};

typedef struct package Package;
Package data;
/*
  struct bot
  {
  float temp = 0;
  float humid = 0;
  float irf = 0;
  float smoke = 0;
  };

  typedef struct bot Bot;
  Bot bot_data;*/
/*struct package1
{
  float temp = 0;
  float humid = 0;
  float irf = 0;
  float smoke = 0;
};

typedef struct package1 Package1;
Package1 data1;*/

char check_irf[] = "IRFx";
char check_mq[] = "MQxX";
//char check_sp420[] = "SP420x";
char response[10] = "";
char check_yes[] = "111";
char check_no[] = "000";

byte c = 0;
int i = 0;
int y = 0;
//int x = 0;

float store_temperature[total_temp];
float store_humidity[total_temp];
char check_temp[] = "DHTx";
//char check_bot[] = "BOT";

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  radio.begin();
  pinMode(dht_alert_pin, OUTPUT);
  pinMode(irf_alert_pin, OUTPUT);
  pinMode(mqx_alert_pin, OUTPUT);
 // pinMode(vib_alert_pin, OUTPUT);
  //  pinMode(activate_pin, INPUT);
  // pinMode(deactivate_pin, OUTPUT);
  radio.openWritingPipe(addresses[1]); // 00001
  radio.openReadingPipe(1, addresses[0]); // 00002
  radio.setPALevel(RF24_PA_MIN);
  delay(100);

  digitalWrite(dht_alert_pin, LOW);
  digitalWrite(irf_alert_pin, LOW);
  digitalWrite(mqx_alert_pin, LOW);
 // digitalWrite(vib_alert_pin, LOW);
  // digitalWrite(deactivate_pin, LOW);
}

void loop() {
  c = 0;
  dht();
  i = 0;
  y = 0;
  delay(1000);

  irf();
  i = 0;
  //response[10] = "";
  c = 0;
  y = 0;
  delay(1000);

  mqx();
  i = 0;
  y = 0;
  //response[10] = "";
  delay(1000);
/*
  vib();
  i = 0;
  //response[10] = "";
  delay(500);
  read_bot();
    delay(1000);
    y = 0;*/
}

//*********************************************************************************************************************************
//*********************************************************************************************************************************

void dht() {
  //if (digitalRead(activate_pin) == HIGH) {
  for (int c = 1; c <= total_temp; c++) {
    y = 0;
    int j = 0;
    delay(5);
retransmit:
    Serial.println("Transmitting...");                     //Setting to Transmission mode                              //Temporary*********
    radio.stopListening();
    if (i == total_temp)
      i = 0;

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
        //  break;
      }
      j++;
    }
    radio.read(&data, sizeof(data));
    Serial.print("Temperature = ");                                           //Temporary*********
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
      //digitalWrite(deactivate_pin, HIGH);
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
      //delay(500);
      // digitalWrite(deactivate_pin, LOW);
      transmit();
      /* if (check() == 1) {
         digitalWrite(dht_alert_pin, HIGH);
         delay(1000);
         digitalWrite(dht_alert_pin, LOW);
        }*/
    }
  }
  delay(100);                                        //Check-Time delay to give time for this controller to receive low signal
  // call_php();
  // }
}
/*
  int check() {
  for (int c = 0; c < total_temp; c++) {
    Serial.print(store_temperature[c]);
    Serial.print(",");
    Serial.print(store_humidity[c]);
    if (c != total_temp - 1)
      Serial.print(",");
  }
  Serial.println("...");

  int a = 0;
  for (int c = 0; c < total_temp; c++)
    if (store_temperature[c] < 27 || store_temperature[c] > 35) {
      a = 1;
      break;
    }
  if (a == 1)
    return 0;
  else
    return 1;
  }*/

void transmit() {
  Serial.println("Transmitting to NodeMCU...");
  String str = "";
  for (int b = 0; b < total_temp; b++) {
    if (b == 0)
      str += String("t") + String(b + 1) + String("=") + String(store_temperature[b]) + String("&h") + String(b + 1) + String("=") + String(store_humidity[b]);
    else
      str += String("&t") + String(b + 1) + String("=") + String(store_temperature[b]) + String("&h") + String(b + 1) + String("=") + String(store_humidity[b]);
  }
  //str += "&t4=00.00&h4=00.00";
  Serial.println(str);
  mySerial.println(str);
}

//*************************************************************************************************************************
//*************************************************************************************************************************

void irf() {
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
    Serial.println("Receiving...for IRF Response");                                                       //Temporary*********
    radio.startListening();

    while (!radio.available()) {
      //Serial.println("...");
      if (j == limit)
        break;
      j++;
    }
    Serial.println("Waiting...IRF");
    radio.read(&response, sizeof(response));
    Serial.println(response);                                                          //Temporary*********

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

      digitalWrite(irf_alert_pin, HIGH);
      Serial.println("Flame Alert!");
      delay(1000);
      digitalWrite(irf_alert_pin, LOW);
    }
  }
  delay(100);
}

//*********************************************************************************************************************************
//*********************************************************************************************************************************
void mqx() {
  for (int x = 1; x <= total_mq; x++) {
    int j = 0;
    delay(5);
    //   Serial.println("Transmitting...");                                                    //Temporary*********
    //   radio.stopListening();
    if (i == total_mq)
      i = 0;
    if (i == total_mq - 1)
      c = 1;

    check_mq[3] = (char)(i + 65);
    i++;
    //   radio.write(&check_mq, sizeof(check_mq));
    //   Serial.println(check_mq);                                                            //Temporary*********

    delay(5);
    Serial.println("Receiving...for Smoke Response");                                                       //Temporary*********
    radio.startListening();

    while (!radio.available()) {
      if (j == limit)
        break;
      j++;
    }

    radio.read(&response, sizeof(response));
    Serial.println(response);                                                          //Temporary*********
    Serial.println(response[1]);
    if ((response[1] == 'Q') && (response[2] == 'X')) {
      switch (response[3]) {
        case 'A': Serial.println("Node A detected Smoke!");
          break;
        case 'B': Serial.println("Node B detected Smoke!");
          break;
        case 'C': Serial.println("Node C detected Smoke!");
          break;
        default:
          break;
      }
      delay(5);
      Serial.println("Transmitting...");                                               //Temporary*********
      radio.stopListening();
      radio.write(&check_yes, sizeof(check_yes));
      Serial.println(check_yes);                                                       //Temporary*********

      digitalWrite(mqx_alert_pin, HIGH);
      Serial.println("Smoke Alert!");
      delay(1000);
      digitalWrite(mqx_alert_pin, LOW);
    }
    /* if (c == 1) {
       digitalWrite(deactivate_pin, HIGH);
       delay(500);
       digitalWrite(deactivate_pin, LOW);
      }*/
  }
  delay(100);
}
