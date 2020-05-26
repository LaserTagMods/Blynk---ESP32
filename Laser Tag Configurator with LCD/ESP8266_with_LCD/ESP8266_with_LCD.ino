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
 *  If the ESP8266 is powered by the ESP32, then the Vcc and GRND pins should
 *  be connected as well
 *  
 *  ****************************************************
 *  Updates and logs: (date - individual - changes)
 *  
 *  4/3/2020 - Jay - started incorporating and testing blynk in code, added annotations
 *  4/3/2020 - Jay - added blynk write commands
 *  4/3/2020 - Jay - Reconfigured sketch for running on local blynk server
 *  4/8/2020 - Jay - Added volume control for taggers, also added battle royale mode selection for future use
 *  4/10/2020 - Jay - Added serial print indicators to troubleshoot the receiving of data from esp32, having trouble processing data properly
 *  4/10/2020 - Jay - Cleaned up LCD printing data and added variables for desired status updates for LCD
 *  4/13/2020 - Jay - Cleaned up LCD printing data even more in debuging data sent to LCD
 *  4/15/2020 - Jay - Adjusted Ammunition settings to match Blynk update to have three setting options for the option to ensure there are options ;)
 *  4/16/2020 - Jay - enabled wifi out of range functionality, so that players can leave network and no errors occur in reading and processing data from devices
 *  4/19/2020 - Jay - edited game start serial monitor output only
 *  4/22/2020 - Jay - added code to identify if esp32 is sending weapon selection data to display on lcd
 *  4/27/2020 - Jay - finished the processing of the sync score command, request data from esp32, recieve data, process scores, send via Bridge to control device (Primary Game Hub) limited to 52 scores for players because of limited Virtual Pins (alternatively a second Device could be used to breakd up the score reporting.
 *  5/5/2020 - Jay - added lockout function when game is engaged so that commands cannot be sent except for end game after pressing start game.
 *  5/25/2020 - Jay - added blynk enabled button lockout instead of auto lockout from esp32 connection (see 1801)
 *  5/26/2020 - Jay - modified scoring reporting to function with new score accumulator device code
 */ 

/* 
 *  Section intended for logging of action variable and intended results when
 *  transmitted to ESP32, using variable "ToESP32". 
 *  
 *  Each time a value is sent to the ESP32 the value needs to be reset to "0" or default
 *  
 *  Format: (Variable Value to be sent - Intended Action to be processed by ESP32)
Format below: (ToESP32 value)(Setting)(Selection) (Virtual Pin) (Pin Value)
0   Do nothing    None    N/A   N/A
1   Weapon Slot 0 =   Player's Choice   V0    1
2   Weapon Slot 0 =   Unarmed   V0    2
3   Weapon Slot 0 =   AMR   V0    3
4   Weapon Slot 0 =   Assault Rifle   V0    4
5   Weapon Slot 0 =   Bolt Rifle    V0    5
6   Weapon Slot 0 =   BurstRifle    V0    6
7   Weapon Slot 0 =   ChargeRifle   V0    7
8   Weapon Slot 0 =   Energy Launcher   V0    8
9   Weapon Slot 0 =   Energy Rifle    V0    9
10    Weapon Slot 0 =   Force Rifle   V0    10
11    Weapon Slot 0 =   Ion Sniper    V0    11
12    Weapon Slot 0 =   Laser Cannon    V0    12
13    Weapon Slot 0 =   Plasma Sniper   V0    13
14    Weapon Slot 0 =   Rail Gun    V0    14
15    Weapon Slot 0 =   Rocket Launcher   V0    15
16    Weapon Slot 0 =   Shotgun   V0    16
17    Weapon Slot 0 =   SMG   V0    17
18    Weapon Slot 0 =   Sniper Rifle    V0    18
19    Weapon Slot 0 =   Stinger   V0    19
20    Weapon Slot 0 =   Suppressor    V0    20
101   Weapon Slot 0 =   Player's Choice   V1    1
102   Weapon Slot 0 =   Unarmed   V1    2
103   Weapon Slot 0 =   AMR   V1    3
104   Weapon Slot 0 =   Assault Rifle   V1    4
105   Weapon Slot 0 =   Bolt Rifle    V1    5
106   Weapon Slot 0 =   BurstRifle    V1    6
107   Weapon Slot 0 =   ChargeRifle   V1    7
108   Weapon Slot 0 =   Energy Launcher   V1    8
109   Weapon Slot 0 =   Energy Rifle    V1    9
110   Weapon Slot 0 =   Force Rifle   V1    10
111   Weapon Slot 0 =   Ion Sniper    V1    11
112   Weapon Slot 0 =   Laser Cannon    V1    12
113   Weapon Slot 0 =   Plasma Sniper   V1    13
114   Weapon Slot 0 =   Rail Gun    V1    14
115   Weapon Slot 0 =   Rocket Launcher   V1    15
116   Weapon Slot 0 =   Shotgun   V1    16
117   Weapon Slot 0 =   SMG   V1    17
118   Weapon Slot 0 =   Sniper Rifle    V1    18
119   Weapon Slot 0 =   Stinger   V1    19
120   Weapon Slot 0 =   Suppressor    V1    20
225   Objective/Goals =   25    V2    25
224   Objective/Goals =   24    V2    24
223   Objective/Goals =   23    V2    23
222   Objective/Goals =   22    V2    22
221   Objective/Goals =   21    V2    21
220   Objective/Goals =   20    V2    20
219   Objective/Goals =   19    V2    19
218   Objective/Goals =   18    V2    18
217   Objective/Goals =   17    V2    17
216   Objective/Goals =   16    V2    16
215   Objective/Goals =   15    V2    15
214   Objective/Goals =   14    V2    14
213   Objective/Goals =   13    V2    13
212   Objective/Goals =   12    V2    12
211   Objective/Goals =   11    V2    11
210   Objective/Goals =   10    V2    10
209   Objective/Goals =   9   V2    9
208   Objective/Goals =   8   V2    8
207   Objective/Goals =   7   V2    7
206   Objective/Goals =   6   V2    6
205   Objective/Goals =   5   V2    5
204   Objective/Goals =   4   V2    4
203   Objective/Goals =   3   V2    3
202   Objective/Goals =   2   V2    2
201   Objective/Goals =   1   V2    1
200   Objective/Goals =   Unlimited   V2    0
325   Kills to Win    25    V3    25
324   Kills to Win    24    V3    24
323   Kills to Win    23    V3    23
322   Kills to Win    22    V3    22
321   Kills to Win    21    V3    21
320   Kills to Win    20    V3    20
319   Kills to Win    19    V3    19
318   Kills to Win    18    V3    18
317   Kills to Win    17    V3    17
316   Kills to Win    16    V3    16
315   Kills to Win    15    V3    15
314   Kills to Win    14    V3    14
313   Kills to Win    13    V3    13
312   Kills to Win    12    V3    12
311   Kills to Win    11    V3    11
310   Kills to Win    10    V3    10
309   Kills to Win    9   V3    9
308   Kills to Win    8   V3    8
307   Kills to Win    7   V3    7
306   Kills to Win    6   V3    6
305   Kills to Win    5   V3    5
304   Kills to Win    4   V3    4
303   Kills to Win    3   V3    3
302   Kills to Win    2   V3    2
301   Kills to Win    1   V3    1
300   Kills to Win    Unlimited   V3    0
425   Lives   25    V4    25
424   Lives   24    V4    24
423   Lives   23    V4    23
422   Lives   22    V4    22
421   Lives   21    V4    21
420   Lives   20    V4    20
419   Lives   19    V4    19
418   Lives   18    V4    18
417   Lives   17    V4    17
416   Lives   16    V4    16
415   Lives   15    V4    15
414   Lives   14    V4    14
413   Lives   13    V4    13
412   Lives   12    V4    12
411   Lives   11    V4    11
410   Lives   10    V4    10
409   Lives   9   V4    9
408   Lives   8   V4    8
407   Lives   7   V4    7
406   Lives   6   V4    6
405   Lives   5   V4    5
404   Lives   4   V4    4
403   Lives   3   V4    3
402   Lives   2   V4    2
401   Lives   1   V4    1
400   Lives   Unlimited   V4    0
501   Game Time   1 Minute    V5    1
502   Game Time   5 Minutes   V5    2
503   Game Time   10 Minutes    V5    3
504   Game Time   15 Minutes    V5    4
505   Game Time   20 Minutes    V5    5
506   Game Time   25 Minutes    V5    6
507   Game Time   30 Minutes    V5    7
508   Game Time   Unlimited   V5    8
601   Lighting/Ambience   Outdoor Mode    V6    1
602   Lighting/Ambience   Indoor Mode   V6    2
603   Lighting/Ambience   Stealth   V6    3
701   Teams   Free For All    V7    1
702   Teams   Two Teams (odds/evens)    V7    2
703   Teams   Three Teams (every three)   V7    3
704   Teams   Four Teams (every four)   V7    4
705   Teams   Player's Choice   V7    5
801   Game Mode   Death Match   V8    1
802   Game Mode   Capture the Flag    V8    2
803   Game Mode   Assault   V8    3
804   Game Mode   King of the Hill    V8    4
805   Game Mode   Survival    V8    5
806   Game Mode   Troule in Terrorist Town    V8    6
807   Game Mode   You only Live Twice   V8    7
808   Game Mode   One Shot Kills (pistols)    V8    8
809   Game Mode   Gun Game    V8    9
810   Game Mode   Domination    V8    10
901   Respawn   Immediate (auto)    V9    1
902   Respawn   15 seconds (auto)   V9    2
903   Respawn   30 seconds (auto)   V9    3
904   Respawn   45 seconds  (auto)    V9    4
905   Respawn   60 seconds (auto)   V9    5
906   Respawn   90 seconds (auto)   V9    6
907   Respawn   Ramp 45 (auto)    V9    7
908   Respawn   Ramp 90 (auto)    V9    8
909   Respawn   Respawn Station (manual)    V9    9
1001    Delayed Start   Immediate   V10   1
1002    Delayed Start   15 seconds    V10   2
1003    Delayed Start   30 seconds    V10   3
1004    Delayed Start   45 seconds    V10   4
1005    Delayed Start   60 seconds    V10   5
1006    Delayed Start   90 seconds    V10   6
1007    Delayed Start   5 Minutes   V10   7
1008    Delayed Start   10 Minutes    V10   8
1009    Delayed Start   15 Minutes    V10   9
1101    Special Abilities   item 1    V11   1
1102    Special Abilities   item 2    V11   2
1200    Player Gender   Male    V12   0
1201    Player Gender   Female    V12   1
1300    Ammo Settings   Unlimited   V13   0
1301    Ammo Settings   Limited   V13   1
1400    Friendly Fire   Off   V14   0
1401    Friendly Fire   On    V14   1
1500    Reserved    item    V15   0
1501    Reserved    item    V15   1
1600    Start Game    unpressed   V16   0
1601    Start Game    pressed   V16   1
 *  
 *  
 *  
 *  
 */
 
/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

// Libraries needed to accomodate sketch
#include <LiquidCrystal_I2C.h> // used to write to LCD screen
#include <SoftwareSerial.h> // used to write serial comms to esp32
#include <ESP8266WiFi.h> // used for setting up wifi comms
#include <BlynkSimpleEsp8266.h> // used to run all necessary blynk objects/functions

// set up bridge widget on V1 of Device 1
// this creates the widget needed to send data to the controller
WidgetBridge bridge1(V1);

//**********************************************************************************
//****************************  UPDATE THIS SECTION!!!!! ***************************
//**********************************************************************************
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
//char auth[] = "wHfjglHvyD4qBv7PB8C9DKTY-NNp8VGr";
char auth[] = "S6xltBO40T-9nhFavpR2Zsn8WXLTCeJx";
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "maxipad";
char pass[] = "9165047812";
// set the bridge token
BLYNK_CONNECTED() {
  bridge1.setAuthToken("nngeMu8Nz6CAjzmPfFR89d31VBoSSRff"); // Token of the device 2
}

//**********************************************************************************
//****************************  UPDATE THIS SECTION!!!!! ***************************
//**********************************************************************************

// set the LCD number of columns and rows (for a 4x20 LCD)
int lcdColumns = 20;
int lcdRows = 4;

// Variables needed:
int ToESP32=0;
String tokenStrings[8];
String ScoreTokenStrings[73];
/*
 * edited out for alternate conrols
long gametime=2000000000; // used as a timer for games
long gamestart=0; // used as a start for a game
long gamedelay=4000; // sets min delay for start a game time accumulation, only add to this value
*/

bool GAMESTART=false;
bool SYNCSCORE=false;
bool ENABLESERIAL=true;
bool LOCKOUT=false;

unsigned long startt = millis();

// timers for running certain applications periodically with the blynk program
BlynkTimer ESP32Send; // created a timer object called "ESP32Send"
BlynkTimer ESP32Read; // created a timer object called "ESP32Read"
BlynkTimer SyncScoreRead; // created a timer object called "SyncScoreRead"


#ifndef RX
#define RX 13
#endif
#ifndef TX
#define TX 15
#endif
int PowerPin = D7;  // Pin used to override the main power switch on tagger as a lockout function

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
    //ammo,weap,health,armor,shield,lives,magazineammo,GunID
    Serial.println("Data Recieved from ESP32");
    Serial.println(tokenStrings[0]+","+tokenStrings[1]+","+tokenStrings[2]+","+tokenStrings[3]+","+tokenStrings[4]+","+tokenStrings[5]+","+tokenStrings[6]+","+tokenStrings[7]);
    
    lcd.clear();

    if (tokenStrings[0] == "500") {
      lcd.setCursor(0,0);
      lcd.print("Choose a Weapon");
      lcd.setCursor(0,1);
      lcd.print("Weap:");
      if(tokenStrings[1] == "1") {lcd.print("Unarmed");}
      if(tokenStrings[1] == "2") {lcd.print("AMR");}
      if(tokenStrings[1] == "3") {lcd.print("Assault Rifle");}
      if(tokenStrings[1] == "4") {lcd.print("Bolt Rifle");}
      if(tokenStrings[1] == "5") {lcd.print("Burst Rifle");}
      if(tokenStrings[1] == "6") {lcd.print("Charge Rifle");}
      if(tokenStrings[1] == "7") {lcd.print("Enrg Launcher");}
      if(tokenStrings[1] == "8") {lcd.print("Enrg Rifle");}
      if(tokenStrings[1] == "9") {lcd.print("Force Rifle");}
      if(tokenStrings[1] == "10") {lcd.print("Ion Sniper");}
      if(tokenStrings[1] == "11") {lcd.print("Laser Cannon");}
      if(tokenStrings[1] == "12") {lcd.print("Plsm Sniper");}
      if(tokenStrings[1] == "13") {lcd.print("Rail Gun");}
      if(tokenStrings[1] == "14") {lcd.print("Rckt Launcher");}
      if(tokenStrings[1] == "15") {lcd.print("Shotgun");}
      if(tokenStrings[1] == "16") {lcd.print("SMG");}
      if(tokenStrings[1] == "17") {lcd.print("Sniper Rifle");}
      if(tokenStrings[1] == "18") {lcd.print("Stinger");}
      if(tokenStrings[1] == "19") {lcd.print("Suppressor");}
      lcd.setCursor(0,3);
      lcd.print("Confirm W/ Alt Fire");
    } else {

    lcd.setCursor(0,0);
    lcd.print("ID:");
    Serial.println("Player ID: " + String(tokenStrings[7]));
    lcd.print(tokenStrings[7]);

    lcd.setCursor(8,0);
    lcd.print("Ammo:");
    Serial.println("Ammo: " + String(tokenStrings[0]) + "/" + String(tokenStrings[6]));
    lcd.print(tokenStrings[0]);
    lcd.print("/");
    lcd.print(tokenStrings[6]);

    lcd.setCursor(0,1);
    lcd.print("Weap:");
    if(tokenStrings[1] == "1") {lcd.print("Unarmed");}
    if(tokenStrings[1] == "2") {lcd.print("AMR");}
    if(tokenStrings[1] == "3") {lcd.print("Assault Rifle");}
    if(tokenStrings[1] == "4") {lcd.print("Bolt Rifle");}
    if(tokenStrings[1] == "5") {lcd.print("Burst Rifle");}
    if(tokenStrings[1] == "6") {lcd.print("Charge Rifle");}
    if(tokenStrings[1] == "7") {lcd.print("Enrg Launcher");}
    if(tokenStrings[1] == "8") {lcd.print("Enrg Rifle");}
    if(tokenStrings[1] == "9") {lcd.print("Force Rifle");}
    if(tokenStrings[1] == "10") {lcd.print("Ion Sniper");}
    if(tokenStrings[1] == "11") {lcd.print("Laser Cannon");}
    if(tokenStrings[1] == "12") {lcd.print("Plsm Sniper");}
    if(tokenStrings[1] == "13") {lcd.print("Rail Gun");}
    if(tokenStrings[1] == "14") {lcd.print("Rckt Launcher");}
    if(tokenStrings[1] == "15") {lcd.print("Shotgun");}
    if(tokenStrings[1] == "16") {lcd.print("SMG");}
    if(tokenStrings[1] == "17") {lcd.print("Sniper Rifle");}
    if(tokenStrings[1] == "18") {lcd.print("Stinger");}
    if(tokenStrings[1] == "19") {lcd.print("Suppressor");}
    
    lcd.setCursor(0,2);
    lcd.print("HP:");
    Serial.println("Health: " + String(tokenStrings[2]));
    lcd.print(tokenStrings[2]);
    lcd.setCursor(8,2);
    lcd.print("Armr:");
    lcd.print(tokenStrings[3]);
    Serial.println("Armor: " + String(tokenStrings[3]));

    
    lcd.setCursor(0,3);
    lcd.print("Shld:");
    lcd.print(tokenStrings[4]);
    Serial.println("Shield: " + String(tokenStrings[4]));
    lcd.setCursor(8,3);
    lcd.print("Lives:");
    tokenStrings[5] = tokenStrings[5] + "\0";
    if(tokenStrings[5].toInt() < 100) {lcd.print(tokenStrings[5]);}
    else {lcd.print("Unlim");}
    Serial.println("Lives: " + String(tokenStrings[5]));
    //ammo,weap,health,armor,shield,lives,magazineammo,GunID
    }
  }
}

