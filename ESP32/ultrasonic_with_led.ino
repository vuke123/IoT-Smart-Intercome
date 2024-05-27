#include <WiFi.h>
#include <PubSubClient.h>
#include <Arduino.h>
#include <Ultrasonic.h>

const char* ssid = "Luka";
const char* password = "23092001";
const char* mqtt_server = "192.168.0.20";

// Define pins for the ultrasonic sensor
#define TRIGGER_PIN  2
#define ECHO_PIN     3
#define LED_PIN      9

// Define an object of the Ultrasonic class
Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
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
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect with a client ID
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  pinMode(LED_PIN, OUTPUT); // Sets the ledPin as an Output
  digitalWrite(LED_PIN, LOW);
}

void loop() {
   if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Read the distance from the ultrasonic sensor in centimeters
  float distance = ultrasonic.distanceRead(CM);

  // Print the distance to the serial monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  char distanceStr[10];
  dtostrf(distance, 1, 2, distanceStr);

  if (client.publish("home/sensor/ultrasonic", distanceStr)) {
    Serial.println("Publish successful");
  } else {
    Serial.println("Publish failed");
  }

  if (distance > 0 && distance < 10) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  // Add a delay before the next reading
  delay(5000); // You can adjust the delay according to your requirements
}
