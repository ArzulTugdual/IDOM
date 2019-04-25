
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

void setup() {
  //TCCR0B=TCCR0B&0xf8|0x01;    // warning!! it will change millis()
  TCCR1B=TCCR1B&0xf8|0x01;    // Pin9,Pin10 PWM 31250Hz
  TCCR2B=TCCR2B&0xf8|0x01;    // Pin3,Pin11 PWM 31250Hz

  Omni.PIDEnable(0.31,0.01,0,10);
    
}

void loop() {
  
  td(37,500,10,false);

  
 
 
 
    
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
 void carresansrotation(int distance){
 /*
 a ajuster pour avoir un carré de 20 cm avec d=v*t
*/
 const int speedMMPS=40;
  const int uptime=distance*1000/speedMMPS;
  const int duration=500;
  const boolean debug=false;
  av(speedMMPS,uptime,duration,debug);
  g(speedMMPS,uptime,duration,debug);
  re(speedMMPS,uptime,duration,debug);
  d(speedMMPS,uptime,duration,debug); 
 }
  void carreavecrotation(int distance){
 /*
 a ajuster pour avoir un carré de 20 cm avec d=v*t
*/
 const int speedMMPS=40;
  const int uptime=distance*100/speedMMPS;
  const int duration=500;
  const boolean debug=false;
   av(speedMMPS,uptime,duration,debug);
  td(speedMMPS,uptime,duration,debug);
  av(speedMMPS,uptime,duration,debug);
   td(speedMMPS,uptime,duration,debug);
  av(speedMMPS,uptime,duration,debug);
   td(speedMMPS,uptime,duration,debug);
  av(speedMMPS,uptime,duration,debug);
 }
 //pour le rond voir les choses deja faite dans R2wd
 
 void rectanglesansrotation(int largeur , int longueur){
 /*
 a ajuster pour avoir un carré de 20 cm avec d=v*t
*/
 const int speedMMPS=40;
  const int uptime1=largeur*1000/speedMMPS;
  const int uptime2=longueur*1000/speedMMPS;
  const int duration=500;
  const boolean debug=false;
    av(speedMMPS,uptime1,duration,debug);
  g(speedMMPS,uptime2,duration,debug);
  re(speedMMPS,uptime1,duration,debug);
  d(speedMMPS,uptime2,duration,debug); 
 }
 
 void cercle(){
 
 while(true){
 const int speedMMPS=40;
  const int duration=10;
  const boolean debug=false;
  td(speedMMPS,0,duration,debug);
  av(speedMMPS,0,duration,debug);
  }
 }
  
 