//******************************************************************
// object used to receive data from ESP32 BLE device
void ReadSyncScoreData() {
if (SerialLCD.available()) {
    String readStr = SerialLCD.readStringUntil('\n');
    char *ptr = strtok((char*)readStr.c_str(), ",");
    int index = 0;
    while (ptr != NULL)
    {
      ScoreTokenStrings[index] = ptr;
      index++;
      ptr = strtok(NULL, ",");  // takes a list of delimiters
    }
    // create a string that looks like this: 
    // (Player ID, token 0), (Player Team, token 1), (Player Objective Score, token 2) (Team scores, tokens 3-8), (player kill counts, tokens 9-72 
    Serial.println("Score Data Recieved from ESP32");
    Serial.println(readStr); // printing what was read from esp32
    SYNCSCORE=false;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Syncing Scores");

      int Deaths=0;
      int Data[64];
      int count=0;
      while (count<73) {
      Data[count]=ScoreTokenStrings[count].toInt();
      Serial.println("Converting String character "+String(count)+" to integer");
      count++;
      }
      Deaths = Data[3] + Data[4] + Data[5] + Data[6] + Data[7] + Data[8]; // accumulating the kill counts recorded from each team to determine total deaths
      // add all kills
      // send kills to lcd
      lcd.setCursor(0,1);
      lcd.print("Deaths: ");
      Serial.println("Player Deaths: "+String(Deaths));
      lcd.setCursor(0,2);
      lcd.print("Player Objectives completed: "+String(Data[2]));
      Serial.println("Player Objectives completed: "+String(Data[2]));
      // send scores to blynk accumulator device over bridge to be summed and then posted to blynk server/app
      // designating certain pins for score accumulation. 
      // Pins 0-63 are for player kill counts, this sends all player scores
      bridge1.virtualWrite(V0, readStr); // sending the whole string from esp32
      Serial.println("finished sending data to score reporting device");
        
        /*
         * not used since we are sending a string instead and this is individual score reporting methods
        bridge1.virtualWrite(V0, Data[9]); // sending the value to controller
        bridge1.virtualWrite(V1, Data[10]); // sending the value to controller
        bridge1.virtualWrite(V2, Data[11]); // sending the value to controller
        bridge1.virtualWrite(V3, Data[12]); // sending the value to controller
        bridge1.virtualWrite(V4, Data[13]); // sending the value to controller
        bridge1.virtualWrite(V5, Data[14]); // sending the value to controller
        bridge1.virtualWrite(V6, Data[15]); // sending the value to controller
        bridge1.virtualWrite(V7, Data[16]); // sending the value to controller
        bridge1.virtualWrite(V8, Data[17]); // sending the value to controller
        bridge1.virtualWrite(V9, Data[18]); // sending the value to controller
        bridge1.virtualWrite(V10, Data[19]); // sending the value to controller
        bridge1.virtualWrite(V11, Data[20]); // sending the value to controller
        bridge1.virtualWrite(V12, Data[21]); // sending the value to controller
        bridge1.virtualWrite(V13, Data[22]); // sending the value to controller
        bridge1.virtualWrite(V14, Data[23]); // sending the value to controller
        bridge1.virtualWrite(V15, Data[24]); // sending the value to controller
        bridge1.virtualWrite(V16, Data[25]); // sending the value to controller
        bridge1.virtualWrite(V17, Data[26]); // sending the value to controller
        bridge1.virtualWrite(V18, Data[27]); // sending the value to controller
        bridge1.virtualWrite(V19, Data[28]); // sending the value to controller
        bridge1.virtualWrite(V20, Data[29]); // sending the value to controller
        bridge1.virtualWrite(V21, Data[30]); // sending the value to controller
        bridge1.virtualWrite(V22, Data[31]); // sending the value to controller
        bridge1.virtualWrite(V23, Data[32]); // sending the value to controller
        bridge1.virtualWrite(V24, Data[33]); // sending the value to controller
        bridge1.virtualWrite(V25, Data[34]); // sending the value to controller
        bridge1.virtualWrite(V26, Data[35]); // sending the value to controller
        bridge1.virtualWrite(V27, Data[36]); // sending the value to controller
        bridge1.virtualWrite(V28, Data[37]); // sending the value to controller
        bridge1.virtualWrite(V29, Data[38]); // sending the value to controller
        bridge1.virtualWrite(V30, Data[39]); // sending the value to controller
        bridge1.virtualWrite(V31, Data[40]); // sending the value to controller
        bridge1.virtualWrite(V32, Data[41]); // sending the value to controller
        bridge1.virtualWrite(V33, Data[42]); // sending the value to controller
        bridge1.virtualWrite(V34, Data[43]); // sending the value to controller
        bridge1.virtualWrite(V35, Data[44]); // sending the value to controller
        bridge1.virtualWrite(V36, Data[45]); // sending the value to controller
        bridge1.virtualWrite(V37, Data[46]); // sending the value to controller
        bridge1.virtualWrite(V38, Data[47]); // sending the value to controller
        bridge1.virtualWrite(V39, Data[48]); // sending the value to controller
        bridge1.virtualWrite(V40, Data[49]); // sending the value to controller
        bridge1.virtualWrite(V41, Data[50]); // sending the value to controller
        bridge1.virtualWrite(V42, Data[51]); // sending the value to controller
        bridge1.virtualWrite(V43, Data[52]); // sending the value to controller
        bridge1.virtualWrite(V44, Data[53]); // sending the value to controller
        bridge1.virtualWrite(V45, Data[54]); // sending the value to controller
        bridge1.virtualWrite(V46, Data[55]); // sending the value to controller
        bridge1.virtualWrite(V47, Data[56]); // sending the value to controller
        bridge1.virtualWrite(V48, Data[57]); // sending the value to controller
        bridge1.virtualWrite(V49, Data[58]); // sending the value to controller
        bridge1.virtualWrite(V50, Data[59]); // sending the value to controller
        bridge1.virtualWrite(V51, Data[60]); // sending the value to controller
        bridge1.virtualWrite(V52, Data[61]); // sending the value to controller
        bridge1.virtualWrite(V53, Data[62]); // sending the value to controller
        bridge1.virtualWrite(V54, Data[63]); // sending the value to controller
        bridge1.virtualWrite(V55, Data[64]); // sending the value to controller
        bridge1.virtualWrite(V56, Data[65]); // sending the value to controller
        bridge1.virtualWrite(V57, Data[66]); // sending the value to controller
        bridge1.virtualWrite(V58, Data[67]); // sending the value to controller
        bridge1.virtualWrite(V59, Data[68]); // sending the value to controller
        bridge1.virtualWrite(V60, Data[69]); // sending the value to controller
        bridge1.virtualWrite(V61, Data[70]); // sending the value to controller
        bridge1.virtualWrite(V62, Data[71]); // sending the value to controller
        bridge1.virtualWrite(V63, Data[72]); // sending the value to controller
      // team kill scores (6 pins) are located on pins 64-69
        bridge1.virtualWrite(V64, Data[3]); // sending the value to controller
        bridge1.virtualWrite(V65, Data[4]); // sending the value to controller
        bridge1.virtualWrite(V66, Data[5]); // sending the value to controller
        bridge1.virtualWrite(V67, Data[6]); // sending the value to controller
        bridge1.virtualWrite(V68, Data[7]); // sending the value to controller
        bridge1.virtualWrite(V69, Data[8]); // sending the value to controller
      // now we need to send objective points for the teams. This will be on pins 70-75
      // we are only sending on one pin because were using the players objective points
      // to accumulate on the right teams pin.
        if (Data[2]==0) {bridge1.virtualWrite(V70, Data[1]);}  // sending the value to controller
        if (Data[2]==1) {bridge1.virtualWrite(V71, Data[1]);}  // sending the value to controller
        if (Data[2]==2) {bridge1.virtualWrite(V72, Data[1]);}  // sending the value to controller
        if (Data[2]==3) {bridge1.virtualWrite(V73, Data[1]);}  // sending the value to controller
        if (Data[2]==4) {bridge1.virtualWrite(V74, Data[1]);}  // sending the value to controller
        if (Data[2]==5) {bridge1.virtualWrite(V75, Data[1]);}  // sending the value to controller
      // now we send the player objective score, pins 76-139 are reserved for player objective scores
        if (Data[0]==0) {bridge1.virtualWrite(V76, Data[2]);}  // sending the value to controller
        if (Data[0]==1) {bridge1.virtualWrite(V77, Data[2]);}  // sending the value to controller
        if (Data[0]==2) {bridge1.virtualWrite(V78, Data[2]);}  // sending the value to controller
        if (Data[0]==3) {bridge1.virtualWrite(V79, Data[2]);}  // sending the value to controller
        if (Data[0]==4) {bridge1.virtualWrite(V80, Data[2]);}  // sending the value to controller
        if (Data[0]==5) {bridge1.virtualWrite(V81, Data[2]);}  // sending the value to controller
        if (Data[0]==6) {bridge1.virtualWrite(V82, Data[2]);}  // sending the value to controller
        if (Data[0]==7) {bridge1.virtualWrite(V83, Data[2]);}  // sending the value to controller
        if (Data[0]==8) {bridge1.virtualWrite(V84, Data[2]);}  // sending the value to controller
        if (Data[0]==9) {bridge1.virtualWrite(V85, Data[2]);}  // sending the value to controller
        if (Data[0]==10) {bridge1.virtualWrite(V86, Data[2]);}  // sending the value to controller
        if (Data[0]==11) {bridge1.virtualWrite(V87, Data[2]);}  // sending the value to controller
        if (Data[0]==12) {bridge1.virtualWrite(V88, Data[2]);}  // sending the value to controller
        if (Data[0]==13) {bridge1.virtualWrite(V89, Data[2]);}  // sending the value to controller
        if (Data[0]==14) {bridge1.virtualWrite(V90, Data[2]);}  // sending the value to controller
        if (Data[0]==15) {bridge1.virtualWrite(V91, Data[2]);}  // sending the value to controller
        if (Data[0]==16) {bridge1.virtualWrite(V92, Data[2]);}  // sending the value to controller
        if (Data[0]==17) {bridge1.virtualWrite(V93, Data[2]);}  // sending the value to controller
        if (Data[0]==18) {bridge1.virtualWrite(V94, Data[2]);}  // sending the value to controller
        if (Data[0]==19) {bridge1.virtualWrite(V95, Data[2]);}  // sending the value to controller
        if (Data[0]==20) {bridge1.virtualWrite(V96, Data[2]);}  // sending the value to controller
        if (Data[0]==21) {bridge1.virtualWrite(V97, Data[2]);}  // sending the value to controller
        if (Data[0]==22) {bridge1.virtualWrite(V98, Data[2]);}  // sending the value to controller
        if (Data[0]==23) {bridge1.virtualWrite(V99, Data[2]);}  // sending the value to controller
        if (Data[0]==24) {bridge1.virtualWrite(V100, Data[2]);}  // sending the value to controller
        if (Data[0]==25) {bridge1.virtualWrite(V101, Data[2]);}  // sending the value to controller
        if (Data[0]==26) {bridge1.virtualWrite(V102, Data[2]);}  // sending the value to controller
        if (Data[0]==27) {bridge1.virtualWrite(V103, Data[2]);}  // sending the value to controller
        if (Data[0]==28) {bridge1.virtualWrite(V104, Data[2]);}  // sending the value to controller
        if (Data[0]==29) {bridge1.virtualWrite(V105, Data[2]);}  // sending the value to controller
        if (Data[0]==30) {bridge1.virtualWrite(V106, Data[2]);}  // sending the value to controller
        if (Data[0]==31) {bridge1.virtualWrite(V107, Data[2]);}  // sending the value to controller
        if (Data[0]==32) {bridge1.virtualWrite(V108, Data[2]);}  // sending the value to controller
        if (Data[0]==33) {bridge1.virtualWrite(V109, Data[2]);}  // sending the value to controller
        if (Data[0]==34) {bridge1.virtualWrite(V110, Data[2]);}  // sending the value to controller
        if (Data[0]==35) {bridge1.virtualWrite(V111, Data[2]);}  // sending the value to controller
        if (Data[0]==36) {bridge1.virtualWrite(V112, Data[2]);}  // sending the value to controller
        if (Data[0]==37) {bridge1.virtualWrite(V113, Data[2]);}  // sending the value to controller
        if (Data[0]==38) {bridge1.virtualWrite(V114, Data[2]);}  // sending the value to controller
        if (Data[0]==39) {bridge1.virtualWrite(V115, Data[2]);}  // sending the value to controller
        if (Data[0]==40) {bridge1.virtualWrite(V116, Data[2]);}  // sending the value to controller
        if (Data[0]==41) {bridge1.virtualWrite(V117, Data[2]);}  // sending the value to controller
        if (Data[0]==42) {bridge1.virtualWrite(V118, Data[2]);}  // sending the value to controller
        if (Data[0]==43) {bridge1.virtualWrite(V119, Data[2]);}  // sending the value to controller
        if (Data[0]==44) {bridge1.virtualWrite(V120, Data[2]);}  // sending the value to controller
        if (Data[0]==45) {bridge1.virtualWrite(V121, Data[2]);}  // sending the value to controller
        if (Data[0]==46) {bridge1.virtualWrite(V122, Data[2]);}  // sending the value to controller
        if (Data[0]==47) {bridge1.virtualWrite(V123, Data[2]);}  // sending the value to controller
        if (Data[0]==48) {bridge1.virtualWrite(V124, Data[2]);}  // sending the value to controller
        if (Data[0]==49) {bridge1.virtualWrite(V125, Data[2]);}  // sending the value to controller
        if (Data[0]==50) {bridge1.virtualWrite(V126, Data[2]);}  // sending the value to controller
        if (Data[0]==51) {bridge1.virtualWrite(V127, Data[2]);}  // sending the value to controller
                
        // Limitations without additional programing to expand Virtual pins on Blynk 
        if (Data[0]==52) {bridge1.virtualWrite(V128, Data[2]);}  // sending the value to controller
        if (Data[0]==53) {bridge1.virtualWrite(V129, Data[2]);}  // sending the value to controller
        if (Data[0]==54) {bridge1.virtualWrite(V130, Data[2]);}  // sending the value to controller
        if (Data[0]==55) {bridge1.virtualWrite(V131, Data[2]);}  // sending the value to controller
        if (Data[0]==56) {bridge1.virtualWrite(V132, Data[2]);}  // sending the value to controller
        if (Data[0]==57) {bridge1.virtualWrite(V133, Data[2]);}  // sending the value to controller
        if (Data[0]==58) {bridge1.virtualWrite(V134, Data[2]);}  // sending the value to controller
        if (Data[0]==59) {bridge1.virtualWrite(V135, Data[2]);}  // sending the value to controller
        if (Data[0]==60) {bridge1.virtualWrite(V136, Data[2]);}  // sending the value to controller
        if (Data[0]==61) {bridge1.virtualWrite(V137, Data[2]);}  // sending the value to controller
        if (Data[0]==62) {bridge1.virtualWrite(V138, Data[2]);}  // sending the value to controller
        if (Data[0]==63) {bridge1.virtualWrite(V139, Data[2]);}  // sending the value to controller
        */
    }
  }
