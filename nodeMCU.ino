#include <ESP8266WiFi.h> 
#include <ESP8266WebServer.h>

const char* ssid = "Redmi Note 7";
const char* password = "copperTube";

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

  server.on("/test", handleGenericArgs);  // привязать функцию обработчика к URL-пути

  server.enableCORS(true);
  server.begin();                                // запуск сервера
  Serial.println("HTTP server started");  

}

void loop() 
{
  server.handleClient();    // обработка входящих запросов

}

void handleGenericArgs() //обработчик
{
  String message = "Number of args received:";
  message += server.args();      // получить количество параметров
  message += "\n";               // переход на новую строку

  for (int i = 0; i < server.args(); i++) 
  {
    message += "Arg nº" + (String)i + " –> "; // добавить текущее значение счетчика
    message += server.argName(i) + ": ";      // получить имя параметра
    message += server.arg(i) + "\n";          // получить значение параметра    
  } 
  Serial.println(server.args());

  server.send(200, "text/plain", message);    // ответить на HTTP запрос
}
