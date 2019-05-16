#include <PinChangeInt.h>
#include <PinChangeIntConfig.h>
#include <EEPROM.h>
#define _NAMIKI_MOTOR   //for Namiki 22CL-103501PG80:1
#include <fuzzy_table.h>
#include <PID_Beta6.h>
#include <MotorWheel.h>
#include <Omni4WD.h>
#define boutonS1_5 A7   //définit le pin des boutons

irqISR(irq1,isr1);
MotorWheel wheel1(3,2,4,5,&irq1);

irqISR(irq2,isr2);
MotorWheel wheel2(11,12,14,15,&irq2);

irqISR(irq3,isr3);
MotorWheel wheel3(9,8,16,17,&irq3);

irqISR(irq4,isr4);
MotorWheel wheel4(10,7,18,19,&irq4);


Omni4WD Omni(&wheel1,&wheel2,&wheel3,&wheel4);

unsigned long seuilInit = 3000;
unsigned long seuil = 0; //tps d'exécution d'un état
unsigned long t0; //temps de départ d'un état
unsigned long t;
int figure=7; //carré sans rotation:0 carré avec rotation:1 triangle:2  ...
int etatEnCours=0; //0:stop >0: différents états en fonction de la figure
int vitessePWM = 40;  //vitesse de marche du robot

void setup()
{
  TCCR1B=TCCR1B&0xf8|0x01;    // Pin9,Pin10 PWM 31250Hz
  TCCR2B=TCCR2B&0xf8|0x01;    // Pin3,Pin11 PWM 31250Hz

  Omni.PIDEnable(0.31,0.01,0,10);

  pinMode(boutonS1_5,INPUT);  //pin correspondant aux boutons poussoirs S1 à S5
  
  t0 = millis();
}

void loop()
{
  t = millis();
  int bouton = Bouton();
  bool chang_fig = bouton <=5;
  if(t-t0>=seuil || chang_fig) //fin de l'état en cours -> changement d'état OU changement manuel de la figure
  {
    if(chang_fig)
    {
      figure = bouton;
      etatEnCours = 0;
    }
    switch(figure)
    {
      case 0: etatEnCours = carreSansRotation(etatEnCours); break;  //figure en cours: carré sans rotation
      case 1: etatEnCours = carreAvecRotation(etatEnCours); break;  //figure en cours: carré avec rotation
      case 2: etatEnCours = triangle(etatEnCours); break;  //figure en cours: triangle equilatéral
      case 3: etatEnCours = cercle(etatEnCours);  break;  //figure en cours: cercle
      default:  stopp();  break;
    }
    t0=t; //réinitialisation du temps de départ de l'état
  }
}

/**
 * lit la valeur du pin des boutons poussoirs de la carte
 */
int Bouton()
{
  return (analogRead(boutonS1_5)+66)/145;  //renvoie la valeur du bouton pressé
}

/*figures*/
/**
 * change l'état du robot (avant, arrière...) en fonction de l'état suivant sur la figure "carré sans rotation"
 * @param etat: état du robot (0:stop 1:avant 2:droite 3:arrière 4:gauche)
 */
int carreSansRotation(int etat)
{
  seuil = seuilInit;
  //passe d'un "etat" à l'"etat" suivant
  switch(etat)
  {
    case 0: av(vitessePWM); break;
    case 1: d(vitessePWM); break;
    case 2: re(vitessePWM); break;
    case 3: g(vitessePWM); break;
    default:
    {
      stopp();
      seuil = 1000; //délais avec la prochaine figure
      figure = 10;  //empêche de touner la figure en boucle
    }
    break;
  }
  etat = (etat+1)%5;
  return etat;
}

/**
 * change l'état du robot (avant, tourner droite) en fonction de l'état suivant sur la figure "carré avec rotation"
 * @param etat: état du robot (0:stop   impaire(1-7):avant    paire(2-8):tourner droite)
 */
