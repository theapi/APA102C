
const char ws_html[] PROGMEM = R"=====(

<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
.slidecontainer {
    width: 100%;
}

.slider {
    -webkit-appearance: none;
    width: 100%;
    height: 15px;
    border-radius: 5px;
    background: #d3d3d3;
    outline: none;
    opacity: 0.7;
    -webkit-transition: .2s;
    transition: opacity .2s;
}

.slider:hover {
    opacity: 1;
}

.slider::-webkit-slider-thumb {
    -webkit-appearance: none;
    appearance: none;
    width: 50px;
    height: 50px;
    border-radius: 50%;
    background: #3c229b;
    cursor: pointer;
}

.slider::-moz-range-thumb {
    width: 50px;
    height: 50px;
    border-radius: 50%;
    background: #3c229b;
    cursor: pointer;
}
</style>
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

</script>
</head>
<body>
<h1>LED Control</h1>

<div class="slidecontainer">
<h4>Brightness:</h4>  
<input type="range" min="0" max="255" value="50" class="slider" id="brightness_range">
<p>Value: <span id="brightness_value"></span></p>
</div>


<script>
var slider = document.getElementById("brightness_range");
var output = document.getElementById("brightness_value");
output.innerHTML = slider.value;

slider.oninput = function() {
  output.innerHTML = this.value;

    var v = this.value;
  if (v.length < 2) {
    v = '0' + v; 
  }
   
  var val = '#'+v;    
  console.log('Send: ' + val); 
  connection.send(val);
}

</script>
</body>
</html>


)=====";

