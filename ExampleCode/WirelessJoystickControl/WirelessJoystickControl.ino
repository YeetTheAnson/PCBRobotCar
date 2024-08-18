#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>

//NOTE TO REVIEWER: The javascript part of this html is written by chatgpt 😅
const char joystickPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32 Joystick</title>
  <style>
    body { display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0; background: #222; color: #fff; font-family: Arial, sans-serif; }
    #joystick { width: 300px; height: 300px; background: #555; border-radius: 50%; position: relative; }
    #handle { width: 100px; height: 100px; background: #f00; border-radius: 50%; position: absolute; top: 50%; left: 50%; transform: translate(-50%, -50%); touch-action: none; }
  </style>
</head>
<body>
  <div id="joystick">
    <div id="handle"></div>
  </div>

<script>
  const handle = document.getElementById('handle');
  const joystick = document.getElementById('joystick');
  const ws = new WebSocket('ws://' + window.location.hostname + '/ws');

  joystick.addEventListener('pointerdown', startDrag);
  joystick.addEventListener('pointerup', endDrag);
  joystick.addEventListener('pointermove', drag);

  let dragging = false;

  function startDrag(e) {
    dragging = true;
    drag(e);
  }

  function endDrag(e) {
    dragging = false;
    handle.style.transform = 'translate(-50%, -50%)';
    ws.send('0,0');
  }

  function drag(e) {
    if (!dragging) return;

    const rect = joystick.getBoundingClientRect();
    const offsetX = e.clientX - rect.left - rect.width / 2;
    const offsetY = e.clientY - rect.top - rect.height / 2;
    const maxRadius = rect.width / 2;

    const distance = Math.min(Math.sqrt(offsetX ** 2 + offsetY ** 2), maxRadius);
    const angle = Math.atan2(offsetY, offsetX);

    const x = Math.round(Math.cos(angle) * distance / maxRadius * 100);
    const y = Math.round(Math.sin(angle) * distance / maxRadius * 100);

    const handleX = Math.cos(angle) * distance;
    const handleY = Math.sin(angle) * distance;

    handle.style.transform = `translate(${handleX}px, ${handleY}px)`;

    ws.send(`${x},${y}`);
  }
</script>

</body>
</html>
)rawliteral";

// ap ssid and password
const char* ssid = "ESP32-Joystick";
const char* password = "123456789";
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

const int motorA1 = 5;  
const int motorA2 = 6; 
const int motorB1 = 7;  
const int motorB2 = 15;
const int motorENA = 18;
const int motorENB = 17; 

void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);

void setup() {
  Serial.begin(115200);
  
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);
  pinMode(motorENA, OUTPUT);
  pinMode(motorENB, OUTPUT);

  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, LOW);
  digitalWrite(motorB1, LOW);
  digitalWrite(motorB2, LOW);
  analogWrite(motorENA, 0);
  analogWrite(motorENB, 0);

  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP Address: ");
  Serial.println(IP);
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", joystickPage);
  });

  ws.onEvent(onWebSocketEvent);
  server.addHandler(&ws);
  server.begin();
}

void loop() {
  ws.cleanupClients();
}

void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, 
                      void *arg, uint8_t *data, size_t len) {
  if(type == WS_EVT_DATA) {
    String message = (char*)data;
    int separatorIndex = message.indexOf(',');
    int x = message.substring(0, separatorIndex).toInt();
    int y = message.substring(separatorIndex + 1).toInt();
    
    int speedA = map(abs(y), 0, 100, 0, 255);
    int speedB = map(abs(y), 0, 100, 0, 255);

    if (y > 0) { 
      digitalWrite(motorA1, HIGH);
      digitalWrite(motorA2, LOW);
      digitalWrite(motorB1, HIGH);
      digitalWrite(motorB2, LOW);
    } else if (y < 0) {
      digitalWrite(motorA1, LOW);
      digitalWrite(motorA2, HIGH);
      digitalWrite(motorB1, LOW);
      digitalWrite(motorB2, HIGH);
    } else {
      digitalWrite(motorA1, LOW);
      digitalWrite(motorA2, LOW);
      digitalWrite(motorB1, LOW);
      digitalWrite(motorB2, LOW);
    }

    if (x < 0) { 
      speedB = speedB - map(abs(x), 0, 100, 0, speedB);
    } else if (x > 0) { 
      speedA = speedA - map(abs(x), 0, 100, 0, speedA);
    }

    analogWrite(motorENA, speedA);
    analogWrite(motorENB, speedB);

    Serial.print("X: ");
    Serial.print(x);
    Serial.print(", Y: ");
    Serial.println(y);
  }
}
