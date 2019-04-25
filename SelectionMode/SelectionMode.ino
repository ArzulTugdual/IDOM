#define boutonS1 S1
#define boutonS2 S2
#define boutonS3 S3
#define boutonS4 S4
#define boutonS5 S5
#define boutonS6 S6

void setup()
{
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
        if(etatS1 && etatS2){
            //appel fonction carrée sans rotation
        }
        else if(etatS1 && etatS3){
            //appel fonction carrée avec rotation
        }
        else if(etatS1 && etatS4){
            //appel fonction cercle
        }
        else if(etatS1 && etatS5){
            //appel fonction triangle
        }
        else if(etatS1 && etatS6){
            //appel fonction losange
        }
    }
    else{                                      //mode manuel (joystick)
        
    }
}
