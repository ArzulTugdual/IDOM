#include <PinChangeInt.h>
#include <PinChangeIntConfig.h>
#include <EEPROM.h>
#define _NAMIKI_MOTOR   //for Namiki 22CL-103501PG80:1
#include <fuzzy_table.h>
#include <PID_Beta6.h>
#include <MotorWheel.h>
#include <Omni4WD.h>

const byte PIN_ultrason = 2;     // Broche capteur ultrason (envoie/réception)

irqISR(irq1,isr1);
MotorWheel wheel1(3,2,4,5,&irq1);

irqISR(irq2,isr2);
MotorWheel wheel2(11,12,14,15,&irq2);

irqISR(irq3,isr3);
MotorWheel wheel3(9,8,16,17,&irq3);

irqISR(irq4,isr4);
MotorWheel wheel4(10,7,18,19,&irq4);


Omni4WD Omni(&wheel1,&wheel2,&wheel3,&wheel4);

/*variables*/
const int speedMMPS=40;
const int tps_arret=3000;
const boolean debug=false;
const int tps_etat=500;
//seuil distance (mm)
const float seuil=50;

boolean etat = true;

void setup() {
  TCCR1B=TCCR1B&0xf8|0x01;    // Pin9,Pin10 PWM 31250Hz
  TCCR2B=TCCR2B&0xf8|0x01;    // Pin3,Pin11 PWM 31250Hz

  Omni.PIDEnable(0.31,0.01,0,10);

  //initialise le robot en avant
  //av(40);
  
  //capteur ultrason
  Serial.begin(9600);
  pinMode(PIN_ultrason, OUTPUT);
  digitalWrite(PIN_ultrason, LOW); // La broche doit être à LOW au repos
}

void loop() {
  if(etat)
  {
    av(40);
    etat=false;
  }
  float distance_mur = mesure_distance();
  if(distance_mur != 0 &&  distance_mur < seuil) stop_motors();
  else if(distance_mur > seuil || distance_mur == 0) av(40);
}

//mesure la distance entre le robot et le prochain obstacle devant
float mesure_distance()
{
  //capteur parallax (3 broches)
  /*création des variables de résultat*/
  float measure, distance_mm;
  //float c = 20.05*sqrt(temp)*pow(10,-3);   //vitesse du son dans l'air en fonction de la température (en mm/microsec)
  float c = 0.34;
  
  pinMode(PIN_ultrason, OUTPUT);       //broche en sortie pour envoyer l'impulsion
  digitalWrite(PIN_ultrason, LOW);     // La broche doit être à LOW au départ
  
  /*impulsion de 10 microsecondes*/
  digitalWrite(PIN_ultrason, HIGH);      
  delayMicroseconds(10);
  digitalWrite(PIN_ultrason, LOW);       //envoi de l'onde sonore ___---___

  pinMode(PIN_ultrason, INPUT);          //broche en entrée en attente de l'impulsion
  
  measure = pulseIn(PIN_ultrason, HIGH, 25000UL);  //attente du signal retour puis calcul du temps de réponse en microsec
  return measure / 2.0 * c;               //MESURE en mm
}
void av(int rapportPWM)
{
  wheel1.advancePWM(rapportPWM);
  wheel2.advancePWM(rapportPWM);
  wheel3.backoffPWM(rapportPWM);
  wheel4.backoffPWM(rapportPWM);
}
void stop_motors()
{
  wheel1.advancePWM(0);
  wheel2.advancePWM(0);
  wheel3.backoffPWM(0);
  wheel4.backoffPWM(0);
}
