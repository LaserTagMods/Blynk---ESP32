/*
 * update 4/2/2020 annotations added and copied over objects for setting up games
 * update 4/3/2020 Cleaned up for more compatibility to serial comms programing
 * update 4/3/2020 Was able to plug in set setting from serial for friendly fire and outdoor/indoor mode
 * update 4/4/2020 was able to get team settings and manual input team settings working as well as gender settings
 *
 * Written by Jay "the man" Burden
 *
 * keep device in range of laser tag BLE enabled compatible device for it to work
 * This code requires an esp8266 to be paired via serial (reference esp8266 code)
 *      use pins 16 & 17 to connect to D8 & D7, respectively, of ESP8266
 * optionally power from laser tag battery to the GRND and the VCC pins or power
 *      via usb or standalone battery
 * optionally install a transistor across reset pins on laser tag device to pin 4
 * this devices uses the serial communication to set tagger and game configuration settings
 * Note the sections labeled *****IMPORTANT***** as it requires customization on your part
 *
 *Serial data recieved from ESP8266 is intended to trigger actions to perform upon the laser
 *tag rifle
 *
 *Serial data sent to the ESP8266 is intended to be displayed on the LCD or for reporting
 *to a server.
 */

 /* 
 *  Section intended for logging of action variable and intended results when
 *  recieved from ESP8266. 
 *  
 *  Each time a value is sent to this ESP32 an action needs to perform once only
 *  to configure settings as listed below
 *  

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
 */
 


//****************************************************************
// libraries to include:
#include "BLEDevice.h"
#include <HardwareSerial.h>
HardwareSerial SerialLCD( 1 );
//****************************************************************

//******************* IMPORTANT *********************
//******************* IMPORTANT *********************
//******************* IMPORTANT *********************
//*********** YOU NEED TO CHANGE INFO IN HERE FOR EACH GUN!!!!!!***********
#define BLE_SERVER_SERVICE_NAME "jayeburden" // CHANGE ME!!!!
// this is important it is how we filter
// out unwanted guns or other devices that use UART BLE characteristics you need to change 
// this to the name of the gun ble server

// The remote service we wish to connect to, all guns should be the same as
// this is an uart comms set up
static BLEUUID serviceUUID("6E400001-B5A3-F393-E0A9-E50E24DCCA9E"); // CHANGE ME!!!!!!!
// The characteristic of the remote service we are interested in.
// these uuids are used to send and recieve data
static BLEUUID    charRXUUID("6E400002-B5A3-F393-E0A9-E50E24DCCA9E"); // CHANGE ME!!!!
static BLEUUID    charTXUUID("6E400003-B5A3-F393-E0A9-E50E24DCCA9E"); // CHANGE ME !!!!
int GunID = 0; // this is the gun or player ID, each esp32 needs a different one, set "0-63"
//******************* IMPORTANT *********************
//******************* IMPORTANT *********************
//******************* IMPORTANT *********************

//****************************************************************
// variables used for ble set up
static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
static BLERemoteCharacteristic* pRemoteRXCharacteristic;
static BLERemoteCharacteristic* pRemoteTXCharacteristic;
static BLEAdvertisedDevice* myDevice;
BLEClient*  pClient;

// variables used to provide notifications of pairing device 
// as well as connection status and recieving data from gun
char notifyData[100];
int notifyDataIndex = 0;
String tokenStrings[100];
char *ptr = NULL;
int paired;

// these are variables im using to create settings for the game mode and
// gun settings
int settingsallowed = 0; // trigger variable used to walk through steps for configuring gun(s)
int SetSlotA; // this is for weapon slot 0
int SetSlotB; // this is for weapon slot 1
int SetSlotC; // this is for weapon slot 4 or melee used in pickups only (future)
int SetTeam=0; // used to configure team player settings, default is 0
int SetTime; // used for in game timer functions on esp32 (future
int SetODMode=0; // used to set indoor and outdoor modes
int SetWSMode; // used to enable player gun selection with lcd... (future)
int SetGNDR=0; // used to change player to male 0/female 1, male is default 
int SetLives; // used for esp based game programs should be functional
int SetRSPNMode; // used to set auto or manual respawns from bases/ir (future)
int SetKillCnt; // to limit max kills
int SetObjct; // to set objective goal counts
int SetFF=1; // set game to friendly fire on/off (default is on))
int SetAmmo; // enable auto replenish of ammunition or from bases/players only
int AmmoGndr=0; // used to split ammo and gender variables because i piggy backed them on one ir protocol set

int MaxKills = 32767; // setting limit on kill counts
int Objectives = 32000; // objective goals
int PlayerLives = 32767; // setting max player lives
int MaxTeamLives = 32767; // setting maximum team lives
long GameTimer = 2000000000; // setting maximum game time
int PlayerKillCount[64] = {0}; // so its players 0-63 as the player id.
int TeamKillCount[6] = {0}; // teams 0-6, Red-0, blue-1, yellow-2, green-3, purple-4, cyan-5
int DelayStart = 10000; // set delay count down to 30 seconds for start
bool OutofAmmoA = false; // trigger for auto respawn of ammo weapon slot 0
bool OutofAmmoB = false; // trigger for auto respawn of ammo weapon slot 1
String AudioSelection; // used to play stored audio files with tagger

int lastTaggedPlayer = -1;  // used to capture player id who last shot gun, for kill count attribution
int lastTaggedTeam = -1;  // used to captures last player team who shot gun, for kill count attribution

int ammo = 0;
int weap = 0;
int health = 0;
int armor = 0;
int shield =0;
int lives = 0;

bool RESPAWN = false; // trigger to enable auto respawn when killed in game
bool GAMEOVER = false; // used to trigger game over and boot a gun out of play mode
bool TAGGERUPDATE = false; // used to trigger sending data to ESP8266
bool AUDIO = false; // used to trigger an audio on tagger

long startScan = 0; // part of BLE enabling

bool WEAP = false; // not used anymore but was used to auto load gun settings on esp boot

//**************************************************************
//**************************************************************
// this bad boy captures any ble data from gun and analyzes it based upon the
// protocol predecessor, a lot going on and very important as we are able to 
// use the ble data from gun to decode brx protocol with the gun, get data from 
// guns health status and ammo reserve etc.

