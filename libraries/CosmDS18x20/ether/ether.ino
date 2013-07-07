  /*
  
  These are mostly backend functions:
  DHCP client
  DNS resolver
  
  */
  
  
 
  #ifdef USE_DHCP
  void acquireIPAddress(){
    uint16_t dat_p;
    long lastDhcpRequest = millis();
    uint8_t dhcpState = 0;
    Serial.println("Sending initial DHCP Discover");
    es.ES_dhcp_start( buf, mymac, myip, mynetmask,gwip, dnsip, dhcpsvrip );
  
    while(1) {
      // handle ping and wait for a tcp packet
      int plen = es.ES_enc28j60PacketReceive(BUFFER_SIZE, buf);
  
      dat_p=es.ES_packetloop_icmp_tcp(buf,plen);
      //    dat_p=es.ES_packetloop_icmp_tcp(buf,es.ES_enc28j60PacketReceive(BUFFER_SIZE, buf));
      if(dat_p==0) {
        int retstat = es.ES_check_for_dhcp_answer( buf, plen);
        dhcpState = es.ES_dhcp_state();
        // we are idle here
        if( dhcpState != DHCP_STATE_OK ) {
          if (millis() > (lastDhcpRequest + 10000L) ){
            lastDhcpRequest = millis();
            // send dhcp
            Serial.println("Sending DHCP Discover");
            es.ES_dhcp_start( buf, mymac, myip, mynetmask,gwip, dnsip, dhcpsvrip );
          }
        } 
        else {
          return;        
        }
      }
    }   
  }
  #endif
  
  // hostName is an input parameter, ipAddress is an outputParame
  void resolveHost(char *hostName, uint8_t *ipAddress){
    es.ES_dnslkup_request(buf, (uint8_t*)hostName );
    while(1){
      int plen = es.ES_enc28j60PacketReceive(BUFFER_SIZE, buf);
      es.ES_packetloop_icmp_tcp(buf,plen);   
      if(es.ES_udp_client_check_for_dns_answer(buf, plen)) {
        uint8_t *websrvipptr = es.ES_dnslkup_getip();
        for(int on=0; on <4; on++ ) {
          ipAddress[on] = *websrvipptr++;
        }     
        return;
      }    
    }
  }  
  
  
  
  
  void sensor_feed_post_callback(uint8_t statuscode,uint16_t datapos){
    Serial.println();
    Serial.print("Status Code: ");
    Serial.println(statuscode, HEX);
    Serial.print("Datapos: ");
    Serial.println(datapos, DEC);
    Serial.println("PAYLOAD");
    for(int i = 0; i < 100; i++){
       Serial.print(byte(buf[i]));
    }
    
    Serial.println();
    Serial.println();  
  }
  
  // Output a ip address from buffer from startByte
  void printIP( uint8_t *buf ) {
    for( int i = 0; i < 4; i++ ) {
      Serial.print( buf[i], DEC );
      if( i<3 )
        Serial.print( "." );
    }
  }
  
  void printNetworkParameters(){
    Serial.print( "My IP: " );
    printIP( myip );
    Serial.println();
  
    Serial.print( "Netmask: " );
    printIP( mynetmask );
    Serial.println();
  
    Serial.print( "DNS IP: " );
    printIP( dnsip );
    Serial.println();
  
    Serial.print( "GW IP: " );
    printIP( gwip );
    Serial.println();  
  }
  
  
  
  
        
        
      
      
      // function to print a device address of temp sensors (obsolete?)
      void printAddress(DeviceAddress deviceAddress)
      {
        for (uint8_t i = 0; i < 8; i++)
        {
          if (deviceAddress[i] < 16) Serial.print("0");
          Serial.print(deviceAddress[i], HEX);
        }
      }
      
      //function to print the temperature for a device
      void printTemperature(DeviceAddress deviceAddress)
      {
       
        // method 2 - faster
        float tempC = sensors.getTempC(deviceAddress);
        Serial.print("Temp C: ");
        Serial.print(tempC);
      
      }
      
      
      
      

