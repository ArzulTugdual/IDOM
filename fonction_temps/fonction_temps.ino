#include <PinChangeInt.h>
#include <PinChangeIntConfig.h>
#include <EEPROM.h>
#define _NAMIKI_MOTOR   //for Namiki 22CL-103501PG80:1
#include <fuzzy_table.h>
#include <PID_Beta6.h>
#include <MotorWheel.h>
#include <Omni4WD.h>

irqISR(irq1,isr1);
MotorWheel wheel1(3,2,4,5,&irq1);

irqISR(irq2,isr2);
MotorWheel wheel2(11,12,14,15,&irq2);

irqISR(irq3,isr3);
MotorWheel wheel3(9,8,16,17,&irq3);

irqISR(irq4,isr4);
MotorWheel wheel4(10,7,18,19,&irq4);


Omni4WD Omni(&wheel1,&wheel2,&wheel3,&wheel4);

unsigned long seuil = 3000; //tps d'exécution d'un état
int figure=0; //carré:0 triangle:1  ...
int t0; //temps de départ d'un état
int etatEnCours=0; //0:stop 1:avant 2:droite 3:arrière 4:gauche

void setup()
{
  TCCR1B=TCCR1B&0xf8|0x01;    // Pin9,Pin10 PWM 31250Hz
  TCCR2B=TCCR2B&0xf8|0x01;    // Pin3,Pin11 PWM 31250Hz

  Omni.PIDEnable(0.31,0.01,0,10);

  t0 = millis();
  
  av(40);
  etatEnCours=1;
}

void loop()
{
  unsigned long t = millis();
  if(t-t0>=seuil) //fin de l'état en cours -> changement d'état
  {
    switch(figure)
    {
      case 0: etatEnCours = carreSansRotation(etatEnCours); break;
      default: stopp(); break;
    }
    t0=t;
  }
}

/**
 * réalise un carré sans rotation
 * @param etat: état du robot (0:stop 1:avant 2:droite 3:arrière 4:gauche)
 */
int carreSansRotation(int etat)
{
  int vitessePWM =40;
  stopp();
  switch(etat)
  {
    case 0: av(vitessePWM); break;
    case 1: d(vitessePWM); break;
    case 2: re(vitessePWM); break;
    case 3: g(vitessePWM); break;
    default: stopp(); break;
  }
  etat = (etat+1)%5;
  return etat;
}

/*fonctions primaires du robot*/
void av(int rapportPWM)
{
  /*Omni.setCarAdvance();
  Omni.setCarSpeedMMPS(vitesse);*/
  wheel1.advancePWM(rapportPWM);
  wheel2.advancePWM(rapportPWM);
  wheel3.backoffPWM(rapportPWM);
  wheel4.backoffPWM(rapportPWM);
}
void d(int rapportPWM)
{
  /*Omni.setCarRight();
  Omni.setCarSpeedMMPS(vitesse);*/
  wheel1.advancePWM(rapportPWM);
  wheel2.backoffPWM(rapportPWM);
  wheel3.advancePWM(rapportPWM);
  wheel4.backoffPWM(rapportPWM);
}
void re(int rapportPWM)
{
  /*Omni.setCarBackoff();
  Omni.setCarSpeedMMPS(vitesse);*/
  wheel1.backoffPWM(rapportPWM);
  wheel2.backoffPWM(rapportPWM);
  wheel3.advancePWM(rapportPWM);
  wheel4.advancePWM(rapportPWM);
}
void g(int rapportPWM)
{
  /*Omni.setCarLeft();
  Omni.setCarSpeedMMPS(vitesse);*/
  wheel1.backoffPWM(rapportPWM);
  wheel2.advancePWM(rapportPWM);
  wheel3.backoffPWM(rapportPWM);
  wheel4.advancePWM(rapportPWM);
}
void stopp()
{
  wheel1.advancePWM(0);
  wheel2.backoffPWM(0);
  wheel3.advancePWM(0);
  wheel4.backoffPWM(0);
}
