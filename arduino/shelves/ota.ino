

void otaSetup() {
  ArduinoOTA.setHostname("shelves");
  
  ArduinoOTA.onStart([]() {
    // Led ON
    digitalWrite(DEBUG_LED, LOW);
    // Turn off the strip
    stripSetAllPixels(0);
    stripShow();
  });
  ArduinoOTA.onEnd([]() {
    // Led Off
    digitalWrite(DEBUG_LED, HIGH);
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    //Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    ESP.restart();
  });
  
  ArduinoOTA.begin();
}

