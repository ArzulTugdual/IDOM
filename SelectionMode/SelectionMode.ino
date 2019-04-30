#define boutonS1_5 A7
#define boutonS6 2
int etatS6 = 0;              //Variable pour l'état actuel du bouton poussoir
int etatS6_prec = 0;         //Variable pour l'état précédent du bouton poussoir

void setup()
{
    Serial.begin(9600);//test
     
    pinMode(boutonS1_5,INPUT);//pin correspondant aux figures programmées sur les boutons S1 à S5
    pinMode(boutonS6,INPUT);//pin correspondant au mode joystick quand à 0 et au mode figure quant à 1
}

int Bouton(){
    int bouton = (analogRead(boutonS1_5)+66)/145;//renvoie la valeur du bouton pressé
    return bouton;
}

void loop()
{
    // lit l'état actuel du bouton poussoir S6 et le mémorise dans la variable
    etatS6 = digitalRead(boutonS6);
    int bouton = Bouton();

    if(etatS6 != etatS6_prec){
    if(etatS6==HIGH){                               //mode boutons (figures)
    Serial.println("Mode Boutons");//test
    delay(1000);
    /*    if(etatS6==HIGH && bouton==0){
            //appel fonction carré sans rotation
            Serial.println("Carré sans rotation");//test
            delay(1000);
        }
        else if(etatS6==HIGH && bouton==1){
            //appel fonction carré avec rotation
            Serial.println("Carré avec rotation");//test
            delay(1000);
        }
        else if(etatS6==HIGH && bouton==2){
            //appel fonction cercle
            Serial.println("Cercle");//test
            delay(1000);
        }
        else if(etatS6==HIGH && bouton==3){
            //appel fonction triangle
            Serial.println("Triangle");//test
            delay(1000);
        }
        else if(etatS6==HIGH && bouton==5){
            //appel fonction losange
            Serial.println("Losange");//test
            delay(1000);
        }*/
    etatS6_prec = HIGH;
    }
    }
    if(etatS6 != etatS6_prec){
    if(etatS6==LOW){                                 //mode manuel (joystick)
    Serial.println("Mode Joystick");//test
    delay(1000);
    }
    etatS6_prec = LOW;
    }
}
