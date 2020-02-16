#include <WiFi.h>
#include <PubSubClient.h>
#include <TFT_eSPI.h>
#include <SPI.h>
 
const char* ssid = "mySSID";
const char* password =  "myPass";
const char* mqttServer = "192.168.1.79";
const int mqttPort = 1883;
String mqttTopic = "domain/object/topic";
 
TFT_eSPI tft = TFT_eSPI(135, 240); // Invoke custom library

void println(String msg){
  Serial.println(msg);
  tft.println(msg);
}

// void fillScreen(String msg){
//   switch (msg){
//     case "":
//       tft.fillScreen(TFT_GREEN);
//     break;
//     case "":
//       tft.fillScreen(TFT_YELLOW);
//     break;
//     case "":
//       tft.fillScreen(TFT_RED);
//     break;
//     default:
//       tft.fillScreen(TFT_YELLOW);
//       }
// }


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String msg;
  for (int i=0;i<length;i++) {

    Serial.print((char)payload[i]);
    msg += (char)payload[i];
  }
  Serial.println();
  tft.fillScreen(TFT_YELLOW);
  tft.setTextColor(TFT_WHITE);
  tft.setTextDatum(MC_DATUM);
  tft.drawString(msg,  tft.width() / 2, tft.height() / 2 );
}


WiFiClient espClient;
PubSubClient client(espClient);

 
void setup() {
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_GREEN);
  tft.setCursor(0, 0);
  tft.setTextDatum(MC_DATUM);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    println("Connecting to WiFi..");
  }
 
  println("Connected to the WiFi network");
 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  while (!client.connected()) {
    println("Connecting to MQTT...");
 
    if (client.connect("ESP32Client")) {
 
      println("connected");
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
  client.subscribe("stat/thermometre_salon/temperature");
}
 
void loop() {
  client.loop();
}