//****************************************************************
// object used to send data to esp32 BLE device
void SendESP32Data() {
    SerialLCD.println(ToESP32); // sends the current variable
    Serial.println("Sent: (" + String(ToESP32) + ") to ESP32 via serial and resetting to default"); // notifies action via monitor
    ToESP32=0; // reseting the command variable to default or no action
}

//*****************************************************************************************
//***************************************************************
// Blynk Functions and Objects to configure settings on ESP32
// these assign a value to the ToESP32 variable and sends the
// value to the ESP32 for processing and applying specific settings

// Sets Weapon Slot 0
BLYNK_WRITE(V0) {
int b=param.asInt();
if (LOCKOUT==false){
if (b==1) {ToESP32=1; SendESP32Data(); Serial.println("Weapon Slot 0 is set to Player's Choice");}
if (b==2) {ToESP32=2; SendESP32Data(); Serial.println("Weapon Slot 0 is set to Unarmed");}
if (b==3) {ToESP32=3; SendESP32Data(); Serial.println("Weapon Slot 0 is set to AMR");}
if (b==4) {ToESP32=4; SendESP32Data(); Serial.println("Weapon Slot 0 is set to Assault Rifle");}
if (b==5) {ToESP32=5; SendESP32Data(); Serial.println("Weapon Slot 0 is set to Bolt Rifle");}
if (b==6) {ToESP32=6; SendESP32Data(); Serial.println("Weapon Slot 0 is set to BurstRifle");}
if (b==7) {ToESP32=7; SendESP32Data(); Serial.println("Weapon Slot 0 is set to ChargeRifle");}
if (b==8) {ToESP32=8; SendESP32Data(); Serial.println("Weapon Slot 0 is set to Energy Launcher");}
if (b==9) {ToESP32=9; SendESP32Data(); Serial.println("Weapon Slot 0 is set to Energy Rifle");}
if (b==10) {ToESP32=10; SendESP32Data(); Serial.println("Weapon Slot 0 is set to Force Rifle");}
if (b==11) {ToESP32=11; SendESP32Data(); Serial.println("Weapon Slot 0 is set to Ion Sniper");}
if (b==12) {ToESP32=12; SendESP32Data(); Serial.println("Weapon Slot 0 is set to Laser Cannon");}
if (b==13) {ToESP32=13; SendESP32Data(); Serial.println("Weapon Slot 0 is set to Plasma Sniper");}
if (b==14) {ToESP32=14; SendESP32Data(); Serial.println("Weapon Slot 0 is set to Rail Gun");}
if (b==15) {ToESP32=15; SendESP32Data(); Serial.println("Weapon Slot 0 is set to Rocket Launcher");}
if (b==16) {ToESP32=16; SendESP32Data(); Serial.println("Weapon Slot 0 is set to Shotgun");}
if (b==17) {ToESP32=17; SendESP32Data(); Serial.println("Weapon Slot 0 is set to SMG");}
if (b==18) {ToESP32=18; SendESP32Data(); Serial.println("Weapon Slot 0 is set to Sniper Rifle");}
if (b==19) {ToESP32=19; SendESP32Data(); Serial.println("Weapon Slot 0 is set to Stinger");}
if (b==20) {ToESP32=20; SendESP32Data(); Serial.println("Weapon Slot 0 is set to Suppressor");}
}
}

