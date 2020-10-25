const int led_1 = 16;
const int led_2 = 5;
const int led_3 = 4;
const int interval_1 = 10000 ;
const int interval_2 = 3000 ;
const int interval_3 = 7000 ;
unsigned long prev_mils= 0;
boolean turnGreen = 0;
boolean turnYellow_1 = 0;
boolean turnYellow_2 = 0;
boolean turnRed = 1;

void setup() {
  Serial.begin(9600);
  pinMode(led_1,OUTPUT);
  pinMode(led_2,OUTPUT);
  pinMode(led_3,OUTPUT);
}

void loop() {
   unsigned long current_mils = millis() ;
   Serial.println(current_mils);
   delay(1000);
    if ( current_mils - prev_mils >= interval_2 and turnRed) {
    prev_mils = current_mils;
    if ( digitalRead(led_1) == LOW) {
      Serial.println("LED 1");
      digitalWrite(led_3,LOW);
      digitalWrite(led_2,LOW);
      digitalWrite(led_1,HIGH);
      turnRed = 0;
      turnYellow_1 = 1;
    }
   }

   else if ( current_mils - prev_mils >= interval_1 and turnYellow_1) { 
    prev_mils = current_mils;

    if ( digitalRead(led_2) == LOW) {
      Serial.println("LED 2");
      digitalWrite(led_1,LOW);
      digitalWrite(led_2,HIGH);
      turnYellow_1 = 0;
      turnGreen = 1;
    }
   }
   
   else if (current_mils - prev_mils >= interval_2 and turnGreen) {
    prev_mils = current_mils;

    if ( digitalRead(led_3) == LOW) {
      Serial.println("LED 3");
      digitalWrite(led_2,LOW);
      digitalWrite(led_3,HIGH);
      turnGreen = 0;
      turnYellow_2 = 1;
    }
   }

   else if (current_mils - prev_mils >= interval_3 and turnYellow_2) {
    prev_mils = current_mils;

    if ( digitalRead(led_2) == LOW) {
      Serial.println("LED 2");
      digitalWrite(led_3,LOW);
      digitalWrite(led_2,HIGH);
      turnYellow_2 = 0;
      turnRed = 1;
    }
   }
      
}