int carreAvecRotation(int etat)
{
  //passe d'un "etat" à l'"etat" suivant
  switch(etat)
  {
    case 0:
    {
      av(vitessePWM);
      seuil = seuilInit; //rétablissement de la durée d'un état
    }
    break;
    case 2:
    {
      av(vitessePWM);
      seuil = seuilInit; //rétablissement de la durée d'un état
    }
    break;
    case 4:
    {
      av(vitessePWM);
      seuil = seuilInit; //rétablissement de la durée d'un état
    }
    break;
    case 6:
    {
      av(vitessePWM);
      seuil = seuilInit; //rétablissement de la durée d'un état
    }
    break;
    case 1:
    {
    td(90);
    seuil = 0;  //état terminé donc seuil à 0
    } 
    case 3:
    {
    td(90);
    seuil = 0;  //état terminé donc seuil à 0
    } 
    case 5:
    {
    td(90);
    seuil = 0;  //état terminé donc seuil à 0
    } 
    break;
    case 7:
    {
      td(90);
      seuil = 0;  //état terminé donc seuil à 0
    } 
    break;
    default:
    {
      stopp();
      seuil = 1000; //délais avec la prochaine figure
      figure = 10;  //empêche de touner la figure en boucle
    }
    break;
  }
  etat = (etat+1)%9;
  return etat;
} 

/**
 * change l'état du robot (avant, tourner droite) en fonction de l'état suivant sur la figure "triangle"
 * @param etat: état du robot (0:stop   impaire(1-5):avant    paire(2-6):tourner droite)
 */
int triangle(int etat)
{
  //passe d'un "etat" à l'"etat" suivant
  switch(etat)
  {
    case 0:
    {
      av(vitessePWM);
      seuil = seuilInit; //rétablissement de la durée d'un état
    }
    case 2:
    {
      av(vitessePWM);
      seuil = seuilInit; //rétablissement de la durée d'un état
    }
    case 4:
    {
      av(vitessePWM);
      seuil = seuilInit; //rétablissement de la durée d'un état
    }
    break;
    case 1:
    {
      td(120);
      seuil = 0;  //état terminé donc seuil à 0
    } 
    case 3:
    {
      td(120);
      seuil = 0;  //état terminé donc seuil à 0
    }
    case 5:
    {
      td(120);
      seuil = 0;  //état terminé donc seuil à 0
    } 
    break;
    default:
    {
      stopp();
      seuil = 1000; //délais avec la prochaine figure
      figure = 10;  //empêche de touner la figure en boucle
    }
    break;
  }
  etat = (etat+1)%7;
  return etat;
}
/**
 * change l'état du robot (rotation) en fonction de l'état suivant sur la figure "cercle"
 * @param etat: état du robot (0:stop   1:rotation)
 */
int cercle(int etat)
{
  if(etat == 0)
  {
    seuil = 9750;
    wheel1.advancePWM(55);
    wheel2.advancePWM(55);
    wheel3.backoffPWM(3);
    wheel4.backoffPWM(3);
  }
  else
  {
    stopp();
    seuil = 1000; //délais avec la prochaine figure
    figure = 10;  //empêche de touner la figure en boucle
  }
  etat = (etat+1)%2;
  return etat;
}

/*fonctions primaires du robot*/
void av(int rapportPWM)
{
  wheel1.advancePWM(rapportPWM);
  wheel2.advancePWM(rapportPWM);
  wheel3.backoffPWM(rapportPWM);
  wheel4.backoffPWM(rapportPWM);
}
void d(int rapportPWM)
{
  wheel1.backoffPWM(rapportPWM);
  wheel2.advancePWM(rapportPWM);
  wheel3.advancePWM(rapportPWM);
  wheel4.backoffPWM(rapportPWM);
}
void re(int rapportPWM)
{
  wheel1.backoffPWM(rapportPWM);
  wheel2.backoffPWM(rapportPWM);
  wheel3.advancePWM(rapportPWM);
  wheel4.advancePWM(rapportPWM);
}
void g(int rapportPWM)
{
  wheel1.advancePWM(rapportPWM);
  wheel2.backoffPWM(rapportPWM);
  wheel3.backoffPWM(rapportPWM);
  wheel4.advancePWM(rapportPWM);
}
void td(int angle)
{
  wheel1.advancePWM(57);
  wheel2.advancePWM(57);
  wheel3.advancePWM(57);
  wheel4.advancePWM(57);
  if(angle >= 0)
  {
    delay((unsigned long)angle*(5013*1.0/360));
    stopp();
  }
}
void tg(int angle)
{
  wheel1.backoffPWM(57);
  wheel2.backoffPWM(57);
  wheel3.backoffPWM(57);
  wheel4.backoffPWM(57);
  if(angle >= 0)
  {
    delay((unsigned long)angle*(5013*1.0/360));
    stopp();
  }
}
/*stop le robot en avant*/
void stopp()
{
  wheel1.advancePWM(0);
  wheel2.advancePWM(0);
  wheel3.backoffPWM(0);
  wheel4.backoffPWM(0);
}