// Sets Weapon Slot 1
BLYNK_WRITE(V1) {
int b=param.asInt();
if (LOCKOUT==false){
if (b==1) {ToESP32=101; SendESP32Data(); Serial.println("Weapon Slot 1 is set to Player's Choice");}
if (b==2) {ToESP32=102; SendESP32Data(); Serial.println("Weapon Slot 1 is set to Unarmed");}
if (b==3) {ToESP32=103; SendESP32Data(); Serial.println("Weapon Slot 1 is set to AMR");}
if (b==4) {ToESP32=104; SendESP32Data(); Serial.println("Weapon Slot 1 is set to Assault Rifle");}
if (b==5) {ToESP32=105; SendESP32Data(); Serial.println("Weapon Slot 1 is set to Bolt Rifle");}
if (b==6) {ToESP32=106; SendESP32Data(); Serial.println("Weapon Slot 1 is set to BurstRifle");}
if (b==7) {ToESP32=107; SendESP32Data(); Serial.println("Weapon Slot 1 is set to ChargeRifle");}
if (b==8) {ToESP32=108; SendESP32Data(); Serial.println("Weapon Slot 1 is set to Energy Launcher");}
if (b==9) {ToESP32=109; SendESP32Data(); Serial.println("Weapon Slot 1 is set to Energy Rifle");}
if (b==10) {ToESP32=110; SendESP32Data(); Serial.println("Weapon Slot 1 is set to Force Rifle");}
if (b==11) {ToESP32=111; SendESP32Data(); Serial.println("Weapon Slot 1 is set to Ion Sniper");}
if (b==12) {ToESP32=112; SendESP32Data(); Serial.println("Weapon Slot 1 is set to Laser Cannon");}
if (b==13) {ToESP32=113; SendESP32Data(); Serial.println("Weapon Slot 1 is set to Plasma Sniper");}
if (b==14) {ToESP32=114; SendESP32Data(); Serial.println("Weapon Slot 1 is set to Rail Gun");}
if (b==15) {ToESP32=115; SendESP32Data(); Serial.println("Weapon Slot 1 is set to Rocket Launcher");}
if (b==16) {ToESP32=116; SendESP32Data(); Serial.println("Weapon Slot 1 is set to Shotgun");}
if (b==17) {ToESP32=117; SendESP32Data(); Serial.println("Weapon Slot 1 is set to SMG");}
if (b==18) {ToESP32=118; SendESP32Data(); Serial.println("Weapon Slot 1 is set to Sniper Rifle");}
if (b==19) {ToESP32=119; SendESP32Data(); Serial.println("Weapon Slot 1 is set to Stinger");}
if (b==20) {ToESP32=120; SendESP32Data(); Serial.println("Weapon Slot 1 is set to Suppressor");}
}
}

