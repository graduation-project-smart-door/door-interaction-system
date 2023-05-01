#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>
#define RELAY_PIN D1
#define LOCK_DELAY 10



const char* ssid = "HONOR 70";
const char* password = "copperTube";
float pos;

enum DOOR_STATE {
  closed,
  closing,
  opening,
  opened
};

DOOR_STATE currentState  = closed;

ESP8266WebServer server(80);
Servo servo_1;

void setup()
{
  Serial.begin(115200);
  delay(100);
  servo_1.attach(4);
  pinMode(RELAY_PIN, OUTPUT);

  Serial.println("Connecting to ");
  Serial.println(ssid);

  // подключиться к вашей локальной wi-fi сети
  WiFi.begin(ssid, password);

  // проверить, подключился ли wi-fi модуль к wi-fi сети
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());

  server.on("/door/open", handleDoorStateChange);
  server.on("/door/healthcheck", handleHealthcheck);

  server.enableCORS(true);
  server.begin();                                // запуск сервера
  Serial.println("HTTP server started");

  if (currentState == closed) {
    deactivateLock();
    delay(500);
    for (pos = 90; pos <= 170; pos += 1) {
     servo_1.write(pos);
     Serial.println(pos);
     delay(28);
    }
    delay(2000);
    for (pos = 170; pos >= 45; pos -= 1) {
     servo_1.write(pos);
     Serial.println(pos);
     delay(28);
    }
    activateLock();
    delay(2000);
  }
}

void loop()
{
  server.handleClient();    // обработка входящих запросов
}

void handleDoorStateChange() //обработчик
{
  if (currentState == closed) {
    currentState = opening;
    deactivateLock();
    delay(300);
    openDoor();
    delay(300);
    server.send(200, "text/json", "{ status : Door is open }");
  }

  if (currentState == opening) {
    currentState = opened;
    delay(5000);
  }

  if (currentState == opened) {
    currentState = closing;
    closeDoor();
    delay(300);
    activateLock();
  }

  if (currentState == closing) {
    currentState = closed;
    delay(550);
  }
}

void handleHealthcheck() //обработчик
{
  server.send(200, "text/json", "{Door Opened}");
}

void openDoor() {
  for (pos = 45; pos <= 170; pos += 1) {
     servo_1.write(pos);
     Serial.println(pos);
     delay(28);
   }
}

void closeDoor() {
  for (pos = 170; pos >= 45; pos -= 1) {
     servo_1.write(pos);
     Serial.println(pos);
     delay(28);
   }
}

void activateLock() {
  digitalWrite(RELAY_PIN, HIGH);
}

void deactivateLock() {
  digitalWrite(RELAY_PIN, LOW);
}
