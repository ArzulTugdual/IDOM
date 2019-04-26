#define boutonS15 A7
#define boutonS6 2

void setup()
{
    Serial.begin(9600);//test
     
    pinMode(boutonS15,INPUT);//pin correspondant aux figures programmées sur les boutons S1 à S5
    pinMode(boutonS6,INPUT);//pin correspondant au mode joystick quand à 0 et au mode figure quant à 1
}

int Bouton(){
    float etatS15=analogRead(boutonS15);
    int bouton;
    if((etatS15)>=0 && (etatS15)<=1){
      bouton=1;
    }
    else if((etatS15)>1 && (etatS15)<=2){
      bouton=2;
    }
    else if((etatS15)>2 && (etatS15)<=3){
      bouton=3;
    }
    else if((etatS15)>3 && (etatS15)<=4){
      bouton=4;
    }
    else if((etatS15)>4 && (etatS15)<=5){
      bouton=5;
    }
    return bouton;
}

void loop()
{
    const float etatS15=analogRead(boutonS15);
    boolean etatS6=digitalRead(boutonS6);
    const int bouton = Bouton();
    
    if(etatS6==HIGH){                               //mode boutons (figures)
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
    else if(etatS6==LOW){                                      //mode manuel (joystick)
    Serial.println("Mode Joystick");//test
    }
}
