#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>

#define DHTPIN 2
#define DHT11 11

DHT dht(DHTPIN,DHT11);
float temp = 0;

const int button = 16;
const int led_1 = 5;
const int led_2 = 4;
const int led_3 = 0;
bool check = 0;

void setup() {
  Serial.begin(9600);
  pinMode(button,INPUT);
  pinMode(led_1,OUTPUT);  
  pinMode(led_2,OUTPUT);
  pinMode(led_3,OUTPUT);
  dht.begin();
}

void loop() {
       
   if ( digitalRead(button) == HIGH && !check ) {
      check = 1;
      Serial.println("Sistem Diaktifkan");
      delay(500);
     }
   else if( digitalRead(button) == HIGH && check){
      check = 0;
      Serial.println("Sistem Dimatikan");
      delay(500);
     }

   if(check){
      float temp = dht.readTemperature();
      
      if(!isnan(temp)){
        Serial.print("Suhu saat ini adalah ");
        Serial.print(temp);
        Serial.println("°C");
      }else{
        Serial.println("Suhu belum dibaca");
      }

      if( temp <= 35.0 ){
        digitalWrite(led_1,HIGH);
        digitalWrite(led_2,LOW);
        digitalWrite(led_3,LOW);
      }
      else if( temp >35.0 and temp <= 50 ){
        digitalWrite(led_1,HIGH);
        digitalWrite(led_2,HIGH);
        digitalWrite(led_3,LOW);
      }
      else if( temp > 50){
        digitalWrite(led_1,HIGH);
        digitalWrite(led_2,HIGH);
        digitalWrite(led_3,HIGH);
      }
      
   }else{
    digitalWrite(led_1,LOW);
    digitalWrite(led_2,LOW);
    digitalWrite(led_3,LOW);        
   }
}