// Sets Objective Goals
BLYNK_WRITE(V2) {
int b=param.asInt();
if (LOCKOUT==false){
if (b==25) {ToESP32=225; SendESP32Data(); Serial.println("Objective/Goals is set to 25");}
if (b==24) {ToESP32=224; SendESP32Data(); Serial.println("Objective/Goals is set to 24");}
if (b==23) {ToESP32=223; SendESP32Data(); Serial.println("Objective/Goals is set to 23");}
if (b==22) {ToESP32=222; SendESP32Data(); Serial.println("Objective/Goals is set to 22");}
if (b==21) {ToESP32=221; SendESP32Data(); Serial.println("Objective/Goals is set to 21");}
if (b==20) {ToESP32=220; SendESP32Data(); Serial.println("Objective/Goals is set to 20");}
if (b==19) {ToESP32=219; SendESP32Data(); Serial.println("Objective/Goals is set to 19");}
if (b==18) {ToESP32=218; SendESP32Data(); Serial.println("Objective/Goals is set to 18");}
if (b==17) {ToESP32=217; SendESP32Data(); Serial.println("Objective/Goals is set to 17");}
if (b==16) {ToESP32=216; SendESP32Data(); Serial.println("Objective/Goals is set to 16");}
if (b==15) {ToESP32=215; SendESP32Data(); Serial.println("Objective/Goals is set to 15");}
if (b==14) {ToESP32=214; SendESP32Data(); Serial.println("Objective/Goals is set to 14");}
if (b==13) {ToESP32=213; SendESP32Data(); Serial.println("Objective/Goals is set to 13");}
if (b==12) {ToESP32=212; SendESP32Data(); Serial.println("Objective/Goals is set to 12");}
if (b==11) {ToESP32=211; SendESP32Data(); Serial.println("Objective/Goals is set to 11");}
if (b==10) {ToESP32=210; SendESP32Data(); Serial.println("Objective/Goals is set to 10");}
if (b==9) {ToESP32=209; SendESP32Data(); Serial.println("Objective/Goals is set to 9");}
if (b==8) {ToESP32=208; SendESP32Data(); Serial.println("Objective/Goals is set to 8");}
if (b==7) {ToESP32=207; SendESP32Data(); Serial.println("Objective/Goals is set to 7");}
if (b==6) {ToESP32=206; SendESP32Data(); Serial.println("Objective/Goals is set to 6");}
if (b==5) {ToESP32=205; SendESP32Data(); Serial.println("Objective/Goals is set to 5");}
if (b==4) {ToESP32=204; SendESP32Data(); Serial.println("Objective/Goals is set to 4");}
if (b==3) {ToESP32=203; SendESP32Data(); Serial.println("Objective/Goals is set to 3");}
if (b==2) {ToESP32=202; SendESP32Data(); Serial.println("Objective/Goals is set to 2");}
if (b==1) {ToESP32=201; SendESP32Data(); Serial.println("Objective/Goals is set to 1");}
if (b==0) {ToESP32=200; SendESP32Data(); Serial.println("Objective/Goals is set to Unlimited");}
}
}

