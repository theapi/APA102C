
const char ws_html[] PROGMEM = R"=====(

<html>
<head>
<script>
var connection = new WebSocket('ws://'+location.hostname+':81/', ['arduino']);

connection.onopen = function () {
  connection.send('Connect ' + new Date()); 
};

connection.onerror = function (error) {
    console.log('WebSocket Error ', error);
};

connection.onmessage = function (e) {
  console.log('Server: ', e.data);
};

function sendVal() {
  var v = parseInt(document.getElementById('v').value).toString(16);   

  if (v.length < 2) {
    v = '0' + v; 
  }
   
  var val = '#'+v;    
  console.log('Send: ' + val); 
  connection.send(val); 
}

</script>
</head>
<body>
<h1>LED Control</h1>
<div>
Brightness: <input id="v" type="range" min="0" max="255" step="1" onchange="sendVal();" />
</div>
</body>
</html>


)=====";

