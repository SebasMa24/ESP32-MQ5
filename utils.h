#include <Arduino_BuiltIn.h>

#include "certs.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include "time.h"
#include <string>

#define AWS_IOT_PUBLISH_TOPIC   "esp32/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"

WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);
String resultS = "";
int var = 0;

void messageHandler(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensaje recibido [");
  Serial.print(topic);
  Serial.print("] ");

  char payload_string[length + 1];
  
  int resultI;

  memcpy(payload_string, payload, length);
  payload_string[length] = '\0';
  resultI = atoi(payload_string);
  
  var = resultI;
  Serial.print("Mensaje en formato int");
  Serial.print(var);
  resultS = "";
  
  for (int i=0;i<length;i++) {
    resultS= resultS + (char)payload[i];
  }
  Serial.println();
}


void connectAWS() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
 
  Serial.println("Connecting to Wi-Fi");
 
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
 
  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);
 
  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.setServer(AWS_IOT_ENDPOINT, 8883);
 
  // Create a message handler
  client.setCallback(messageHandler);
 
  Serial.println("Connecting to AWS IOT");
 
  while (!client.connect(THINGNAME)) {
    Serial.print(".");
    delay(100);
  }
 
  if (!client.connected()) {
    Serial.println("AWS IoT Timeout!");
    return;
  }
 
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
 
  Serial.println("AWS IoT Connected!");
}

void publishMessage(float sensor101Value, float sensor102Value) {
  StaticJsonDocument<512> doc;
  time_t tiempo = time(NULL);
  // Datos de Asset y Property
  doc["assetId"] = "e6b980cc-84c3-4875-a586-4560dcf620c1";
  doc["propertyId"] = "adc78532-4ed8-4ace-9f53-83a0f72423cd";

  // Metrics
  JsonObject metrics = doc.createNestedObject("metrics");
  metrics["sensor101Value"] = sensor101Value;
  metrics["sensor102Value"] = sensor102Value;

  
  
  // Crear el objeto "timestamp"
  JsonObject timestamp = doc.createNestedObject("timestamp");
  timestamp["timeInSeconds"] = tiempo;
  timestamp["offsetInNanos"] = 0;

  
  
  // Serializar el JSON a buffer
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);

  // Publicar el mensaje en el tópico
  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}

bool getBoolValue(String json, String key) {
    int pos = json.indexOf(key);
    if (pos != -1) {
        int valuePos = json.indexOf(":", pos) + 1;
        while (valuePos < json.length() && (json[valuePos] == ' ' || json[valuePos] == '\"')) {
            valuePos++; // Saltar espacios y comillas
        }
        return json.substring(valuePos, valuePos + 4) == "true"; // Comparar con "true"
    }
    return false; // Si no se encuentra la clave, retornar false
}
