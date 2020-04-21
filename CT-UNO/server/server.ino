#include <SPI.h>
#include <RH_RF95.h>

#define RFM95_CS 10
#define RFM95_INT 2
#define RF95_FREQ 915.0

RH_RF95 rf95(RFM95_CS, RFM95_INT);

int led = 9;

void setup() 
{
  pinMode(led, OUTPUT);     
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
  if (rf95.available())
  {
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (rf95.recv(buf, &len))
    {
      digitalWrite(led, HIGH);
//      RH_RF95::printBuffer("request: ", buf, len);
//      Serial.print("payload: ");
      Serial.println((char*)buf);
//      Serial.print("\tRSSI: ");
//      Serial.println(rf95.lastRssi(), DEC);
      
//      uint8_t data[] = "And hello back to you";
//      rf95.send(data, sizeof(data));
//      rf95.waitPacketSent();
//      Serial.println("Sent a reply");
//       digitalWrite(led, LOW);
    }
    else
    {
      Serial.println("recv failed");
    }
  }
}
