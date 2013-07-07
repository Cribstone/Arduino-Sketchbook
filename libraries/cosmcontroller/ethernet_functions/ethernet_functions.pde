Client localClient(remoteServer, 80);
unsigned int interval;

char buff[64];
int pointer = 0;

void setupEthernet(){
  resetEthernetShield();
  Client remoteClient(255);
  delay(500);
  interval = UPDATE_INTERVAL;
  Serial.println("setup complete");
}

void clean_buffer() {
  pointer = 0;
  memset(buff,0,sizeof(buff)); 
}

void resetEthernetShield(){
  Serial.println("reset ethernet");
  Ethernet.begin(mac, ip);
}

