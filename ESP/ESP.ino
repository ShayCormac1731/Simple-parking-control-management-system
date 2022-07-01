#include <SoftwareSerial.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char *ssid     = "TANG01";
const char *password = "0974129903";
const long utcOffsetInSeconds = 3600;
// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

SoftwareSerial mySerial(D4, D5); //RX, TX
// D4 RX - PA9
// D5 TX - PA10
// (Send and Receive)

String tmp = "";
void setup() {
  Serial.begin(115200);
  mySerial.begin(115200);

  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
    Serial.println( "wifi connected" );
  timeClient.begin();
  timeClient.setTimeOffset(+7*60*60);

}

void loop() {

   if(mySerial.available() > 1){//Read from  STM module and send to serial monitor
          String input = mySerial.readString();
          Serial.println( "From Stm32" );
          Serial.println(input);    
        }       
      int  gio,phut,giay;
        timeClient.update();
        gio = timeClient.getHours();
        phut = timeClient.getMinutes();
        giay = timeClient.getSeconds();
      
      String s = "";
      s += String(gio);
      s+= ":";
      s += String(phut);
      s+= ":";
      s += String(giay);
          
        if(tmp != s)
          mySerial.println(s),
          Serial.println(s),
          tmp = s;
do
      { char t = Serial.read();
      } while (Serial.available() > 0);
      delay(500);
}
