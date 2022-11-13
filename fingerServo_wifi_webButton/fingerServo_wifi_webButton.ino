#include <ArduinoHttpClient.h>
//#include <WiFi101.h>    // use this for MKR1000
#include <WiFiNINA.h>     // use this for MKR1010 or Nano 33 IoT
#include <WiFiUdp.h>
#include <RTCZero.h>
#include "arduino_secrets.h"
#include <Servo.h>
#include <string>

const char serverAddress[] = "10.18.143.97";  // server address
int port = 65156;                  // HTTP request port

WiFiClient wifi;                // WiFi instance
int wiFiStatus = WL_IDLE_STATUS;  // WiFi status

// HTTP client instance:
HttpClient http = HttpClient(wifi, serverAddress, port);
bool requesting = false;
Servo myservo;  // create servo object to control a servo
// a maximum of eight servo objects can be created

int pos = 0;    // variable to store the servo position

void moveFinger(int times) {
  while (times--)
  {
    for (pos = 0; pos < 90; pos += 1) // goes from 0 degrees to 180 degrees
    { // in steps of 1 degree
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(5);                       // waits 15ms for the servo to reach the position
    }
    for (pos = 90; pos >= 1; pos -= 1) // goes from 180 degrees to 0 degrees
    {
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(5);                       // waits 15ms for the servo to reach the position
    }
  }
}
void setup() {
  myservo.attach(2);  // attaches the servo on pin 2 to the servo object

  Serial.begin(9600);

  // try to connect to the network:
  while ( wiFiStatus != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(SECRET_SSID);       // print the network name (SSID);
    // Connect to WPA/WPA2 network:
    wiFiStatus = WiFi.begin(SECRET_SSID, SECRET_PASS);
  }

  // print the SSID of the network you're attached to:
  Serial.print("Connected to: ");
  Serial.println(WiFi.SSID());

  // print your IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  sendRequest();

}

void loop() {
  if (requesting) return;
  arduino::String res = sendRequest();
  Serial.println(res);
  if (res == arduino::String("true")) moveFinger(3);
}


// send an HTTP request:
arduino::String sendRequest() {
  requesting = true;
  String path = "/motor";
  http.get(path);
  arduino::String res =  http.responseBody();
  http.stop();
  delay(300); 
  requesting = false;   // set requesting false to prevent multiple reads
  return res;
}
