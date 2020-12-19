#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>

//Inisiasi Sensor LDR
#define ldrPin A0
int ldrValue = 0;
float lux = 0;

//Inisiasi Sensor Suhu
#define DHTPIN 2
#define DHT11 11
DHT dht(DHTPIN,DHT11);
float tempValue = 0;

//Inisiasi Koneksi ke Local Server dan WiFi
char auth[] = "K3pSt9TyQjTxRxplubyCxhl4IjZqaOo7";
char server[] = "192.168.100.3";
char ssid[] = "cie MisQueen";
char pass[] = "wowsogood";

//Inisiasi Waktu
BlynkTimer timer;

//Inisiasi LED Suhu
WidgetLED led_suhu(V7);
int led_suhu_red = 5;

//Inisiasi LED LDR
WidgetLED led_ldr(V6);
int led_ldr_red = 4;

//Pembacaan Virtual Button
int booleanCheck;
BLYNK_WRITE(V8)
{
  booleanCheck = param.asInt();
  Serial.println(booleanCheck);
}

//Pembacaan Slider Suhu
float sliderValue_1;
BLYNK_WRITE(V5)
{
  sliderValue_1 = param.asFloat();
  Serial.println(sliderValue_1);
}

//Pembacaan Slider Cahaya
int sliderValue_2;
BLYNK_WRITE(V4)
{
  sliderValue_2 = param.asInt();
  sliderValue_2 = ( sliderValue_2*0.009768 )*4 + 10;
  Serial.println(sliderValue_2);
}

void sendSensor()
{
  if(booleanCheck == 1){
       tempValue = dht.readTemperature();
       ldrValue = analogRead(ldrPin)*4;
       lux = ( ldrValue*0.009768 ) + 10;
       
    if (isnan(tempValue)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }else{
      Blynk.virtualWrite(V3, tempValue);

      //Kontrol LED Suhu
      if( sliderValue_1 < tempValue ) {
        led_suhu.on();
        Blynk.setProperty(V7, "color", "#D54062");
        digitalWrite(led_suhu_red,HIGH);
      }else{
        led_suhu.on();
        Blynk.setProperty(V7, "color", "#91D18B");
        digitalWrite(led_suhu_red,LOW);
      }
      
    }
    
    Blynk.virtualWrite(V2, lux);

    //Kontrol LED LDR
      if( sliderValue_2 < lux ) {
        led_ldr.on();
        Blynk.setProperty(V6, "color", "#D54062");
        digitalWrite(led_ldr_red,HIGH);
      }else{
        led_ldr.on();
        Blynk.setProperty(V6, "color", "#91D18B");
        digitalWrite(led_ldr_red,LOW);
      }
      
  }
 
  Blynk.virtualWrite(V0, tempValue);
  Blynk.virtualWrite(V1, lux);
  
}

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass, server, 8080);
  dht.begin();
  pinMode(led_ldr_red,OUTPUT);
  pinMode(led_suhu_red,OUTPUT);
  timer.setInterval(1000L,sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();
}
