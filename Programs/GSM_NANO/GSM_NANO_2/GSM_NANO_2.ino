// libraries
#include <GSM.h>

// PIN Number
#define PINNUMBER ""

// APN data
#define GPRS_APN       "tre.it" // replace your GPRS APN
#define GPRS_LOGIN     ""    // replace with your GPRS login
#define GPRS_PASSWORD  "" // replace with your GPRS password

// initialize the library instance
GSMClient client;
GPRS gprs;
GSM gsmAccess;

// URL, path & port
char server[] = "mysite.altervista.org";
char path[] = "/write_data2.php";
int port = 80; // port 80 is the default for HTTP

//The variable that will have the data as a string to send it, i read sending an int with POST may cause trouble
//String strBoton = "";
void setup(){
  // initialize serial communications and wait for port to open:
  Serial.begin(9600);
 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  Serial.println("Starting Arduino Web Client.");
  // connection state
  boolean notConnected = true;

  // After starting the modem with GSM.begin()
  // attach the shield to the GPRS network with the APN, login and password
  while(notConnected)
  {
    if((gsmAccess.begin(PINNUMBER)==GSM_READY) &
      (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD)==GPRS_READY)){
      notConnected = false;
      } else
    {
      Serial.println("Not connected");
      delay(1000);
    }
  }

  Serial.println("connecting...");
}

void loop(){
 
  //convert the button state in string to send it
  //strBoton is the variable that the GET method will search for
   //strBoton="strBoton=77";
  //
 
  // if you get a connection, report back via serial:
  if (client.connect(server, port))
  { 
    Serial.println("conected and sending data");

    // Make a HTTP request:
 client.print("GET /write_data2.php?strBoton=66");
 client.println(" HTTP/1.1");
 client.println("Host: www.mysite.altervista.org");
 client.println("Connection: close");
    
  }
  else
  {
    // if you didn't get a connection to the server:
    Serial.println("the connection failed");
    Serial.print("");
  }
  delay(10000); // delay to send data every 10 seconds
 
}
