#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

#define DHTPIN D4
#define DHTTYPE DHT11

// LED & Buzzer Pins
#define GREEN_LED D5
#define YELLOW_LED D6
#define RED_LED D7
#define BUZZER D8

const char* ssid = "Pavithra Madhan";
const char* password = "veeralak";

DHT dht(DHTPIN, DHTTYPE);
ESP8266WebServer server(80);

String webpage() {

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  String weatherStatus = "Normal";

  // Turn OFF everything first
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BUZZER, LOW);

  // Weather Logic
  if (h > 80) {
    weatherStatus = "🌧 Rain Possible";
    digitalWrite(RED_LED, HIGH);
    digitalWrite(BUZZER, HIGH);
  }
  else if (t >= 25 && t <= 30) {
    weatherStatus = "☁ Cloudy";
    digitalWrite(YELLOW_LED, HIGH);
  }
  else if (t > 30 && h < 60) {
    weatherStatus = "☀ Sunny";
    digitalWrite(GREEN_LED, HIGH);
  }
  else {
    weatherStatus = "🌤 Normal";
  }

  String html = R"====(
<!DOCTYPE html>
<html>
<head>

<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">

<title>Smart Weather Monitoring System</title>

<style>

*{
margin:0;
padding:0;
box-sizing:border-box;
font-family:'Segoe UI',sans-serif;
}

body{
height:100vh;
display:flex;
justify-content:center;
align-items:center;
background:linear-gradient(135deg,#0d47a1,#1976d2,#42a5f5);
}

.container{
text-align:center;
width:90%;
max-width:900px;
}

.title{
color:white;
font-size:38px;
font-weight:bold;
margin-bottom:15px;
}

.datetime{
color:white;
font-size:22px;
margin-bottom:25px;
}

.cards{
display:flex;
justify-content:center;
gap:20px;
flex-wrap:wrap;
}

.card{
background:white;
padding:25px;
width:220px;
border-radius:20px;
box-shadow:0 10px 25px rgba(0,0,0,0.3);
transition:0.3s;
}

.card:hover{
transform:translateY(-5px);
}

.label{
font-size:22px;
font-weight:bold;
color:#1565c0;
margin-bottom:15px;
}

.value{
font-size:35px;
font-weight:bold;
color:#333;
}

.footer{
margin-top:25px;
color:white;
font-size:16px;
}

</style>

<meta http-equiv="refresh" content="5">

<script>
function updateTime(){
var now = new Date();
document.getElementById("datetime").innerHTML =
now.toLocaleString();
}
setInterval(updateTime,1000);
window.onload = updateTime;
</script>

</head>

<body>

<div class="container">

<div class="title">
🌡️ Smart Weather Monitoring System
</div>

<div id="datetime" class="datetime"></div>

<div class="cards">

<div class="card">
<div class="label">Temperature</div>
<div class="value">
)====";

  html += String(t,1);

  html += R"====(
°C
</div>
</div>

<div class="card">
<div class="label">Humidity</div>
<div class="value">
)====";

  html += String(h,1);

  html += R"====(
%
</div>
</div>

<div class="card">
<div class="label">Weather</div>
<div class="value" style="font-size:26px;">
)====";

  html += weatherStatus;

  html += R"====(
</div>
</div>

</div>

<div class="footer">
ESP8266 Local Web Server | Auto Refresh Every 5 Seconds
</div>

</div>

</body>
</html>
)====";

  return html;
}

void handleRoot() {
  server.send(200, "text/html", webpage());
}

void setup() {

  Serial.begin(115200);

  dht.begin();

  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BUZZER, LOW);

  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();

  Serial.println("Web Server Started");
}

void loop() {
  server.handleClient();
}
