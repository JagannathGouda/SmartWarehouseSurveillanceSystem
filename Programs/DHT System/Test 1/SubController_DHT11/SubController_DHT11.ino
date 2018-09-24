// For ARDUINO SubController-Temperature__________________________________
//*******************************************************//
int total_temp = 4;
//int limit = 500;
//Edit line numbers: 66,88,91-107,119,121
//*******************************************************//
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
//#include <SoftwareSerial.h>

RF24 radio(7, 8); // CNS, CE
//SoftwareSerial mySerial(9, 10);
const byte addresses[][6] = {"00001", "00002"};
int activate_pin = 2;
int deactivate_pin = 3;

struct package
{
  float temperature = 0;
  float humidity = 0;
};

typedef struct package Package;
Package data;

int c = 0;
int i = 0;
//float store_temperature[total_temp];
//float store_humidity[total_temp];
char check_temp[] = "TEMP";
//char response[10] = "";
char ackw[] = "111";
//char check_no[] = "000";

void setup() {
  // mySerial.begin(9600);   // Setting the baud rate of GSM Module
  Serial.begin(9600);
  radio.begin();
  pinMode(activate_pin, INPUT);                                                 //Temporary*********
  pinMode(deactivate_pin, OUTPUT);
  radio.openWritingPipe(addresses[1]); // 00001
  radio.openReadingPipe(1, addresses[0]); // 00002
  radio.setPALevel(RF24_PA_MIN);
  delay(100);
}

void loop() {
  if (digitalRead(activate_pin) == HIGH)
    c = 1;

  if (c == 1) {
    //  int j = 0;
    delay(5);
    Serial.println("Transmitting...");                     //Setting to Transmission mode                              //Temporary*********
    radio.stopListening();
    if (i == total_temp) {
      i = 0;
      c = 0;
    }

    check_temp[3] = (char)(i + 65);
    i++;
    radio.write(&check_temp, sizeof(check_temp));            //Transmitting A,B,C,D...
    Serial.println(check_temp);                                                            //Temporary*********

    delay(5);
    Serial.println("Receiving...");                         //Setting to Receiving mode                              //Temporary*********
    radio.startListening();

    while (!radio.available());
    radio.read(&data, sizeof(data));
    Serial.print("Temperature = ");                                                          //Temporary*********
    Serial.print(data.temperature);
   // store_temperature[i-1]=data.temperature;
    Serial.print(char(223));
    Serial.println("C");
    Serial.print("Humidity = ");
    Serial.print(data.humidity);
  //  store_humidity[i-1]=data.humidity;
    Serial.println("%");
    Serial.println("");

    delay(5);
    Serial.println("Transmitting...");                                               //Temporary*********
    radio.stopListening();
    radio.write(&ackw, sizeof(ackw));
    Serial.println(ackw);                                                                   //Temporary*********

    if (c == 0)
      digitalWrite(deactivate_pin, HIGH);
      delay(1000);                                        //Check-Time delay to give time for this controller to receive low signal
     // call_php();
  }
}
/*
void call_php(){
  
}*/
