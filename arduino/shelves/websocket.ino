
void websocketSetup() {
  // start webSocket server
  websocket.begin();
  websocket.onEvent(websocketEvent);
}

void websocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    if (type == WStype_TEXT) {
      //Serial.printf("[%u]: %s\n", num, payload);
      if (payload[0] == '#') {
         uint32_t rgb = (uint32_t) strtol((const char *) &payload[1], NULL, 16);
         int max = STRIP_MAX_VALUE * 3;
         int val = constrain(rgb, 0, max);
         stripSetAllPixels(val);
         stripShow();
      }
    }
}

