
void websocketSetup() {
  // start webSocket server
  websocket.begin();
  websocket.onEvent(websocketEvent);
}

void websocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case WStype_DISCONNECTED:
            break;
        case WStype_CONNECTED: {
            IPAddress ip = websocket.remoteIP(num);
            
            // send message to client
            // Respond with json
            char buffer[64];
            snprintf(buffer, 63, "{\"brightness\":%d}", brightness);
            websocket.sendTXT(num, buffer);
        }
            break;
        case WStype_TEXT:
          //Serial.printf("[%u]: %s\n", num, payload);
          if (payload[0] == '#') {
             int v = strtol((const char *) &payload[1], NULL, 16);
             int max = STRIP_MAX_VALUE * 3;
             int val = constrain(v, 0, max);

             stripSetAllPixels(val);
             stripShow();
          }
          break;
    }
}

