void setupWiFi() {
  if (enableWiFi == 1) {
    AsyncWiFiManager wifiManager(&server, &dns);
    wifiManager.setTimeout(180);
    if (!wifiManager.autoConnect("MPPT")) { //192.168.4.1
    
      WiFi.softAP("MPPT-AP");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
    }
    Serial.println(WiFi.localIP());
    Serial.println("WiFi connected");
    if (!MDNS.begin("monitor")) {
        Serial.println("Error setting up MDNS responder!");
        while(1) {
            delay(1000);
        }
    }
    Serial.println("mDNS responder started");
    MDNS.addService("http", "tcp", 80);
    AsyncElegantOTA.begin(&server);  // Start AsyncElegantOTA
    server.begin();
    WIFI = 1;
  }
}

void Wireless_Telemetry() {
  /*
  ////////// WIFI TELEMETRY ////////// 
  if(enableWiFi==1){
    int LED1, LED2, LED3, LED4;                      //Declare LED brightness variable 
    if(buckEnable==1)      {LED1=200;}else{LED1=0;}  //BATTERY CHARGING STATUS
    if(batteryPercent>=99 ){LED2=200;}else{LED2=0;}  //FULL CHARGE STATUS
    if(batteryPercent<=10) {LED3=200;}else{LED3=0;}  //LOW BATTERY STATUS
    if(IUV==0)             {LED4=200;}else{LED4=0;}  //PV INPUT PRESENCE STATUS

    Blynk.run();  
    Blynk.virtualWrite(1,powerInput); 
    Blynk.virtualWrite(2,batteryPercent);
    Blynk.virtualWrite(3,voltageInput);    
    Blynk.virtualWrite(4,currentInput);   
    Blynk.virtualWrite(5,voltageOutput); 
    Blynk.virtualWrite(6,currentOutput); 
    Blynk.virtualWrite(7,temperature); 
    Blynk.virtualWrite(8,Wh/1000); 
    Blynk.virtualWrite(9,energySavings);       
    Blynk.virtualWrite(10,LED1);               //LED - Battery Charging Status
    Blynk.virtualWrite(11,LED2);               //LED - Full Battery Charge Status
    Blynk.virtualWrite(12,LED3);               //LED - Low Battery Charge Status
    Blynk.virtualWrite(13,LED4);               //LED - PV Harvesting
    
    Blynk.virtualWrite(14,voltageBatteryMin);  //Minimum Battery Voltage (Read & Write)
    Blynk.virtualWrite(15,voltageBatteryMax);  //Maximum Battery Voltage (Read & Write)
    Blynk.virtualWrite(16,currentCharging);    //Charging Current  (Read & Write)
    Blynk.virtualWrite(17,electricalPrice);    //Electrical Price  (Write)
  }
  ////////// WIFI TELEMETRY ////////// 
  if(enableBluetooth==1){
    //ADD BLUETOOTH CODE
  }
  */

  StaticJsonDocument<200> jsonDoc;
  jsonDoc["voltageInput"] = voltageInput;
  jsonDoc["currentInput"] = currentInput;
  jsonDoc["voltageOutput"] = voltageOutput;
  jsonDoc["currentOutput"] = currentOutput;
  jsonDoc["powerInput"] = powerInput;
  jsonDoc["voltageBatteryMin"] = voltageBatteryMin;
  jsonDoc["voltageBatteryMax"] = voltageBatteryMax;
  jsonDoc["currentCharging"] = currentCharging;
  jsonDoc["temperature"] = temperature;
  jsonDoc["Wh"] = Wh/1000;
  serializeJson(jsonDoc, jsonString);
}

void webmonitor(){
  server.on("/", HTTP_GET, handleRoot);

  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "application/json", jsonString);
  });


  server.on("/setting", HTTP_POST, [](AsyncWebServerRequest *request) {
    String d;
    if (request->hasParam("batterymin", true)) {
      d = request->getParam("batterymin", true)->value();
      voltageBatteryMin = d.toFloat();
      saveSettings();
    }
    if (request->hasParam("batterymax", true)) {
      d = request->getParam("batterymax", true)->value();
      voltageBatteryMax = d.toFloat();
      saveSettings();
    }
    if (request->hasParam("currentcharge", true)) {
      d = request->getParam("currentcharge", true)->value();
      currentCharging = d.toFloat();
      saveSettings();
    }
    if (request->hasParam("Restart", true)) {
      ESP.restart(); 
    }

    request->send(200, "text/plain", d);
  });

}

void handleRoot(AsyncWebServerRequest *request) {
  request->send_P(200, "text/html", reinterpret_cast<const char *>(FILEUP_HTML));
}
