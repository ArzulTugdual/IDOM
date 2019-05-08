#define boutonS1_5 A7
int flag=1;

void setup()
{
    Serial.begin(9600);  //test
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
        Serial.println("->  Joystick");//test
        delay(1000);
    }
    /*  Mode BOUTONS   */
    else if(flag==1){   
    if(bouton==1){                /* Bouton S2 */
        //appel fonction carré
        Serial.println("->  Carré");//test
        delay(1000);
    }
    else if(bouton==2){           /* Bouton S3 */
        //appel fonction cercle
        Serial.println("->  Cercle");//test
        delay(1000);
    }
    else if(bouton==3){           /* Bouton S4 */
        //appel fonction triangle
        Serial.println("->  Triangle");//test
        delay(1000);
    }
    else if(bouton==5){           /* Bouton S5 */
        //appel fonction losange
        Serial.println("->  Losange");//test
        delay(1000);
    }
    }
}
