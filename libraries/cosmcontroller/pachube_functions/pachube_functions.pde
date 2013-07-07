char pachube_data[70];

boolean found_status_200 = false;
boolean found_session_id = false;
boolean found_control_values = false;
char *found;
unsigned int successes = 0;
unsigned int failures = 0;
boolean ready_to_update = true;
boolean reading_pachube = false;

boolean request_pause = false;
boolean found_content = false;

unsigned long last_connect;

boolean found_d = false;
boolean found_a = false;

int content_length;

void pachube_in_out(){

  if (millis() < last_connect) last_connect = millis();

  if (request_pause){
    if ((millis() - last_connect) > interval){
      ready_to_update = true;
      reading_pachube = false;
      request_pause = false;
      found_status_200 = false;
      found_session_id = false;
      found_control_values = false;
      found_a = false;
      found_d = false;
      Serial.print("Ready to connect: ");
      Serial.println(millis());
    }
  }

  if (ready_to_update){
    Serial.println("Connecting...");
    if (localClient.connect()) {

      // here we assign comma-separated values to 'data', which will update Pachube datastreams
      // we use all the analog-in values, but could of course use anything else millis(), digital
      // inputs, etc. . i also like to keep track of successful and failed connection
      // attempts, sometimes useful for determining whether there are major problems.

      sprintf(pachube_data,"%d,%d,%d,%d,%d,%d,%d,%d",analogRead(0),analogRead(1),analogRead(2),analogRead(3),analogRead(4),analogRead(5), successes + 1, failures);
      content_length = strlen(pachube_data);

      Serial.println("GET request to retrieve");

      localClient.print("GET /api/");
      localClient.print(CONTROL_FEED_ID);
      localClient.print(".csv HTTP/1.1\nHost: pachube.com\nX-PachubeApiKey: ");
      localClient.print(PACHUBE_API_KEY);
      localClient.print("\nUser-Agent: Arduino (Pachube controller v1.1)");
      localClient.println("\n");

      Serial.println("finished GET now PUT, to update");

      localClient.print("PUT /api/");
      localClient.print(SHARE_FEED_ID);
      localClient.print(".csv HTTP/1.1\nHost: pachube.com\nX-PachubeApiKey: ");
      localClient.print(PACHUBE_API_KEY);

      localClient.print("\nUser-Agent: Arduino (Pachube controller v1.1)");
      localClient.print("\nContent-Type: text/csv\nContent-Length: ");
      localClient.print(content_length);
      localClient.print("\nConnection: close\n\n");
      localClient.print(pachube_data);



      localClient.print("\n");

      ready_to_update = false;
      reading_pachube = true;
      request_pause = false;
      interval = UPDATE_INTERVAL;

      Serial.print("finished PUT: ");
      Serial.println(millis());

    } 
    else {
      Serial.print("connection failed!");
      Serial.print(++failures);
      found_status_200 = false;
      found_session_id = false;
      found_control_values = false;
      ready_to_update = false;
      reading_pachube = false;
      request_pause = true;
      last_connect = millis();
      interval = RESET_INTERVAL;
      setupEthernet();
    }
  }

  while (reading_pachube){
    while (localClient.available()) {
      checkForResponse();
    } 

    if (!localClient.connected()) {
      disconnect_pachube();
    }
  } 
}

void disconnect_pachube(){
  Serial.println("disconnecting.\n=====\n\n");
  localClient.stop();
  ready_to_update = false;
  reading_pachube = false;
  request_pause = true;
  last_connect = millis();
  found_content = false;
  resetEthernetShield();
}



void checkForResponse(){  
  char c = localClient.read();
  Serial.print(c);
  buff[pointer] = c;
  if (pointer < 64) pointer++;
  if (c == '\n') {
    found = strstr(buff, "200 OK");
    if (found != 0){
      found_status_200 = true; 
      Serial.println("Status 200 found");
    }
    buff[pointer]=0;
    found_content = true;
    clean_buffer();    
  }

  if ((found_session_id) && (!found_control_values)){
    if (!found_d) {
      found = strstr(buff, "d=");
      if (found != 0){
        char dLine[9];
        for (int i = 0; i < 10; i++){
          dLine[i] = localClient.read();
        }
        found_d = true;
        Serial.println("d:");     
        Serial.println(dLine);
        for (int i = 0; i < 10; i++){
          digital_state[i] = (int) (dLine[i] - 48);
          Serial.print(digital_state[i]);
          Serial.print(", ");
        }
      }
    }

    if (!found_a) {
      found = strstr(buff, "a=");
      if (found != 0){
        char aLine[12];
        for (int i = 0; i < 13; i++){
          aLine[i] = localClient.read();
        }
        found_a = true;
        Serial.println("a:");     
        Serial.println(aLine);

        for (int j = 0; j < 4; j++){
          char *temp = "000";
          temp[0] = aLine[j*3];
          temp[1] = aLine[j*3 + 1];
          temp[2] = aLine[j*3 + 2];
          analog_state[j] = (int) atoi(temp);
          Serial.print(analog_state[j]);
          Serial.print(", ");
        }
        //clean_buffer();
      }
    }

    found = strstr(buff, "HTTP/1.1");
    if (found != 0){
      char csvLine[strlen(buff)-9];
      strncpy (csvLine,buff,strlen(buff)-9);
      found_control_values = true;

      Serial.print("\nsuccessful updates=");
      Serial.println(++successes);
    }
  }

  if (found_status_200){
    found = strstr(buff, "_id=");
    if (found != 0){
      clean_buffer();
      found_session_id = true; 
    }
  }
}
