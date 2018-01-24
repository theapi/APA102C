
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
  uint8_t set = 0;
  for (uint8_t i = 0; i < webserver.args(); i++ ) {
    if (webserver.argName(i).indexOf('b') == 0) {
       String d = webserver.argName(i).substring(1);
       int requested_val = webserver.arg(i).toInt();
       int max = STRIP_MAX_VALUE * 3;
       int val = constrain(requested_val, 0, max);
       set = stripSetAllPixels(val);
       stripShow();
    }
  }
  
  // Respond with json
  String message = "{";
  message += "\"set\": \"" + String(set) + "\"";
  message += "}\n";
  
  webserver.send(200, "application/json", message);
}

