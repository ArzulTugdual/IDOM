int analogPin = A7;
int digitalPin = 2;
int val = 0;
int val2 = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  val = analogRead(analogPin);
  val2 = digitalRead(digitalPin);
  Serial.print("val = ");
  Serial.println(val);
  Serial.print("val2 = ");
  Serial.println(val2);
  
}
