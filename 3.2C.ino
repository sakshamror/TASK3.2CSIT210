// Include the necessary libraries for Wi-Fi, light sensor, and I2C communication
#include <WiFiNINA.h>
#include <BH1750.h>
#include <Wire.h>

// Define Wi-Fi credentials
char ssid[] = "Choudhary";
char pass[] = "12345678";

// Initialize Wi-Fi client and BH1750 light sensor instances
WiFiClient client;
BH1750 lightMeter;

// Define constants for IFTTT server communication
char HOST_NAME[] = "maker.ifttt.com";  // IFTTT server hostname
String PATH_NAME  = "/trigger/lightsensor/with/key/dcJl9S4EQ_As8TCSrLZ63CyopDnr7hj-9TlYNMhkag"; // Path for triggering the IFTTT event, including the unique key
String queryString = "?value1=0&value2=0"; // Query string parameters to send along with the request, representing sensor values


void setup() {
  // Initialize serial communication at 9600 bps
  Serial.begin(9600);
  // Print "START" to the Serial Monitor
  Serial.print("START");
  // Wait for the serial connection to be established
  while (!Serial);

  // Connect to Wi-Fi using provided credentials
  WiFi.begin(ssid, pass);
  // Initialize I2C communication
  Wire.begin();

  // Keep attempting to connect to the IFTTT server
  while (true) {
    if (client.connect(HOST_NAME, 80)) {
      Serial.println("Connected to server");
      break;
    } else {
      Serial.println("Connection failed");
    }
    delay(500);
  }
  // Initialize the BH1750 light sensor
  lightMeter.begin();
  // Print "Connected to server" to the Serial Monitor
  Serial.println("Connected to server");
}

void loop() {
  // Print "START" to the Serial Monitor
  Serial.print("START");
  // Read the current light level from the BH1750 sensor
  float lux = lightMeter.readLightLevel();

  // Update the query string with the current light level
  queryString = "?value1=" + String(lux);
  // Print the updated query string to the Serial Monitor
  Serial.println(queryString);

  // If the light level is above 600, send a GET request to the IFTTT server
  if (lux > 600) {
    client.println("GET " + PATH_NAME + queryString + " HTTP/1.1");
    client.println("Host: " + String(HOST_NAME));
    client.println("Connection: close");
    client.println();

    // While the client is connected, read and print the server's response
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.print(c);
      }
    }
    // Close the client connection
    client.stop();
    // Print a newline to the Serial Monitor
    Serial.println();
    // Print "Disconnected" to the Serial Monitor
    Serial.println("Disconnected");
  }

  // Reset the query string
  queryString = "";
  // Wait for 3 seconds before repeating the loop
  delay(3000);
}
