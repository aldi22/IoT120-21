#include <ESP8266WiFi.h>
#include <MQTT.h>
#include <DHT.h>
#include <Servo.h>

//Inisiasi State 
int button_state ;

//Inisiasi Wifi
const char* ssid = "AldinO";
const char* pass = "Canyouguessmine?#2204";

//Inisiasi Servo
#define servopin 0
Servo servo;
int derajat = 0;

//Inisiasi Sensor Suhu
#define DHTPIN 2
#define DHT11 11
DHT dht(DHTPIN,DHT11);
float tempValue = 0;
float treeshold = 60;

//Inisiasi LED
const int led_1 = 16;
const int led_2 = 5;
const int led_3 = 4;

//Inisiasi Client Wifi dan MQTT
WiFiClient net;
MQTTClient client;

float readSuhu(){
  float value = dht.readTemperature();
   if (isnan(value)) {
      Serial.println("Failed to read from DHT sensor!");
      return 0;
    }
   return value;
}

void changeAngle(){
    float ratio = tempValue/treeshold;
    derajat = ratio*180;
    servo.write(derajat);
}

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("esp8266", "b3a0d6f9", "e1a119896905e0a6")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");
  client.subscribe("/button");
}

void messageReceived(String &topic, String &payload) {

  Serial.println(topic + "and" + payload);
  if( topic == "/button" ) {
    if ( payload == "1" ) {
      button_state = payload.toInt();
    }else if ( payload == "0"){
      button_state = payload.toInt();
    }
  }
}

void setup() {
  Serial.begin(115200);
  servo.attach(servopin);
  dht.begin();
  pinMode(led_1,OUTPUT);  
  pinMode(led_2,OUTPUT);
  pinMode(led_3,OUTPUT);
  WiFi.begin(ssid, pass);
  client.begin("broker.shiftr.io", net);
  client.onMessage(messageReceived);
  connect();
}

void loop() {
  client.loop();
  delay(10);

  if (!client.connected()) {
    connect();
  }

  if(button_state == 1){
    tempValue = readSuhu();
    changeAngle();
    
    if( derajat > 0 and derajat <= 60 ){
        digitalWrite(led_1,HIGH);
        digitalWrite(led_2,LOW);
        digitalWrite(led_3,LOW);
        client.publish("DHT11",String(tempValue));
        client.publish("Servo",String(derajat));
      }
      else if( derajat > 60  and derajat <= 120 ){
        digitalWrite(led_1,HIGH);
        digitalWrite(led_2,HIGH);
        digitalWrite(led_3,LOW);
        client.publish("DHT11",String(tempValue));
        client.publish("Servo",String(derajat));
      }
      else if( derajat > 120 and derajat <= 180){
        digitalWrite(led_1,HIGH);
        digitalWrite(led_2,HIGH);
        digitalWrite(led_3,HIGH);
        client.publish("DHT11",String(tempValue));
        client.publish("Servo",String(derajat));
      }
  delay(1000);
    }else{
      digitalWrite(led_1,LOW);
      digitalWrite(led_2,LOW);
      digitalWrite(led_3,LOW);
      servo.write(0);
    }
    
}
  
