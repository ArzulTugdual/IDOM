#include <PinChangeInt.h>
#include <PinChangeIntConfig.h>
#include <EEPROM.h>
#define _NAMIKI_MOTOR   //for Namiki 22CL-103501PG80:1
#include <fuzzy_table.h>
#include <PID_Beta6.h>
#include <MotorWheel.h>
#include <Omni4WD.h>

#define boutonS1_5  A7    //définit le pin des boutons

#define PIN         6     //pin du capteur ultrason
#define distanceMin 300   //distance min à un obstacle

#define rapportPWM  40    //rapport PWM du robot

#define seuilInit   3000  //temps d'exécution initial

#define dist_csr    500   //distance de chaque côté de la figure "carré sans rotation"
#define dist_car    600   //distance de chaque côté de la figure "carré avec rotation"
#define dist_tri    700   //distance de chaque côté de la figure "triangle"

irqISR(irq1,isr1);
MotorWheel wheel1(3,2,4,5,&irq1);

irqISR(irq2,isr2);
MotorWheel wheel2(11,12,14,15,&irq2);

irqISR(irq3,isr3);
MotorWheel wheel3(9,8,16,17,&irq3);

irqISR(irq4,isr4);
MotorWheel wheel4(10,7,18,19,&irq4);


Omni4WD Omni(&wheel1,&wheel2,&wheel3,&wheel4);

/*variables de temps*/
unsigned long seuil = 0; //tps d'exécution d'un état
unsigned long t0; //temps de départ d'un état
unsigned long t;

int figure=10; //carré sans rotation:0 carré avec rotation:1 triangle:2  ...
int etatEnCours=0; //0:stop >0: différents états en fonction de la figure
const float vitesseMMPS = -4*pow(10,-5)*pow(rapportPWM,2) + 0.0076*rapportPWM;

/*capteur ultrason*/
//const byte PIN = 6;     // Broche capteur ultrason (envoi/réception) attention, pin 6 car non utilisé par le robot
//const float distanceMin = 300; //distance min à un obstacle

void setup()
{
  /*robot*/
  TCCR1B=TCCR1B&0xf8|0x01;    // Pin9,Pin10 PWM 31250Hz
  TCCR2B=TCCR2B&0xf8|0x01;    // Pin3,Pin11 PWM 31250Hz

  Omni.PIDEnable(0.31,0.01,0,10);

  /*boutons*/
  pinMode(boutonS1_5,INPUT);  //pin correspondant aux boutons poussoirs S1 à S5

  /*capteur ultrason*/
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN, LOW); // La broche doit être à LOW au repos

  /*début du compteur*/
  t0 = millis();
}

void loop()
{
  t = millis();
  int bouton = Bouton();
  bool chang_fig = bouton<=5;
  if(mesure_distance()<distanceMin) //obstacle
  {
    stopp();
    delay(1000);
    td(90); //état stop à la fin
    etatEnCours = 0;  //retour au début de la figure
    t0 = t; //réinitialisation du temps de départ de l'état
    seuil = 1000;  //1 sec d'arrêt
  }
  if(chang_fig) //changement manuel de la figure
  {
    figure = bouton;  //changement de figure
    etatEnCours = 0;  //retour au début de la figure
    t0=t; //réinitialisation du temps de départ de l'état
    stopp();
    seuil = 200;
  }
  if(t-t0>=seuil) //fin de l'état en cours -> changement d'état
  {
    stopp();
    delay(150);
    switch(figure)
    {
      case 0: etatEnCours = chEt_csr(etatEnCours, dist_csr); break;  //figure en cours: carré sans rotation
      case 1: etatEnCours = chEt_car(etatEnCours, dist_car); break;  //figure en cours: carré avec rotation
      case 2: etatEnCours = chEt_tri(etatEnCours, dist_tri); break;  //figure en cours: triangle equilatéral
      case 3: etatEnCours = chEt_cer(etatEnCours);  break;  //figure en cours: cercle
      default:  stopp();  break;
    }
    t0=t; //réinitialisation du temps de départ de l'état
  }
}

/**
 * lit la valeur du pin des boutons poussoirs de la carte
 * return 0:boutonS0 1:boutonS1 2:boutonS2 3:boutonS3 5:boutonS5
 */
int Bouton()
{
  return (analogRead(boutonS1_5)+66)/145;  //renvoie la valeur du bouton pressé
}

