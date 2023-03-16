#include <ESP8266WiFi.h> 
#include <ESP8266WebServer.h>
#include <Servo.h>

Servo door_servo;

#define MAX_SERVO 160
#define DEFAULT_SERVO 0
const char* ssid = "DIR-2150-C2AE";
const char* password = "34583500";
ESP8266WebServer server(80);   

void setup() 
{
  Serial.begin(9600);  
  delay(100);

  door_servo.attach(5); //подключение сервопривода

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

  server.on("/test", handleGenericArgs1);  // привязать функцию обработчика к URL-пути
  server.on("/healthcheck", handleHealthcheck);  // привязать функцию обработчика к URL-пути

//  server.enableCORS(true);
  server.sendHeader("Access-Control-Allow-Origin","*");
  server.begin();                                // запуск сервера
  Serial.println("HTTP server started"); 
}

void loop() 
{
  server.handleClient();    // обработка входящих запросов
}

void doorOpen() {
  Serial.println("Open process...");
  int servo_now = DEFAULT_SERVO;
  while(servo_now < MAX_SERVO) {
    servo_now++;
    Serial.println(servo_now);
    door_servo.write(servo_now);
    delay(15);
  }
  Serial.println("Close process...");
  delay(3000);  
  if(servo_now >= 160) {
    while(servo_now >= DEFAULT_SERVO) {
    servo_now--;
    Serial.println(servo_now);
    door_servo.write(servo_now);
    delay(15);
  }
  }
  Serial.println("Door close.");
}

void handleGenericArgs1() //обработчик
{
  doorOpen();
  server.send(200, "text/plain", "Hello World");    // ответить на HTTP запрос
}

void handleHealthcheck() //обработчик
{
  //...

  server.send(200, "text/plain", "Hello World");    // ответить на HTTP запрос
}
