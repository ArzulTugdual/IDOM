#define boutonS1 S1
#define boutonS2 S2
#define boutonS3 S3
#define boutonS4 S4
#define boutonS5 S5
#define boutonS6 S6

void setup()
{
    Serial.begin(9600);//test
     
    pinMode(boutonS1,INPUT);
    pinMode(boutonS2,INPUT);
    pinMode(boutonS3,INPUT);
    pinMode(boutonS4,INPUT);
    pinMode(boutonS5,INPUT);
    pinMode(boutonS6,INPUT);
}
void loop()
{
    boolean etatS1=digitalRead(boutonS1);
    boolean etatS2=digitalRead(boutonS2);
    boolean etatS3=digitalRead(boutonS3);
    boolean etatS4=digitalRead(boutonS4);
    boolean etatS5=digitalRead(boutonS5);
    boolean etatS6=digitalRead(boutonS6);
    
    if(etatS1){                               //mode boutons (figures)
    Serial.println("Mode Boutons");//test
        if(etatS1 && etatS2){
            //appel fonction carré sans rotation
            Serial.println("Carré sans rotation");//test
        }
        else if(etatS1 && etatS3){
            //appel fonction carré avec rotation
            Serial.println("Carré avec rotation");//test
        }
        else if(etatS1 && etatS4){
            //appel fonction cercle
            Serial.println("Cercle");//test
        }
        else if(etatS1 && etatS5){
            //appel fonction triangle
            Serial.println("Triangle");//test
        }
        else if(etatS1 && etatS6){
            //appel fonction losange
            Serial.println("Losange");//test
        }
    }
    else{                                      //mode manuel (joystick)
    Serial.println("Mode Joystick");//test
    }
}