static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
  Serial.print("Notify callback for characteristic ");
  Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
  Serial.print(" of data length ");
  Serial.println(length);
  Serial.print("data: ");
  Serial.println((char*)pData);

  memcpy(notifyData + notifyDataIndex, pData, length);
  notifyDataIndex = notifyDataIndex + length;
  if (notifyData[notifyDataIndex - 1] == '*') { // complete receviing
    notifyData[notifyDataIndex] = '\0';
    notifyDataIndex = 0; // reset index
    //Lets tokenize by ","
    String receData = notifyData;
    byte index = 0;
    ptr = strtok((char*)receData.c_str(), ",");  // takes a list of delimiters
    while (ptr != NULL)
    {
      tokenStrings[index] = ptr;
      index++;
      ptr = strtok(NULL, ",");  // takes a list of delimiters
    }
    Serial.println("We have found " + String(index ) + " tokens");
    
    //*******************************************************
    // first signal we get from gun is this one and it tells the esp
    // that gun is happy with the connection
    if (tokenStrings[0] == "$#CONNECT") {
      paired=1;
    }
    // this analyzes every single time a trigger is pulled
    // or a button is pressed or reload handle pulled. pretty cool
    // i dont do much with it yet here but will be used later to allow
    // players to select weapon pickups etc or options when starting a game
    // ideally with LCD installed this will be very usefull! can use it with
    // limited gun choices, like the basic smg, tar33 etc as we can use gun 
    // audio to help players know what they are picking but not implemented yet
    if (tokenStrings[0] == "$BUT") {
      if (tokenStrings[1] == "0") {
        if (tokenStrings[2] == "1") {
          Serial.println("Trigger pulled"); // as indicated this is the trigger
        }
        if (tokenStrings[2] == "0") {
          Serial.println("Trigger Released"); // goes without sayin... you let go of the trigger
          // upon release of a trigger, team settings can be changed if the proper allowance is in place
          if (settingsallowed==2){
            if (SetTeam==100) {SetTeam=0; AudioSelection="VA13"; AUDIO=true;}
            if (SetTeam==0) {SetTeam=1; AudioSelection="VAL1"; AUDIO=true;}
            if (SetTeam==1) {SetTeam=2; AudioSelection="VA1R"; AUDIO=true;}
            if (SetTeam==2) {SetTeam=3; AudioSelection="VA27"; AUDIO=true;}
            if (SetTeam==3) {SetTeam=4; AudioSelection="VA2G"; AUDIO=true;}
            if (SetTeam==4) {SetTeam=5; AudioSelection="VA2Y"; AUDIO=true;}
            if (SetTeam==5) {SetTeam=0; AudioSelection="VA13"; AUDIO=true;}          
          }
        }
      }
      if (tokenStrings[1] == "1") {
        if (tokenStrings[2] == "1") {
          Serial.println("Alt fire pulled"); // yeah.. you pushed the red/yellow button
        }
        if (tokenStrings[2] == "0") {
          Serial.println("Alt fire Released"); // now you released the button
          if (settingsallowed>0) {settingsallowed=99;}
        }
      } // we can do more for left right select and reload but yeah... maybe another time
    }
    
    // here is where we use the incoming ir to the guns sensors to our benefit in many ways
    // we can use it to id the player landing a tag and every aspect of that tag
    // but why stop there? we can use fabricated tags to set variables on the esp32 for programming game modes!
    if (tokenStrings[0] == "$HIR") {
      /*space 1 is the direction the tag came from or what sensor was hit, in above example sensor 4
        space 2 is for he type of tag recieved/shot. almost all are 0 and im thinking other types are medic etc.
        space 3 is the player id, in this case it was player 0 hitting player 1.
        space 4 is the team id, this was team 0 or red team
        space 5 is the damage dealt to the player, this took 13 hit points off the player 1
        space 6 is "is critical" if critical a damage multiplier would apply, rare.
        space 7 is "power", not sure what that does.*/
      //been tagged
      lastTaggedPlayer = tokenStrings[3].toInt();
      lastTaggedTeam = tokenStrings[4].toInt();
      Serial.println("Just tagged by: " + String(lastTaggedPlayer) + " on team: " + String(lastTaggedTeam));

    }
    
    // this is a cool pop up... lots of info provided and broken out below.
    // this pops up everytime you pull the trigger and deplete your ammo.hmmm you
    // can guess what im using that for... see more below
// ****** note i still need to assign the ammo to the lcd outputs********
    if (tokenStrings[0] == "$ALCD") {
      /* ammunition status update occured 
       *  space 2 is rounds remaining in clip
       *  space 3 is accuracy of weapon
       *  space 4 is slot of weapon
       *  space 5 is remaining ammo outside clip
       *  space 6 is the overheating feature if aplicable
       *  
       *  more can be done with this, like using the ammo info to post to lcd
       */
       TAGGERUPDATE=true;
      if ((tokenStrings[2] == "0") && (tokenStrings[4] == "0")) { // yes thats right folks... if we are out of ammo we set that cool trigger variable
        if ((tokenStrings[3] == "0")) {
          OutofAmmoA=true; // here is the variable for weapon slot 0
        }
        else {
          OutofAmmoB=true; // trigger variable for weapon slot 1
        }
      }    
    }
    
    // this guy is recieved from brx every time your health changes... usually after
    // getting shot.. from the HIR indicator above. but this can tell us our health status
    // and we can send it to an LCD or use it to know when were dead and award points to the 
    // player who loast tagged the gun... hence the nomenclature "lasttaggedPlayer" etc
    // add in some kill counts and we can deplete lives and use it to enable or disable respawn functions
    if (tokenStrings[0] == "$HP") {
      /*health status update occured
       * can be used for updates on health as well as death occurance
       */
       TAGGERUPDATE=true;
      health = tokenStrings[1].toInt();
      armor = tokenStrings[2].toInt();
      shield = tokenStrings[3].toInt();
      if ((tokenStrings[1] == "0") && (tokenStrings[2] == "0") && (tokenStrings[3] == "0")) {
        PlayerKillCount[lastTaggedPlayer]++; // adding a point to the last player who killed us
        TeamKillCount[lastTaggedTeam]++; // adding a point to the team who caused the last kill
        PlayerLives--; // taking our preset lives and subtracting one life then talking about it on the monitor
        Serial.println("Lives Remaining = " + String(PlayerLives));
        Serial.println("Killed by: " + String(lastTaggedPlayer) + " on team: " + String(lastTaggedTeam));
        Serial.println("Team: " + String(lastTaggedTeam) + "Score: " + String(TeamKillCount[lastTaggedTeam]));
        Serial.println("Player: " + String(lastTaggedPlayer) + " Score: " + String(PlayerKillCount[lastTaggedPlayer]));
        if (PlayerLives > 0) { // doing a check if we still have lives left after dying
          RESPAWN = true;
          Serial.println("respawn enabled");
          GAMEOVER = false;
        }
        else {
          GAMEOVER = true;
        }
      }
    }
  } else {
    //hold data and keep receving
  }
}

