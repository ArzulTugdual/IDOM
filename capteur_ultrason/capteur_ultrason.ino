const byte PIN = 6;     // Broche capteur ultrason (envoie/réception) attention, pin 6 car non utilisé par le robot

void setup() 
{
  Serial.begin(9600);
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN, LOW); // La broche doit être à LOW au repos
}

void loop()
{
  Serial.print(mesure_distance());                 
  Serial.println("mm");                          //lecture de la valeur;
  delay(1000);
}

/*mesure la distance entre le robot et le prochain obstacle devant*/
float mesure_distance()
{
  //capteur parallax (3 broches)
  /*création des variables de résultat*/
  float measure, distance_mm;
  //float c = 20.05*sqrt(temp)*pow(10,-3);   //vitesse du son dans l'air en fonction de la température en mm/microsec
  float c = 0.34;
  
  pinMode(PIN, OUTPUT);       //broche en sortie pour envoyer l'impulsion
  digitalWrite(PIN, LOW);     // La broche doit être à LOW au départ
  
  /*impulsion de 10 micros*/
  digitalWrite(PIN, HIGH);      
  delayMicroseconds(10);
  digitalWrite(PIN, LOW);       //envoi de l'onde sonore ___---___

  pinMode(PIN, INPUT);          //broche en entrée en attente de l'impulsion
  
  measure = pulseIn(PIN, HIGH, 25000UL);  //attente du signal retour puis calcul du temps de réponse en microsec
  return measure / 2.0 * c;               //MESURE en mm
}
