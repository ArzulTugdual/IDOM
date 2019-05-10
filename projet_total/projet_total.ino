/*librairies*/
// joystick
#include <TinkerKit.h>

/*Definitions*/
#define PIN_temperature   A0  // Broche capteur température
#define PIN_ultrason      10  // Broche capteur ultrason (envoie/réception)

/*objets joystick*/
TKJoystick joystick(I0, I1);  // creating the object 'joystick' that belongs to the 'TKJoystick' class 
                               // and giving the values to the desired input pins

TKLed xLed(O0), yLed(O1);     // creating the objects 'xLed' & 'yLed' that both belongs to the 'TKLed' class 
                              // and giving the values to the desired output pins

/*variables*/
int x = 0;      // a variable for the Joystick's x value
int y = 0;      // a variable for the Joystick's y value

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
      //avance en diagonal droite
    }
    else if(y<=ymin)
    {
      //reculer en diagonal droite
    }
    else
    {
      //aller à droite
    }
  }
  else if(x<=xmin)
  {
    if(y>=ymax)
    {
      //avance en diagonal gauche
    }
    else if(y<=ymin)
    {
      //reculer en diagonal gauche
    }
    else
    {
      //aller à gauche
    }
  }
  else
  {
    if(y>=ymax)
    {
      //avancer
    }
    else if(y<=ymin)
    {
      //reculer
    }
    else
    {
      //s'arrêter
    }
  }
}

//mesure la distance entre le robot et le prochain obstacle devant
float mesure_distance(float temp)
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

/*mesure la température ambiante*/
float temperature()
{
  return analogRead(PIN_temperature)*500.0/1023.0;    //température ambiante en °C
}
