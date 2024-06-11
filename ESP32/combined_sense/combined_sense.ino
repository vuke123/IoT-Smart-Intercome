#include <WiFi.h>
#include <PubSubClient.h>
#include <Arduino.h>
#include <Ultrasonic.h>
 
const char* ssid = "Tina's Galaxy A54 5G";
const char* password = "hwvw7kxsaj6728x";
const char* mqtt_server = "192.168.223.218";

// Define pins for the ultrasonic sensor
#define TRIGGER_PIN  2
#define ECHO_PIN     3
#define LED_PIN      9
#define PIR_PIN      10

#define TOPIC_MOTION "home/back-door/sensor/motion"
#define TOPIC_DISTANCE "home/back-door/sensor/ultrasonic"
#define TOPIC_LED "home/back-door/actuator/buzzer"
 
int pirState = LOW;
int val=0;
 
// Define an object of the Ultrasonic class
Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);
 
WiFiClient espClient;
PubSubClient client(espClient);
 
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  payload[length] = '\0'; // Null-terminate the payload
  String message = String((char*)payload);
  Serial.println(message);
 
  if (String(topic) == TOPIC_LED) {
    if (message == "ON") {
      Serial.println("Turning LED on for 5 seconds");
      digitalWrite(LED_PIN, HIGH);
      delay(5000);
      digitalWrite(LED_PIN, LOW);
    }
  }
}
 
void setup_wifi() {
  Serial.begin(115200);
  delay(10);
  Serial.println("Scanning for available networks...");
 
  int n = WiFi.scanNetworks();
  Serial.println("Scan complete.");
  if (n == 0) {
    Serial.println("No networks found.");
  } else {
    Serial.print(n);
    Serial.println(" networks found:");
    for (int i = 0; i < n; ++i) {
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.println(" dBm)");
      delay(10);
    }
  }
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  int attempts = 0;
  const int maxAttempts = 40; // Try for 20 seconds
  while (WiFi.status() != WL_CONNECTED && attempts < maxAttempts) {
    delay(500);
    Serial.print(".");
    attempts++;
    // Print the Wi-Fi status code
    Serial.print(" Status: ");
    Serial.println(WiFi.status());
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("");
    Serial.println("Failed to connect to WiFi");
  }
}
 
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect with a client ID
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
      client.subscribe(TOPIC_LED); // Subscribe to the LED topic
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
  client.setCallback(callback);
  pinMode(LED_PIN, OUTPUT); // Sets the ledPin as an Output
  pinMode(PIR_PIN, INPUT);
  digitalWrite(LED_PIN, LOW);
}
 
void loop() {
   if (!client.connected()) {
    reconnect();
  }
  client.loop();
 
  val = digitalRead(PIR_PIN);  // read input value
  if (val == HIGH) {            // check if the input is HIGH
    if (pirState == LOW) {
      // we have just turned on
      Serial.println("Motion detected!");
      // We only want to print on the output change, not state
      pirState = HIGH;
 
      // Read the distance from the ultrasonic sensor in centimeters
      float distance = ultrasonic.distanceRead(CM);
 
      // Print the distance to the serial monitor
      Serial.print("Distance: ");
      Serial.print(distance);
      Serial.println(" cm");
 
      char distanceStr[10];
      dtostrf(distance, 1, 2, distanceStr);
 
      if (client.publish(TOPIC_MOTION, "Motion detected") && client.publish(TOPIC_DISTANCE, distanceStr)) {
	      Serial.println("Publish successful");
      } else {
	      Serial.println("Publish failed");
      }
   }
  } else {
    if (pirState == HIGH){
          // we have just turned of
          Serial.println("Motion ended!");
          // We only want to print on the output change, not state
          pirState = LOW;
  } 
   // Add a delay before the next reading
  delay(1000); // You can adjust the delay according to your requirements
}
}