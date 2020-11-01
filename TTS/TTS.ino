#include <Blynk.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "7svfpxLu0nt_Pfn6JbdNqdqvD5qJgdPk";
char ssid[] = "AldinO";
char pass[] = "Canyouguessmine?#2204";

void setup()
{
  // Debug console
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
}
