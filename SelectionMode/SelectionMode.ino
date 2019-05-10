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
#define boutonS1_5 A7
int flag=1;

irqISR(irq1,isr1);
MotorWheel wheel1(3,2,4,5,&irq1);

irqISR(irq2,isr2);
MotorWheel wheel2(11,12,14,15,&irq2);

irqISR(irq3,isr3);
MotorWheel wheel3(9,8,16,17,&irq3);

irqISR(irq4,isr4);
MotorWheel wheel4(10,7,18,19,&irq4);

Omni4WD Omni(&wheel1,&wheel2,&wheel3,&wheel4);

void setup()
{
    TCCR1B=TCCR1B&0xf8|0x01;    // Pin9,Pin10 PWM 31250Hz
    TCCR2B=TCCR2B&0xf8|0x01;    // Pin3,Pin11 PWM 31250Hz
    Omni.PIDEnable(0.31,0.01,0,10);

    pinMode(boutonS1_5,INPUT);  //pin correspondant aux boutons poussoirs S1 à S5
}

int Bouton(){
    int bouton = (analogRead(boutonS1_5)+66)/145;  //renvoie la valeur du bouton pressé
    return bouton;
}

void loop()
{
    // lit la valeur du bouton pressé et le mémorise dans la variable bouton
    int bouton = Bouton();

    if(bouton==0){
        flag=flag*(-1);
    }
    /*  Mode JOYSTICK   */
    if(flag==-1){                 /* Bouton S1 */
        //appel fonction joystick
    }
    /*  Mode BOUTONS   */
    else if(flag==1){   
    if(bouton==1){                /* Bouton S2 */
        //appel fonction carré
        carreavecrotation(4);
        delay(1000);
    }
    else if(bouton==2){           /* Bouton S3 */
        //appel fonction cercle
        cercle();
        delay(1000);
    }
    else if(bouton==3){           /* Bouton S4 */
        //appel fonction triangle
        triangle();
        delay(1000);
    }
    else if(bouton==5){           /* Bouton S5 */
        //appel fonction rect
        rectanglesansrotation(2,4);
        delay(1000);
    }
    }
}

/* Fonctions de déplacement du robot */
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

/* Tourner en fonction de l'angle */
void td(int taille)
 {
  int uptime =500;
  boolean debug = false;
  int speedMMPS = 37; 
  int duration = 3650 ;
  switch  (taille) {
  case 90 :  
  duration = 500; break;
  case 45 :
  duration =10; 
  break;
  default : break;
  }
  Omni.setCarRotateRight();
  Omni.setCarSpeedMMPS(speedMMPS,uptime);
  Omni.delayMS(duration,debug);
  Omni.setCarSlow2Stop(uptime); 
 }
void tg(int taille)
 {
  int uptime =500;
  boolean debug = false;
  int speedMMPS = 37; 
  int duration =3600 ;
  switch  (taille) {
  case 90 : 
  duration = 500; break;
  case 45 :
  duration =10; break;
  default :  break;
  }
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

/* Fonctions correspondant aux figures */
void carresansrotation(int tpscote){
  const int speedMMPS=40;
  const int uptime=500;
  const int duration=tpscote*1000-uptime;
  const boolean debug=false;
  av(speedMMPS,uptime,duration,debug);
  g(speedMMPS,uptime,duration,debug);
  re(speedMMPS,uptime,duration,debug);
  d(speedMMPS,uptime,duration,debug); 
}
void triangle (){
const int speedMMPS=40;
  const int uptime=500;
  const int duration=2500;
  const boolean debug=false; 
  td(45);
  td(90);
  av(speedMMPS,uptime,duration,debug);
  td(90);
  av(speedMMPS,uptime,duration,debug);
  td(45);
  td(90);
  av(speedMMPS,uptime,duration,debug);
}
void carreavecrotation(int tpscote){
  const int speedMMPS=40;
  const int uptime=500;
  const int duration=tpscote*1000-uptime;
  const boolean debug=false;
  av(speedMMPS,uptime,duration,debug);
  td(90);
  av(speedMMPS,uptime,duration,debug);
  td(90);
  av(speedMMPS,uptime,duration,debug);
  td(90);
  av(speedMMPS,uptime,duration,debug);
}
void rectanglesansrotation(int tmpslargeur,int tmpslongueur){
  const int speedMMPS=40;
  const int uptime=500;
  const int duration1=tmpslargeur*1000-uptime;
  const int duration2=tmpslongueur*1000-uptime;
  const boolean debug=false;
  av(speedMMPS,uptime,duration1,debug);
  g(speedMMPS,uptime,duration2,debug);
  re(speedMMPS,uptime,duration1,debug);
  d(speedMMPS,uptime,duration2,debug); 
 }
void cercle(){
  wheel1.advancePWM(55);
  wheel2.advancePWM(55);
  wheel3.backoffPWM(10);
  wheel4.backoffPWM(10);
  delay(14000);
  wheel1.advancePWM(0);
  wheel2.advancePWM(0);
  wheel3.backoffPWM(0);
  wheel4.backoffPWM(0);
}
 
