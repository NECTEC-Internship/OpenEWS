#include <SPI.h>
#include <RH_RF95.h>
#include <ArduinoJson.h>

#define RFM95_CS 10
#define RFM95_INT 2
#define RF95_FREQ 915.0

#define INTERVAL 1000
#define SENSOR_ID "CT-UNO-2"

unsigned long time;

RH_RF95 rf95(RFM95_CS, RFM95_INT);

int pin_temperature = A0;
int pin_moisture = A3;

void setup() 
{
  Serial.begin(9600);
  while (!Serial) ;
  if (!rf95.init()){
    Serial.println("init failed");
    while (1);
  }
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("set frequency failed");
    while (1);
  }
  rf95.setTxPower(23, false);
}

void loop()
{
  time = millis();
  uint8_t payload[128];
  DynamicJsonDocument json(sizeof(payload));

  json["sensor node"]       = SENSOR_ID;
  json["temperature"]       = analogRead(pin_temperature);
  json["soil moisture"]     = analogRead(pin_moisture);
    
  serializeJson(json, (unsigned char*)payload, sizeof(payload));
  
  rf95.send(payload, sizeof(payload));
  rf95.waitPacketSent();

  Serial.println((char*)payload);
  delay(INTERVAL*random(1,10));
  //Serial.println(millis()-time);
}
