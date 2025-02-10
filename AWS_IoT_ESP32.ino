#include <Arduino_BuiltIn.h>
#include "utils.h"
#include <PubSubClient.h>
#include <WiFi.h>
#include "time.h"

const int sensor101PIN = 36;
const int buzzerHallPIN = 33;
const int sensor102PIN = 39;
const int led101PIN = 14;
const int ledHallPIN = 25;
const int led102PIN = 26;

const char* ntpServer = "pool.ntp.org";

void setup() {
    Serial.begin(115200);
    pinMode(sensor101PIN , INPUT);
    pinMode(sensor102PIN , INPUT);
    pinMode(buzzerHallPIN , OUTPUT);
    pinMode(led101PIN , OUTPUT);
    pinMode(ledHallPIN , OUTPUT);
    pinMode(led102PIN , OUTPUT);
    
    connectAWS();
    configTime(0, 0, ntpServer);
}

void loop() {
  client.loop();
  float sensor101Value = analogRead(sensor101PIN);
  float sensor102Value = analogRead(sensor102PIN);
  publishMessage(sensor101Value,sensor102Value);
  delay(1000);
  Serial.print("Mensaje: ");
  Serial.println(resultS);
  bool autoMode = getBoolValue(resultS, "\"auto\"");
  Serial.println(autoMode);
  bool led101 = getBoolValue(resultS, "\"led101\"");
  Serial.println(led101);
  bool ledHall = getBoolValue(resultS, "\"ledHall\"");
  Serial.println(ledHall);
  bool led102 = getBoolValue(resultS, "\"led102\"");
  Serial.println(led102);
  bool buzzyHall = getBoolValue(resultS, "\"buzzyHall\"");
  Serial.println(buzzyHall);
  if(autoMode){
    if(sensor101Value>2500 && sensor102Value>2500){
      for(int i=0;i<5;i++){
        digitalWrite(led101PIN, HIGH);
        digitalWrite(led102PIN, HIGH);
        digitalWrite(ledHallPIN, HIGH);
        tone(buzzerHallPIN, 1000);
        delay(1000);
        digitalWrite(led101PIN, LOW);
        digitalWrite(led102PIN, LOW);
        digitalWrite(ledHallPIN, LOW);
        noTone(buzzerHallPIN);
      }
    }else if(sensor101Value>2500){
      for(int i=0;i<5;i++){
        digitalWrite(led101PIN, HIGH);
        delay(1000);
        digitalWrite(led101PIN, LOW);
    }
    }else if(sensor102Value>2500){
      for(int i=0;i<5;i++){
        digitalWrite(led102PIN, HIGH);
        delay(1000);
        digitalWrite(led102PIN, LOW);
      }
    }
  }
  for(int i=0;i<5;i++){
    if (led101) digitalWrite(led101PIN, HIGH);
    if (led102) digitalWrite(led102PIN, HIGH);
    if (ledHall) digitalWrite(ledHallPIN, HIGH);
    if (buzzyHall) tone(buzzerHallPIN, 1000);
    delay(1000);
    digitalWrite(led101PIN, LOW);
    digitalWrite(led102PIN, LOW);
    digitalWrite(ledHallPIN, LOW);
    noTone(buzzerHallPIN);
  }
}

