#include <ArduinoJson.h>
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
#include <DHT.h>
#include <Servo.h>

#define FIREBASE_HOST "https://smarthomeiot-8626f.firebaseio.com/"
#define FIREBASE_AUTH "H8QPqSiYnYllGEU9jrIQGI4vNJgYKGsxGdM5rYi5"
#define WIFI_SSID "AldinO"
#define WIFI_PASSWORD "Canyouguessmine?#2204"

//JSON
DynamicJsonDocument doc(200);

//State
const char* button_on = "1";

//Pembuatan variabel firebase
FirebaseData fbsdata;

//Path di Firebase
String pathSensor = "/sensor";
String pathIndikator = "/indikator";

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

void setup()
{
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  tempValue = readSuhu();
  dht.begin();
  servo.attach(servopin);

  pinMode(led_1,OUTPUT);  
  pinMode(led_2,OUTPUT);
  pinMode(led_3,OUTPUT);
}

void loop()
{

  Firebase.getJSON( fbsdata ,"/button");

  DeserializationError error = deserializeJson(doc, fbsdata.jsonString());
  
  //Inisiasi State 
  const char* button = doc["button"];

  Serial.println(strcmp( button, button_on ) == 0);
  
  if( strcmp( button, button_on ) == 0 ){
    
    if( tempValue != readSuhu() ) {
      tempValue = readSuhu();
      changeAngle();

      if( derajat > 0 and derajat <= 60 ){
        digitalWrite(led_1,HIGH);
        digitalWrite(led_2,LOW);
        digitalWrite(led_3,LOW);
        Firebase.setInt( fbsdata ,pathIndikator + "/led1", 1 );
        Firebase.setInt( fbsdata ,pathIndikator + "/led2", 0 );
        Firebase.setInt( fbsdata ,pathIndikator + "/led3", 0 );
      }
      else if( derajat > 60  and derajat <= 120 ){
        digitalWrite(led_1,HIGH);
        digitalWrite(led_2,HIGH);
        digitalWrite(led_3,LOW);
        Firebase.setInt( fbsdata ,pathIndikator + "/led1", 1 );
        Firebase.setInt( fbsdata ,pathIndikator + "/led2", 1 );
        Firebase.setInt( fbsdata ,pathIndikator + "/led3", 0 );
      }
      else if( derajat > 120 and derajat <= 180){
        digitalWrite(led_1,HIGH);
        digitalWrite(led_2,HIGH);
        digitalWrite(led_3,HIGH);
        Firebase.setInt( fbsdata ,pathIndikator + "/led1", 1 );
        Firebase.setInt( fbsdata ,pathIndikator + "/led2", 1 );
        Firebase.setInt( fbsdata ,pathIndikator + "/led3", 1 );
      }
      
      Firebase.setFloat( fbsdata ,pathSensor + "/dht11", tempValue );
      Firebase.setInt( fbsdata ,pathSensor + "/servo", derajat );
   
    } 
    
  }else{
    digitalWrite(led_1,LOW);
    digitalWrite(led_2,LOW);
    digitalWrite(led_3,LOW);
    
    Firebase.setInt( fbsdata ,pathIndikator + "/led1", 0 );
    Firebase.setInt( fbsdata ,pathIndikator + "/led2", 0 );
    Firebase.setInt( fbsdata ,pathIndikator + "/led3", 0 );
    Firebase.setInt( fbsdata ,pathSensor + "/dht11", 0 );
    Firebase.setInt( fbsdata ,pathSensor + "/servo", 0 );
    tempValue = 0 ;
    servo.write(0);  
  }
    
}
