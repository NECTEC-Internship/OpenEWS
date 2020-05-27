#include <SPI.h>
#include <RH_RF95.h>
#include <ArduinoJson.h>

#define RFM95_CS 10
#define RFM95_INT 2
#define RF95_FREQ 915.0

#define INTERVAL 6e4
#define SENSOR_NODE_ID "CT-UNO-2"

unsigned long seq;
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

  json[SENSOR_NODE_ID]["sensor_node"]       = SENSOR_NODE_ID;
  json[SENSOR_NODE_ID]["soil_temperature"]  = analogRead(pin_temperature);
  json[SENSOR_NODE_ID]["soil_moisture"]     = analogRead(pin_moisture);
  json[SENSOR_NODE_ID]["seq"]               = seq++;
  
  serializeJson(json, (unsigned char*)payload, sizeof(payload));
  
  rf95.send(payload, sizeof(payload));
  rf95.waitPacketSent();

  Serial.println((char*)payload);
  delay(INTERVAL);
  //Serial.println(millis()-time);
}
