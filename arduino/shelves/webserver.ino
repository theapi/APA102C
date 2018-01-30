
void webserverSetup() {
  webserver.on("/", webserverRoot);
  webserver.on("/ws", webserverWebsocket);
  webserver.begin();
}

void webserverWebsocket() {
  // send ws.html
  webserver.send(200, "text/html", ws_html);
}

void webserverRoot() {
  for (uint8_t i = 0; i < webserver.args(); i++ ) {
    if (webserver.argName(i).indexOf('b') == 0) {
       String d = webserver.argName(i).substring(1);
       int requested_val = webserver.arg(i).toInt();
       int val = constrain(requested_val, 0, STRIP_MAX_VALUE);
       brightness_changed = 1;
       brightness = val;
       //stripSetAllPixels(val);
       //stripShow();
    }
  }

  // Respond with json
  char buffer[64];
  snprintf(buffer, 63, "{\"brightness\":%d}", brightness);
  webserver.send(200, "application/json", buffer);
}

