/* ESP8266 Laser Tag Game Controller And Comms Device
 *  
 *  **************************************************
 *  this sketch is for an esp8266 to work in conjunction with an esp32
 *  the esp8266 is used for obtaining network/server data and relaying
 *  that data to the BLE paired ESP32 with a laser tag gun
 *  ***************************************************
 *  Instruction: 
 *  Connect the pin D1 to the SLC input of the LCD
 *  Connect The Pin D2 to the SDA input of the LCD
 *  Connect 5v or 3.3v to the VCC of the LCD
 *  Connect Grnd pin to the Grnd pin of the LCD
 *  (this sketch calls for a 4 x 20 LCD sketch with an I2C at address 0x27)
 *  Connect Pin D7 to Pin 17 of the ESP32 and D8 to Pin 16 of the ESP32
 *  If the ESP8266 is powered by the ESP32, then the Vcc and GRND pins should be connected as well  
 */ 

// Libraries needed to accomodate sketch
#include <LiquidCrystal_I2C.h> // used to write to LCD screen
#include <SoftwareSerial.h> // used to write serial comms to esp32

// set the LCD number of columns and rows (for a 4x20 LCD)
int lcdColumns = 20;
int lcdRows = 4;

// Variables needed:
int ToESP32=0;
String tokenStrings[6];
unsigned long startt = millis();

#ifndef RX
#define RX 13
#endif
#ifndef TX
#define TX 15
#endif

//SoftwareSerial SerialLCD(RX, TX, false, 256); //nodemcu D8-rx D7-tx
SoftwareSerial SerialLCD;
constexpr SoftwareSerialConfig swSerialConfig = SWSERIAL_8N1;
// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

//******************************************************************
// object used to receive data from ESP32 BLE device
void ReadESP32Data() {
if (SerialLCD.available()) {
    String readStr = SerialLCD.readStringUntil('\n');
    char *ptr = strtok((char*)readStr.c_str(), ",");
    int index = 0;
    while (ptr != NULL)
    {
      tokenStrings[index] = ptr;
      index++;
      ptr = strtok(NULL, ",");  // takes a list of delimiters
    }
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Ammo:");
    Serial.println("Ammo: " + String(tokenStrings[0]));
    lcd.print(tokenStrings[0]);
    lcd.setCursor(10,0);
    lcd.print("Health:");
    Serial.println("HP: " + String(tokenStrings[2]));
    lcd.print(tokenStrings[2]);
    lcd.setCursor(0,1);
    lcd.print("Armor:");
    lcd.print(tokenStrings[3]);
    lcd.setCursor(0,2);
    Serial.println("Armor: " + String(tokenStrings[3]));
    lcd.print("Shiled:");
    lcd.print(tokenStrings[4]);
    Serial.println("Shield: " + String(tokenStrings[4]));
    lcd.setCursor(0,3);
    lcd.print("Lives:");
    tokenStrings[5] = tokenStrings[5] + "\0";
    lcd.print(tokenStrings[5]);
    Serial.println("Lives: " + String(tokenStrings[5]));
    //ammo,weap,health,armor,shield,lives
  }
}
//****************************************************************
// object used to send data to esp32 BLE device
void SendESP32Data() {
  if (millis() - startt > 5000) {
    SerialLCD.println(ToESP32);
    startt = millis();
  }
}
//***************************************************************


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //SerialLCD.begin(9600);
  SerialLCD.begin(9600, swSerialConfig, RX, TX);
  // initialize LCD
  lcd.init();
  // turn on LCD backlight
  lcd.backlight();
  lcd.print("Welcome to Battle Company!");
}

void loop() {
  ReadESP32Data();
  SendESP32Data();
}
