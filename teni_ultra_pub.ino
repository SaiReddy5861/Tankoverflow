#include <PubSubClient.h>
#include <Ethernet.h>
#include "PubSubClient.h"


#define CLIENT_ID       "teni"
#define PUBLISH_DELAY   3000
#define PUB_TOPIC "eswar"

uint8_t mac[6] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x06};
EthernetClient eClient;
PubSubClient mqtt;


//define pin numbers

int trigPin=4;
int echoPin=3;




// define variable names

int distance;
long duration;
char mesg[30];


void setup() {
  
  // put your setup code here, to run once:
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  Serial.begin(9600); 
 
while (!Serial) {};
  Serial.println("MQTT and Ultrasonic Sensor Arduino Demo");
  Serial.println();

  // setup ethernet communication using DHCP
if (Ethernet.begin(mac) == 0) {
    Serial.println("Unable to configure Ethernet using DHCP");
    for (;;);
  }

  Serial.println(F("Ethernet configured via DHCP"));
  Serial.print("IP address: ");
  Serial.println(Ethernet.localIP());
  Serial.println();


  mqtt.setClient(eClient);
  mqtt.setServer("m11.cloudmqtt.com",16665);
  boolean rc = mqtt.connect("tankauto", "aymlefmh", "tD9wdMqziDQP");
  //mqttClient.setCallback(callback);
  Serial.println("MQTT INITIALIZED");


}

void loop() {
  // put your main code here, to run repeatedly:

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

  
  
  if (mqtt.connect(CLIENT_ID)) {
    Serial.println("sending message");

    mqtt.publish(PUB_TOPIC,mesg);  
    
  
  }
}
 
  
