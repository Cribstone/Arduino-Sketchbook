#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "ssid";
const char* password = "password";
const char* mqtt_server = "192.168.1.1";

char* deviceId  = "buttonswitch";
char* stateTopic = "home/bedroom/buttonswitch";
char* commandTopic = "home/bedroom/buttonswitch/set";
char buf[4];
int updateInterval = 2000;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg0[20];

const int buttonPin1 = 0;
const int ledPin1 = 2;

int buttonState1 = 0;
int lastButtonState1 = 0;
int ledState1 = 0;

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

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived: ");
  Serial.print(topic);
  Serial.print(" ");

  String recieveTopic = (char*)topic;

  Serial.print("Payload: ");
  Serial.write(payload, length);
  Serial.println();

  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;
  String message(p);

  if (recieveTopic == (char*)commandTopic) {
    if (message.equals("1")) {
      digitalWrite(4, HIGH);
    } else {
      digitalWrite(4, LOW);
    }
  } else {
    Serial.print("Topic unknown");
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(deviceId)) {
      Serial.println("connected");
      client.publish(stateTopic, "ready");
      client.subscribe(commandTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  pinMode(buttonPin1, INPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(4, OUTPUT);


  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  buttonState1 = digitalRead(buttonPin1);
  if (buttonState1 != lastButtonState1) {
    if (buttonState1 == HIGH) {
      if (ledState1 == HIGH) {
        ledState1 = LOW;
      } else {
        ledState1 = HIGH;
      }
    }
    lastButtonState1 = buttonState1;
  }
  digitalWrite(ledPin1, ledState1);
  delay(20);

  long now = millis();
  if (now - lastMsg > updateInterval) {
    lastMsg = now;

    snprintf(msg0, 75, "%ld", ledState1);
    client.publish(stateTopic, msg0);
  }
}
