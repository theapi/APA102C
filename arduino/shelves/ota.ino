

void otaSetup() {
  ArduinoOTA.setHostname("shelves");
  
  ArduinoOTA.onStart([]() {
    // Led OFF
    digitalWrite(DEBUG_LED, HIGH);
    // Turn off the strip
    stripSetAllPixels(0);
    stripShow();
  });
  ArduinoOTA.onEnd([]() {
    // Led On
    digitalWrite(DEBUG_LED, LOW);
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    //Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    ESP.restart();
  });
  
  ArduinoOTA.begin();
}

