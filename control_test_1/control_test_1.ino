#include <WiFi.h>
#include <PubSubClient.h>

#define RELAY_1 21
#define RELAY_2 38
#define RELAY_3 40
#define TOUCH_1 2
#define TOUCH_2 7

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
// MQTT Broker Settings
const char *mqtt_broker = "broker.emqx.io";
const char *mqtt_topic = "emqx/esp32";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int mqtt_port = 1883;

const char *ssid = "Shot";
const char *password = "asdfghjkl#";

WiFiClient espClient;
PubSubClient mqtt_client(espClient);

long lastMsg = 0;
char msg[50];
int value = 0;
const int ledPin = 4;


// toggle button
int lastButtonOneState = 0;
int lastButtonTwoState = 0;
int ledOneState = 0;
int ledTwoState = 0;

unsigned long debounceDuration = 50;  // millis
unsigned long lastTimeButtonStateChanged = 0;


unsigned long ms_from_start = 0;

void initWiFi();
void connectToMQTT();
void mqttCallback(char *mqtt_topic, byte *payload, unsigned int length);



void setup() {
  Serial.begin(115200);

  pinMode(RELAY_1, OUTPUT);  // Relay 1
  pinMode(RELAY_2, OUTPUT);  // Relay 2
  pinMode(RELAY_3, OUTPUT);  // Relay 3
  pinMode(TOUCH_1, INPUT);   // Touch 1
  pinMode(TOUCH_2, INPUT);   // Touch 2

  pinMode(ledPin, OUTPUT);
  initWiFi();
  mqtt_client.setServer(mqtt_broker, mqtt_port);
  mqtt_client.setKeepAlive(60);
  mqtt_client.setCallback(mqttCallback);
  connectToMQTT();
  mqtt_client.subscribe("synergy/button/#");
}

void loop() {
  if (!mqtt_client.connected()) {
    connectToMQTT();
  }
  mqtt_client.loop();

  ms_from_start = millis();

  if (ms_from_start - lastTimeButtonStateChanged > debounceDuration) {
    int buttonOneState = digitalRead(TOUCH_1);

    if (buttonOneState != lastButtonOneState) {

      lastButtonOneState = buttonOneState;
      if (!buttonOneState) {
        ledOneState = (ledOneState == 1) ? LOW : HIGH;
        digitalWrite(RELAY_2, !ledOneState);
        Serial.print("button one ");
        Serial.print(ledOneState);
        Serial.println("");
        mqtt_client.publish("synergy/button/one", String(ledOneState).c_str());
      }
      // // Serial.print("button two current and last: ");
      // // Serial.print(buttonTwoState);
      // // Serial.print(" ");
      // // Serial.print(lastButtonTwoState);
      // // Serial.println(" ");

      // }
      //  if (buttonTwoState != lastButtonTwoState) {
      //   lastTimeButtonStateChanged = ms_from_start;
      //   lastButtonTwoState = buttonTwoState;
      //   if (!buttonTwoState) {
      //     ledTwoState = (ledTwoState == 1) ? LOW: HIGH;
      //     digitalWrite(RELAY_1, !ledTwoState);
      //     Serial.print("button two: ");
      //     Serial.print(ledTwoState);
      //     Serial.println("");
      //     mqtt_client.publish("synergy/button/two",String(ledTwoState).c_str());
      //   }
      // }
    }
  }
}


void initWiFi() {
  delay(10);
  // We start by connecting to a WiFi network
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
void connectToMQTT() {
  while (!mqtt_client.connected()) {
    String client_id = "esp32-client-" + String(WiFi.macAddress());
    Serial.printf("Connecting to MQTT Broker as %s.....\n", client_id.c_str());
    if (mqtt_client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Connected to MQTT broker");

      mqtt_client.publish(mqtt_topic, "Hi EMQX I'm ESP32 ^^");  // Publish message upon successful connection
    } else {
      Serial.print("Failed, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
void mqttCallback(char *mqtt_topic, byte *payload, unsigned int length) {
  String buttonState;
  Serial.print("Message received on mqtt_topic: ");
  Serial.println(mqtt_topic);
  Serial.print("Message: ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    buttonState += (char)payload[i];
  }
  Serial.println("\n-----------------------");

  if (String(mqtt_topic) == "synergy/button/one") {
    Serial.print("Changing button one output to ");
    if (buttonState == "1") {
      Serial.println("on");
      digitalWrite(RELAY_1, 1);
    } else if (buttonState == "0") {
      Serial.println("off");
      digitalWrite(RELAY_1, 0);
    }
  }
  if (String(mqtt_topic) == "synergy/button/two") {
    Serial.print("Changing button two output to ");
    if (buttonState == "1") {
      Serial.println("on");
      digitalWrite(RELAY_2, 1);
    } else if (buttonState == "0") {
      Serial.println("off");
      digitalWrite(RELAY_2, 0);
    }
  }
  if (String(mqtt_topic) == "synergy/button/three") {
    Serial.print("Changing button three output to ");
    if (buttonState == "1") {
      Serial.println("on");
      digitalWrite(RELAY_3, 1);
    } else if (buttonState == "0") {
      Serial.println("off");
      digitalWrite(RELAY_3, 0);
    }
  }
}
