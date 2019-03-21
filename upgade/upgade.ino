//Definition des blibliotec
#include <Wire.h>
#include "DFRobot_BMP280.h"
#include "Wire.h"                 
#include "I2Cdev.h"
#include "MPU6050.h"
MPU6050 accelgyro;  
#include <SPI.h>
#include <TFT.h> 
//Atribution des n° de pin
#define LED_PIN 13
#define cs      45
#define dc      46
#define rst     47
#define E1      3     // rose sens 1
#define M1      4     // bleu moteur 1
#define E2      5     // rose sens 2
#define M2      6     // bleu moteur 2
#define lighPin 5     // (Analogique) capteur détection luminosité
#define Pinled  22    // LED qui va s'allumer en fonction de la luminosité
//Definition des etape
#define A       0
#define V       1
#define T       2
 
TFT screen = TFT(cs, dc, rst);       

bool blinkState = false;

DFRobot_BMP280 bmp280; 
int16_t ax, ay, az;             // define accel as ax,ay,az
int16_t gx, gy, gz;             // define gyro as gx,gy,gz
bool humeur=0;                  //0->contant 1->pas contant          
int vitesse;
int thisNote = 0;
int sens;
int i=0,l=0,m=0;
int lum=255;
int light[10];
const byte TRIGGER_PIN = 10;     // Broche TRIGGER
const byte ECHO_PIN = 11;        // Broche ECHO
int etatmoteur = A ;
long measure;
long distance_mm;        
unsigned long temps;    
unsigned long delta;                   
int melody[] = {  392, 523, 392, 440, 494, 330, 330, 440, 392, 349, 392 /*NOTE_C4, NOTE_G3,NOTE_G3, NOTE_A3, NOTE_G3,0, NOTE_B3, NOTE_C4*/ };

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = { 2, 1, 1.33, 4, 1, 2, 2, 1, 1.33, 4, 1 };

int vitesse_max = 200;           //une valeure entre 150 et 255
int Nboucle = 15;                //nombre d'incrémentation pour l'acceletation, une valeur entre 5 et 30

void setup() {  
  screen.begin();                     // allumage écran
  screen.background(lum,lum,lum);     // écran blanc
  ecran(humeur);                      // affiche un smile contant
  
  Serial.begin(9600);                 // crée un discution avec l'ordinateur
  
  Wire.begin();                                                                                                 // join I2C bus   
  accelgyro.initialize();                                                                                       // verify connection
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");   //

  pinMode(LED_PIN, OUTPUT);       // configure LED pin
  pinMode(M1, OUTPUT);            // configure les pin des moteur
  pinMode(M2, OUTPUT);            //
  pinMode(Pinled, OUTPUT);        //
  
  bmp280.begin();
  pinMode(TRIGGER_PIN, OUTPUT);
  digitalWrite(TRIGGER_PIN, LOW); // La broche TRIGGER doit être à LOW au repos
  pinMode(ECHO_PIN, INPUT);
  temps=millis();
}

void loop() {
  digitalWrite(TRIGGER_PIN, HIGH);      //
  delayMicroseconds(10);                //
  digitalWrite(TRIGGER_PIN, LOW);       //envoi de l'onde sonore

  measure = pulseIn(ECHO_PIN, HIGH, 25000UL);       //
  distance_mm = measure / 2.0 * 0.34;               //MESURE
  //Serial.print(distance_mm);                 
  //Serial.println("mm") ;                          //lecture de la valeur
                                      
  if(distance_mm <= 400) {
    if (etatmoteur == A || etatmoteur == V){        //Detection MUrs
      etatmoteur = T ;
      i=0 ;
      humeur = 1;                                   //pas contant
      screen.background(lum,lum,lum);
      ecran(humeur);
    }
  }
  if(etatmoteur == T) {
    vitesse = 120;
    sens = HIGH ;
    i++ ;
    if (distance_mm <= 600) {                       //Timer tourner
      i=0;
    }
    if(i>20) {
      sens=LOW ;
      etatmoteur = A ;
      i=0;
    }
    thisNote++;
    int noteDuration = 1000/noteDurations[thisNote];
    tone(8, melody[thisNote],noteDuration);      
    int pauseBetweenNotes = 100;
    delay(pauseBetweenNotes);
    noTone(8);
    if(thisNote>11){
      thisNote=0;
    }
  }
  else if (etatmoteur == A) {        //Accéleration
    vitesse = vitesse + i*(vitesse_max-vitesse)/Nboucle;
    i++ ;
    if (i>=Nboucle) {
      etatmoteur = V ;
      i=0 ;
      humeur = 0;       //contant
      screen.background(lum,lum,lum);
      ecran(humeur);
    }
  }
  else {
    vitesse = vitesse_max ;
  }
                           
  digitalWrite(M1,HIGH);
  digitalWrite(M2,sens);
  analogWrite(E1, vitesse);
  analogWrite(E2, vitesse);
  
  if(m==1){
    if (l<10){
      light[l]= analogRead(lightPin);
      Serial.println(light[l]);
      l++;
    }
    else{
      if(light[0]+light[1]+light[2]+light[3]+light[4]+light[5]+light[6]+light[7]+light[8]+light[9]>100){          // somme des valeurs de luminosité pour avoir des 
        digitalWrite(Pinled,0);            // valeurs plus faciles a exploiter
        lum=255;
        screen.background(lum,lum,lum);
        ecran(humeur);
      }
      else{
        digitalWrite(Pinled,1);
        lum=0;
        screen.background(lum,lum,lum);
        ecran(humeur);
      }
      l=0;
      m=2;
    }
  }
  else if(m==2){
    Serial.print("Temperature = ");
    Serial.print(bmp280.readTemperatureValue());
    Serial.println(" *C");
    Serial.print("Pressure = ");
    Serial.print(bmp280.readPressureValue());
    Serial.println(" Pa");
    Serial.print("Altitude = ");
    Serial.print(bmp280.readAltitudeValue(1013.25)); // this should be adjusted to your local forcase
    Serial.println(" m");
    Serial.println();
    m=3;
  }
  else if(m==3){
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);  // read measurements from device
    Serial.print("a/g:\t");                              // display tab-separated accel/gyro x/y/z values
    Serial.print(ax); 
    Serial.print("\t");
    Serial.print(ay); 
    Serial.print("\t");
    Serial.print(az); 
    Serial.print("\t");
    Serial.print(gx); 
    Serial.print("\t");
    Serial.print(gy); 
    Serial.print("\t");
    Serial.println(gz);
 
    blinkState = !blinkState;                      // blink LED to indicate activity
    digitalWrite(LED_PIN, blinkState);
    m=0;
  }
  else if(millis()-temps>3000||temps>millis()){
    temps=millis();
    m=1;
  }
  

  delay(50);
}



