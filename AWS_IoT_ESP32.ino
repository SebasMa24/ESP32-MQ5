#include <PubSubClient.h>

const int ledPIN = 26;

void setup() {
    Serial.begin(115200);
    pinMode(ledPIN , OUTPUT);
    connectAWS();
}

void loop() {
  client.loop();
  
  float gas_analog_value = analogRead(35);
  Serial.print(F("metrics: "));
  Serial.println(gas_analog_value);

  publishMessage(gas_analog_value);

  Serial.print("Mensaje: ");
  Serial.print(resultS);

  if(var == 0)
  {
    Serial.print("No hay riesgo apagando led.");
    digitalWrite(ledPIN ,LOW);
  }
  else if (var == 1)
  {
    Serial.print("Hay riesgo!!! enciendiendo led.");
    digitalWrite(ledPIN ,HIGH);
  }

  delay(1000);
}
