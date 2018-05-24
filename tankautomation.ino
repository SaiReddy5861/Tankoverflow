#include <SPI.h>
#include <Ethernet.h>
#include "PubSubClient.h"


#define CLIENT_ID  "tankauto"
#define PUBLISH_DELAY   3000
#define PUB_TOPIC "sai"
#define SUB_TOPIC "saireddy"

char light;
uint8_t mac[6] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x06};
EthernetClient ethClient;
PubSubClient mqttClient;
int trigPin=7;
int echoPin=6;
int Distance=0;

int distance;
long duration;
char mesg[30];


void callback(char* topic, byte* payload, unsigned int length) {
  
  Serial.print("Message arrived [");
  
  Serial.print(topic);
  Serial.print("] ");
  
  String recv_mesg;
   
  for (int i=0;i<length;i++) {
    Serial.println((char)payload[i]);
    recv_mesg+=(char)payload[i];
  }
  
  if (recv_mesg == "21"){
      Serial.println("ON");
      digitalWrite(2,HIGH);
      delay(5000);
      digitalWrite(2,LOW);
      
    }
 if (recv_mesg == "31"){
      Serial.println("ON");
      digitalWrite(3,HIGH);
      delay(5000);
      digitalWrite(3,LOW);
    }
  }

  
void reconnect() {
  // Loop until we're reconnected
  
  while (!mqttClient.connected()) {
   
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect(CLIENT_ID)) {
      Serial.println("connected");
      mqttClient.subscribe(SUB_TOPIC);
    } 

  }
}
 
void setup()
{
  Serial.begin(9600);

   // initializing digital pin 2 as an output.
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
   
  while (!Serial){};
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Unable to configure Ethernet using DHCP");
    for (;;);
  }
  Serial.println(F("Ethernet configured via DHCP"));
  Serial.print("IP address: ");
  Serial.println(Ethernet.localIP());

  
  mqttClient.setClient(ethClient);
  mqttClient.setServer("m11.cloudmqtt.com",16665);
  boolean rc = mqttClient.connect("tankauto", "aymlefmh", "tD9wdMqziDQP");
  Serial.println("MQTT INITIALIZED");
  
  mqttClient.subscribe(SUB_TOPIC);
  
  mqttClient.setCallback(callback);
  
  
}
 
void loop()
{
  
  mqttClient.loop();

  if (!mqttClient.connected()) {
    reconnect();
  }
    digitalWrite(trigPin,HIGH);
    delayMicroseconds(20);
    digitalWrite(trigPin,LOW);
    
    duration=pulseIn(echoPin,HIGH);
    Serial.println(duration);
    distance=duration*0.034/2;
    
    
    String Distance = String(distance);
    String data;
    data += "distance"; data+= Distance;
    data.toCharArray( mesg, 100 );
    Serial.println(mesg);
    sendData();
    
    
    // Prints the distance on the Serial Monitor
    Serial.print("Distance: ");
    Serial.println(distance);
    
    delay(4000);
  
  }
  void sendData() {
  if(mqttClient.connect(CLIENT_ID)) {
      Serial.println("sending message");
      mqttClient.publish(PUB_TOPIC,mesg);  
     }
}