//*****Declaration de nos fonction*****//





#define TONE_TIMER TC1
#define TONE_CHNL 0
#define TONE_IRQ TC3_IRQn

// TIMER_CLOCK4   84MHz/128 with 16 bit counter give 10 Hz to 656KHz
//  piano 27Hz to 4KHz

static uint8_t pinEnabled[PINS_COUNT];
static uint8_t TCChanEnabled = 0;
static boolean pin_state = false ;
static Tc *chTC = TONE_TIMER;
static uint32_t chNo = TONE_CHNL;

volatile static int32_t toggle_count;
static uint32_t tone_pin;

// frequency (in hertz) and duration (in milliseconds).

void tone(uint32_t ulPin, uint32_t frequency, int32_t duration)
{
		const uint32_t rc = VARIANT_MCK / 256 / frequency;
		tone_pin = ulPin;
		toggle_count = 0;  // strange  wipe out previous duration
		if (duration > 0 ) toggle_count = 2 * frequency * duration / 1000;
		else toggle_count = -1;

		if (!TCChanEnabled) {
			pmc_set_writeprotect(false);
			pmc_enable_periph_clk((uint32_t)TONE_IRQ);
			TC_Configure(chTC, chNo,
				TC_CMR_TCCLKS_TIMER_CLOCK4 |
				TC_CMR_WAVE |         // Waveform mode
				TC_CMR_WAVSEL_UP_RC ); // Counter running up and reset when equals to RC
	
			chTC->TC_CHANNEL[chNo].TC_IER=TC_IER_CPCS;  // RC compare interrupt
			chTC->TC_CHANNEL[chNo].TC_IDR=~TC_IER_CPCS;
			NVIC_EnableIRQ(TONE_IRQ);
                         TCChanEnabled = 1;
		}
		if (!pinEnabled[ulPin]) {
			pinMode(ulPin, OUTPUT);
			pinEnabled[ulPin] = 1;
		}
		TC_Stop(chTC, chNo);
                TC_SetRC(chTC, chNo, rc);    // set frequency
		TC_Start(chTC, chNo);
}

void noTone(uint32_t ulPin)
{
	TC_Stop(chTC, chNo);  // stop timer
	digitalWrite(ulPin,LOW);  // no signal on pin
}

// timer ISR  TC1 ch 0
void TC3_Handler ( void ) {
	TC_GetStatus(TC1, 0);
	if (toggle_count != 0){
		// toggle pin  TODO  better
		digitalWrite(tone_pin,pin_state= !pin_state);
		if (toggle_count > 0) toggle_count--;
	} else {
		noTone(tone_pin);
	}
}

void ecran(int smile){
  if(smile==0){
    //smile contant
    screen.fill(0,255,0); //fond vert
    screen.circle(screen.width()/2.1, screen.height()/2, 60);  //gros cercle
    screen.fill(0,0,0); //noir
    screen.circle(screen.width()/1.5, screen.height()/1.5, 15); //extérieur oeil gauche
    screen.fill(0,0,0);  //noir
    screen.circle(screen.width()/3.5, screen.height()/1.5, 15);  //extérieur oeil droit
    screen.fill(255,255,255);  //blanc
    screen.circle(screen.width()/3.5, screen.height()/1.5, 5);  //intérieur oeil gauche 
    screen.fill(255,255,255);  //blanc
    screen.circle(screen.width()/1.5, screen.height()/1.5, 5);  //intérieur oeil droit
    screen.fill(0,0,0);  //noir
    screen.circle(screen.width()/2.1, screen.height()/3, 25);  //sourire
    screen.fill(0,255,0);  //vert
    screen.circle(screen.width()/2.1, screen.height()/2.7, 25);  //forme du sourire
    }
  else{
    //smile pas contant
    screen.fill(255,0,0);
    screen.circle(screen.width()/2.1, screen.height()/2, 60);
    // screen.circle(160/2.1, 150/2, 60);
    screen.fill(0,0,0);
    screen.circle(screen.width()/1.5, screen.height()/1.5, 15);
    screen.fill(0,0,0);
    screen.circle(screen.width()/3.5, screen.height()/1.5, 15);
    screen.fill(255,255,255);
    screen.circle(screen.width()/3.5, screen.height()/1.5, 5);
    screen.fill(255,255,255);
    screen.circle(screen.width()/1.5, screen.height()/1.5, 5);
    screen.fill(0,0,0);
    screen.circle(screen.width()/2.1, screen.height()/3, 25);
    screen.fill(255,0,0);
    screen.circle(screen.width()/2.1, screen.height()/3.5, 25);
    }
  }



