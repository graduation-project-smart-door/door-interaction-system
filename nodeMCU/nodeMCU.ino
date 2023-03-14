#include <ESP8266WiFi.h> 
#include <ESP8266WebServer.h>

const char* ssid = "RT-GPON-7D0D";
const char* password = "p6keKG5P";

ESP8266WebServer server(80);   

void setup() 
{
  Serial.begin(115200);
  delay(100);

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

  server.enableCORS(true);
  server.begin();                                // запуск сервера
  Serial.println("HTTP server started");  

}

void loop() 
{
  server.handleClient();    // обработка входящих запросов

}

void doSometing() {
  Serial.println("I`m working now");
}

void handleGenericArgs1() //обработчик
{
  doSometing();
  
  //...

  server.send(200, "text/plain", "Hello World");    // ответить на HTTP запрос
}

void handleHealthcheck() //обработчик
{
  doSometing();
  
  //...

  server.send(200, "text/plain", "Hello World");    // ответить на HTTP запрос
}
