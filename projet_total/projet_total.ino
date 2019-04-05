// include the TinkerKit library
#include <TinkerKit.h>

TKJoystick joystick(I0, I1);  // creating the object 'joystick' that belongs to the 'TKJoystick' class 
                               // and giving the values to the desired input pins

TKLed xLed(O0), yLed(O1);     // creating the objects 'xLed' & 'yLed' that both belongs to the 'TKLed' class 
                              // and giving the values to the desired output pins

int x = 0;      // a variable for the Joystick's x value
int y = 0;      // a variable for the Joystick's y value

void setup() 
{
  // initialize serial communications at 9600 bps
  Serial.begin(9600);
}

void loop()
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