//******************************************************************************************************
// BLE client stuff, cant say i understan all of it but yeah... had some help with this
class MyClientCallback : public BLEClientCallbacks {
    void onConnect(BLEClient* pclient) {
    }

    void onDisconnect(BLEClient* pclient) {
      connected = false;
      doConnect = true;
      doScan = true;
      WEAP = false;
      Serial.println("onDisconnect");

    }
};

bool connectToServer() {
  Serial.print("Forming a connection to ");
  Serial.println(myDevice->getAddress().toString().c_str());


  Serial.println(" - Created client");



  // Connect to the remove BLE Server.
  if (pClient->connect(myDevice)) { // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
    Serial.println(" - Connected to server");

    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) {
      Serial.print("Failed to find our service UUID: ");
      Serial.println(serviceUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our service");


    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteRXCharacteristic = pRemoteService->getCharacteristic(charRXUUID);
    if (pRemoteRXCharacteristic == nullptr) {
      Serial.print("Failed to find our characteristic UUID: ");
      Serial.println(charRXUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our RX characteristic");

    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteTXCharacteristic = pRemoteService->getCharacteristic(charTXUUID);
    if (pRemoteTXCharacteristic == nullptr) {
      Serial.print("Failed to find our characteristic UUID: ");
      Serial.println(charTXUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our TX characteristic");

    // Read the value of the characteristic.
    if (pRemoteRXCharacteristic->canRead()) {
      std::string value = pRemoteRXCharacteristic->readValue();
      Serial.print("The characteristic value was: ");
      Serial.println(value.c_str());
    }

    if (pRemoteTXCharacteristic->canRead()) {
      std::string value = pRemoteTXCharacteristic->readValue();
      Serial.print("The characteristic value was: ");
      Serial.println(value.c_str());
    }

    if (pRemoteTXCharacteristic->canNotify())
      pRemoteTXCharacteristic->registerForNotify(notifyCallback);

    connected = true;
    return true;
  }
  return false;
}
/**
   Scan for BLE servers and find the first one that advertises the service we are looking for.
*/
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    /**
        Called for each advertising BLE server.
    */
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      Serial.print("BLE Advertised Device found: ");
      Serial.println(advertisedDevice.toString().c_str());

      // We have found a device, let us now see if it contains the service we are looking for.
      //if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {
      if (advertisedDevice.getName() == BLE_SERVER_SERVICE_NAME) {
        BLEDevice::getScan()->stop();
        myDevice = new BLEAdvertisedDevice(advertisedDevice);
        doConnect = true;
        doScan = false;

      } // Found our server
      else {
        doScan = true;
      }
    } // onResult
}; // MyAdvertisedDeviceCallbacks


//******************************************************************************************
// after recieving the first notification from gun that says connect we enable this function
// to give you a notification that the gun paired properly with esp device
void notifyconnection() {
  sendString("$PLAY,VA20,3,9,,,,,*");
  Serial.println("sending connection notification");
  paired=0;
}
//******************************************************************************************
// disclaimer... incomplete... 
// this function will be used when a player is eliminated and needs to respawn off of a base
// or player signal to respawn them... a lot to think about still on this and im using auto respawn 
// for now untill this is further thought out and developed
void taggedoutmode() {
  /*
  sendString("$START,*");
  sendString("$GSET,1,0,1,0,1,0,50,1,*");
  sendString("$PSET,54,5,45,70,70,50,,H44,JAD,V33,V3I,V3C,V3G,V3E,V37,H06,H55,H13,H21,H02,U15,W71,A10,*");
  sendString("$SIR,0,0,,1,0,0,1,,*");
  sendString("$SIR,0,1,,36,0,0,1,,*");
  sendString("$SIR,0,3,,37,0,0,1,,*");
  sendString("$SIR,8,0,,38,0,0,1,,*");
  sendString("$SIR,9,3,,24,10,0,,,*");
  sendString("$SIR,10,0,X13,1,0,100,2,60,*");
  sendString("$SIR,6,0,H02,1,0,90,1,40,*");
  sendString("$SIR,13,1,H57,1,0,0,1,,*");
  sendString("$SIR,13,0,H50,1,0,0,1,,*");
  sendString("$SIR,13,3,H49,1,0,100,0,60,*");
  sendString("$BMAP,0,0,,,,,*");
  sendString("$BMAP,1,100,0,1,99,99,*");
  sendString("$BMAP,2,97,,,,,*");
  sendString("$BMAP,3,98,,,,,*");
  sendString("$BMAP,4,98,,,,,*");
  sendString("$BMAP,5,98,,,,,*");
  sendString("$BMAP,8,4,,,,,*");
  sendString("$SPAWN,,*");
  sendString("$WEAP,0,*"); // cleared out weapon 0
  sendString("$WEAP,1,*"); // cleared out weapon 1
  sendString("$WEAP,4,*"); // cleared out melee weapon
  sendString("$GLED,,,,5,,,*"); // changes headset to tagged out color
  */
  RESPAWN = false;
}
//****************************************************************************************
//************** This sends Settings to Tagger *******************************************
//****************************************************************************************
// loads all the game configuration settings into the gun
void gameconfigurator() {
  Serial.println("Running Game Configurator based upon recieved inputs");
  sendString("$CLEAR,*");
  sendString("$START,*");
  SetFFOutdoor();
  playersettings();
  weaponsettingsA();
  weaponsettingsB();
  sendString("$WEAP,4,1,90,13,1,90,0,,,,,,,,1000,100,1,0,0,10,13,100,100,,0,0,,M92,,,,,,,,,,,,1,0,20,*");
  sendString("$SIR,0,0,,1,0,0,1,,*");
  sendString("$SIR,0,1,,36,0,0,1,,*");
  sendString("$SIR,0,3,,37,0,0,1,,*");
  sendString("$SIR,8,0,,38,0,0,1,,*");
  sendString("$SIR,9,3,,24,10,0,,,*");
  sendString("$SIR,10,0,X13,1,0,100,2,60,*");
  sendString("$SIR,6,0,H02,1,0,90,1,40,*");
  sendString("$SIR,13,1,H57,1,0,0,1,,*");
  sendString("$SIR,13,0,H50,1,0,0,1,,*");
  sendString("$SIR,13,3,H49,1,0,100,0,60,*");
  sendString("$BMAP,0,0,,,,,*");
  sendString("$BMAP,1,100,0,1,99,99,*");
  sendString("$BMAP,2,97,,,,,*");
  sendString("$BMAP,3,98,,,,,*");
  sendString("$BMAP,4,98,,,,,*");
  sendString("$BMAP,5,98,,,,,*");
  sendString("$BMAP,8,4,,,,,*");
  settingsallowed=6; // this is the next step... automatic now but maybe IR later
  Serial.println("Finished Game Configuration set up");
}

//****************************************************************************************
//************************ This starts a game *******************************************
//****************************************************************************************
// this starts a game
void delaystart() {
  Serial.println("Starting Delayed Game Start");
  sendString("$VOL,100,0,*"); // sets max volume on gun 0-100 feet distance
  sendString("$PLAY,VA84,4,5,,,,,*"); // plays a ten second countdown
  delay(DelayStart); // delays ten seconds or as configured
  // sendString("$PLAY,VA81,4,6,,,,,*"); // plays the .. nevermind
  sendString("$SPAWN,,*");
  Serial.println("Delayed Start Complete, should be in game play mode now");
}

//******************************************************************************************

// function called for to get the first burst of ir protocol settings
void roundonesettings() {
  SetSlotB=lastTaggedPlayer; // weapon slot 1 designated by player id
  SetKillCnt=lastTaggedTeam; // kill count set by team id
  SetLives=tokenStrings[2].toInt(); // lives set by bullet type
  SetObjct=tokenStrings[5].toInt(); // objectives set by damage type
  SetRSPNMode=tokenStrings[6].toInt(); // respawn mode determined by is critical
  SetFF=tokenStrings[7].toInt(); // friendly fire enabled by power indicator
  Serial.println("Round 2 settings loaded!");
  Serial.print("Weapon Slot 1 set to: ");
  Serial.println(SetSlotB);
  Serial.print("Set kill count limit to: ");
  Serial.println(SetKillCnt);
  Serial.print("Set max lives to: ");
  Serial.println(SetLives);
  Serial.print("Set game objectives to: ");
  Serial.println(SetObjct);
  Serial.print("Set Respawn mode to: ");
  Serial.println(SetRSPNMode);
  Serial.print("Set friendly fire to: ");
  Serial.println(SetFF);
  gametimesettings();
  gamelivessettings();
  killcountsettings();
  objectivesettings();
  SetSlotA=lastTaggedPlayer; // using the player id for weapon slot 0
  SetTeam=lastTaggedTeam; // using team id for team id
  SetTime=tokenStrings[2].toInt(); // using bullet type protocol for in game timer
  SetODMode=tokenStrings[5].toInt(); // using damage protocol for outdoor mode
  SetWSMode=tokenStrings[6].toInt(); // using is critical protocol to set gun selection option
  AmmoGndr=tokenStrings[7].toInt(); // using power type for gender set and ammo mode
  if(AmmoGndr == 0) {SetGNDR=0; SetAmmo=0;} // just separating the two and assigning them separately
  if(AmmoGndr == 1) {SetGNDR=1; SetAmmo=0;}
  if(AmmoGndr == 2) {SetGNDR=0; SetAmmo=1;}
  if(AmmoGndr == 3) {SetGNDR=1; SetAmmo=1;}
  tokenStrings[0] = "null";
  Serial.println("Round 1 settings loaded!"); // just said that!
  Serial.print("Weapon slot 0 set to: ");
  Serial.println(SetSlotA);
  Serial.print("Team ID set to: ");
  Serial.println(SetTeam);
  Serial.print("Outdoor mode set to: ");
  Serial.println(SetODMode);
  Serial.print("Weapon Selection Mode: ");
  Serial.println(SetWSMode);
  Serial.print("Gender set to: ");
  Serial.println(SetGNDR);
  Serial.print("Ammo reload setting set to: ");
  Serial.println(SetAmmo);
  Serial.println("awaiting second ir programing tag");
}

//******************************************************************************************

// process used to send string properly to gun... splits up longer strings in bytes of 20
// to make sure gun understands them all... not sure about all of what does what below...
// had some major help from Sri Lanka Guy!
void sendString(String value) {
  const char * c_string = value.c_str();
  uint8_t buf[21] = {0};
  int sentSize = 0;
  Serial.println("sending ");
  if (value.length() > 20) {
    for (int i = 0; i < value.length() / 20; i++) {
      memcpy(buf, c_string + i * 20, 20);
      Serial.print((char*)buf);
      pRemoteRXCharacteristic->writeValue(buf, 20, true);
      sentSize += 20;
    }
    int remaining = value.length() - sentSize;
    memcpy(buf, c_string + sentSize, remaining);
    pRemoteRXCharacteristic->writeValue(buf, remaining, true);
    for (int i = 0; i < remaining; i++)
      Serial.print((char)buf[i]);
    Serial.println();
  }
  else {
    pRemoteRXCharacteristic->writeValue((uint8_t*)value.c_str(), value.length(), true);
  }
}

//******************************************************************************************

void gametimesettings() {
  /* ideally we want setting options for minutes: 1,3,5,10,20,30,60, unlimited
   *  we will be using the bullet type ir or blynk for setting the time limits
   */
   if (SetTime = 0) {GameTimer=60000;} // timer set to one minute
   if (SetTime = 1) {GameTimer=180000;} // timer set to 3 minutes
   if (SetTime = 2) {GameTimer=300000;} // timer set to 5 minutes
   if (SetTime = 3) {GameTimer=600000;} // timer set to 10 minutes
   if (SetTime = 4) {GameTimer=1200000;} // timer set to 20 minutes
   if (SetTime = 5) {GameTimer=1800000;} // timer set to 30 minutes
   if (SetTime = 6) {GameTimer=3600000;} // timer set to 60 minutes
   if (SetTime > 6) {GameTimer=2000000000;} // set to unlimited minutes
  Serial.println("Timer set to " + String(GameTimer) + " milliseconds"); 
}

//******************************************************************************************

void gamelivessettings() {
  /* ideally we want setting options for minutes: 1,3,5,10,20,30,60, unlimited
   *  we will be using the bullet type ir or blynk for setting the time limits
   */
   if (SetLives = 0) {PlayerLives=1;} // lives set to one 
   if (SetLives = 1) {PlayerLives=3;} // lives set to 3 
   if (SetLives = 2) {PlayerLives=5;} // lives set to 5 
   if (SetLives = 3) {PlayerLives=10;} // lives set to 10 
   if (SetLives = 4) {PlayerLives=20;} // lives set to 20 
   if (SetLives = 5) {PlayerLives=30;} // lives set to 30 
   if (SetLives = 6) {PlayerLives=60;} // lives set to 60 
   if (SetLives > 6) {PlayerLives=32000;} // set to unlimited 
   Serial.println("Lives set to " + String(PlayerLives) + " max lives"); 

}

//******************************************************************************************

void killcountsettings() {
  /* ideally we want setting options for max kills: 10,20,50, unlimited
   *  we will be using the team type ir or blynk for setting the time limits
   */
   if (SetKillCnt = 0) {MaxKills=10;} // max kills set to 10 
   if (SetKillCnt = 1) {MaxKills=20;} // max kills set to 20 
   if (SetKillCnt = 2) {MaxKills=50;} // max kills set to 50 
   if (SetKillCnt = 3) {MaxKills=32000;} // max kills set to unlimited
   Serial.println("Kill limit set to " + String(MaxKills) + " total kills"); 
}

//******************************************************************************************

void objectivesettings() {
  /* ideally we want setting options for objectives: 10,20,50, unlimited
   *  we will be using the team type ir or blynk for setting the time limits
   */
   if (SetObjct = 1) {Objectives=1;} // objectives set to 1 
   if (SetObjct = 2) {Objectives=3;} // objectives set to 3 
   if (SetObjct = 3) {Objectives=5;} // objectives set to 5 
   if (SetObjct = 4) {Objectives=10;} // objectives set to 10
   if (SetObjct > 4) {Objectives=32000;} // objectives set to unlimited
   Serial.println("Objective limit set to " + String(Objectives) + " max points"); 
}

//******************************************************************************************

// sets and sends player settings to gun based upon configuration settings
void playersettings() {
  // token 2 is player id or gun id other tokens were interested in modification
  // are 2 for team ID, 3 for max HP, 4 for max Armor, 5 for Max Shields, 
  // 6 is critical damage bonus
  // We really are only messing with Gender and Team though
  // Gender is determined by the audio call outs listed, tokens 9 and on
  // male is default as 0, female is 1
  if(SetGNDR == 0) {sendString("$PSET,"+String(GunID)+","+String(SetTeam)+",45,70,70,50,,H44,JAD,V33,V3I,V3C,V3G,V3E,V37,H06,H55,H13,H21,H02,U15,W71,A10,*");}
  else {sendString("$PSET,"+String(GunID)+","+String(SetTeam)+",45,70,70,50,,H44,JAD,VB3,VBI,VBC,VBG,VBE,VB7,H06,H55,H13,H21,H02,U15,W71,A10,*");}
  
}

//******************************************************************************************

// sets and sends gun type to slot 0 based upon stored settings
void weaponsettingsA() {
  if(SetSlotA == 1) {Serial.println("Weapon 0 set to Unarmed"); sendString("$WEAP,0,*");} // cleared out weapon 0
  if(SetSlotA == 2) {Serial.println("Weapon 0 set to AMR"); sendString("$WEAP,0,,100,0,3,18,0,,,,,,,,360,850,14,56,1400,0,7,100,100,,0,,,S07,D20,D19,,D04,D03,D21,D18,,,,,14,28,75,,*");}
  if(SetSlotA == 3) {Serial.println("Weapon 0 set to Assault Rifle"); sendString("$WEAP,0,,100,0,0,9,0,,,,,,,,100,850,32,384,1400,0,0,100,100,,0,,,R01,,,,D04,D03,D02,D18,,,,,32,192,75,,*");}
  if(SetSlotA == 4) {Serial.println("Weapon 0 set to Bolt Rifle"); sendString("$WEAP,0,,100,0,3,13,0,,,,,,,,225,850,18,180,2000,0,7,100,100,,0,,,R12,,,,D04,D03,D02,D18,,,,,18,90,75,,*");}
  if(SetSlotA == 5) {Serial.println("Weapon 0 set to BurstRifle"); sendString("$WEAP,0,,100,0,3,9,0,,,,,,,,75,850,36,216,1700,0,9,100,100,275,0,,,R18,,,,D04,D03,D02,D18,,,,,36,108,75,,*");}
  if(SetSlotA == 6) {Serial.println("Weapon 0 set to ChargeRifle"); sendString("$WEAP,0,,100,8,0,100,0,,,,,,,,1250,850,100,200,2500,0,14,100,100,,14,,,E03,C15,C17,,D30,D29,D37,A73,C19,C04,20,150,100,100,75,,*");}
  if(SetSlotA == 7) {Serial.println("Weapon 0 set to Energy Launcher"); sendString("$WEAP,0,,100,9,3,115,0,,,,,,,,360,850,1,6,1400,0,0,100,100,,0,,,J15,,,,D14,D13,D12,D18,,,,,1,3,75,,*");}
  if(SetSlotA == 8) {Serial.println("Weapon 0 set to Energy Rifle"); sendString("$WEAP,0,,100,0,0,9,0,,,,,,,,90,850,300,600,2400,0,0,100,100,,6,,,E12,,,,D17,D16,D15,A73,D122,,,,300,300,75,,*");}
  if(SetSlotA == 9) {Serial.println("Weapon 0 set to Force Rifle"); sendString("$WEAP,0,,100,0,1,9,0,,,,,,,,100,850,36,144,1700,0,9,100,100,250,0,,,R23,D20,D19,,D23,D22,D21,D18,,,,,36,72,75,,*");}
  if(SetSlotA == 10) {Serial.println("Weapon 0 set to Ion Sniper"); sendString("$WEAP,0,,100,0,0,115,0,,,,,,,,1000,850,2,12,2000,0,7,100,100,,0,,,E07,D32,D31,,D17,D16,D15,A73,,,,,2,6,75,,*");}
  if(SetSlotA == 11) {Serial.println("Weapon 0 set to Laser Cannon"); sendString("$WEAP,0,,100,0,0,115,0,,,,,,,,1500,850,4,8,2000,0,3,100,100,,0,,,C06,C11,,,D17,D16,D15,A73,,,,,4,4,75,,*");}
  if(SetSlotA == 12) {Serial.println("Weapon 0 set to Plasma Sniper"); sendString("$WEAP,0,2,100,0,0,80,0,,,,,,80,80,225,850,10,80,2000,0,7,100,100,,30,,,E17,,,,D35,D34,D36,A73,D122,,,,10,40,75,40,*");}
  if(SetSlotA == 13) {Serial.println("Weapon 0 set to Rail Gun"); sendString("$WEAP,0,0,100,6,0,115,0,,,,,,,,1200,850,1,6,2400,0,2,100,100,,0,,,C03,C08,,,D36,D35,D34,A73,,,,,1,3,75,,*");}
  if(SetSlotA == 14) {Serial.println("Weapon 0 set to Rocket Launcher"); sendString("$WEAP,0,2,100,10,0,115,0,,,,,,115,80,1000,850,2,8,1200,0,7,100,100,,0,,,C03,,,,D14,D13,D12,D18,,,,,2,4,75,30,*");}
  if(SetSlotA == 15) {Serial.println("Weapon 0 set to Shotgun"); sendString("$WEAP,0,2,100,0,0,45,0,,,,,,70,80,900,850,6,24,400,2,7,100,100,,0,,,T01,,,,D01,D28,D27,D18,,,,,6,12,75,30,*");}
  if(SetSlotA == 16) {Serial.println("Weapon 0 set to SMG"); sendString("$WEAP,0,,100,0,0,8,0,,,,,,,,90,850,72,288,2500,0,0,100,100,,5,,,G03,,,,D26,D25,D24,D18,D11,,,,72,144,75,,*");}
  if(SetSlotA == 17) {Serial.println("Weapon 0 set to Sniper Rifle"); sendString("$WEAP,0,,100,0,1,80,0,,,,,,,,300,850,4,24,1700,0,7,100,100,,0,,,S16,D20,D19,,D04,D03,D21,D18,,,,,4,12,75,,*");}
  if(SetSlotA == 18) {Serial.println("Weapon 0 set to Stinger"); sendString("$WEAP,0,,100,0,0,15,0,,,,,,,,120,850,18,72,1700,0,0,100,100,,0,,,E11,,,,D17,D16,D15,A73,,,,,18,36,75,,*");}
  if(SetSlotA == 19) {Serial.println("Weapon 0 set to Suppressor"); sendString("$WEAP,0,,100,0,0,8,0,,,,,,,,75,850,48,288,2000,0,0,100,100,,0,2,50,Q06,,,,D26,D25,D24,D18,,,,,48,144,75,,*");}
}

//******************************************************************************************

// sets and sends gun for slot 0 based upon stored settings
void weaponsettingsB() {
  if(SetSlotB == 1) {Serial.println("Weapon 1 set to Unarmed"); sendString("$WEAP,1,*");} // cleared out weapon 1
  if(SetSlotB == 2) {Serial.println("Weapon 1 set to AMR"); sendString("$WEAP,0,,100,0,3,18,0,,,,,,,,360,850,14,56,1400,0,7,100,100,,0,,,S07,D20,D19,,D04,D03,D21,D18,,,,,14,28,75,,*");}
  if(SetSlotB == 3) {Serial.println("Weapon 1 set to Assault Rifle"); sendString("$WEAP,0,,100,0,0,9,0,,,,,,,,100,850,32,384,1400,0,0,100,100,,0,,,R01,,,,D04,D03,D02,D18,,,,,32,192,75,,*");}
  if(SetSlotB == 4) {Serial.println("Weapon 1 set to Bolt Rifle"); sendString("$WEAP,0,,100,0,3,13,0,,,,,,,,225,850,18,180,2000,0,7,100,100,,0,,,R12,,,,D04,D03,D02,D18,,,,,18,90,75,,*");}
  if(SetSlotB == 5) {Serial.println("Weapon 1 set to BurstRifle"); sendString("$WEAP,0,,100,0,3,9,0,,,,,,,,75,850,36,216,1700,0,9,100,100,275,0,,,R18,,,,D04,D03,D02,D18,,,,,36,108,75,,*");}
  if(SetSlotB == 6) {Serial.println("Weapon 1 set to ChargeRifle"); sendString("$WEAP,0,,100,8,0,100,0,,,,,,,,1250,850,100,200,2500,0,14,100,100,,14,,,E03,C15,C17,,D30,D29,D37,A73,C19,C04,20,150,100,100,75,,*");}
  if(SetSlotB == 7) {Serial.println("Weapon 1 set to Energy Launcher"); sendString("$WEAP,0,,100,9,3,115,0,,,,,,,,360,850,1,6,1400,0,0,100,100,,0,,,J15,,,,D14,D13,D12,D18,,,,,1,3,75,,*");}
  if(SetSlotB == 8) {Serial.println("Weapon 1 set to Energy Rifle"); sendString("$WEAP,0,,100,0,0,9,0,,,,,,,,90,850,300,600,2400,0,0,100,100,,6,,,E12,,,,D17,D16,D15,A73,D122,,,,300,300,75,,*");}
  if(SetSlotB == 9) {Serial.println("Weapon 1 set to Force Rifle"); sendString("$WEAP,0,,100,0,1,9,0,,,,,,,,100,850,36,144,1700,0,9,100,100,250,0,,,R23,D20,D19,,D23,D22,D21,D18,,,,,36,72,75,,*");}
  if(SetSlotB == 10) {Serial.println("Weapon 1 set to Ion Sniper"); sendString("$WEAP,0,,100,0,0,115,0,,,,,,,,1000,850,2,12,2000,0,7,100,100,,0,,,E07,D32,D31,,D17,D16,D15,A73,,,,,2,6,75,,*");}
  if(SetSlotB == 11) {Serial.println("Weapon 1 set to Laser Cannon"); sendString("$WEAP,0,,100,0,0,115,0,,,,,,,,1500,850,4,8,2000,0,3,100,100,,0,,,C06,C11,,,D17,D16,D15,A73,,,,,4,4,75,,*");}
  if(SetSlotB == 12) {Serial.println("Weapon 1 set to Plasma Sniper"); sendString("$WEAP,0,2,100,0,0,80,0,,,,,,80,80,225,850,10,80,2000,0,7,100,100,,30,,,E17,,,,D35,D34,D36,A73,D122,,,,10,40,75,40,*");}
  if(SetSlotB == 13) {Serial.println("Weapon 1 set to Rail Gun"); sendString("$WEAP,0,0,100,6,0,115,0,,,,,,,,1200,850,1,6,2400,0,2,100,100,,0,,,C03,C08,,,D36,D35,D34,A73,,,,,1,3,75,,*");}
  if(SetSlotB == 14) {Serial.println("Weapon 1 set to Rocket Launcher"); sendString("$WEAP,0,2,100,10,0,115,0,,,,,,115,80,1000,850,2,8,1200,0,7,100,100,,0,,,C03,,,,D14,D13,D12,D18,,,,,2,4,75,30,*");}
  if(SetSlotB == 15) {Serial.println("Weapon 1 set to Shotgun"); sendString("$WEAP,0,2,100,0,0,45,0,,,,,,70,80,900,850,6,24,400,2,7,100,100,,0,,,T01,,,,D01,D28,D27,D18,,,,,6,12,75,30,*");}
  if(SetSlotB == 16) {Serial.println("Weapon 1 set to SMG"); sendString("$WEAP,0,,100,0,0,8,0,,,,,,,,90,850,72,288,2500,0,0,100,100,,5,,,G03,,,,D26,D25,D24,D18,D11,,,,72,144,75,,*");}
  if(SetSlotB == 17) {Serial.println("Weapon 1 set to Sniper Rifle"); sendString("$WEAP,0,,100,0,1,80,0,,,,,,,,300,850,4,24,1700,0,7,100,100,,0,,,S16,D20,D19,,D04,D03,D21,D18,,,,,4,12,75,,*");}
  if(SetSlotB == 18) {Serial.println("Weapon 1 set to Stinger"); sendString("$WEAP,0,,100,0,0,15,0,,,,,,,,120,850,18,72,1700,0,0,100,100,,0,,,E11,,,,D17,D16,D15,A73,,,,,18,36,75,,*");}
  if(SetSlotB == 19) {Serial.println("Weapon 1 set to Suppressor"); sendString("$WEAP,0,,100,0,0,8,0,,,,,,,,75,850,48,288,2000,0,0,100,100,,0,2,50,Q06,,,,D26,D25,D24,D18,,,,,48,144,75,,*");} 
}

//******************************************************************************************

// sets and sends game settings based upon the stored settings
void SetFFOutdoor() {
  // token one of the following command is free for all, 0 is off and 1 is on
  if(SetODMode == 0 && SetFF == 1) {sendString("$GSET,1,0,1,0,1,0,50,1,*");}
  if(SetODMode == 1 && SetFF == 1) {sendString("$GSET,1,1,1,0,1,0,50,1,*");}
  if(SetODMode == 1 && SetFF == 0) {sendString("$GSET,0,1,1,0,1,0,50,1,*");}
  if(SetODMode == 0 && SetFF == 0) {sendString("$GSET,0,0,1,0,1,0,50,1,*");}
}

//******************************************************************************************

// ends game... thats all
void gameover() {
  sendString("STOP,*"); // stops everything going on
  sendString("CLEAR,*"); // clears out anything stored for game settings
  sendString("$PLAY,VS6,4,6,,,,,*"); // says game over
  GAMEOVER = false;
  settingsallowed=0;
}

//******************************************************************************************

// as the name says... respawn a player!
void respawnplayer() {
  Serial.println("Respawning Player");
  sendString("$WEAP,0,*"); // cleared out weapon 0
  sendString("$WEAP,1,*"); // cleared out weapon 1
  sendString("$WEAP,4,*"); // cleared out melee weapon
  weaponsettingsA();
  weaponsettingsB();
  //sendString("$WEAP,0,,100,0,1,9,0,,,,,,,,100,850,36,144,1700,0,9,100,100,250,0,,,R23,D20,D19,,D23,D22,D21,D18,,,,,36,72,75,*");
  //sendString("$WEAP,1,2,100,0,0,45,0,,,,,,70,80,900,850,6,24,400,2,7,100,100,,0,,,T01,,,,D01,D28,D27,D18,,,,,6,12,75,30,*");
  sendString("$WEAP,4,1,90,13,1,90,0,,,,,,,,1000,100,1,0,0,10,13,100,100,,0,0,,M92,,,,,,,,,,,,1,0,20,*");
  sendString("$HLOOP,0,0,*"); // not sure what this does
  sendString("$GLED,,,,5,,,*"); // changes headset to tagged out color
  delay(3000);
  sendString("$WEAP,0,*"); // cleared out weapon 0
  sendString("$WEAP,1,*"); // cleared out weapon 1
  sendString("$WEAP,4,*"); // cleared out melee weapon
  weaponsettingsA();
  weaponsettingsB();
  //sendString("$WEAP,0,,100,0,1,9,0,,,,,,,,100,850,36,144,1700,0,9,100,100,250,0,,,R23,D20,D19,,D23,D22,D21,D18,,,,,36,72,75,*");
  //sendString("$WEAP,1,2,100,0,0,45,0,,,,,,70,80,900,850,6,24,400,2,7,100,100,,0,,,T01,,,,D01,D28,D27,D18,,,,,6,12,75,30,*");
  sendString("$WEAP,4,1,90,13,1,90,0,,,,,,,,1000,100,1,0,0,10,13,100,100,,0,0,,M92,,,,,,,,,,,,1,0,20,*");
  //sendString("$PLAYX,0,*");
  //sendString("$PLAY,VA81,4,6,,,,,*");
  sendString("$SPAWN,,*"); // respawns player back in game
  Serial.println("Player Respawned");
  RESPAWN = false;
}
//****************************************************************************************
// this object is activated if a manual input is needed for gun settings
// exitsettings object needs to be ran to leave this mode
void getsettings() {
  sendString("$START,*");
  sendString("$GSET,1,0,1,0,1,0,50,1,*");
  sendString("$PSET,64,5,200,200,200,,,,,,,,,,,,,,,,,,,*");
  sendString("$VOL,75,0,*"); // sets max volume on gun 0-100 feet distance
  sendString("$SIR,0,0,,1,0,0,1,,*");
  sendString("$SIR,0,1,,36,0,0,1,,*");
  sendString("$SIR,0,3,,37,0,0,1,,*");
  sendString("$SIR,8,0,,38,0,0,1,,*");
  sendString("$SIR,9,3,,24,10,0,,,*");
  sendString("$SIR,10,0,X13,1,0,100,2,60,*");
  sendString("$SIR,6,0,H02,1,0,90,1,40,*");
  sendString("$SIR,13,1,H57,1,0,0,1,,*");
  sendString("$SIR,13,0,H50,1,0,0,1,,*");
  sendString("$SIR,13,3,H49,1,0,100,0,60,*");
  sendString("$BMAP,0,0,,,,,*"); // button mapping
  sendString("$BMAP,1,100,0,1,99,99,*"); // button mapping
  sendString("$BMAP,2,97,,,,,*"); // button maping
  sendString("$BMAP,3,98,,,,,*"); // button mapping
  sendString("$BMAP,4,98,,,,,*"); // button mapping
  sendString("$BMAP,5,98,,,,,*"); // button mapping
  sendString("$BMAP,8,4,,,,,*"); // button mapping
  sendString("$PLAYX,0,*");
  sendString("$PLAY,SW04,4,6,,,,,*"); // plays starwars music
  sendString("$SPAWN,,*");
  sendString("$WEAP,0,*"); // cleared out weapon 0
  sendString("$WEAP,1,*"); // cleared out weapon 1
  sendString("$WEAP,4,*"); // cleared out melee weapon
  sendString("$GLED,,,,5,,,*"); // changes headset to tagged out color
  if (SetTeam=100) {settingsallowed=2;} // enables team selection from trigger input and exit game from alt fire input
}

void exitgetsettings() {
  sendString("$GLED,,,,5,,,*"); // changes headset to tagged out color
  delay(3000);
  sendString("STOP,*"); // stops everything going on
  sendString("CLEAR,*"); // clears out anything stored for game settings in gun, not esp
  settingsallowed=0; // resets settings enabling trigger
  Serial.println("successfully exited get settings gun state");
}

//******************************************************************************************
//******************************************************************************************
//******************************************************************************************
// serial communications that is pinned to second core in set up
// this is where we write to variable to configure settings to send over BLE to tagger
void serialTask(void * params){
  for(;;){
    
    if (TAGGERUPDATE){
      String LCDText = String(ammo)+","+String(weap)+","+String(health)+","+String(armor)+","+String(shield)+","+String(lives);
      SerialLCD.println(LCDText);
      TAGGERUPDATE=false;
    }
     if(SerialLCD.available()){
      String readtxt = SerialLCD.readStringUntil('\n');
      Serial.println(readtxt);
      // set the BLE core to play audio
      // it is important to ensure that all following processes only assign a value
      // for a variable that is used by the other core to process and send BLE data
      // to tagger, dont call objects in here that are potentially used by the other core
      // What we need to do here is analyze the serial data from ESP8266 and do something with it
      // So we set a programing variable, print the change to serial, and make the tagger confirm via audio

      if(readtxt.toInt()==601) {SetODMode=0; Serial.println("Outdoor Mode On"); AudioSelection="VA4W";}
      if(readtxt.toInt()==602) {SetODMode=1; Serial.println("Indoor Mode On"); AudioSelection="VA3W";}
      if(readtxt.toInt()==603) {SetODMode=1; Serial.println("Stealth Mode On"); AudioSelection="VA60";}

      if(readtxt.toInt()==701) {Serial.println("Free For All"); SetTeam=0; AudioSelection="VA4W";}
      if(readtxt.toInt()==702) {Serial.println("Teams Mode Two Teams (odds/evens)"); if (GunID % 2) {SetTeam=0; AudioSelection="VA13";} else {SetTeam=1; AudioSelection="VA1L";}}
      if(readtxt.toInt()==705) {Serial.println("Teams Mode Player's Choice"); settingsallowed=1; SetTeam=100; AudioSelection="VA5E";} // this one allows for manual input of settings... each gun will need to select a team now
            
      if(readtxt.toInt()==1200) {SetGNDR=0; Serial.println("Gender set to Male"); AudioSelection="V3I";}
      if(readtxt.toInt()==1201) {SetGNDR=1; Serial.println("Gender set to Female"); AudioSelection="VBI";}
      
      if(readtxt.toInt()==1401) {SetFF=1; Serial.println("Friendly Fire On"); AudioSelection="VA32";}
      if(readtxt.toInt()==1400) {SetFF=0; Serial.println("Friendly Fire Off"); AudioSelection="VA31";}

      if(1600 > readtxt.toInt() && readtxt.toInt() > 0) {AUDIO=true;} // sets trigger for other core BLE core to play an audio sound
    }
  }
}
TaskHandle_t  TaskSerial;
//****************************************************************************
void Audio() {
  sendString("$PLAY,"+AudioSelection+",4,6,,,,,*");
  AUDIO=false;
}
//******************************************************************************************
//******************************************************************************************
//******************************************************************************************

void setup() {
  Serial.begin(115200);
  Serial.println("Starting Arduino BLE Client application...");
  SerialLCD.begin(9600,SERIAL_8N1, 16, 17); // setting up serial communication with ESP8266 on pins 16/17 w baud rate of 9600
  delay(5000);
  BLEDevice::init("");

  // Retrieve a Scanner and set the callback we want to use to be informed when we
  // have detected a new device.  Specify that we want active scanning and start the
  // scan to run for 5 seconds.
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(10, true);
  BLEDevice::setPower(ESP_PWR_LVL_N14);

  pClient  = BLEDevice::createClient();
  pClient->setClientCallbacks(new MyClientCallback());

  xTaskCreatePinnedToCore( // pins the serial task loop to the second core
    serialTask,
    "SerialLoop",
    8000,
    NULL,
    1,
    &TaskSerial,
    0);

} // End of setup.

//******************************************************************************************
//******************************************************************************************
//******************************************************************************************

// This is the Arduino main loop function.
void loop() {
  // the main loop for BLE activity is here, it is devided in three sections....
  // sections are for when connected, when not connected and to connect again

  //***********************************************************************************
  // If the flag "doConnect" is true then we have scanned for and found the desired
  // BLE Server with which we wish to connect.  Now we connect to it.  Once we are
  // connected we set the connected flag to be true.
  if (doConnect == true) {
    if (connectToServer()) {
      Serial.println("We are now connected to the BLE Server.");
      doConnect = false; // to try and make the connection again.
    } else {
      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
    }
  }
  //*************************************************************************************
  // If we are connected to a peer BLE Server, update the characteristic each time we are reached
  // with the current time since boot.
  if (connected) {
    // Set the characteristic's value to be the array of bytes that is actually a string.
    //pRemoteCharacteristic->writeValue(newValue.c_str(), newValue.length());
    //pRemoteCharacteristic->writeValue((uint8_t*)newValue.c_str(), newValue.length(),true);
    
    // here we put in processes to run based upon conditions to make a game function

    if (settingsallowed==1) {getsettings();} // this is triggered if a manual option is required for game settings
    if (settingsallowed==99) {exitgetsettings();} // this ends get settings mode
    if (RESPAWN) { // checks if respawn was triggered to respawn a player
      respawnplayer(); // respawns player
    }
    if (GAMEOVER) { // checks if something triggered game over (out of lives, objective met)
      gameover(); // runs object to kick player out of game
    }
    if (AUDIO) {
      Audio();
    }
  }
  //************************************************************************************
  else if (doScan) {
    if (millis() - startScan > 11000) {
      Serial.println("Scanning again");
      BLEDevice::init("");
      BLEDevice::getScan()->start(10, true); // this is just eample to start scan after disconnect, most likely there is better way to do it in arduino
      startScan = millis();
    }
  }
} // End of loop
