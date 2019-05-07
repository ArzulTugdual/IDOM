#define boutonS1_5 A7
#define boutonS6 2
int etatS6 = HIGH;   //Variable pour l'état actuel du bouton poussoir
int flag=0;
int ind=1;

void setup()
{
    Serial.begin(9600);  //test
     
    pinMode(boutonS1_5,INPUT);  //pin correspondant aux figures programmées sur les boutons S1 à S5
    pinMode(boutonS6,INPUT);  //pin correspondant au mode joystick quand à 0 et au mode figure quant à 1
}

int Bouton(){
    int bouton = (analogRead(boutonS1_5)+66)/145;  //renvoie la valeur du bouton pressé
    return bouton;
}

void loop()
{
    // lit l'état actuel du bouton poussoir S6 et le mémorise dans la variable
    etatS6 = digitalRead(boutonS6);
    int bouton = Bouton();

    //flag qui permet de différencier les 2 modes
    /*if(etatS6==HIGH){
        flag=flag*(-1);
    }
    if(etatS6==LOW){
        flag=flag*(1);
    }*//*
    if(etatS6==HIGH){
      if(f1%2==0){
        flag=flag*(-1);
        f1++;
      }
      else{
        flag=flag*(1);
        f1++;
      }
    }*/
    /*if(etatS6==LOW){
        flag=flag*(1);
    }*/

    if(etatS6==LOW){
        ind==ind*(-1);
    }
    
    /*  Mode BOUTONS   */
    if(etatS6==HIGH && ind==1){
    Serial.println("Mode Boutons");//test
    delay(1000);
    /*    if(etatS6==HIGH && bouton==0){
            //appel fonction carré sans rotation
            Serial.println("->  Carré sans rotation");//test
            delay(1000);
        }
        else if(etatS6==HIGH && bouton==1){
            //appel fonction carré avec rotation
            Serial.println("->  Carré avec rotation");//test
            delay(1000);
        }
        else if(etatS6==HIGH && bouton==2){
            //appel fonction cercle
            Serial.println("->  Cercle");//test
            delay(1000);
        }
        else if(etatS6==HIGH && bouton==3){
            //appel fonction triangle
            Serial.println("->  Triangle");//test
            delay(1000);
        }
        else if(etatS6==HIGH && bouton==5){
            //appel fonction losange
            Serial.println("->  Losange");//test
            delay(1000);
        }*/
    }
    

    /*  Mode JOYSTICK   */
    if(etatS6==HIGH && ind==-1){
        Serial.println("Mode Joystick");//test
        delay(1000);
    }
}
