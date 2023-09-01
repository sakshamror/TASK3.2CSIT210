#include <WiFiNINA.h>
#include <BH1750.h>
#include <Wire.h>

char ssid[] = "Choudhary";
char pass[] = "12345678";

WiFiClient client;
BH1750 lightMeter;

char   HOST_NAME[] = "maker.ifttt.com";
String PATH_NAME  = "/trigger/lightsensor/with/key/dcJl9S4EQ_As8TCSrLZ63CyopDnr7hj-9TlYNMhkag";
String queryString = "?value1=57&value2=25"; 
void setup() {
  Serial.begin(9600);
  Serial.print("START");
  while (!Serial);

  WiFi.begin(ssid, pass);
  Wire.begin();

  while (true) {
    if (client.connect(HOST_NAME, 80)) {
      Serial.println("Connected to server");
      break;
    } else {
      Serial.println("Connection failed");
    }
    delay(500);
  }
  lightMeter.begin();
  Serial.println("Connected to server");
}

void loop() {
  Serial.print("START");
  float lux = lightMeter.readLightLevel();

  queryString = "?value1=" + String(lux);
  Serial.println(queryString);

  if (lux > 600) {
    client.println("GET " + PATH_NAME + queryString + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println();

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.print(c);
      }
    }
    client.stop();
    Serial.println();
    Serial.println("Disconnected");
  }

  queryString = "";
  delay(3000);
}