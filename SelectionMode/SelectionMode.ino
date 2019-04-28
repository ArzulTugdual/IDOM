#define boutonS1_5 A7
#define boutonS6 2

void setup()
{
    Serial.begin(9600);//test
     
    pinMode(boutonS1_5,INPUT);//pin correspondant aux figures programmées sur les boutons S1 à S5
    pinMode(boutonS6,INPUT);//pin correspondant au mode joystick quand à 0 et au mode figure quant à 1
}

int Bouton(){
    int bouton = (analogRead(boutonS1_5)+102)/204;//renvoie la valeur du bouton pressé
    return bouton;
}

void loop()
{
    boolean etatS6 = digitalRead(boutonS6);
    int bouton = Bouton();

    if(etatS6==LOW){                                      //mode manuel (joystick)
    Serial.println("Mode Joystick");//test
    }
    else if(etatS6==HIGH){                               //mode boutons (figures)
    Serial.println("Mode Boutons");//test
        if(etatS6==HIGH && bouton==1){
            //appel fonction carré sans rotation
            Serial.println("Carré sans rotation");//test
        }
        else if(etatS6==HIGH && bouton==2){
            //appel fonction carré avec rotation
            Serial.println("Carré avec rotation");//test
        }
        else if(etatS6==HIGH && bouton==3){
            //appel fonction cercle
            Serial.println("Cercle");//test
        }
        else if(etatS6==HIGH && bouton==4){
            //appel fonction triangle
            Serial.println("Triangle");//test
        }
        else if(etatS6==HIGH && bouton==5){
            //appel fonction losange
            Serial.println("Losange");//test
        }
    }
}
