/* ESP8266 Laser Tag Game Controller And Comms Device
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
 *    or 16 to D6 and 17 to D5
 *    or esp32 mini plus esp8266 mini build - 
 *      D4 to 16 and D3 to 17
 *  If the ESP8266 is powered by the ESP32, then the Vcc and GRND pins should
 *  be connected as well
 *  For OTA Updates: be sure to download the latest python build and install with the option to add "python to path" this allows your pc to upload OTA
 *  https://www.python.org/ftp/python/3.8.5/python-3.8.5.exe
 *  Also make sure you have Bonjour installed and sometimes it is finicky so be prepared to restart your computer to make it work.
 *  
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
 *  7/27/2020 - Jay - added definitions for pins for complying with D1 mini esp32
 *  8/10/2020 - Jay - updated order of operations for syncing scores with scoring device
 *  8/14/2020 - Jay - Added a FAKESCORE trigger and functions inside the scoreing object to create a faux score for testing the score reporting process with random numbers
 *  8/14/2020 - Jay - Fixed Misc. bugs i saw with values and broke out the process of analyzing the score received from brx as is own object for use with the standard score reporting process as well as the faux scores
 *  8/14/2020 - Jay - Added score clearing object to occur when a game is started
 *  8/16/2020 - Jay - Added an IR Debug Mode for receiving IR tag data from BRX and populating to the app, this will be used for determining tag read abilities in game mode for base respawns etc.
 *  8/18/2020 - Jay - Added OTA update capability to the device
 */ 
 
/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

// Libraries needed to accomodate sketch
#include <LiquidCrystal_I2C.h> // used to write to LCD screen
#include <SoftwareSerial.h> // used to write serial comms to esp32
#include <ESP8266WiFi.h> // used for setting up wifi comms
#include <BlynkSimpleEsp8266.h> // used to run all necessary blynk objects/functions
// OTA update libraries
#include <ArduinoOTA.h>


// set up bridge widget on V1 of Device 1
// this creates the widget needed to send data to the controller
WidgetBridge bridge1(V1);

//**********************************************************************************
//****************************  UPDATE THIS SECTION!!!!! ***************************
//**********************************************************************************
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "g3Nl5QsEc_CseHRSXNUjMyWv3PU8La9Y";
//char auth[] = "nhyqI7ejfjTebH-KI5x9kndOhKoAhyTz"; // this is the Configurator blynk authentication token
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "SSID"; // this is your WiFi name
char pass[] = "12345678"; // this is your WiFi password
// set the bridge token
BLYNK_CONNECTED() {
  bridge1.setAuthToken("slcCz6BNVlNAFh5XKRBrREydDDfXIEoB"); // Token of the device 2 or scoring device
}
int GunID = 6; // this is the gun or player ID, each configurator needs a different one, set "0-63"
                // be sure to change both esp8266 and 32 to the same for each configurator

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
String IRDataString[7];
String readStr;
/*
 * edited out for alternate conrols
long gametime=2000000000; // used as a timer for games
long gamestart=0; // used as a start for a game
long gamedelay=4000; // sets min delay for start a game time accumulation, only add to this value
*/

int TeamKillCount[6];
int PlayerKillCount[64];
int CompletedObjectives=0;
int SetTeam=0;

bool GAMESTART=false;
bool SYNCSCORE=false;
bool ENABLESERIAL=true;
bool LOCKOUT=false;
bool CLEARLCD = false;
bool FAKESCORE = false;
bool IRDEBUG = false;
bool OTAUPDATE = false;

unsigned long startt = millis();
long blinktimer = 0;

// timers for running certain applications periodically with the blynk program
BlynkTimer ESP32Send; // created a timer object called "ESP32Send"
BlynkTimer ESP32Read; // created a timer object called "ESP32Read"
BlynkTimer SyncScoreRead; // created a timer object called "SyncScoreRead"
BlynkTimer IRDataRead; // created a timer object called "IRDataRead"
BlynkTimer OTAUpdateLoop; // timer object for updating over wifi

#ifndef RX
//#define RX 14 // D5=14 to pin 17 on esp32
//#define RX 13 // D7=13 to pin 17 on esp32
#define RX 0 // D3=0 to pin 17 on esp32
#endif
#ifndef TX
//#define TX 12 // D6=12 to pin 16 on esp32
// #define TX 15 // D8=15 to pin 16 on esp32
#define TX 2 // D4=2 to pin 16 on esp32 
#endif
int PowerPin = D7;  // D7 is 13 - Pin used to override the main power switch on tagger as a lockout function
const int ESP_BUILTIN_LED = 2;

