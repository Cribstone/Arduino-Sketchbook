#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include "MFRC522.h"

/* wiring the MFRC522 to ESP8266 (ESP-12)
RST     = GPIO4
SDA(SS) = GPIO2 
MOSI    = GPIO13
MISO    = GPIO12
SCK     = GPIO14
GND     = GND
3.3V    = 3.3V
*/

#define RST_PIN  4 // RST-PIN GPIO4 
#define SS_PIN  2  // SDA-PIN GPIO2 

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

// Wifi Connection details
const char* ssid = "SSID";
const char* password = "PASSWORD";

// MQTT Server address
const char* mqtt_server = "SERVER_IP";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int state;



void setup() {
  Serial.begin(9600);

  SPI.begin();           // Init SPI bus
  mfrc522.PCD_Init();    // Init MFRC522
    
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}


// Connect to Wifi
void setup_wifi() {

  pinMode(5, OUTPUT); // Red LED
  pinMode(5, HIGH); 
  pinMode(0, OUTPUT); // Greem :LED
  pinMode(0, HIGH);

  delay(2);
  // We start by connecting to a WiFi network
  Serial.println();
  
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


// Check for incoming messages
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }


  // Alarm is on
  if ((char)payload[1] == 'n') {
    Serial.println();
    Serial.print("Alarm is on");
    Serial.println();
    state=1;
    on_red_led();    
  } 
  
  // Alarm is off
  if ((char)payload[1] == 'f') {
    Serial.println();
    Serial.print("Alarm is off");
    Serial.println();
    state=0;
    on_green_led();    
  } 
  
  // Alarm is arming
  if ((char)payload[2] == 'm') {
    Serial.println();
    Serial.print("Alarm is arming");
    Serial.println();
    state=2;
    flash_red_led();    
  }   

  // Alarm is disarming
  if ((char)payload[2] == 's') {
    Serial.println();
    Serial.print("Alarm is disarming");
    Serial.println();
    state=2;
    flash_green_led();    
  } 

  // Error
  if ((char)payload[3] == 'o') {
    Serial.println();
    Serial.print("Error wrong token");
    Serial.println();
    state=3;
    blink_red_led();    
  }

}


// Reconnect to wifi if connection lost
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("/rfid/state", "connected");
      // ... and resubscribe
      client.subscribe("/alarm/state");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


// Main functions
void loop() {

  if (!client.connected()) {
    reconnect();
  }

  client.loop();
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    delay(50);
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    delay(50);
    return;
  }

  // if still arming or disarming then return
  if (state == 2) { 
  return; 
  }

  // if error delay
  if (state == 3) { 
  delay(2000); 
  }
  
  // Show some details of the PICC (that is: the tag/card)
  Serial.print(F("Card UID:"));
  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.println();

  // Send data to MQTT
  String rfidUid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    rfidUid += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
    rfidUid += String(mfrc522.uid.uidByte[i], HEX);
  }
  const char* id = rfidUid.c_str();

  // Blink led once
  if (state == 1) { 
  beep_red_led(); 
  }
  if (state != 1) { 
  beep_green_led();  
  }
  
  client.publish("/rfid/user", id);
  //delay(2000);
}


// LED Loop
void blink_red_led(){
  Serial.println("Blinking Red");
  digitalWrite(0, LOW);
  // Flash 6 times:
  for(int i = 0; i < 5; i++)
  {
  digitalWrite(5, HIGH);
  delay(200);
}}

void flash_green_led(){
  Serial.println("Flashing Green LED");
  digitalWrite(5, LOW);
  // Flash 15 times:
  for(int i = 0; i < 14; i++)
  {
  digitalWrite(0, HIGH);
  delay(1000);
  digitalWrite(0, LOW);
  delay(500);
}}


void beep_red_led(){
  Serial.println("Beep Red LED");
  digitalWrite(5, LOW);
  delay(500);
  digitalWrite(5, HIGH);
}

void beep_green_led(){
  Serial.println("Beep Red LED");
  digitalWrite(0, LOW);
  delay(500);
  digitalWrite(0, HIGH);
}

void flash_red_led(){
  Serial.println("Flashing Red LED");
  digitalWrite(0, LOW);
  // Flash 20 times:
  for(int i = 0; i < 19; i++)
  {
  digitalWrite(5, HIGH);
  delay(1000);
  digitalWrite(5, LOW);
  delay(500);
}}


void on_green_led(){
  Serial.println("Turning on green led");
  digitalWrite(5, LOW);
  digitalWrite(0, HIGH);
}

void on_red_led(){
  Serial.println("Turning on red led");
  digitalWrite(0, LOW);
  digitalWrite(5, HIGH);
}


// Helper routine to dump a byte array as hex values to Serial
void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}