/**
 * capteur de distance
 * return : distance au prochain obstacle (en mm)
 */
 float mesure_distance()
{
  //capteur parallax (3 broches)
  /*création des variables de résultat*/
  float measure, distance_mm;
  //float c = 20.05*sqrt(temp)*pow(10,-3);   //vitesse du son dans l'air en fonction de la température en mm/microsec
  float c = 0.34;
  
  pinMode(PIN, OUTPUT);       //broche en sortie pour envoyer l'impulsion
  digitalWrite(PIN, LOW);     // La broche doit être à LOW au départ
  
  /*impulsion de 10 micros*/
  digitalWrite(PIN, HIGH);      
  delayMicroseconds(10);
  digitalWrite(PIN, LOW);       //envoi de l'onde sonore ___---___

  pinMode(PIN, INPUT);          //broche en entrée en attente de l'impulsion
  
  measure = pulseIn(PIN, HIGH, 25000UL);  //attente du signal retour puis calcul du temps de réponse en microsec
  return measure / 2.0 * c;               //MESURE en mm
}

/*figures*/
/**
 * change l'état du robot (avant, arrière...) en fonction de l'état suivant sur la figure "carré sans rotation"
 * @param etat: état du robot (0:stop 1:avant 2:droite 3:arrière 4:gauche)
 * @param longueur: longueur d'un côté de la figure(en mm)
 */
int chEt_csr(int etat, unsigned long longueur)
{
  seuil = convertion_seuil(longueur);
  //passe d'un "etat" à l'"etat" suivant
  switch(etat)
  {
    case 0: av(rapportPWM); break;
    case 1: d(rapportPWM); break;
    case 2: re(rapportPWM); break;
    case 3: g(rapportPWM); break;
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
 * @param tempsEtat: temps à un état(en ms)
 */
int chEt_car(int etat, unsigned long longueur)
{
  //passe d'un "etat" à l'"etat" suivant
  switch(etat)
  {
    case 0:
    case 2:
    case 4:
    case 6:
      {
      av(rapportPWM);
      seuil = convertion_seuil(longueur); //rétablissement de la durée d'un état
      }
      break;
    case 1:
    case 3:
    case 5:
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
 * chawnge l'état du robot (avant, tourner droite) en fonction de l'état suivant sur la figure "triangle"
 * @param etat: état du robot (0:stop   impaire(1-5):avant    paire(2-6):tourner droite)
 * @param tempsEtat: temps à un état(en ms)
 */
int chEt_tri(int etat, unsigned long longueur)
{
  //passe d'un "etat" à l'"etat" suivant
  switch(etat)
  {
    case 0:
    case 2:
    case 4:
      {
      av(rapportPWM);
      seuil = convertion_seuil(longueur); //rétablissement de la durée d'un état
      }
      break;
    case 1:
    case 3:
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
int chEt_cer(int etat)
{
  if(etat == 0)
  {
    seuil = 10000;
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

/**
 * converti la distance en temps
 * @param d: distance (en mm)
 */
unsigned long convertion_seuil(unsigned long d)
{
  return (unsigned long) d / vitesseMMPS;
}

/*fonctions primaires du robot*/
void av(int v)
{
  wheel1.advancePWM(v);
  wheel2.advancePWM(v);
  wheel3.backoffPWM(v);
  wheel4.backoffPWM(v);
}
void d(int v)
{
  wheel1.backoffPWM(v);
  wheel2.advancePWM(v);
  wheel3.advancePWM(v);
  wheel4.backoffPWM(v);
}
void re(int v)
{
  wheel1.backoffPWM(v);
  wheel2.backoffPWM(v);
  wheel3.advancePWM(v);
  wheel4.advancePWM(v);
}
void g(int v)
{
  wheel1.advancePWM(v);
  wheel2.backoffPWM(v);
  wheel3.backoffPWM(v);
  wheel4.advancePWM(v);
}
void td(int angle)
{
  wheel1.advancePWM(50);
  wheel2.advancePWM(50);
  wheel3.advancePWM(50);
  wheel4.advancePWM(50);
  if(angle >= 0)
  {
    delay((unsigned long)angle*(4700*1.0/360));
    stopp();
  }
}
void tg(int angle)
{
  wheel1.backoffPWM(50);
  wheel2.backoffPWM(50);
  wheel3.backoffPWM(50);
  wheel4.backoffPWM(50);
  if(angle >= 0)
  {
    delay((unsigned long)angle*(4700*1.0/360));
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