// Sets Kills to Win
BLYNK_WRITE(V3) {
int b=param.asInt();
if (LOCKOUT==false){
if (b==25) {ToESP32=325; SendESP32Data(); Serial.println("Kills to Win is set to 25");}
if (b==24) {ToESP32=324; SendESP32Data(); Serial.println("Kills to Win is set to 24");}
if (b==23) {ToESP32=323; SendESP32Data(); Serial.println("Kills to Win is set to 23");}
if (b==22) {ToESP32=322; SendESP32Data(); Serial.println("Kills to Win is set to 22");}
if (b==21) {ToESP32=321; SendESP32Data(); Serial.println("Kills to Win is set to 21");}
if (b==20) {ToESP32=320; SendESP32Data(); Serial.println("Kills to Win is set to 20");}
if (b==19) {ToESP32=319; SendESP32Data(); Serial.println("Kills to Win is set to 19");}
if (b==18) {ToESP32=318; SendESP32Data(); Serial.println("Kills to Win is set to 18");}
if (b==17) {ToESP32=317; SendESP32Data(); Serial.println("Kills to Win is set to 17");}
if (b==16) {ToESP32=316; SendESP32Data(); Serial.println("Kills to Win is set to 16");}
if (b==15) {ToESP32=315; SendESP32Data(); Serial.println("Kills to Win is set to 15");}
if (b==14) {ToESP32=314; SendESP32Data(); Serial.println("Kills to Win is set to 14");}
if (b==13) {ToESP32=313; SendESP32Data(); Serial.println("Kills to Win is set to 13");}
if (b==12) {ToESP32=312; SendESP32Data(); Serial.println("Kills to Win is set to 12");}
if (b==11) {ToESP32=311; SendESP32Data(); Serial.println("Kills to Win is set to 11");}
if (b==10) {ToESP32=310; SendESP32Data(); Serial.println("Kills to Win is set to 10");}
if (b==9) {ToESP32=309; SendESP32Data(); Serial.println("Kills to Win is set to 9");}
if (b==8) {ToESP32=308; SendESP32Data(); Serial.println("Kills to Win is set to 8");}
if (b==7) {ToESP32=307; SendESP32Data(); Serial.println("Kills to Win is set to 7");}
if (b==6) {ToESP32=306; SendESP32Data(); Serial.println("Kills to Win is set to 6");}
if (b==5) {ToESP32=305; SendESP32Data(); Serial.println("Kills to Win is set to 5");}
if (b==4) {ToESP32=304; SendESP32Data(); Serial.println("Kills to Win is set to 4");}
if (b==3) {ToESP32=303; SendESP32Data(); Serial.println("Kills to Win is set to 3");}
if (b==2) {ToESP32=302; SendESP32Data(); Serial.println("Kills to Win is set to 2");}
if (b==1) {ToESP32=301; SendESP32Data(); Serial.println("Kills to Win is set to 1");}
if (b==0) {ToESP32=300; SendESP32Data(); Serial.println("Kills to Win is set to Unlimited");}
}
}

