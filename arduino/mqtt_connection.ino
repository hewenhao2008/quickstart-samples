#include <SPI.h>
#include <PubSubClient.h>
#include <Ethernet.h>
 
/*
 * A simple ThingFabric platform demo for Arduino.
 */
 
#define MQTT_SERVER "q.thingfabric.com"
#define USER ""               // Replace these values with those found in your ThingFabric Credentials.
#define PASSWORD ""
#define TOPIC "[domain]/test-stuff/test-thing"
#define PAYLOAD "{\"Hello\":\"World!\"}"

// MAC Address of Arduino Ethernet Sheild (on sticker on shield)
byte MAC_ADDRESS[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x31, 0xAF };

PubSubClient client;
EthernetClient ethClient;
char message_buff[100];

void setup()
{
   // init serial link for debugging
  Serial.begin(9600);
 
  if (Ethernet.begin(MAC_ADDRESS) == 0)
  {
      Serial.println("Failed to configure Ethernet using DHCP");
      return;
  }else{
    Serial.println("Serial connection successful.");
  }
 
  client = PubSubClient(MQTT_SERVER, 1883, callback, ethClient);
}

void loop()
{
  if (!client.connected())
  {
      // clientID, username, MD5 encoded password
      client.connect("arduino-mqtt", USER, PASSWORD);
      client.publish(TOPIC, PAYLOAD);
      client.subscribe(TOPIC);
  }
 
  // MQTT client loop processing
  client.loop();
}
 
// handles message arrived on subscribed topic(s)
void callback(char* topic, byte* payload, unsigned int length) {

  int i = 0;

  Serial.println("Message arrived:  topic: " + String(topic));
  Serial.println("Length: " + String(length,DEC));

  // create character buffer with ending null terminator (string)
  for(i=0; i<length; i++) {
    message_buff[i] = payload[i];
  }
  message_buff[i] = '\0';

  String msgString = String(message_buff);

  Serial.println("Payload: " + msgString);
}