#include <SoftwareSerial.h>
#include <XBee.h>

SoftwareSerial xbee(3,2);

void setup(){
  xbee.begin(9600);
  Serial.begin(9600);
}
           
void loop(){
  if(xbee.available()){
    while(xbee.available())  {
      Serial.write(xbee.read());
    }
  }
}