// Sets Lives
BLYNK_WRITE(V4) {
int b=param.asInt();
if (LOCKOUT==false){
if (b==25) {ToESP32=425; SendESP32Data(); Serial.println("Lives is set to 25");}
if (b==24) {ToESP32=424; SendESP32Data(); Serial.println("Lives is set to 24");}
if (b==23) {ToESP32=423; SendESP32Data(); Serial.println("Lives is set to 23");}
if (b==22) {ToESP32=422; SendESP32Data(); Serial.println("Lives is set to 22");}
if (b==21) {ToESP32=421; SendESP32Data(); Serial.println("Lives is set to 21");}
if (b==20) {ToESP32=420; SendESP32Data(); Serial.println("Lives is set to 20");}
if (b==19) {ToESP32=419; SendESP32Data(); Serial.println("Lives is set to 19");}
if (b==18) {ToESP32=418; SendESP32Data(); Serial.println("Lives is set to 18");}
if (b==17) {ToESP32=417; SendESP32Data(); Serial.println("Lives is set to 17");}
if (b==16) {ToESP32=416; SendESP32Data(); Serial.println("Lives is set to 16");}
if (b==15) {ToESP32=415; SendESP32Data(); Serial.println("Lives is set to 15");}
if (b==14) {ToESP32=414; SendESP32Data(); Serial.println("Lives is set to 14");}
if (b==13) {ToESP32=413; SendESP32Data(); Serial.println("Lives is set to 13");}
if (b==12) {ToESP32=412; SendESP32Data(); Serial.println("Lives is set to 12");}
if (b==11) {ToESP32=411; SendESP32Data(); Serial.println("Lives is set to 11");}
if (b==10) {ToESP32=410; SendESP32Data(); Serial.println("Lives is set to 10");}
if (b==9) {ToESP32=409; SendESP32Data(); Serial.println("Lives is set to 9");}
if (b==8) {ToESP32=408; SendESP32Data(); Serial.println("Lives is set to 8");}
if (b==7) {ToESP32=407; SendESP32Data(); Serial.println("Lives is set to 7");}
if (b==6) {ToESP32=406; SendESP32Data(); Serial.println("Lives is set to 6");}
if (b==5) {ToESP32=405; SendESP32Data(); Serial.println("Lives is set to 5");}
if (b==4) {ToESP32=404; SendESP32Data(); Serial.println("Lives is set to 4");}
if (b==3) {ToESP32=403; SendESP32Data(); Serial.println("Lives is set to 3");}
if (b==2) {ToESP32=402; SendESP32Data(); Serial.println("Lives is set to 2");}
if (b==1) {ToESP32=401; SendESP32Data(); Serial.println("Lives is set to 1");}
if (b==0) {ToESP32=400; SendESP32Data(); Serial.println("Lives is set to Unlimited");}
}
}

// Sets Game Time
BLYNK_WRITE(V5) {
int b=param.asInt();
if (LOCKOUT==false){
/*
if (b==1) {ToESP32=501; SendESP32Data(); Serial.println("Game Time is set to 1 Minute"); gametime=60000+gamedelay;}
if (b==2) {ToESP32=502; SendESP32Data(); Serial.println("Game Time is set to 5 Minutes"); gametime=300000+gamedelay;}
if (b==3) {ToESP32=503; SendESP32Data(); Serial.println("Game Time is set to 10 Minutes"); gametime=600000+gamedelay;}
if (b==4) {ToESP32=504; SendESP32Data(); Serial.println("Game Time is set to 15 Minutes"); gametime=900000+gamedelay;}
if (b==5) {ToESP32=505; SendESP32Data(); Serial.println("Game Time is set to 20 Minutes"); gametime=1200000+gamedelay;}
if (b==6) {ToESP32=506; SendESP32Data(); Serial.println("Game Time is set to 25 Minutes"); gametime=1500000+gamedelay;}
if (b==7) {ToESP32=507; SendESP32Data(); Serial.println("Game Time is set to 30 Minutes"); gametime=1800000+gamedelay;}
if (b==8) {ToESP32=508; SendESP32Data(); Serial.println("Game Time is set to Unlimited"); gametime=2000000000+gamedelay;}
*/
if (b==1) {ToESP32=501; SendESP32Data(); Serial.println("Game Time is set to 1 Minute");}
if (b==2) {ToESP32=502; SendESP32Data(); Serial.println("Game Time is set to 5 Minutes");}
if (b==3) {ToESP32=503; SendESP32Data(); Serial.println("Game Time is set to 10 Minutes");}
if (b==4) {ToESP32=504; SendESP32Data(); Serial.println("Game Time is set to 15 Minutes");}
if (b==5) {ToESP32=505; SendESP32Data(); Serial.println("Game Time is set to 20 Minutes");}
if (b==6) {ToESP32=506; SendESP32Data(); Serial.println("Game Time is set to 25 Minutes");}
if (b==7) {ToESP32=507; SendESP32Data(); Serial.println("Game Time is set to 30 Minutes");}
if (b==8) {ToESP32=508; SendESP32Data(); Serial.println("Game Time is set to Unlimited");}
}
}

// Sets Lighting-Ambience
BLYNK_WRITE(V6) {
int b=param.asInt();
if (LOCKOUT==false){
if (b==1) {ToESP32=601; SendESP32Data(); Serial.println("Lighting/Ambience is set to Outdoor Mode");}
if (b==2) {ToESP32=602; SendESP32Data(); Serial.println("Lighting/Ambience is set to Indoor Mode");}
if (b==3) {ToESP32=603; SendESP32Data(); Serial.println("Lighting/Ambience is set to Stealth");}
}
}

// Sets Team Modes
BLYNK_WRITE(V7) {
int b=param.asInt();
if (LOCKOUT==false){
if (b==1) {ToESP32=701; SendESP32Data(); Serial.println("Teams is set to Free For All");}
if (b==2) {ToESP32=702; SendESP32Data(); Serial.println("Teams is set to Two Teams (odds/evens)");}
if (b==3) {ToESP32=703; SendESP32Data(); Serial.println("Teams is set to Three Teams (every three)");}
if (b==4) {ToESP32=704; SendESP32Data(); Serial.println("Teams is set to Four Teams (every four)");}
if (b==5) {ToESP32=705; SendESP32Data(); Serial.println("Teams is set to Player's Choice");}
}
}

// Sets Game Mode
BLYNK_WRITE(V8) {
int b=param.asInt();
if (LOCKOUT==false){
if (b==1) {ToESP32=801; SendESP32Data(); Serial.println("Game Mode is set to Death Match");}
if (b==2) {ToESP32=802; SendESP32Data(); Serial.println("Game Mode is set to Capture the Flag");}
if (b==3) {ToESP32=803; SendESP32Data(); Serial.println("Game Mode is set to Assault");}
if (b==4) {ToESP32=804; SendESP32Data(); Serial.println("Game Mode is set to King of the Hill");}
if (b==5) {ToESP32=805; SendESP32Data(); Serial.println("Game Mode is set to Survival");}
if (b==6) {ToESP32=806; SendESP32Data(); Serial.println("Game Mode is set to Troule in Terrorist Town");}
if (b==7) {ToESP32=807; SendESP32Data(); Serial.println("Game Mode is set to You only Live Twice");}
if (b==8) {ToESP32=808; SendESP32Data(); Serial.println("Game Mode is set to One Shot Kills (pistols)");}
if (b==9) {ToESP32=809; SendESP32Data(); Serial.println("Game Mode is set to Gun Game");}
if (b==10) {ToESP32=810; SendESP32Data(); Serial.println("Game Mode is set to Domination");}
if (b==11) {ToESP32=811; SendESP32Data(); Serial.println("Game Mode is set to Battle Royale");}
}
}