//SoftwareSerial SerialLCD(RX, TX, false, 256); //nodemcu D8-rx D7-tx
SoftwareSerial SerialLCD;
constexpr SoftwareSerialConfig swSerialConfig = SWSERIAL_8N1;
// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

//******************************************************************
// OTA Update loop
void OTAUpdate() {
  ArduinoOTA.handle();
   if (millis() - blinktimer > 500) {digitalWrite(ESP_BUILTIN_LED, LOW);}
   if (millis() - blinktimer > 1000) {digitalWrite(ESP_BUILTIN_LED, HIGH); blinktimer = millis();}
}

//******************************************************************
void AnalyzeScoreData() {
     // now we process he data received from the string array
     lcd.print("Scores sent to server");
     bridge1.virtualWrite(V0, readStr); // sending the whole string from esp32
     char *ptr = strtok((char*)readStr.c_str(), ",");
     int index = 0;
     while (ptr != NULL)
     {
       ScoreTokenStrings[index] = ptr;
       index++;
       ptr = strtok(NULL, ",");  // takes a list of delimiters
     }
   // read a string that looks like this: 
   // (Player ID, token 0), (Player Team, token 1), (Player Objective Score, token 2) (Team scores, tokens 3-6), (player kill counts, tokens 7-70)
   // 71 TOTAL TOKENS!!! hope this receives well LOL!    Serial.println("Score Data Recieved from ESP32");
     Serial.println(readStr); // printing what was read from esp32
     SYNCSCORE=false;
     ENABLESERIAL=true;
       lcd.clear();
       lcd.setCursor(0,0);
       Serial.println("finished sending data to score reporting device");
       // print scores to serial
       Serial.println("Player ID = "+String(ScoreTokenStrings[0]));
       Serial.println("Team ID = "+String(ScoreTokenStrings[1]));
       Serial.println("Player Objective Score = "+String(ScoreTokenStrings[2]));
       Serial.println("Team 0 Kills = "+String(ScoreTokenStrings[3]));
       Serial.println("Team 1 Kills = "+String(ScoreTokenStrings[4]));
       Serial.println("Team 2 Kills = "+String(ScoreTokenStrings[5]));
       Serial.println("Team 3 Kills = "+String(ScoreTokenStrings[6]));
       int count = 9;
       int playercount = 0;
       while (count<73) {
        Serial.println("Player " + String(playercount) + " kills = " + String(ScoreTokenStrings[count]));
        count++;
        playercount++;
       }
}

//***********************************************************************************************
// process to clear out scores
void ClearScores() {
  int playercounter = 0;
  while (playercounter < 64) {
    PlayerKillCount[playercounter] = 0;
    playercounter++;
  }
  int teamcounter = 0;
  while (teamcounter < 4) {
    TeamKillCount[teamcounter] = 0;
    teamcounter++;
  }
  CompletedObjectives=0;
  Serial.println("Reset all stored scores from previous game");
}

//**************************************************************************************************

