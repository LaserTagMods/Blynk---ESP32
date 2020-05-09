/*void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Hello from ESP32");
  delay(3000);
}*/
#include <HardwareSerial.h>
HardwareSerial SerialBT( 1 );

 
void setup() {
  // Note the format for setting a serial port is as follows: Serial2.begin(baud-rate, protocol, RX pin, TX pin);
  Serial.begin(9600);
  //Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2);
  SerialBT.begin(9600, SERIAL_8N1, 16, 17);
}
 
void loop() { //Choose Serial1 or Serial2 as required
  while (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
  while(Serial.available()){
    SerialBT.write(Serial.read());
  }
}
