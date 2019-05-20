/*librairies*/
// joystick
#include <TinkerKit.h>
//robot
#include <PinChangeInt.h>
#include <PinChangeIntConfig.h>
#include <EEPROM.h>
#define _NAMIKI_MOTOR   //for Namiki 22CL-103501PG80:1
#include <fuzzy_table.h>
#include <PID_Beta6.h>
#include <MotorWheel.h>
#include <Omni4WD.h>
/*Definitions*/
#define PIN_ultrason  6  // Broche capteur ultrason (envoie/réception)
#define boutonS1_5  A7   //définit le pin des boutons

irqISR(irq1,isr1);
MotorWheel wheel1(3,2,4,5,&irq1);

irqISR(irq2,isr2);
MotorWheel wheel2(11,12,14,15,&irq2);

irqISR(irq3,isr3);
MotorWheel wheel3(9,8,16,17,&irq3);

irqISR(irq4,isr4);
MotorWheel wheel4(10,7,18,19,&irq4);


Omni4WD Omni(&wheel1,&wheel2,&wheel3,&wheel4);

/*objets joystick*/
TKJoystick joystick(I0, I1);  // creating the object 'joystick' that belongs to the 'TKJoystick' class 
                               // and giving the values to the desired input pins

TKLed xLed(O0), yLed(O1);     // creating the objects 'xLed' & 'yLed' that both belongs to the 'TKLed' class 
                              // and giving the values to the desired output pins

/*axes horizontal et vertical*/
int x = 0;      // a variable for the Joystick's x value
int y = 0;      // a variable for the Joystick's y value

int vitessePWM = 40;

void setup() 
{
  // initialize serial communications at 9600 bps
  Serial.begin(9600);
  
  /*initialisation des PIN*/
  //capteur ultrason
  pinMode(PIN_ultrason, OUTPUT);
  digitalWrite(PIN_ultrason, LOW);  
}

void loop()
{
  mode_joystick();
}

/* déclaration des fonctions */
void mode_joystick()
{
  // read the both joystick axis values:
  x = joystick.readX();  
  y = joystick.readY(); 

  // set the leds brightness
  xLed.brightness(x);
  yLed.brightness(y);

/*On pose:
 * x: horizontal (gauche/droite)
 * y: vertical (devant/derrière)
 */
 //définition des bornes du joystick
 int xmax,xmin,ymax,ymin;
 //execution
  if(x>=xmax)
  {
    if(y>=ymax)
    {
      dhg(vitessePWM);  //avance en diagonal droite
    }
    else if(y<=ymin)
    {
      dbd(vitessePWM) //reculer en diagonal droite
    }
    else
    {
      d(vitessePWM);  //aller à droite
    }
  }
  else if(x<=xmin)
  {
    if(y>=ymax)
    {
      dhg(vitessePWM);  //avance en diagonal gauche
    }
    else if(y<=ymin)
    {
      dbg(vitessePWM);  //reculer en diagonal gauche
    }
    else
    {
      g(vitessePWM);  //aller à gauche
    }
  }
  else
  {
    if(y>=ymax)
    {
      av(vitessePWM); //avancer
    }
    else if(y<=ymin)
    {
      re(vitessePWM); //reculer
    }
    else
    {
      stopp();  //s'arrêter
    }
  }
}

//mesure la distance entre le robot et le prochain obstacle devant
float mesure_distance()
{
  //capteur parallax (3 broches)
  /*création des variables de résultat*/
  float measure, distance_mm;
  float c = 20.05*sqrt(temp)*pow(10,-3);   //vitesse du son dans l'air en fonction de la température en mm/microsec
  //float c = 0.34;
  
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

/*fonctions primaires du robot*/
void av(int rapportPWM)
{
  wheel1.advancePWM(rapportPWM);
  wheel2.advancePWM(rapportPWM);
  wheel3.backoffPWM(rapportPWM);
  wheel4.backoffPWM(rapportPWM);
}
void re(int rapportPWM)
{
  wheel1.backoffPWM(rapportPWM);
  wheel2.backoffPWM(rapportPWM);
  wheel3.advancePWM(rapportPWM);
  wheel4.advancePWM(rapportPWM);
}
void d(int rapportPWM)
{
  wheel1.backoffPWM(rapportPWM);
  wheel2.advancePWM(rapportPWM);
  wheel3.advancePWM(rapportPWM);
  wheel4.backoffPWM(rapportPWM);
}
void g(int rapportPWM)
{
  wheel1.advancePWM(rapportPWM);
  wheel2.backoffPWM(rapportPWM);
  wheel3.backoffPWM(rapportPWM);
  wheel4.advancePWM(rapportPWM);
}
void dhg(int rapportPWM)
{
  wheel1.advancePWM(rapportPWM);
  wheel2.advancePWM(0);
  wheel3.backoffPWM(rapportPWM);
  wheel4.backoffPWM(0);
}
void dhd(int rapportPWM)
{
  wheel1.advancePWM(0);
  wheel2.advancePWM(rapportPWM);
  wheel3.backoffPWM(0);
  wheel4.backoffPWM(rapportPWM);
}
void dbg(int rapportPWM)
{
  wheel1.backoffPWM(0);
  wheel2.backoffPWM(rapportPWM);
  wheel3.advancePWM(0);
  wheel4.advancePWM(rapportPWM);
}
void dbd(int rapportPWM)
{
  wheel1.backoffPWM(rapportPWM);
  wheel2.backoffPWM(0);
  wheel3.advancePWM(rapportPWM);
  wheel4.advancePWM(0);
}
void td(int angle)
{
  wheel1.advancePWM(50);
  wheel2.advancePWM(50);
  wheel3.advancePWM(50);
  wheel4.advancePWM(50);
  if(angle >= 0)
  {
    delay((unsigned long)angle*(5400*1.0/360));
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
    delay((unsigned long)angle*(5400*1.0/360));
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
