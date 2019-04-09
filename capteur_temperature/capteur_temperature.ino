#include <math.h>
#define PIN_temperature   A0  // Broche capteur température

float B = 3975.0;
void setup()
{
  Serial.begin(9600);
  pinMode(PIN_temperature,INPUT);
}

void loop()
{
  int valeur_brute = analogRead(PIN_temperature);

  //float temperature_celcius = valeur_brute*5.0/10.24-50.0;
  //float temperature_celcius = 0.1463*valeur_brute - 51.713;
  float temperature_celcius = -(1/(log((1023-valeur_brute)/valeur_brute)/B+1/298.15)-273.15);


  Serial.println(temperature_celcius);
  //Serial.println(valeur_brute);
  delay(250);
}
