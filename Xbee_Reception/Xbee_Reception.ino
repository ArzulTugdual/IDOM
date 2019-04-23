#include <SoftwareSerial.h>
#include <XBee.h>

int val;
SoftwareSerial xbee(3,2);

void setup(){
            Serial.begin(9600);
           }
           
void loop(){
            int Byte;
            if(Serial.available()){
                                   int dataByte = Serial.read();
                                   val = dataByte;
                                   Serial.print(val,Byte);
                                   }
                                   

}
