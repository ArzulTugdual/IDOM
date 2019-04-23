#include <XBee.h>
#include <SoftwareSerial.h>

SoftwareSerial xbee(2,3);
char val;
void setup() {
              Serial.begin(9600);
              xbee.begin(9600);
              }
void loop() {
              if(Serial.available()){
                                     val=Serial.read();
                                     Serial.println(val);
                                     xbee.print(val);
                                     delay(150);
                                     }
             
              else{
                    Serial.write(0);
                    }
   
}