// object used to receive data from ESP32 BLE device
void ReadESP32Data() {
if (SerialLCD.available()) {
    readStr = SerialLCD.readStringUntil('\n');
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
// object used to receive IR data from ESP32 BLE device
void ReadIRData() {
  if (SerialLCD.available()) {
    readStr = SerialLCD.readStringUntil('\n');
    // now we process he data received from the string array
    Serial.println("IR Debug Data Received");
    Serial.println(readStr);
    char *ptr = strtok((char*)readStr.c_str(), ",");
    int index = 0;
    while (ptr != NULL){
       IRDataString[index] = ptr;
       index++;
       ptr = strtok(NULL, ",");  // takes a list of delimiters
     }
     // read a string that looks like this: 
     // (IR Direction, 0), (Bullet Type, token 1), (Player ID, token 2) (Team, tokens 3), (Damage, 4), (Is Critical, 5), (Power, 6)
     // 7 TOTAL TOKENS!!! hope this receives well LOL!
     Serial.println("IR Direction = "+String(IRDataString[0]));
     Serial.println("Bullet Type = "+String(IRDataString[1]));
     Serial.println("Player ID = "+String(IRDataString[2]));
     Serial.println("Team ID = "+String(IRDataString[3]));
     Serial.println("Damage = "+String(IRDataString[4]));
     Serial.println("Critcal = "+String(IRDataString[5]));
     Serial.println("Power = "+String(IRDataString[6]));
     Serial.println("Now sending IR Data Debug to Blynk Application");
     Blynk.virtualWrite(V27, IRDataString[0]); // sending the value to controller
     Blynk.virtualWrite(V21, IRDataString[1]); // sending the value to controller
     Blynk.virtualWrite(V22, IRDataString[2]); // sending the value to controller
     Blynk.virtualWrite(V23, IRDataString[3]); // sending the value to controller
     Blynk.virtualWrite(V24, IRDataString[4]); // sending the value to controller
     Blynk.virtualWrite(V25, IRDataString[5]); // sending the value to controller
     Blynk.virtualWrite(V26, IRDataString[6]); // sending the value to controller
  }
}
//******************************************************************
// object used to receive data from ESP32 BLE device
void ReadSyncScoreData() {
  if (CLEARLCD) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Syncing Scores...");
    CLEARLCD = false;
  }
if (SerialLCD.available()) {
    readStr = SerialLCD.readStringUntil('\n');
    AnalyzeScoreData();  
    ENABLESERIAL = true;   
    }
if (FAKESCORE) {
      delay(65);
      Serial.println("delaying 65 milliseconds");
      SetTeam = random(3);
      CompletedObjectives = random(50);
      int teamcounter = 0;
      while (teamcounter < 6) {
        TeamKillCount[teamcounter] = random(10,50);
        teamcounter++;
      }
      int playercounter = 0;
      while (playercounter < 64) {
        PlayerKillCount[playercounter] = random(2,25);
        playercounter++;
      }
     readStr = (String(GunID)+","+String(SetTeam)+","+String(CompletedObjectives)+","+String(TeamKillCount[0])+","+String(TeamKillCount[1])+","+String(TeamKillCount[2])+","+String(TeamKillCount[3])+","+String(TeamKillCount[4])+","+String(TeamKillCount[5])+","+String(PlayerKillCount[0])+","+String(PlayerKillCount[1])+","+String(PlayerKillCount[2])+","+String(PlayerKillCount[3])+","+String(PlayerKillCount[4])+","+String(PlayerKillCount[5])+","+String(PlayerKillCount[6])+","+String(PlayerKillCount[7])+","+String(PlayerKillCount[8])+","+String(PlayerKillCount[9])+","+String(PlayerKillCount[10])+","+String(PlayerKillCount[11])+","+String(PlayerKillCount[12])+","+String(PlayerKillCount[13])+","+String(PlayerKillCount[14])+","+String(PlayerKillCount[15])+","+String(PlayerKillCount[16])+","+String(PlayerKillCount[17])+","+String(PlayerKillCount[18])+","+String(PlayerKillCount[19])+","+String(PlayerKillCount[20])+","+String(PlayerKillCount[21])+","+String(PlayerKillCount[22])+","+String(PlayerKillCount[23])+","+String(PlayerKillCount[24])+","+String(PlayerKillCount[25])+","+String(PlayerKillCount[26])+","+String(PlayerKillCount[27])+","+String(PlayerKillCount[28])+","+String(PlayerKillCount[29])+","+String(PlayerKillCount[30])+","+String(PlayerKillCount[31])+","+String(PlayerKillCount[32])+","+String(PlayerKillCount[33])+","+String(PlayerKillCount[34])+","+String(PlayerKillCount[35])+","+String(PlayerKillCount[36])+","+String(PlayerKillCount[37])+","+String(PlayerKillCount[38])+","+String(PlayerKillCount[39])+","+String(PlayerKillCount[40])+","+String(PlayerKillCount[41])+","+String(PlayerKillCount[42])+","+String(PlayerKillCount[43])+","+String(PlayerKillCount[44])+","+String(PlayerKillCount[45])+","+String(PlayerKillCount[46])+","+String(PlayerKillCount[47])+","+String(PlayerKillCount[48])+","+String(PlayerKillCount[49])+","+String(PlayerKillCount[50])+","+String(PlayerKillCount[51])+","+String(PlayerKillCount[52])+","+String(PlayerKillCount[53])+","+String(PlayerKillCount[54])+","+String(PlayerKillCount[55])+","+String(PlayerKillCount[56])+","+String(PlayerKillCount[57])+","+String(PlayerKillCount[58])+","+String(PlayerKillCount[59])+","+String(PlayerKillCount[60])+","+String(PlayerKillCount[61])+","+String(PlayerKillCount[62])+","+String(PlayerKillCount[63]));
     Serial.println("printing Faux score data");
     Serial.println(readStr);
     AnalyzeScoreData();
     ENABLESERIAL = true;
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

// Sets Special Class
BLYNK_WRITE(V2) {
int b=param.asInt();
if (LOCKOUT==false){
if (b==9) {ToESP32=209; SendESP32Data(); Serial.println("");}
if (b==8) {ToESP32=208; SendESP32Data(); Serial.println("");}
if (b==7) {ToESP32=207; SendESP32Data(); Serial.println("");}
if (b==6) {ToESP32=206; SendESP32Data(); Serial.println("VIP");}
if (b==5) {ToESP32=205; SendESP32Data(); Serial.println("Jugernaut");}
if (b==4) {ToESP32=204; SendESP32Data(); Serial.println("Munitions Expert");}
if (b==3) {ToESP32=203; SendESP32Data(); Serial.println("Medic");}
if (b==2) {ToESP32=202; SendESP32Data(); Serial.println("Respawn Allies");}
if (b==1) {ToESP32=201; SendESP32Data(); Serial.println("None - Default");}
}
}

// Sets Kills to Win
BLYNK_WRITE(V3) {
int b=param.asInt();
if (LOCKOUT==false){
if (b==9) {ToESP32=400; SendESP32Data(); Serial.println("Kills is set to Unlimited");}
if (b==8) {ToESP32=450; SendESP32Data(); Serial.println("Kills is set to 50");}
if (b==7) {ToESP32=425; SendESP32Data(); Serial.println("Kills is set to 25");}
if (b==6) {ToESP32=420; SendESP32Data(); Serial.println("Kills is set to 20");}
if (b==5) {ToESP32=415; SendESP32Data(); Serial.println("Kills is set to 15");}
if (b==4) {ToESP32=410; SendESP32Data(); Serial.println("Kills is set to 10");}
if (b==3) {ToESP32=405; SendESP32Data(); Serial.println("Kills is set to 5");}
if (b==2) {ToESP32=403; SendESP32Data(); Serial.println("Kills is set to 3");}
if (b==1) {ToESP32=401; SendESP32Data(); Serial.println("Kills is set to 1");}
}
}

// Sets Lives
BLYNK_WRITE(V4) {
int b=param.asInt();
if (LOCKOUT==false){
if (b==9) {ToESP32=400; SendESP32Data(); Serial.println("Lives is set to Unlimited");}
if (b==8) {ToESP32=450; SendESP32Data(); Serial.println("Lives is set to 50");}
if (b==7) {ToESP32=425; SendESP32Data(); Serial.println("Lives is set to 25");}
if (b==6) {ToESP32=420; SendESP32Data(); Serial.println("Lives is set to 20");}
if (b==5) {ToESP32=415; SendESP32Data(); Serial.println("Lives is set to 15");}
if (b==4) {ToESP32=410; SendESP32Data(); Serial.println("Lives is set to 10");}
if (b==3) {ToESP32=405; SendESP32Data(); Serial.println("Lives is set to 5");}
if (b==2) {ToESP32=403; SendESP32Data(); Serial.println("Lives is set to 3");}
if (b==1) {ToESP32=401; SendESP32Data(); Serial.println("Lives is set to 1");}
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
if (b==7) {ToESP32=907; SendESP32Data(); Serial.println("Respawn is set to 120 seconds (auto)");}
if (b==8) {ToESP32=908; SendESP32Data(); Serial.println("Respawn is set to 150 seconds (auto)");}
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
BLYNK_WRITE(V11) { // this is the virtual pin needed for identifying the lockout request
int b=param.asInt(); // set up a temporary object based variable to assign as the incoming data
if (LOCKOUT==false){ // Double check that not in game mode
if (b==GunID) {ToESP32=1101; SendESP32Data(); Serial.println("Score Sync Request Sent"); SYNCSCORE=true; ENABLESERIAL=false; CLEARLCD=true; OTAUPDATE = false; IRDEBUG = false;}
// check if requesting this player score, assign a value to send to esp32 to request score data, Send the trigger to esp32, print notification to serial, enable trigger to do special processing/reporting with incoming data from esp32, disable normal processing of data from esp32
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

// Sets Volume of taggers
BLYNK_WRITE(V15) {
int b=param.asInt();
if (LOCKOUT==false){
ToESP32 = (1500+b); SendESP32Data(); Serial.println("Volume is set to " + String(b));
}
}

// Start/Ends a game
BLYNK_WRITE(V16) {
int b=param.asInt();
if (b==0) {ToESP32=1600; SendESP32Data(); Serial.println("End Game is set to unpressed"); LOCKOUT=false; Serial.println("Lockout Dis-Engaged");}
if (b==1) {ToESP32=1601; SendESP32Data(); Serial.println("Start Game is set to pressed"); LOCKOUT=true; Serial.println("Lockout Engaged"); ClearScores();}
// if (b==1) {ToESP32=1601; SendESP32Data(); Serial.println("Start Game is set to pressed"); GAMESTART=true; gamestart=millis();}
}

// Locks out Tagger power on/off esp8266 only
BLYNK_WRITE(V17) {
int b=param.asInt();
if (b==1) {Serial.println("Manual Power Switch Override Engaged"); digitalWrite(PowerPin, HIGH);}
if (b==0) {Serial.println("Manual Power Switch Override Dis-Engaged"); digitalWrite(PowerPin, LOW);}
}


// tagger control engaged
BLYNK_WRITE(V18) {
int b=param.asInt();
if (b==1) {ToESP32=1801; SendESP32Data(); Serial.println("Configurator BLE enabled... taking over tagger");}
}

// Sleep ESP32 BLE device
BLYNK_WRITE(V19) {
int b=param.asInt();
if (b==1) {ToESP32=1901; SendESP32Data(); Serial.println("Power save mode for BLE device enabled");}
}

// IR Debug Mode
BLYNK_WRITE(V20) {
  int b = param.asInt();
  if (b == 1) {IRDEBUG = true; SYNCSCORE = false; ENABLESERIAL = false; OTAUPDATE = false; ToESP32 = 2001; SendESP32Data(); Serial.println("IR Debug Mode Engaged.");}
  if (b == 0) {IRDEBUG = false; ENABLESERIAL = true; ToESP32 = 2000; SendESP32Data(); Serial.println("IR Debug Mode Engaged");}
}

// Enable OTA Updates
BLYNK_WRITE(V28) {
  int b = param.asInt();
  if (b == 1) {OTAUPDATE = true; IRDEBUG = false; SYNCSCORE = false; ENABLESERIAL = false; Serial.println("Enabling OTA Updates"); ToESP32=2801; SendESP32Data();}
  if (b == 0) {OTAUPDATE = false; ENABLESERIAL = true; Serial.println("Disabling OTA UPDATE"); ToESP32=2800; SendESP32Data();}
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
  pinMode(ESP_BUILTIN_LED, OUTPUT);
  
//**********************************************************************************
//****************************  UPDATE THIS SECTION!!!!! ***************************
//**********************************************************************************
  Blynk.begin(auth, ssid, pass, IPAddress(10,10,0,67), 8080); // use this if you are running your own local server, UPDATE THE IP ADDRESS FOR YOUR SERVER ADDRESS ON YOUR NETWORK
  //Blynk.begin(auth, ssid, pass); // use this if you are using the blynk cloud server
//**********************************************************************************
//****************************  UPDATE THIS SECTION!!!!! ***************************
//**********************************************************************************

  //OTA Start Functions
  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // timer settings (currently not used) 
  ESP32Read.setInterval(1L, ReadESP32Data); // Reading data from esp32 constantly
  SyncScoreRead.setInterval(1L, ReadSyncScoreData); // Reading data from esp32 constantly
  IRDataRead.setInterval (1L, ReadIRData); // reading IR Debug data from esp32 constantly
  OTAUpdateLoop.setInterval(1L, OTAUpdate); // Puts ESP in update mode
}

void loop()
{
  Blynk.run();
  if (ENABLESERIAL) {ESP32Read.run();}
  if (SYNCSCORE) {SyncScoreRead.run();}
  if (OTAUPDATE) {OTAUpdateLoop.run();}
  if (IRDEBUG) {IRDataRead.run();}
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
