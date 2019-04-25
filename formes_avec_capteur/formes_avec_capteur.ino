
#include <PinChangeInt.h>
#include <PinChangeIntConfig.h>
#include <EEPROM.h>
#define _NAMIKI_MOTOR   //for Namiki 22CL-103501PG80:1
#include <fuzzy_table.h>
#include <PID_Beta6.h>
#include <MotorWheel.h>
#include <Omni4WD.h>

#include <fuzzy_table.h>
#include <PID_Beta6.h>


/*

            \                    /
   wheel1   \                    /   wheel4
   Left     \                    /   Right
    
    
                              power switch
    
            /                    \
   wheel2   /                    \   wheel3
   Right    /                    \   Left

 */

/*
irqISR(irq1,isr1);
MotorWheel wheel1(5,4,12,13,&irq1);

irqISR(irq2,isr2);
MotorWheel wheel2(6,7,14,15,&irq2);

irqISR(irq3,isr3);
MotorWheel wheel3(9,8,16,17,&irq3);

irqISR(irq4,isr4);
MotorWheel wheel4(10,11,18,19,&irq4);
 */

irqISR(irq1,isr1);
MotorWheel wheel1(3,2,4,5,&irq1);

irqISR(irq2,isr2);
MotorWheel wheel2(11,12,14,15,&irq2);

irqISR(irq3,isr3);
MotorWheel wheel3(9,8,16,17,&irq3);

irqISR(irq4,isr4);
MotorWheel wheel4(10,7,18,19,&irq4);


Omni4WD Omni(&wheel1,&wheel2,&wheel3,&wheel4);

const int speedMMPS=40;
const int tps_arret=1500;
const int duration=500;
const boolean debug=false;
//seuil distance
const float seuil;

//mode (av, re...)
String mode = "av";

void setup() {
  //TCCR0B=TCCR0B&0xf8|0x01;    // warning!! it will change millis()
  TCCR1B=TCCR1B&0xf8|0x01;    // Pin9,Pin10 PWM 31250Hz
  TCCR2B=TCCR2B&0xf8|0x01;    // Pin3,Pin11 PWM 31250Hz

  Omni.PIDEnable(0.31,0.01,0,10);

  //capteur ultrason
  Serial.begin(9600);
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN, LOW); // La broche doit être à LOW au repos
}

void loop() {
  float distance = mesure_distance();
  switch(mode)
  {
    case "av":
    if(distance<
  }
  av(speedMMPS,uptime,duration,debug);
  g(speedMMPS,uptime,duration,debug);
  re(speedMMPS,uptime,duration,debug);
  d(speedMMPS,uptime,duration,debug);
  td(speedMMPS,500,500,debug);
}

//mesure la distance entre le robot et le prochain obstacle devant
float mesure_distance()
{
  //capteur parallax (3 broches)
  /*création des variables de résultat*/
  float measure, distance_mm;
  //float c = 20.05*sqrt(temp)*pow(10,-3);   //vitesse du son dans l'air en fonction de la température en mm/microsec
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

 void bouge(String mode, int speedMMPS, boolean debug)
 {
  switch(mode)
  {
    case "av":
    av(speedMMPS,0,0,debug);
    case "re":
    re(speedMMPS,0,0,debug);
    case "d":
    d(speedMMPS,0,0,debug);
    case "g":
    g(speedMMPS,0,0,debug);
    case "td":
    td(speedMMPS,0,0,debug);
    case "tg":
    tg(speedMMPS,0,0,debug);
    case "dhd":
    dhd(speedMMPS,0,0,debug);
    case "dhg":
    dhg(speedMMPS,0,0,debug);
    case "dbd":
    dbd(speedMMPS,0,0,debug);
    case "dbg":
    dbg(speedMMPS,0,0,debug);
  }
 }
 void av(int speedMMPS,int uptime, int duration, boolean debug)
 {
  Omni.setCarAdvance();
  Omni.setCarSpeedMMPS(speedMMPS,uptime);
  Omni.delayMS(duration,debug);
  Omni.setCarSlow2Stop(uptime);
 }

void re(int speedMMPS,int uptime, int duration, boolean debug)
 {
  Omni.setCarBackoff();
  Omni.setCarSpeedMMPS(speedMMPS,uptime);
  Omni.delayMS(duration,debug);
  Omni.setCarSlow2Stop(uptime);
 }

 void d(int speedMMPS,int uptime, int duration, boolean debug)
 {
  Omni.setCarRight();
  Omni.setCarSpeedMMPS(speedMMPS,uptime);
  Omni.delayMS(duration,debug);
  Omni.setCarSlow2Stop(uptime);
 }
void g(int speedMMPS,int uptime, int duration, boolean debug)
 {
  Omni.setCarLeft();
  Omni.setCarSpeedMMPS(speedMMPS,uptime);
  Omni.delayMS(duration,debug);
  Omni.setCarSlow2Stop(uptime);
 }
void td(int speedMMPS,int uptime, int duration, boolean debug)
 {
  Omni.setCarRotateRight();
  Omni.setCarSpeedMMPS(speedMMPS,uptime);
  Omni.delayMS(duration,debug);
  Omni.setCarSlow2Stop(uptime);
 }
void tg(int speedMMPS,int uptime, int duration, boolean debug)
 {
  Omni.setCarRotateLeft();
  Omni.setCarSpeedMMPS(speedMMPS,uptime);
  Omni.delayMS(duration,debug);
  Omni.setCarSlow2Stop(uptime);
 }
 void dhd(int speedMMPS,int uptime, int duration, boolean debug)
 {
  Omni.setCarUpperRight();
  Omni.setCarSpeedMMPS(speedMMPS,uptime);
  Omni.delayMS(duration,debug);
  Omni.setCarSlow2Stop(uptime);
 }
 void dbd(int speedMMPS,int uptime, int duration, boolean debug)
 {
  Omni.setCarLowerRight();
  Omni.setCarSpeedMMPS(speedMMPS,uptime);
  Omni.delayMS(duration,debug);
  Omni.setCarSlow2Stop(uptime);
 }
 void dhg(int speedMMPS,int uptime, int duration, boolean debug)
 {
  Omni.setCarUpperLeft();
  Omni.setCarSpeedMMPS(speedMMPS,uptime);
  Omni.delayMS(duration,debug);
  Omni.setCarSlow2Stop(uptime);
 }
 void dbg(int speedMMPS,int uptime, int duration, boolean debug)
 {
  Omni.setCarLowerLeft();
  Omni.setCarSpeedMMPS(speedMMPS,uptime);
  Omni.delayMS(duration,debug);
  Omni.setCarSlow2Stop(uptime);
 }
 
