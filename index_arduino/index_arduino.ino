#include <ESP8266WiFi.h>
#include <WebSocketClient.h>

const int trigPin = D3;
const int echoPin = D4;

float duration, distance;

boolean handshakeFailed = 0;
String data = "";
char path[] = "/";   //identifier of this device
const char* ssid     = "MAHADEVAPURAM";
const char* password = "P@55word";
char* host = "127.0.1.1";  //replace this ip address with the ip address of your Node.Js server
const int espport = 3000;

WebSocketClient webSocketClient;
unsigned long previousMillis = 0;
unsigned long currentMillis;
unsigned long interval = 300; //interval for sending data to the websocket server in ms
// Use WiFiClient class to create TCP connections
WiFiClient client;
void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(115200);

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  delay(1000);

  wsconnect();
  //  wifi_set_sleep_type(LIGHT_SLEEP_T);
}
void loop() {
  Serial.println(WiFi.localIP());
  if (client.connected()) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distance = (duration * .0343) / 2;
    data += distance;
    webSocketClient.sendData(data);
    data = "";



    //    currentMillis = millis();
    //    webSocketClient.getData(data);
    //    if (data.length() > 0) {
    //      Serial.println(data);
    //      //*************send log data to server in certain interval************************************
    //      //currentMillis=millis();
    //      if (abs(currentMillis - previousMillis) >= interval) {
    //        previousMillis = currentMillis;
    //        data = (String) analogRead(A0); //read adc values, this will give random value, since no sensor is connected.
    //        //For this project we are pretending that these random values are sensor values
    //        webSocketClient.sendData(data);//send sensor data to websocket server
    //      }
    //    }
    //    else {
    //    }

  }
}
//*********************************************************************************************************************
//***************function definitions**********************************************************************************
void wsconnect() {
  // Connect to the websocket server
  Serial.println(host);
  Serial.println(espport);
  if (client.connect(host, espport)) {
    Serial.println("Connected");
  } else {
    Serial.println("Connection failed.");
    delay(1000);

    if (handshakeFailed) {
      handshakeFailed = 0;
      ESP.restart();
    }
    handshakeFailed = 1;
  }
  // Handshake with the server
  webSocketClient.path = path;
  webSocketClient.host = host;
  if (webSocketClient.handshake(client)) {
    Serial.println("Handshake successful");
  } else {

    Serial.println("Handshake failed.");
    delay(4000);

    if (handshakeFailed) {
      handshakeFailed = 0;
      ESP.restart();
    }
    handshakeFailed = 1;
  }
}