// Sets Respawn Mode
BLYNK_WRITE(V9) {
int b=param.asInt();
if (LOCKOUT==false){
if (b==1) {ToESP32=901; SendESP32Data(); Serial.println("Respawn is set to Immediate (auto)");}
if (b==2) {ToESP32=902; SendESP32Data(); Serial.println("Respawn is set to 15 seconds (auto)");}
if (b==3) {ToESP32=903; SendESP32Data(); Serial.println("Respawn is set to 30 seconds (auto)");}
if (b==4) {ToESP32=904; SendESP32Data(); Serial.println("Respawn is set to 45 seconds  (auto)");}
if (b==5) {ToESP32=905; SendESP32Data(); Serial.println("Respawn is set to 60 seconds (auto)");}
if (b==6) {ToESP32=906; SendESP32Data(); Serial.println("Respawn is set to 90 seconds (auto)");}
if (b==7) {ToESP32=907; SendESP32Data(); Serial.println("Respawn is set to Ramp 45 (auto)");}
if (b==8) {ToESP32=908; SendESP32Data(); Serial.println("Respawn is set to Ramp 90 (auto)");}
if (b==9) {ToESP32=909; SendESP32Data(); Serial.println("Respawn is set to Respawn Station (manual)");}
}
}

// Sets Delayed Start Time
BLYNK_WRITE(V10) {
int b=param.asInt();
if (LOCKOUT==false){
if (b==1) {ToESP32=1001; SendESP32Data(); Serial.println("Delayed Start is set to Immediate");}
if (b==2) {ToESP32=1002; SendESP32Data(); Serial.println("Delayed Start is set to 15 seconds");}
if (b==3) {ToESP32=1003; SendESP32Data(); Serial.println("Delayed Start is set to 30 seconds");}
if (b==4) {ToESP32=1004; SendESP32Data(); Serial.println("Delayed Start is set to 45 seconds");}
if (b==5) {ToESP32=1005; SendESP32Data(); Serial.println("Delayed Start is set to 60 seconds");}
if (b==6) {ToESP32=1006; SendESP32Data(); Serial.println("Delayed Start is set to 90 seconds");}
if (b==7) {ToESP32=1007; SendESP32Data(); Serial.println("Delayed Start is set to 5 Minutes");}
if (b==8) {ToESP32=1008; SendESP32Data(); Serial.println("Delayed Start is set to 10 Minutes");}
if (b==9) {ToESP32=1009; SendESP32Data(); Serial.println("Delayed Start is set to 15 Minutes");}
}
}

// Sends Request for Score Syncing
BLYNK_WRITE(V11) {
int b=param.asInt();
if (LOCKOUT==false){
if (b==1) {ToESP32=1101; SendESP32Data(); Serial.println("Score Sync Request Sent");}
}
}

// Sets Player Gender
BLYNK_WRITE(V12) {
int b=param.asInt();
if (LOCKOUT==false){
if (b==0) {ToESP32=1200; SendESP32Data(); Serial.println("Player Gender is set to Male");}
if (b==1) {ToESP32=1201; SendESP32Data(); Serial.println("Player Gender is set to Female");}
}
}

// Sets Ammo Settings
BLYNK_WRITE(V13) {
int b=param.asInt();
if (LOCKOUT==false){
if (b==3) {ToESP32=1303; SendESP32Data(); Serial.println("Ammo Settings is set to Ulimited Rounds");}
if (b==2) {ToESP32=1302; SendESP32Data(); Serial.println("Ammo Settings is set to Unlimited Magazines");}
if (b==1) {ToESP32=1301; SendESP32Data(); Serial.println("Ammo Settings is set to Limited");}
}
}

// Sets Friendly Fire
BLYNK_WRITE(V14) {
int b=param.asInt();
if (LOCKOUT==false){
if (b==0) {ToESP32=1400; SendESP32Data(); Serial.println("Friendly Fire is set to Off");}
if (b==1) {ToESP32=1401; SendESP32Data(); Serial.println("Friendly Fire is set to On");}
}
}

// Sets Unasigned still, space holder for additional settings
BLYNK_WRITE(V15) {
int b=param.asInt();
if (LOCKOUT==false){
ToESP32 = (1500+b); SendESP32Data(); Serial.println("Volume is set to " + String(ToESP32));
}
}

// Start/Ends a game
BLYNK_WRITE(V16) {
int b=param.asInt();
if (b==0) {ToESP32=1600; SendESP32Data(); Serial.println("End Game is set to unpressed"); LOCKOUT=false; Serial.println("Lockout Dis-Engaged");}
if (b==1) {ToESP32=1601; SendESP32Data(); Serial.println("Start Game is set to pressed"); LOCKOUT=true; Serial.println("Lockout Engaged");}
// if (b==1) {ToESP32=1601; SendESP32Data(); Serial.println("Start Game is set to pressed"); GAMESTART=true; gamestart=millis();}
}

// Locks out Tagger power on/off esp8266 only
BLYNK_WRITE(V17) {
int b=param.asInt();
if (b==1) {Serial.println("Manual Power Switch Override Engaged"); digitalWrite(PowerPin, HIGH);}
if (b==0) {Serial.println("Manual Power Switch Override Dis-Engaged"); digitalWrite(PowerPin, LOW);}
}


// player button lockout
BLYNK_WRITE(V18) {
int b=param.asInt();
if (b==1) {ToESP32=1801; SendESP32Data(); Serial.println("Player Button Lockout Engaged");}
}

// player button lockout
BLYNK_WRITE(V19) {
int b=param.asInt();
if (b==1) {ToESP32=1901; SendESP32Data(); Serial.println("Player Button Lockout Engaged");}
}
//*****************************************************************************************
//*****************************************************************************************

void setup()
{
   // put your setup code here, to run once:
  // Debug console
  Serial.begin(115200);
  //SerialLCD.begin(9600);
  SerialLCD.begin(9600, swSerialConfig, RX, TX);
  // initialize LCD
  lcd.init();
  // turn on LCD backlight
  lcd.backlight();
  lcd.print("Welcome to Battle Company!");
  // Start Blynk

  pinMode(PowerPin, OUTPUT); // pin used for power switch override
  
//**********************************************************************************
//****************************  UPDATE THIS SECTION!!!!! ***************************
//**********************************************************************************
  // Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,80), 8080);
  Blynk.begin(auth, ssid, pass);
//**********************************************************************************
//****************************  UPDATE THIS SECTION!!!!! ***************************
//**********************************************************************************

  // timer settings (currently not used)
  ESP32Read.setInterval(1L, ReadESP32Data); // Reading data from esp32 constantly
  SyncScoreRead.setInterval(1L, ReadSyncScoreData); // Reading data from esp32 constantly
  // ESP32Send.setInterval(5000L, SendESP32Data); // sending commands to esp32 every five seconds
}

void loop()
{
  Blynk.run();
  if (ENABLESERIAL) {ESP32Read.run();}
  if (SYNCSCORE) {SyncScoreRead.run();}
  /*
   *  edited out as an alternate method for ignoring blynk in game mode
   /// while (GAMESTART) {
    // if ((millis()-gamestart) < gametime) {
      // ReadESP32Data();
      // Serial.println("reading");
    // } else {
      // GAMESTART=false;
    // }
  // }
  */
}
