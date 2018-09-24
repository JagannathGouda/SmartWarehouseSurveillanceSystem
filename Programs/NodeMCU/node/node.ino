int dht_num = 4;
//------------------------------------------------------------------
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <SoftwareSerial.h>

SoftwareSerial BTserial(D2, D3); // RX | TX
/* Set these to your desired credentials. */
const char *ssid = "ASHA 7716";  //ENTER YOUR WIFI SETTINGS
const char *password = "854cK46]";

String temp = "";
String humid = "";
String str = "";

//=======================================================================
//                    Power on setup
//=======================================================================

void setup() {
  //delay(1000);
  Serial.begin(115200);
  BTserial.begin(9600);
  pinMode(D2, INPUT);
  pinMode(D3, OUTPUT);
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot

  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
  //esp();
}

//=======================================================================
//                    Main Program Loop
//=======================================================================
void loop() {
  char ch;
  while (BTserial.available()) {
    ch = BTserial.read();
    str.concat(ch);
  }
  if (str != "") {
    str.trim();
    //str=String(str);
    Serial.println(str);
    esp();
    //delay(2000);
  }
  str = "";
  /*str = "t=43.56&h=67.85";
    esp();*/

}
/*
  void extract() {
  for (int j = 0; j < str.length(); j++) {
    if (str.charAt(j) == '*') {
      int k = j;
      while (str.charAt(k) != '+')
        k++;
      j = k;
      temp = str.substring(j + 1, k - 1);
      while (str.charAt(k) != '*')
        k++;
      humid = str.substring(j + 1, k - 1);
    }
  }
  Serial.print("Temperature= ");
  Serial.println(temp);
  Serial.print("Humidity= ");
  Serial.println(humid);
  }
*/

void esp() {

  HTTPClient http;    //Declare object of class HTTPClient

  String  Link;
  //Link = "http://cuttackmaryadamahotsava.in/insert.php?";
  // Link = "http://54.167.134.26:8080/api/surveillance/getMethod?";
  Link = "http://192.168.43.196/chartjs/insert_mysql.php?";
  // Link = "http://18.188.209.233/chartjs/insert_mysql.php?";           //global
  // Link = "localhost/chartjs/insert_mysql.php?";
  Link += str;
  Link += "&t2=00&h2=00";
  Link += "&t3=00&h3=00";
  //Link += "&t4=00&h4=00";
  //Link += "&h = ";
  //Link += "humid";
  Serial.println(Link);
  http.begin(Link);     //Specify request destination

  int httpCode = http.GET();            //Send the request
  String payload = http.getString();    //Get the response payload

  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload

  http.end();  //Close connection

  delay(5000);  //GET Data at every 5 seconds
}
