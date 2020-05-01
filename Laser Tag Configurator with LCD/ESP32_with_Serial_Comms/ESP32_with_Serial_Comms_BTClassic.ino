/*
   update 4/2/2020 annotations added and copied over objects for setting up games
   update 4/3/2020 Cleaned up for more compatibility to serial comms programing
   update 4/3/2020 Was able to plug in set setting from serial for friendly fire and outdoor/indoor mode
   update 4/4/2020 was able to get team settings and manual input team settings working as well as gender settings
   updated 4/4/2020 included weapon selection assignment and notification to select in game settings manual options
   updated 4/6/2020 separated many variables to not share writing abilities from both cores (trouble shooting)
   upfsyrf 4/7/2020 finished isolating variables between cores to eliminate both cores writing to same variables
   updated 4/7/2020 fixed the resetting of the BLE function so device stays paired to brx, also integrated the delay start with the send game settings to start the game
   updated 4/8/2020 fixed some menu callout audio selections, reduced the delay time to verify what is best and still functions to avoid disconnects
   updated 4/8/2020 fixed weapon slot 1 loading issue, was set to weapon slot 0 so guns were overwriting.
   updated 4/9/2020 fixed team selection item number and set freindly fire on when free for all selection is made for team selection.
   updated 4/9/2020 fixed audio announcement for teams for free for all to say free for all
   updated 4/9/2020 fixed audio announcement for domination to read control point versus select a game
   updated 4/9/2020 fixed limited ammo, wasnt allowing limited ammo due to incrorrect if then statement setting
   updated 4/9/2020 fixed the manual team selection option to enable players to pick their own teams
   updated 4/10/2020 enabled LCD data sending to esp8266, updated data to be sent to get lives, weapon and other correct indicators sent to the LCD
   updated 4/13/2020 worked on more LCD debuging issues for sending correct data to LCD ESP8266
   updated 4/14/2020 changed power output for the BLE antenna to try to minimize disconnects, was successful
   updated 4/15/2020 fixed unlimited ammo when unarmed, was looping non stop for reloading because no ammo on unarmed
   updated 4/15/2020 added additional ammunition options, limited, unlimited magazines, and unlimited rounds as options - ulimited rounds more for kids
   updated 4/19/2020 improved team selection process, incorporated "End Game" selection (requires app update) disabled buttons/trigger/reload from making noises when pressed upon connection to avoid annoying sounds from players
   updated 4/19/2020 enabled player selection for weapon slots 0/1. tested, debugged and ready to go for todays changes.
   updated 4/22/2020 enabled serial communications to send weapon selection to ESP8266 so that it can be displayed what weapon is what if lCD is installed
   updated 4/22/2020 enabled game timer to terminate a game for a player
   updated 4/27/2020 enabled serial send of game score data to esp8266

   Written by Jay Burden

   keep device in range of laser tag BLE enabled compatible device for it to work
   This code requires an esp8266 to be paired via serial (reference esp8266 code)
        use pins 16 & 17 to connect to D8 & D7, respectively, of ESP8266
   optionally power from laser tag battery to the GRND and the VCC pins or power
        via usb or standalone battery
   optionally install a transistor across reset pins on laser tag device to pin 4
   this devices uses the serial communication to set tagger and game configuration settings
   Note the sections labeled *****IMPORTANT***** as it requires customization on your part

  Serial data recieved from ESP8266 is intended to trigger actions to perform upon the laser
  tag rifle

  Serial data sent to the ESP8266 is intended to be displayed on the LCD or for reporting
  to a server.
*/

/*
   Section intended for logging of action variable and intended results when
   recieved from ESP8266.

   Each time a value is sent to this ESP32 an action needs to perform once only
   to configure settings as listed below

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
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

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

String MACadd = "AA:BB:CC:11:22:33";
uint8_t address[6]  = {0xAA, 0xBB, 0xCC, 0x11, 0x22, 0x33};
char *pin = "1234";
int GunID = 0; // this is the gun or player ID, each esp32 needs a different one, set "0-63"
//******************* IMPORTANT *********************
//******************* IMPORTANT *********************
//******************* IMPORTANT *********************

//****************************************************************
// variables used for ble set up
static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;


// variables used to provide notifications of pairing device
// as well as connection status and recieving data from gun
char notifyData[100];
int notifyDataIndex = 0;
String tokenStrings[100];
char *ptr = NULL;

// these are variables im using to create settings for the game mode and
// gun settings
int settingsallowed = 0; // trigger variable used to walk through steps for configuring gun(s) for serial core
int settingsallowed1 = 0; // trigger variable used to walk through steps for configuring gun(s) for BLE core
int SetSlotA = 2; // this is for weapon slot 0, default is AMR
int SLOTA = 100; // used when weapon selection is manual
int SetSlotB = 1; // this is for weapon slot 1, default is unarmed
int SLOTB = 100; // used when weapon selection is manual
int SetLives = 32000; // used for configuring lives
int SetSlotC; // this is for weapon slot 4 or melee used in pickups only (future)
int SetTeam = 0; // used to configure team player settings, default is 0
long SetTime = 2000000000; // used for in game timer functions on esp32 (future
int SetODMode = 0; // used to set indoor and outdoor modes (default is on)
int SetGNDR = 0; // used to change player to male 0/female 1, male is default
int SetRSPNMode; // used to set auto or manual respawns from bases/ir (future)
int SetObj = 32000; // used to program objectives
int SetFF = 1; // set game to friendly fire on/off (default is on)
int SetVol = 65; // set tagger volume adjustment, default is 65
int CurrentWeapSlot; // used for indicating what weapon slot is being used, primarily for unlimited ammo
int ReloadType; // used for unlimited ammo... maybe 10 is for unlimited

int Team = 0; // team selection used when allowed for custom configuration
int MaxKills = 32000; // setting limit on kill counts
int Objectives = 32000; // objective goals
int CompletedObjectives = 0; // earned objectives by player
int PlayerLives = 32000; // setting max player lives
int MaxTeamLives = 32000; // setting maximum team lives
long GameTimer = 2000000000; // setting maximum game time
long GameStartTime = 0; // used to set the start time when a match begins
int PlayerKillCount[64] = {0}; // so its players 0-63 as the player id.
int TeamKillCount[6] = {0}; // teams 0-6, Red-0, blue-1, yellow-2, green-3, purple-4, cyan-5
int DelayStart = 0; // set delay count down to 0 seconds for default
int GameMode = 1; // for setting up general settings
int Special = 0; // special settings
int AudioPlayCounter = 0; // used to make sure audio is played only once (redundant check)
int UNLIMITEDAMMO = 0; // used to trigger ammo auto replenish if enabled
bool OutofAmmoA = false; // trigger for auto respawn of ammo weapon slot 0
bool OutofAmmoB = false; // trigger for auto respawn of ammo weapon slot 1
String AudioSelection; // used to play stored audio files with tagger FOR SERIAL CORE
String AudioSelection1; // used to play stored audio files with tagger FOR BLE CORE

int lastTaggedPlayer = -1;  // used to capture player id who last shot gun, for kill count attribution
int lastTaggedTeam = -1;  // used to captures last player team who shot gun, for kill count attribution

// used to send to ESP8266 for LCD display
int ammo1 = 0;
int ammo2 = 0;
int weap = 0;
int health = 45;
int armor = 70;
int shield = 70;


bool RESPAWN = false; // trigger to enable auto respawn when killed in game
bool GAMEOVER = false; // used to trigger game over and boot a gun out of play mode
bool TAGGERUPDATE = false; // used to trigger sending data to ESP8266 used for BLE core
bool TAGGERUPDATE1 = false; // used to turn off BLE core lcd send data trigger
bool AUDIO = false; // used to trigger an audio on tagger FOR SERIAL CORE
bool AUDIO1 = false; // used to trigger an audio on tagger FOR BLE CORE
bool GAMESTART = false; // used to trigger game start
bool TurnOffAudio = false; // used to trigger audio off from serial core
bool GETTEAM = false; // used when configuring customized settings
bool STATUSCHANGE = false; // used to loop through selectable customized options
bool GETSLOT0 = false; // used for configuring manual weapon selection
bool GETSLOT1 = false; // used for configuring manual weapon selection

long startScan = 0; // part of BLE enabling

bool WEAP = false; // not used anymore but was used to auto load gun settings on esp boot

//**************************************************************
//**************************************************************
// this bad boy captures any ble data from gun and analyzes it based upon the
// protocol predecessor, a lot going on and very important as we are able to
// use the ble data from gun to decode brx protocol with the gun, get data from
// guns health status and ammo reserve etc.

void notifyCallback() {

  while (SerialBT.available()) {
    notifyData[notifyDataIndex] = SerialBT.read();
    notifyDataIndex++;
  }
  Serial.print("Received : ");
  for(int i=0;i<notifyDataIndex;i++)
    Serial.print(notifyData[i]);
  Serial.println();
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
      AudioSelection1 = "VA20";
      AUDIO1 = true;
    }
    // this analyzes every single time a trigger is pulled
    // or a button is pressed or reload handle pulled. pretty cool
    // i dont do much with it yet here but will be used later to allow
    // players to select weapon pickups etc or options when starting a game
    // ideally with LCD installed this will be very usefull! can use it with
    // limited gun choices, like the basic smg, tar33 etc as we can use gun
    // audio to help players know what they are picking but not implemented yet
    /*
        Trigger pulled: $BUT,0,1,
        Tirgger Released: $BUT,0,0,
        Alt fire pressed:  $BUT,1,1,
        Alt fire released:  $BUT,1,0,
        Reload pulled:  $BUT,2,1,
        Reload released:  $BUT,2,0,
        select button pressed:  $BUT,3,1,
        select button released:  $BUT,3,0,
        left button pressed:  $BUT,4,1,
        left button released:  $BUT,4,0,
        right button pressed:  $BUT,5,1,
        right button released:  $BUT,5,0,
    */
    if (tokenStrings[0] == "$BUT") {
      if (tokenStrings[1] == "0") {
        if (tokenStrings[2] == "1") {
          Serial.println("Trigger pulled"); // as indicated this is the trigger
        }
        if (tokenStrings[2] == "0") {
          Serial.println("Trigger Released"); // goes without sayin... you let go of the trigger
          // upon release of a trigger, team settings can be changed if the proper allowance is in place
          if (GETTEAM) { // used for configuring manual team selection
            if (Team == 5) {
              Team = 0;
              STATUSCHANGE = true;
              AudioSelection1 = "VA13";
              AUDIO1 = true;
              Serial.println("team changed from 5 to 0");
            }
            if (Team == 4) {
              Team = 5;  // foxtrot team
              AudioSelection1 = "VA2Y";
              AUDIO1 = true;
              Serial.println("team changed from 4 to 5");
            }
            if (Team == 3) {
              Team = 4;  // echo team
              AudioSelection1 = "VA2G";
              AUDIO1 = true;
              Serial.println("team changed from 3 to 4");
            }
            if (Team == 2) {
              Team = 3;  // delta team
              AudioSelection1 = "VA27";
              AUDIO1 = true;
              Serial.println("team changed from 2 to 3");
            }
            if (Team == 1) {
              Team = 2;  // charlie team
              AudioSelection1 = "VA1R";
              AUDIO1 = true;
              Serial.println("team changed from 1 to 2");
            }
            if (Team == 0 && STATUSCHANGE == false) {
              Team = 1;  // bravo team
              AudioSelection1 = "VA1L";
              AUDIO1 = true;
              Serial.println("team changed from 0 to 1");
            }
            STATUSCHANGE = false;
          }
          if (GETSLOT0) { // used for configuring manual team selection
            if (SLOTA == 19) {
              SLOTA = 1;
              STATUSCHANGE = true;
              AudioSelection1 = "VA01";
              AUDIO1 = true;
              Serial.println("Weapon changed from 19 to 1");
            }
            if (SLOTA == 18) {
              SLOTA = 19;  //
              AudioSelection1 = "VA0J";
              AUDIO1 = true;
              Serial.println("Weapon 0 changed from 18 to 19");
            }
            if (SLOTA == 17) {
              SLOTA = 18;  //
              AudioSelection1 = "VA0I";
              AUDIO1 = true;
              Serial.println("Weapon 0 changed from 17 to 18");
            }
            if (SLOTA == 16) {
              SLOTA = 17;  //
              AudioSelection1 = "VA0H";
              AUDIO1 = true;
              Serial.println("Weapon 0 changed from 16 to 17");
            }
            if (SLOTA == 15) {
              SLOTA = 16;  //
              AudioSelection1 = "VA0G";
              AUDIO1 = true;
              Serial.println("Weapon 0 changed from 15 to 16");
            }
            if (SLOTA == 14) {
              SLOTA = 15;  //
              AudioSelection1 = "VA0F";
              AUDIO1 = true;
              Serial.println("Weapon 0 changed from 14 to 15");
            }
            if (SLOTA == 13) {
              SLOTA = 14;  //
              AudioSelection1 = "VA0E";
              AUDIO1 = true;
              Serial.println("Weapon 0 changed from 13 to 14");
            }
            if (SLOTA == 12) {
              SLOTA = 13;  //
              AudioSelection1 = "VA0D";
              AUDIO1 = true;
              Serial.println("Weapon 0 changed from 12 to 13");
            }
            if (SLOTA == 11) {
              SLOTA = 12;  //
              AudioSelection1 = "VA0C";
              AUDIO1 = true;
              Serial.println("Weapon 0 changed from 11 to 12");
            }
            if (SLOTA == 10) {
              SLOTA = 11;  //
              AudioSelection1 = "VA0B";
              AUDIO1 = true;
              Serial.println("Weapon 0 changed from 10 to 11");
            }
            if (SLOTA == 9) {
              SLOTA = 10;  //
              AudioSelection1 = "VA0A";
              AUDIO1 = true;
              Serial.println("Weapon 0 changed from 9 to 10");
            }
            if (SLOTA == 8) {
              SLOTA = 9;  //
              AudioSelection1 = "VA09";
              AUDIO1 = true;
              Serial.println("Weapon 0 changed from 8 to 9");
            }
            if (SLOTA == 7) {
              SLOTA = 8;  //
              AudioSelection1 = "VA08";
              AUDIO1 = true;
              Serial.println("Weapon 0 changed from 7 to 8");
            }
            if (SLOTA == 6) {
              SLOTA = 7;  //
              AudioSelection1 = "VA07";
              AUDIO1 = true;
              Serial.println("Weapon 0 changed from 6 to 7");
            }
            if (SLOTA == 5) {
              SLOTA = 6;  //
              AudioSelection1 = "VA06";
              AUDIO1 = true;
              Serial.println("Weapon 0 changed from 5 to 6");
            }
            if (SLOTA == 4) {
              SLOTA = 5;  //
              AudioSelection1 = "VA05";
              AUDIO1 = true;
              Serial.println("Weapon 0 changed from 4 to 5");
            }
            if (SLOTA == 3) {
              SLOTA = 4;  //
              AudioSelection1 = "VA04";
              AUDIO1 = true;
              Serial.println("Weapon 0 changed from 3 to 4");
            }
            if (SLOTA == 2) {
              SLOTA = 3;  //
              AudioSelection1 = "VA03";
              AUDIO1 = true;
              Serial.println("Weapon 0 changed from 2 to 3");
            }
            if (SLOTA == 1 && STATUSCHANGE == false) {
              SLOTA = 2;  //
              AudioSelection1 = "VA02";
              AUDIO1 = true;
              Serial.println("Weapon 0 changed from 1 to 2");
            }
            if (SLOTA == 100) {
              SLOTA = 1;  //
              AudioSelection1 = "VA01";
              AUDIO1 = true;
              Serial.println("Weapon 0 changed from 0 to 1");
            }
            STATUSCHANGE = false;
            TAGGERUPDATE = true;
          }
          if (GETSLOT1) { // used for configuring manual team selection
            if (SLOTB == 19) {
              SLOTB = 1;
              STATUSCHANGE = true;
              AudioSelection1 = "VA01";
              AUDIO1 = true;
              Serial.println("Weapon changed from 19 to 1");
            }
            if (SLOTB == 18) {
              SLOTB = 19;  //
              AudioSelection1 = "VA0J";
              AUDIO1 = true;
              Serial.println("Weapon 1 changed from 18 to 19");
            }
            if (SLOTB == 17) {
              SLOTB = 18;  //
              AudioSelection1 = "VA0I";
              AUDIO1 = true;
              Serial.println("Weapon 1 changed from 17 to 18");
            }
            if (SLOTB == 16) {
              SLOTB = 17;  //
              AudioSelection1 = "VA0H";
              AUDIO1 = true;
              Serial.println("Weapon 1 changed from 16 to 17");
            }
            if (SLOTB == 15) {
              SLOTB = 16;  //
              AudioSelection1 = "VA0G";
              AUDIO1 = true;
              Serial.println("Weapon 1 changed from 15 to 16");
            }
            if (SLOTB == 14) {
              SLOTB = 15;  //
              AudioSelection1 = "VA0F";
              AUDIO1 = true;
              Serial.println("Weapon 1 changed from 14 to 15");
            }
            if (SLOTB == 13) {
              SLOTB = 14;  //
              AudioSelection1 = "VA0E";
              AUDIO1 = true;
              Serial.println("Weapon 1 changed from 13 to 14");
            }
            if (SLOTB == 12) {
              SLOTB = 13;  //
              AudioSelection1 = "VA0D";
              AUDIO1 = true;
              Serial.println("Weapon 1 changed from 12 to 13");
            }
            if (SLOTB == 11) {
              SLOTB = 12;  //
              AudioSelection1 = "VA0C";
              AUDIO1 = true;
              Serial.println("Weapon 1 changed from 11 to 12");
            }
            if (SLOTB == 10) {
              SLOTB = 11;  //
              AudioSelection1 = "VA0B";
              AUDIO1 = true;
              Serial.println("Weapon 1 changed from 10 to 11");
            }
            if (SLOTB == 9) {
              SLOTB = 10;  //
              AudioSelection1 = "VA0A";
              AUDIO1 = true;
              Serial.println("Weapon 1 changed from 9 to 10");
            }
            if (SLOTB == 8) {
              SLOTB = 9;  //
              AudioSelection1 = "VA09";
              AUDIO1 = true;
              Serial.println("Weapon 1 changed from 8 to 9");
            }
            if (SLOTB == 7) {
              SLOTB = 8;  //
              AudioSelection1 = "VA08";
              AUDIO1 = true;
              Serial.println("Weapon 1 changed from 7 to 8");
            }
            if (SLOTB == 6) {
              SLOTB = 7;  //
              AudioSelection1 = "VA07";
              AUDIO1 = true;
              Serial.println("Weapon 1 changed from 6 to 7");
            }
            if (SLOTB == 5) {
              SLOTB = 6;  //
              AudioSelection1 = "VA06";
              AUDIO1 = true;
              Serial.println("Weapon 1 changed from 5 to 6");
            }
            if (SLOTB == 4) {
              SLOTB = 5;  //
              AudioSelection1 = "VA05";
              AUDIO1 = true;
              Serial.println("Weapon 1 changed from 4 to 5");
            }
            if (SLOTB == 3) {
              SLOTB = 4;  //
              AudioSelection1 = "VA04";
              AUDIO1 = true;
              Serial.println("Weapon 1 changed from 3 to 4");
            }
            if (SLOTB == 2) {
              SLOTB = 3;  //
              AudioSelection1 = "VA03";
              AUDIO1 = true;
              Serial.println("Weapon 1 changed from 2 to 3");
            }
            if (SLOTB == 1 && STATUSCHANGE == false) {
              SLOTB = 2;  //
              AudioSelection1 = "VA02";
              AUDIO1 = true;
              Serial.println("Weapon 1 changed from 1 to 2");
            }
            if (SLOTB == 100) {
              SLOTB = 1;  //
              AudioSelection1 = "VA01";
              AUDIO1 = true;
              Serial.println("Weapon 1 changed from 0 to 1");
            }
            STATUSCHANGE = false;
            TAGGERUPDATE = true;
          }
        }
      }
      // alt fire button pressed and released section
      if (tokenStrings[1] == "1") {
        if (tokenStrings[2] == "1") {
          Serial.println("Alt fire pulled"); // yeah.. you pushed the red/yellow button
        }
        if (tokenStrings[2] == "0") {
          Serial.println("Alt fire Released"); // now you released the button
          if (GETTEAM) {
            GETTEAM = false;
            AudioSelection1 = "VAO";
            AUDIO1 = true;
          }
          if (GETSLOT0) {
            GETSLOT0 = false;
            AudioSelection1 = "VAO";
            AUDIO1 = true;
          }
          if (GETSLOT1) {
            GETSLOT1 = false;
            AudioSelection1 = "VAO";
            AUDIO1 = true;
          }
        }
      }
      // charge or reload handle pulled and released section
      if (tokenStrings[1] == "2") {
        if (tokenStrings[2] == "1") {
          Serial.println("charge handle pulled"); // as indicated this is the handle
        }
        if (tokenStrings[2] == "0") {
          Serial.println("Charge handle Released"); // goes without sayin... you let go of the handle
          // upon release of a handle, following steps occur
          if (UNLIMITEDAMMO == 2) { // checking if unlimited ammo is on or not
            if (CurrentWeapSlot == 0) {
              OutofAmmoA = true; // here is the variable for weapon slot 0
              Serial.println("Weapon Slot 0 is out of ammo, enabling weapon reload for weapon slot 0");
            }
            if (CurrentWeapSlot == 1) {
              OutofAmmoB = true; // trigger variable for weapon slot 1
              Serial.println("Weapon Slot 1 is out of ammo, enabling weapon reload for weapon slot 1");
            }
          }
        }
      }
      // we can do more for left right select but yeah... maybe another time
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
      TAGGERUPDATE = true;
      Serial.println("Enabled LCD Data Send");
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
          space 0 is ALCD indicator
          space 1 is rounds remaining in clip
          space 2 is accuracy of weapon
          space 3 is slot of weapon
          space 4 is remaining ammo outside clip
          space 5 is the overheating feature if aplicable

          more can be done with this, like using the ammo info to post to lcd
      */
      ammo1 = tokenStrings[1].toInt(); // sets the variable to be sent to esp8266 for ammo in clip
      ammo2 = tokenStrings[4].toInt(); // sets the total magazine capacity to be sent to 8266
      if (tokenStrings[3].toInt() == 0) {
        weap = SetSlotA;  // if the weapon slot 0 is loaded up, sets the variable to notify weapon type loaded to the right weapon
        CurrentWeapSlot = 0;
      }
      if (tokenStrings[3].toInt() == 1) {
        weap = SetSlotB;  // same thing but for weapon slot 1
        CurrentWeapSlot = 1;
      }
      if (tokenStrings[3].toInt() == 2) {
        CurrentWeapSlot = 4;
      }
      TAGGERUPDATE = true;
      Serial.println("Enabled LCD Data Send");
    }

    // this guy is recieved from brx every time your health changes... usually after
    // getting shot.. from the HIR indicator above. but this can tell us our health status
    // and we can send it to an LCD or use it to know when were dead and award points to the
    // player who loast tagged the gun... hence the nomenclature "lasttaggedPlayer" etc
    // add in some kill counts and we can deplete lives and use it to enable or disable respawn functions
    if (tokenStrings[0] == "$HP") {
      /*health status update occured
         can be used for updates on health as well as death occurance
      */
      TAGGERUPDATE = true;
      Serial.println("Enabled LCD Data Send");
      health = tokenStrings[1].toInt(); // setting variable to be sent to esp8266
      armor = tokenStrings[2].toInt(); // setting variable to be sent to esp8266
      shield = tokenStrings[3].toInt(); // setting variable to be sent to esp8266
      if ((tokenStrings[1] == "0") && (tokenStrings[2] == "0") && (tokenStrings[3] == "0")) {
        PlayerKillCount[lastTaggedPlayer]++; // adding a point to the last player who killed us
        TeamKillCount[lastTaggedTeam]++; // adding a point to the team who caused the last kill
        PlayerLives--; // taking our preset lives and subtracting one life then talking about it on the monitor
        Serial.println("Lives Remaining = " + String(PlayerLives));
        Serial.println("Killed by: " + String(lastTaggedPlayer) + " on team: " + String(lastTaggedTeam));
        Serial.println("Team: " + String(lastTaggedTeam) + "Score: " + String(TeamKillCount[lastTaggedTeam]));
        Serial.println("Player: " + String(lastTaggedPlayer) + " Score: " + String(PlayerKillCount[lastTaggedPlayer]));
        if (PlayerLives > 0 && SetRSPNMode < 9) { // doing a check if we still have lives left after dying
          RESPAWN = true;
          Serial.println("Auto respawn enabled");
        }
        if (PlayerLives > 0 && SetRSPNMode == 9) {
          // run manual respawn object
        }
        if (PlayerLives == 0) {
          GAMEOVER = true;
        }
      }
    }
  } else {
    //hold data and keep receving
  }
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
  Serial.println("Finished Game Configuration set up");
  PlayerLives = SetLives; // sets configured lives from settings received
  GameTimer = SetTime; // sets timer from input of esp8266
  Objectives = SetObj; // sets configured objectives from esp8266
}

//****************************************************************************************
//************************ This starts a game *******************************************
//****************************************************************************************
// this starts a game
void delaystart() {
  Serial.println("Starting Delayed Game Start");
  sendString("$VOL," + String(SetVol) + ",0,*"); // sets max volume on gun 0-100 feet distance
  //sendString("$PLAY,VA84,4,5,,,,,*"); // plays a ten second countdown
  sendString("$HLED,,6,,,,,*"); // changes headset to end of game
  delay(DelayStart); // delays ten seconds or as configured
  sendString("$PLAY,VA81,4,6,,,,,*"); // plays the .. nevermind
  sendString("$PLAYX,0,*");
  sendString("$SPAWN,,*");
  Serial.println("Delayed Start Complete, should be in game play mode now");
  GameStartTime = millis();
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
  /*if (value.length() > 20) {
    for (int i = 0; i < value.length() / 20; i++) {
      memcpy(buf, c_string + i * 20, 20);
      Serial.print((char*)buf);
      SerialBT.write(buf);
      sentSize += 20;
    }
    int remaining = value.length() - sentSize;
    memcpy(buf, c_string + sentSize, remaining);
    SerialBT.write(buf);
    for (int i = 0; i < remaining; i++)
      Serial.print((char)buf[i]);
    Serial.println();
  }
  else {
    SerialBT.write((uint8_t*)value.c_str());
  }*/
  SerialBT.print(value);
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
  // health = 45; armor = 70; shield =70;

  if (SetTeam == 100) {
    if (SetGNDR == 0) {
      sendString("$PSET," + String(GunID) + "," + String(Team) + "," + String(health) + "," + String(armor) + "," + String(shield) + ",50,,H44,JAD,V33,V3I,V3C,V3G,V3E,V37,H06,H55,H13,H21,H02,U15,W71,A10,*");
    }
    else {
      sendString("$PSET," + String(GunID) + "," + String(Team) + ",45,70,70,50,,H44,JAD,VB3,VBI,VBC,VBG,VBE,VB7,H06,H55,H13,H21,H02,U15,W71,A10,*");
    }
  } else {
    if (SetGNDR == 0) {
      sendString("$PSET," + String(GunID) + "," + String(SetTeam) + ",45,70,70,50,,H44,JAD,V33,V3I,V3C,V3G,V3E,V37,H06,H55,H13,H21,H02,U15,W71,A10,*");
    }
    else {
      sendString("$PSET," + String(GunID) + "," + String(SetTeam) + ",45,70,70,50,,H44,JAD,VB3,VBI,VBC,VBG,VBE,VB7,H06,H55,H13,H21,H02,U15,W71,A10,*");
    }
  }
}
//******************************************************************************************

// sets and sends gun type to slot 0 based upon stored settings
void weaponsettingsA() {
  if (SLOTA != 100) {
    SetSlotA = SLOTA;
    SLOTA = 100;
  }
  if (UNLIMITEDAMMO == 3) {
    if (SetSlotA == 1) {
      Serial.println("Weapon 0 set to Unarmed");  // cleared out weapon 0
      sendString("$WEAP,0,*");
    }
    if (SetSlotA == 2) {
      Serial.println("Weapon 0 set to AMR");
      sendString("$WEAP,0,,100,0,3,18,0,,,,,,,,360,850,14,0,1400,10,7,100,100,,0,,,S07,D20,D19,,D04,D03,D21,D18,,,,,14,28,75,,*");
    }
    if (SetSlotA == 3) {
      Serial.println("Weapon 0 set to Assault Rifle");
      sendString("$WEAP,0,,100,0,0,9,0,,,,,,,,100,850,32,0,1400,10,0,100,100,,0,,,R01,,,,D04,D03,D02,D18,,,,,32,192,75,,*");
    }
    if (SetSlotA == 4) {
      Serial.println("Weapon 0 set to Bolt Rifle");
      sendString("$WEAP,0,,100,0,3,13,0,,,,,,,,225,850,18,0,2000,10,7,100,100,,0,,,R12,,,,D04,D03,D02,D18,,,,,18,90,75,,*");
    }
    if (SetSlotA == 5) {
      Serial.println("Weapon 0 set to BurstRifle");
      sendString("$WEAP,0,,100,0,3,9,0,,,,,,,,75,850,36,0,1700,10,9,100,100,275,0,,,R18,,,,D04,D03,D02,D18,,,,,36,108,75,,*");
    }
    if (SetSlotA == 6) {
      Serial.println("Weapon 0 set to ChargeRifle");
      sendString("$WEAP,0,,100,8,0,100,0,,,,,,,,1250,850,100,0,2500,10,14,100,100,,14,,,E03,C15,C17,,D30,D29,D37,A73,C19,C04,20,150,100,100,75,,*");
    }
    if (SetSlotA == 7) {
      Serial.println("Weapon 0 set to Energy Launcher");
      sendString("$WEAP,0,,100,9,3,115,0,,,,,,,,360,850,1,0,1400,10,0,100,100,,0,,,J15,,,,D14,D13,D12,D18,,,,,1,3,75,,*");
    }
    if (SetSlotA == 8) {
      Serial.println("Weapon 0 set to Energy Rifle");
      sendString("$WEAP,0,,100,0,0,9,0,,,,,,,,90,850,300,0,2400,10,0,100,100,,6,,,E12,,,,D17,D16,D15,A73,D122,,,,300,300,75,,*");
    }
    if (SetSlotA == 9) {
      Serial.println("Weapon 0 set to Force Rifle");
      sendString("$WEAP,0,,100,0,1,9,0,,,,,,,,100,850,36,0,1700,10,9,100,100,250,0,,,R23,D20,D19,,D23,D22,D21,D18,,,,,36,72,75,,*");
    }
    if (SetSlotA == 10) {
      Serial.println("Weapon 0 set to Ion Sniper");
      sendString("$WEAP,0,,100,0,0,115,0,,,,,,,,1000,850,2,0,2000,10,7,100,100,,0,,,E07,D32,D31,,D17,D16,D15,A73,,,,,2,6,75,,*");
    }
    if (SetSlotA == 11) {
      Serial.println("Weapon 0 set to Laser Cannon");
      sendString("$WEAP,0,,100,0,0,115,0,,,,,,,,1500,850,4,0,2000,10,3,100,100,,0,,,C06,C11,,,D17,D16,D15,A73,,,,,4,4,75,,*");
    }
    if (SetSlotA == 12) {
      Serial.println("Weapon 0 set to Plasma Sniper");
      sendString("$WEAP,0,2,100,0,0,80,0,,,,,,80,80,225,850,10,0,2000,10,7,100,100,,30,,,E17,,,,D35,D34,D36,A73,D122,,,,10,40,75,40,*");
    }
    if (SetSlotA == 13) {
      Serial.println("Weapon 0 set to Rail Gun");
      sendString("$WEAP,0,0,100,6,0,115,0,,,,,,,,1200,850,1,0,2400,10,2,100,100,,0,,,C03,C08,,,D36,D35,D34,A73,,,,,1,3,75,,*");
    }
    if (SetSlotA == 14) {
      Serial.println("Weapon 0 set to Rocket Launcher");
      sendString("$WEAP,0,2,100,10,0,115,0,,,,,,115,80,1000,850,2,0,1200,10,7,100,100,,0,,,C03,,,,D14,D13,D12,D18,,,,,2,4,75,30,*");
    }
    if (SetSlotA == 15) {
      Serial.println("Weapon 0 set to Shotgun");
      sendString("$WEAP,0,2,100,0,0,45,0,,,,,,70,80,900,850,6,0,400,10,7,100,100,,0,,,T01,,,,D01,D28,D27,D18,,,,,6,12,75,30,*");
    }
    if (SetSlotA == 16) {
      Serial.println("Weapon 0 set to SMG");
      sendString("$WEAP,0,,100,0,0,8,0,,,,,,,,90,850,72,0,2500,10,0,100,100,,5,,,G03,,,,D26,D25,D24,D18,D11,,,,72,144,75,,*");
    }
    if (SetSlotA == 17) {
      Serial.println("Weapon 0 set to Sniper Rifle");
      sendString("$WEAP,0,,100,0,1,80,0,,,,,,,,300,850,4,0,1700,10,7,100,100,,0,,,S16,D20,D19,,D04,D03,D21,D18,,,,,4,12,75,,*");
    }
    if (SetSlotA == 18) {
      Serial.println("Weapon 0 set to Stinger");
      sendString("$WEAP,0,,100,0,0,15,0,,,,,,,,120,850,18,0,1700,10,0,100,100,,0,,,E11,,,,D17,D16,D15,A73,,,,,18,36,75,,*");
    }
    if (SetSlotA == 19) {
      Serial.println("Weapon 0 set to Suppressor");
      sendString("$WEAP,0,,100,0,0,8,0,,,,,,,,75,850,48,0,2000,10,0,100,100,,0,2,50,Q06,,,,D26,D25,D24,D18,,,,,48,144,75,,*");
    }
  } else {
    if (SetSlotA == 1) {
      Serial.println("Weapon 0 set to Unarmed");  // cleared out weapon 0
      sendString("$WEAP,0,*");
    }
    if (SetSlotA == 2) {
      Serial.println("Weapon 0 set to AMR");
      sendString("$WEAP,0,,100,0,3,18,0,,,,,,,,360,850,14,56,1400,0,7,100,100,,0,,,S07,D20,D19,,D04,D03,D21,D18,,,,,14,28,75,,*");
    }
    if (SetSlotA == 3) {
      Serial.println("Weapon 0 set to Assault Rifle");
      sendString("$WEAP,0,,100,0,0,9,0,,,,,,,,100,850,32,384,1400,0,0,100,100,,0,,,R01,,,,D04,D03,D02,D18,,,,,32,192,75,,*");
    }
    if (SetSlotA == 4) {
      Serial.println("Weapon 0 set to Bolt Rifle");
      sendString("$WEAP,0,,100,0,3,13,0,,,,,,,,225,850,18,180,2000,0,7,100,100,,0,,,R12,,,,D04,D03,D02,D18,,,,,18,90,75,,*");
    }
    if (SetSlotA == 5) {
      Serial.println("Weapon 0 set to BurstRifle");
      sendString("$WEAP,0,,100,0,3,9,0,,,,,,,,75,850,36,216,1700,0,9,100,100,275,0,,,R18,,,,D04,D03,D02,D18,,,,,36,108,75,,*");
    }
    if (SetSlotA == 6) {
      Serial.println("Weapon 0 set to ChargeRifle");
      sendString("$WEAP,0,,100,8,0,100,0,,,,,,,,1250,850,100,200,2500,0,14,100,100,,14,,,E03,C15,C17,,D30,D29,D37,A73,C19,C04,20,150,100,100,75,,*");
    }
    if (SetSlotA == 7) {
      Serial.println("Weapon 0 set to Energy Launcher");
      sendString("$WEAP,0,,100,9,3,115,0,,,,,,,,360,850,1,6,1400,0,0,100,100,,0,,,J15,,,,D14,D13,D12,D18,,,,,1,3,75,,*");
    }
    if (SetSlotA == 8) {
      Serial.println("Weapon 0 set to Energy Rifle");
      sendString("$WEAP,0,,100,0,0,9,0,,,,,,,,90,850,300,600,2400,0,0,100,100,,6,,,E12,,,,D17,D16,D15,A73,D122,,,,300,300,75,,*");
    }
    if (SetSlotA == 9) {
      Serial.println("Weapon 0 set to Force Rifle");
      sendString("$WEAP,0,,100,0,1,9,0,,,,,,,,100,850,36,144,1700,0,9,100,100,250,0,,,R23,D20,D19,,D23,D22,D21,D18,,,,,36,72,75,,*");
    }
    if (SetSlotA == 10) {
      Serial.println("Weapon 0 set to Ion Sniper");
      sendString("$WEAP,0,,100,0,0,115,0,,,,,,,,1000,850,2,12,2000,0,7,100,100,,0,,,E07,D32,D31,,D17,D16,D15,A73,,,,,2,6,75,,*");
    }
    if (SetSlotA == 11) {
      Serial.println("Weapon 0 set to Laser Cannon");
      sendString("$WEAP,0,,100,0,0,115,0,,,,,,,,1500,850,4,8,2000,0,3,100,100,,0,,,C06,C11,,,D17,D16,D15,A73,,,,,4,4,75,,*");
    }
    if (SetSlotA == 12) {
      Serial.println("Weapon 0 set to Plasma Sniper");
      sendString("$WEAP,0,2,100,0,0,80,0,,,,,,80,80,225,850,10,80,2000,0,7,100,100,,30,,,E17,,,,D35,D34,D36,A73,D122,,,,10,40,75,40,*");
    }
    if (SetSlotA == 13) {
      Serial.println("Weapon 0 set to Rail Gun");
      sendString("$WEAP,0,0,100,6,0,115,0,,,,,,,,1200,850,1,6,2400,0,2,100,100,,0,,,C03,C08,,,D36,D35,D34,A73,,,,,1,3,75,,*");
    }
    if (SetSlotA == 14) {
      Serial.println("Weapon 0 set to Rocket Launcher");
      sendString("$WEAP,0,2,100,10,0,115,0,,,,,,115,80,1000,850,2,8,1200,0,7,100,100,,0,,,C03,,,,D14,D13,D12,D18,,,,,2,4,75,30,*");
    }
    if (SetSlotA == 15) {
      Serial.println("Weapon 0 set to Shotgun");
      sendString("$WEAP,0,2,100,0,0,45,0,,,,,,70,80,900,850,6,24,400,2,7,100,100,,0,,,T01,,,,D01,D28,D27,D18,,,,,6,12,75,30,*");
    }
    if (SetSlotA == 16) {
      Serial.println("Weapon 0 set to SMG");
      sendString("$WEAP,0,,100,0,0,8,0,,,,,,,,90,850,72,288,2500,0,0,100,100,,5,,,G03,,,,D26,D25,D24,D18,D11,,,,72,144,75,,*");
    }
    if (SetSlotA == 17) {
      Serial.println("Weapon 0 set to Sniper Rifle");
      sendString("$WEAP,0,,100,0,1,80,0,,,,,,,,300,850,4,24,1700,0,7,100,100,,0,,,S16,D20,D19,,D04,D03,D21,D18,,,,,4,12,75,,*");
    }
    if (SetSlotA == 18) {
      Serial.println("Weapon 0 set to Stinger");
      sendString("$WEAP,0,,100,0,0,15,0,,,,,,,,120,850,18,72,1700,0,0,100,100,,0,,,E11,,,,D17,D16,D15,A73,,,,,18,36,75,,*");
    }
    if (SetSlotA == 19) {
      Serial.println("Weapon 0 set to Suppressor");
      sendString("$WEAP,0,,100,0,0,8,0,,,,,,,,75,850,48,288,2000,0,0,100,100,,0,2,50,Q06,,,,D26,D25,D24,D18,,,,,48,144,75,,*");
    }
  }
}

//******************************************************************************************

// sets and sends gun for slot 0 based upon stored settings
void weaponsettingsB() {
  if (SLOTB != 100) {
    SetSlotB = SLOTB;
    SLOTB = 100;
  }
  if (UNLIMITEDAMMO == 3) {
    if (SetSlotB == 1) {
      Serial.println("Weapon 1 set to Unarmed");  // cleared out weapon 0
      sendString("$WEAP,1,*");
    }
    if (SetSlotB == 2) {
      Serial.println("Weapon 1 set to AMR");
      sendString("$WEAP,1,,100,0,3,18,0,,,,,,,,360,850,14,0,1400,10,7,100,100,,0,,,S07,D20,D19,,D04,D03,D21,D18,,,,,14,28,75,,*");
    }
    if (SetSlotB == 3) {
      Serial.println("Weapon 1 set to Assault Rifle");
      sendString("$WEAP,1,,100,0,0,9,0,,,,,,,,100,850,32,0,1400,10,0,100,100,,0,,,R01,,,,D04,D03,D02,D18,,,,,32,192,75,,*");
    }
    if (SetSlotB == 4) {
      Serial.println("Weapon 1 set to Bolt Rifle");
      sendString("$WEAP,1,,100,0,3,13,0,,,,,,,,225,850,18,0,2000,10,7,100,100,,0,,,R12,,,,D04,D03,D02,D18,,,,,18,90,75,,*");
    }
    if (SetSlotB == 5) {
      Serial.println("Weapon 1 set to BurstRifle");
      sendString("$WEAP,1,,100,0,3,9,0,,,,,,,,75,850,36,0,1700,10,9,100,100,275,0,,,R18,,,,D04,D03,D02,D18,,,,,36,108,75,,*");
    }
    if (SetSlotB == 6) {
      Serial.println("Weapon 1 set to ChargeRifle");
      sendString("$WEAP,1,,100,8,0,100,0,,,,,,,,1250,850,100,0,2500,10,14,100,100,,14,,,E03,C15,C17,,D30,D29,D37,A73,C19,C04,20,150,100,100,75,,*");
    }
    if (SetSlotB == 7) {
      Serial.println("Weapon 1 set to Energy Launcher");
      sendString("$WEAP,1,,100,9,3,115,0,,,,,,,,360,850,1,0,1400,10,0,100,100,,0,,,J15,,,,D14,D13,D12,D18,,,,,1,3,75,,*");
    }
    if (SetSlotB == 8) {
      Serial.println("Weapon 1 set to Energy Rifle");
      sendString("$WEAP,1,,100,0,0,9,0,,,,,,,,90,850,300,0,2400,10,0,100,100,,6,,,E12,,,,D17,D16,D15,A73,D122,,,,300,300,75,,*");
    }
    if (SetSlotB == 9) {
      Serial.println("Weapon 1 set to Force Rifle");
      sendString("$WEAP,1,,100,0,1,9,0,,,,,,,,100,850,36,0,1700,10,9,100,100,250,0,,,R23,D20,D19,,D23,D22,D21,D18,,,,,36,72,75,,*");
    }
    if (SetSlotB == 10) {
      Serial.println("Weapon 1 set to Ion Sniper");
      sendString("$WEAP,1,,100,0,0,115,0,,,,,,,,1000,850,2,0,2000,10,7,100,100,,0,,,E07,D32,D31,,D17,D16,D15,A73,,,,,2,6,75,,*");
    }
    if (SetSlotB == 11) {
      Serial.println("Weapon 1 set to Laser Cannon");
      sendString("$WEAP,1,,100,0,0,115,0,,,,,,,,1500,850,4,0,2000,10,3,100,100,,0,,,C06,C11,,,D17,D16,D15,A73,,,,,4,4,75,,*");
    }
    if (SetSlotB == 12) {
      Serial.println("Weapon 1 set to Plasma Sniper");
      sendString("$WEAP,1,2,100,0,0,80,0,,,,,,80,80,225,850,10,0,2000,10,7,100,100,,30,,,E17,,,,D35,D34,D36,A73,D122,,,,10,40,75,40,*");
    }
    if (SetSlotB == 13) {
      Serial.println("Weapon 1 set to Rail Gun");
      sendString("$WEAP,1,0,100,6,0,115,0,,,,,,,,1200,850,1,0,2400,10,2,100,100,,0,,,C03,C08,,,D36,D35,D34,A73,,,,,1,3,75,,*");
    }
    if (SetSlotB == 14) {
      Serial.println("Weapon 1 set to Rocket Launcher");
      sendString("$WEAP,1,2,100,10,0,115,0,,,,,,115,80,1000,850,2,0,1200,10,7,100,100,,0,,,C03,,,,D14,D13,D12,D18,,,,,2,4,75,30,*");
    }
    if (SetSlotB == 15) {
      Serial.println("Weapon 1 set to Shotgun");
      sendString("$WEAP,1,2,100,0,0,45,0,,,,,,70,80,900,850,6,0,400,10,7,100,100,,0,,,T01,,,,D01,D28,D27,D18,,,,,6,12,75,30,*");
    }
    if (SetSlotB == 16) {
      Serial.println("Weapon 1 set to SMG");
      sendString("$WEAP,1,,100,0,0,8,0,,,,,,,,90,850,72,0,2500,10,0,100,100,,5,,,G03,,,,D26,D25,D24,D18,D11,,,,72,144,75,,*");
    }
    if (SetSlotB == 17) {
      Serial.println("Weapon 1 set to Sniper Rifle");
      sendString("$WEAP,1,,100,0,1,80,0,,,,,,,,300,850,4,0,1700,10,7,100,100,,0,,,S16,D20,D19,,D04,D03,D21,D18,,,,,4,12,75,,*");
    }
    if (SetSlotB == 18) {
      Serial.println("Weapon 1 set to Stinger");
      sendString("$WEAP,1,,100,0,0,15,0,,,,,,,,120,850,18,0,1700,10,0,100,100,,0,,,E11,,,,D17,D16,D15,A73,,,,,18,36,75,,*");
    }
    if (SetSlotB == 19) {
      Serial.println("Weapon 1 set to Suppressor");
      sendString("$WEAP,1,,100,0,0,8,0,,,,,,,,75,850,48,0,2000,10,0,100,100,,0,2,50,Q06,,,,D26,D25,D24,D18,,,,,48,144,75,,*");
    }
  } else {
    if (SetSlotB == 1) {
      Serial.println("Weapon 1 set to Unarmed");  // cleared out weapon 0
      sendString("$WEAP,1,*");
    }
    if (SetSlotB == 2) {
      Serial.println("Weapon 1 set to AMR");
      sendString("$WEAP,1,,100,0,3,18,0,,,,,,,,360,850,14,56,1400,0,7,100,100,,0,,,S07,D20,D19,,D04,D03,D21,D18,,,,,14,28,75,,*");
    }
    if (SetSlotB == 3) {
      Serial.println("Weapon 1 set to Assault Rifle");
      sendString("$WEAP,1,,100,0,0,9,0,,,,,,,,100,850,32,384,1400,0,0,100,100,,0,,,R01,,,,D04,D03,D02,D18,,,,,32,192,75,,*");
    }
    if (SetSlotB == 4) {
      Serial.println("Weapon 1 set to Bolt Rifle");
      sendString("$WEAP,1,,100,0,3,13,0,,,,,,,,225,850,18,180,2000,0,7,100,100,,0,,,R12,,,,D04,D03,D02,D18,,,,,18,90,75,,*");
    }
    if (SetSlotB == 5) {
      Serial.println("Weapon 1 set to BurstRifle");
      sendString("$WEAP,1,,100,0,3,9,0,,,,,,,,75,850,36,216,1700,0,9,100,100,275,0,,,R18,,,,D04,D03,D02,D18,,,,,36,108,75,,*");
    }
    if (SetSlotB == 6) {
      Serial.println("Weapon 1 set to ChargeRifle");
      sendString("$WEAP,1,,100,8,0,100,0,,,,,,,,1250,850,100,200,2500,0,14,100,100,,14,,,E03,C15,C17,,D30,D29,D37,A73,C19,C04,20,150,100,100,75,,*");
    }
    if (SetSlotB == 7) {
      Serial.println("Weapon 1 set to Energy Launcher");
      sendString("$WEAP,1,,100,9,3,115,0,,,,,,,,360,850,1,6,1400,0,0,100,100,,0,,,J15,,,,D14,D13,D12,D18,,,,,1,3,75,,*");
    }
    if (SetSlotB == 8) {
      Serial.println("Weapon 1 set to Energy Rifle");
      sendString("$WEAP,1,,100,0,0,9,0,,,,,,,,90,850,300,600,2400,0,0,100,100,,6,,,E12,,,,D17,D16,D15,A73,D122,,,,300,300,75,,*");
    }
    if (SetSlotB == 9) {
      Serial.println("Weapon 1 set to Force Rifle");
      sendString("$WEAP,1,,100,0,1,9,0,,,,,,,,100,850,36,144,1700,0,9,100,100,250,0,,,R23,D20,D19,,D23,D22,D21,D18,,,,,36,72,75,,*");
    }
    if (SetSlotB == 10) {
      Serial.println("Weapon 1 set to Ion Sniper");
      sendString("$WEAP,1,,100,0,0,115,0,,,,,,,,1000,850,2,12,2000,0,7,100,100,,0,,,E07,D32,D31,,D17,D16,D15,A73,,,,,2,6,75,,*");
    }
    if (SetSlotB == 11) {
      Serial.println("Weapon 1 set to Laser Cannon");
      sendString("$WEAP,1,,100,0,0,115,0,,,,,,,,1500,850,4,8,2000,0,3,100,100,,0,,,C06,C11,,,D17,D16,D15,A73,,,,,4,4,75,,*");
    }
    if (SetSlotB == 12) {
      Serial.println("Weapon 1 set to Plasma Sniper");
      sendString("$WEAP,1,2,100,0,0,80,0,,,,,,80,80,225,850,10,80,2000,0,7,100,100,,30,,,E17,,,,D35,D34,D36,A73,D122,,,,10,40,75,40,*");
    }
    if (SetSlotB == 13) {
      Serial.println("Weapon 1 set to Rail Gun");
      sendString("$WEAP,1,0,100,6,0,115,0,,,,,,,,1200,850,1,6,2400,0,2,100,100,,0,,,C03,C08,,,D36,D35,D34,A73,,,,,1,3,75,,*");
    }
    if (SetSlotB == 14) {
      Serial.println("Weapon 1 set to Rocket Launcher");
      sendString("$WEAP,1,2,100,10,0,115,0,,,,,,115,80,1000,850,2,8,1200,0,7,100,100,,0,,,C03,,,,D14,D13,D12,D18,,,,,2,4,75,30,*");
    }
    if (SetSlotB == 15) {
      Serial.println("Weapon 1 set to Shotgun");
      sendString("$WEAP,1,2,100,0,0,45,0,,,,,,70,80,900,850,6,24,400,2,7,100,100,,0,,,T01,,,,D01,D28,D27,D18,,,,,6,12,75,30,*");
    }
    if (SetSlotB == 16) {
      Serial.println("Weapon 1 set to SMG");
      sendString("$WEAP,1,,100,0,0,8,0,,,,,,,,90,850,72,288,2500,0,0,100,100,,5,,,G03,,,,D26,D25,D24,D18,D11,,,,72,144,75,,*");
    }
    if (SetSlotB == 17) {
      Serial.println("Weapon 1 set to Sniper Rifle");
      sendString("$WEAP,1,,100,0,1,80,0,,,,,,,,300,850,4,24,1700,0,7,100,100,,0,,,S16,D20,D19,,D04,D03,D21,D18,,,,,4,12,75,,*");
    }
    if (SetSlotB == 18) {
      Serial.println("Weapon 1 set to Stinger");
      sendString("$WEAP,1,,100,0,0,15,0,,,,,,,,120,850,18,72,1700,0,0,100,100,,0,,,E11,,,,D17,D16,D15,A73,,,,,18,36,75,,*");
    }
    if (SetSlotB == 19) {
      Serial.println("Weapon 1 set to Suppressor");
      sendString("$WEAP,1,,100,0,0,8,0,,,,,,,,75,850,48,288,2000,0,0,100,100,,0,2,50,Q06,,,,D26,D25,D24,D18,,,,,48,144,75,,*");
    }
  }
}


//******************************************************************************************

// sets and sends game settings based upon the stored settings
void SetFFOutdoor() {
  // token one of the following command is free for all, 0 is off and 1 is on
  if (SetODMode == 0 && SetFF == 1) {
    sendString("$GSET,1,0,1,0,1,0,50,1,*");
  }
  if (SetODMode == 1 && SetFF == 1) {
    sendString("$GSET,1,1,1,0,1,0,50,1,*");
  }
  if (SetODMode == 1 && SetFF == 0) {
    sendString("$GSET,0,1,1,0,1,0,50,1,*");
  }
  if (SetODMode == 0 && SetFF == 0) {
    sendString("$GSET,0,0,1,0,1,0,50,1,*");
  }
}

//******************************************************************************************

// ends game... thats all
void gameover() {
  sendString("$STOP,*"); // stops everything going on
  sendString("$CLEAR,*"); // clears out anything stored for game settings
  sendString("$PLAY,VS6,4,6,,,,,*"); // says game over
  GAMEOVER = false;
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
/*
   no longer used see 4/19 update notes

  void getsettings() {
  sendString("$PLAY,"+AudioSelection+",4,6,,,,,*");
  sendString("$START,*");
  sendString("$GSET,1,0,1,0,1,0,50,1,*");
  sendString("$PSET,64,5,200,200,200,,,,,,,,,,,,,,,,,,,*");
  sendString("$VOL,65,0,*"); // sets max volume on gun 0-100 feet distance
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
  }
  void exitgetsettings() {
  sendString("$GLED,,,,5,,,*"); // changes headset to tagged out color
  delay(3000);
  sendString("STOP,*"); // stops everything going on
  sendString("CLEAR,*"); // clears out anything stored for game settings in gun, not esp
  EXITSETTINGS=false; // resets settings enabling trigger
  Serial.println("successfully exited get settings gun state");
  }
*/
//****************************************************************************
void Audio() {
  if (AUDIO) {
    if (AudioPlayCounter == 0) {
      AudioPlayCounter++;
      sendString("$PLAY," + AudioSelection + ",4,6,,,,,*");
      TurnOffAudio = true;
    }
  }
  if (AUDIO1) {
    sendString("$PLAY," + AudioSelection1 + ",4,6,,,,,*");
    AUDIO1 = false;
    TurnOffAudio = false;
  }
  if (AudioSelection1 == "VA20") {
    sendString("$CLEAR,*");
    sendString("$START,*");
  }
}
//******************************************************************************************
void SyncScores() {
  int senddelay = GunID * 1000;
  String LCDText;
  delay(senddelay);
  if (SetTeam == 100) {
    // create a string that looks like this:
    // (Player ID, token 0), (Player Team, token 1), (Player Objective Score, token 2) (Team scores, tokens 3-8), (player kill counts, tokens 9-72
    String LCDText = String(GunID) + "," + String(Team) + "," + String(CompletedObjectives) + "," + String(TeamKillCount[0])+ "," + String(TeamKillCount[1]) + "," + String(TeamKillCount[2]) + "," + String(TeamKillCount[3]) + "," + String(TeamKillCount[4]) + "," + String(TeamKillCount[5]) + "," + String(PlayerKillCount[0]) + "," + String(PlayerKillCount[1]) + "," + String(PlayerKillCount[2]) + "," + String(PlayerKillCount[3]) + "," + String(PlayerKillCount[4]) + "," + String(PlayerKillCount[5]) + "," + String(PlayerKillCount[6]) + "," + String(PlayerKillCount[7]) + "," + String(PlayerKillCount[8]) + "," + String(PlayerKillCount[9]) + "," + String(PlayerKillCount[10]) + "," + String(PlayerKillCount[11]) + "," + String(PlayerKillCount[12]) + "," + String(PlayerKillCount[13]) + "," + String(PlayerKillCount[14]) + "," + String(PlayerKillCount[15]) + "," + String(PlayerKillCount[16]) + "," + String(PlayerKillCount[17]) + "," + String(PlayerKillCount[18]) + "," + String(PlayerKillCount[19]) + "," + String(PlayerKillCount[20]) + "," + String(PlayerKillCount[21]) + "," + String(PlayerKillCount[22]) + "," + String(PlayerKillCount[23]) + "," + String(PlayerKillCount[24]) + "," + String(PlayerKillCount[25]) + "," + String(PlayerKillCount[26]) + "," + String(PlayerKillCount[27]) + "," + String(PlayerKillCount[28]) + "," + String(PlayerKillCount[29]) + "," + String(PlayerKillCount[30]) + "," + String(PlayerKillCount[31]) + "," + String(PlayerKillCount[32]) + "," + String(PlayerKillCount[33]) + "," + String(PlayerKillCount[34]) + "," + String(PlayerKillCount[35]) + "," + String(PlayerKillCount[36]) + "," + String(PlayerKillCount[37]) + "," + String(PlayerKillCount[38]) + "," + String(PlayerKillCount[39]) + "," + String(PlayerKillCount[40]) + "," + String(PlayerKillCount[41]) + "," + String(PlayerKillCount[42]) + "," + String(PlayerKillCount[43]) + "," + String(PlayerKillCount[44]) + "," + String(PlayerKillCount[45]) + "," + String(PlayerKillCount[46]) + "," + String(PlayerKillCount[47]) + "," + String(PlayerKillCount[48]) + "," + String(PlayerKillCount[49]) + "," + String(PlayerKillCount[50]) + "," + String(PlayerKillCount[51]) + "," + String(PlayerKillCount[52]) + "," + String(PlayerKillCount[53]) + "," + String(PlayerKillCount[54]) + "," + String(PlayerKillCount[55]) + "," + String(PlayerKillCount[56]) + "," + String(PlayerKillCount[57]) + "," + String(PlayerKillCount[58]) + "," + String(PlayerKillCount[59]) + "," + String(PlayerKillCount[60]) + "," + String(PlayerKillCount[61]) + "," + String(PlayerKillCount[62]) + "," + String(PlayerKillCount[63]);
  } else {
    String LCDText = String(GunID) + "," + String(SetTeam) + "," + String(CompletedObjectives) + "," + String(TeamKillCount[0]) +"," + String(TeamKillCount[1]) + "," + String(TeamKillCount[2]) + "," + String(TeamKillCount[3]) + "," + String(TeamKillCount[4]) + "," + String(TeamKillCount[5]) + "," + String(PlayerKillCount[0]) + "," + String(PlayerKillCount[1]) + "," + String(PlayerKillCount[2]) + "," + String(PlayerKillCount[3]) + "," + String(PlayerKillCount[4]) + "," + String(PlayerKillCount[5]) + "," + String(PlayerKillCount[6]) + "," + String(PlayerKillCount[7]) + "," + String(PlayerKillCount[8]) + "," + String(PlayerKillCount[9]) + "," + String(PlayerKillCount[10]) + "," + String(PlayerKillCount[11]) + "," + String(PlayerKillCount[12]) + "," + String(PlayerKillCount[13]) + "," + String(PlayerKillCount[14]) + "," + String(PlayerKillCount[15]) + "," + String(PlayerKillCount[16]) + "," + String(PlayerKillCount[17]) + "," + String(PlayerKillCount[18]) + "," + String(PlayerKillCount[19]) + "," + String(PlayerKillCount[20]) + "," + String(PlayerKillCount[21]) + "," + String(PlayerKillCount[22]) + "," + String(PlayerKillCount[23]) + "," + String(PlayerKillCount[24]) + "," + String(PlayerKillCount[25]) + "," + String(PlayerKillCount[26]) + "," + String(PlayerKillCount[27]) + "," + String(PlayerKillCount[28]) + "," + String(PlayerKillCount[29]) + "," + String(PlayerKillCount[30]) + "," + String(PlayerKillCount[31]) + "," + String(PlayerKillCount[32]) + "," + String(PlayerKillCount[33]) + "," + String(PlayerKillCount[34]) + "," + String(PlayerKillCount[35]) + "," + String(PlayerKillCount[36]) + "," + String(PlayerKillCount[37]) + "," + String(PlayerKillCount[38]) + "," + String(PlayerKillCount[39]) + "," + String(PlayerKillCount[40]) + "," + String(PlayerKillCount[41]) + "," + String(PlayerKillCount[42]) + "," + String(PlayerKillCount[43]) + "," + String(PlayerKillCount[44]) + "," + String(PlayerKillCount[45]) + "," + String(PlayerKillCount[46]) + "," + String(PlayerKillCount[47]) + "," + String(PlayerKillCount[48]) + "," + String(PlayerKillCount[49]) + "," + String(PlayerKillCount[50]) + "," + String(PlayerKillCount[51]) + "," + String(PlayerKillCount[52]) + "," + String(PlayerKillCount[53]) + "," + String(PlayerKillCount[54]) + "," + String(PlayerKillCount[55]) + "," + String(PlayerKillCount[56]) + "," + String(PlayerKillCount[57]) + "," + String(PlayerKillCount[58]) + "," + String(PlayerKillCount[59]) + "," + String(PlayerKillCount[60]) + "," + String(PlayerKillCount[61]) + "," + String(PlayerKillCount[62]) + "," + String(PlayerKillCount[63]);
  }
  Serial.println(LCDText);
  SerialLCD.println(LCDText);
  Serial.println("Sent LCD data to ESP8266");
}
//******************************************************************************************
//******************************************************************************************
//******************************************************************************************
// serial communications that is pinned to second core in set up
// this is where we write to variable to configure settings to send over BLE to tagger
void serialTask(void * params) {
  for (;;) {
    if (settingsallowed1 > 0) {
      settingsallowed = 0; // checking that a trigger was set from other core, if so, disabling it on this core
    }
    if (TurnOffAudio) {
      AUDIO = false;
      AudioPlayCounter = 0;
    }
    if (TAGGERUPDATE) {
      Serial.println("Disabled LCD Data Send for BLE Core Triggered");
      TAGGERUPDATE1 = true;
      if (GETSLOT1) {
        ammo1 = 500;
        weap = SLOTB;
        Serial.println("sending weapon selection info for weapon slot 0");
      }
      if (GETSLOT0) {
        ammo1 = 500;
        weap = SLOTA;
        Serial.println("sending weapon selection info for weapon slot 1");
      }
      String LCDText = String(ammo1) + "," + String(weap) + "," + String(health) + "," + String(armor) + "," + String(shield) + "," + String(PlayerLives) + "," + String(ammo2) + "," + String(GunID);
      Serial.println(LCDText);
      SerialLCD.println(LCDText);
      Serial.println("Sent LCD data to ESP8266");
      delay(100);
    } else {
      TAGGERUPDATE1 = false;
    }
    if (SerialLCD.available()) {
      String readtxt = SerialLCD.readStringUntil('\n');
      Serial.println(readtxt);
      // set the BLE core to play audio
      // it is important to ensure that all following processes only assign a value
      // for a variable that is used by the other core to process and send BLE data
      // to tagger, dont call objects in here that are potentially used by the other core
      // What we need to do here is analyze the serial data from ESP8266 and do something with it
      // So we set a programing variable, print the change to serial, and make the tagger confirm via audio
      // setting weapon slot 0
      if (readtxt.toInt() == 1) {
        settingsallowed = 1;
        AudioSelection = "VA5F";
        SetSlotA = 100;
        Serial.println("Weapon Slot 0 set to Manual");
      }
      if (readtxt.toInt() > 1 && readtxt.toInt() < 100) {
        SetSlotA = readtxt.toInt() - 1;
        Serial.println("Weapon Slot 0 set");
        AudioSelection = "VA9T";
      }
      // setting weapon slot 1
      if (readtxt.toInt() == 101) {
        settingsallowed = 2;
        AudioSelection = "VA5F";
        SetSlotB = 100;
        Serial.println("Weapon Slot 1 set to Manual");
      }
      if (readtxt.toInt() > 101 && readtxt.toInt() < 200) {
        SetSlotB = readtxt.toInt() - 101;
        Serial.println("Weapon Slot 1 set");
        AudioSelection = "VA9T";
      }
      // setting objective count
      if (readtxt.toInt() == 200) {
        SetObj = 32000;
        Serial.println("Objectives to win set to Unlimited");
        AudioSelection = "VA6V";
      }
      if (readtxt.toInt() > 200 && readtxt.toInt() < 300) {
        SetObj = (readtxt.toInt() - 200);
        Serial.println("Objectives to win set to " + String(Objectives));
        AudioSelection = "VA7P";
      }
      // setting kill count goal
      if (readtxt.toInt() > 300 && readtxt.toInt() < 400) {
        MaxKills = (readtxt.toInt() - 300);
        Serial.println("Kills to win set to " + String(MaxKills));
        AudioSelection = "VN8";
      }
      if (readtxt.toInt() == 300) {
        MaxKills = 32000;
        Serial.println("Kills to win set to Unlimited");
        AudioSelection = "VA6V";
      }
      // setting player lives
      if (readtxt.toInt() > 400 && readtxt.toInt() < 500) {
        SetLives = (readtxt.toInt() - 300);
        Serial.println("Player Lives set to " + String(PlayerLives));
        AudioSelection = "VA47";
      }
      if (readtxt.toInt() == 400) {
        PlayerLives = 32000;
        Serial.println("Kills to win set to Unlimited");
        AudioSelection = "VA6V";
      }
      // setting game time
      if (readtxt.toInt() == 501) {
        SetTime = 60000;
        Serial.println("Game time set to 1 minute");
        AudioSelection = "VA0V";
      }
      if (readtxt.toInt() == 502) {
        SetTime = 300000;
        Serial.println("Game time set to 5 minute");
        AudioSelection = "VA2S";
      }
      if (readtxt.toInt() == 503) {
        SetTime = 600000;
        Serial.println("Game time set to 10 minute");
        AudioSelection = "VA6H";
      }
      if (readtxt.toInt() == 504) {
        SetTime = 900000;
        Serial.println("Game time set to 15 minute");
        AudioSelection = "VA2P";
      }
      if (readtxt.toInt() == 505) {
        SetTime = 1200000;
        Serial.println("Game time set to 20 minute");
        AudioSelection = "VA6Q";
      }
      if (readtxt.toInt() == 506) {
        SetTime = 1500000;
        Serial.println("Game time set to 25 minute");
        AudioSelection = "VA6P";
      }
      if (readtxt.toInt() == 507) {
        SetTime = 1800000;
        Serial.println("Game time set to 30 minute");
        AudioSelection = "VA0Q";
      }
      if (readtxt.toInt() == 508) {
        SetTime = 2000000000;
        Serial.println("Game time set to Unlimited");
        AudioSelection = "VA6V";
      }
      // set outdoor/indoor settings
      if (readtxt.toInt() == 601) {
        SetODMode = 0;
        Serial.println("Outdoor Mode On");
        AudioSelection = "VA4W";
      }
      if (readtxt.toInt() == 602) {
        SetODMode = 1;
        Serial.println("Indoor Mode On");
        AudioSelection = "VA3W";
      }
      if (readtxt.toInt() == 603) {
        SetODMode = 1;
        Serial.println("Stealth Mode On");
        AudioSelection = "VA60";
      }
      // set team settings
      if (readtxt.toInt() == 701) {
        Serial.println("Free For All");
        SetTeam = 0;
        SetFF = 1;
        AudioSelection = "VA30";
      }
      if (readtxt.toInt() == 702) {
        Serial.println("Teams Mode Two Teams (odds/evens)");
        if (GunID % 2) {
          SetTeam = 0;
          AudioSelection = "VA13";
        } else {
          SetTeam = 1;
          AudioSelection = "VA1L";
        }
      }
      if (readtxt.toInt() == 703) {
        Serial.println("Teams Mode Player's Choice");  // this one allows for manual input of settings... each gun will need to select a team now
        settingsallowed = 3;
        SetTeam = 100;
        AudioSelection = "VA5E";
      }
      // setting game time
      if (readtxt.toInt() == 801) {
        GameMode = 1;
        Serial.println("Game mode set to Deathmatch");
        AudioSelection = "VA26";
      }
      if (readtxt.toInt() == 802) {
        GameMode = 2;
        Serial.println("Game mode set to Capture the Flag");
        AudioSelection = "VA8P";
      }
      if (readtxt.toInt() == 803) {
        GameMode = 3;
        Serial.println("Game mode set to Assault");
        AudioSelection = "VA8F";
      }
      if (readtxt.toInt() == 804) {
        GameMode = 4;
        Serial.println("Game mode set to King of the Hill");
        AudioSelection = "VA93";
      }
      if (readtxt.toInt() == 805) {
        GameMode = 5;
        Serial.println("Game mode set to Survival");
        AudioSelection = "VA64";
      }
      if (readtxt.toInt() == 806) {
        GameMode = 6;
        Serial.println("Game mode set to Trouble in Terrorist Town");
        AudioSelection = "VA5U";
      }
      if (readtxt.toInt() == 807) {
        GameMode = 7;
        Serial.println("Game mode set to You only live twice");
        AudioSelection = "VA8I";
      }
      if (readtxt.toInt() == 808) {
        GameMode = 8;
        Serial.println("Game mode set to One Shot Kills");
        AudioSelection = "VA8K";
      }
      if (readtxt.toInt() == 809) {
        GameMode = 9;
        Serial.println("Game mode set to Gun Game");
        AudioSelection = "VA9T";
      }
      if (readtxt.toInt() == 810) {
        GameMode = 10;
        Serial.println("Game mode set to One Domination");
        AudioSelection = "VA21";
      }
      if (readtxt.toInt() == 811) {
        GameMode = 11;
        Serial.println("Game mode set to Battle Royale");
        AudioSelection = "VA8J";
      }
      // Setting Respawn Settings
      if (readtxt.toInt() == 901) {
        SetRSPNMode = 1;
        Serial.println("Respawn Set to Immediate");
        AudioSelection = "VA54";
      }
      if (readtxt.toInt() == 902) {
        SetRSPNMode = 2;
        Serial.println("Respawn Set to 15 seconds");
        AudioSelection = "VA2Q";
      }
      if (readtxt.toInt() == 903) {
        SetRSPNMode = 3;
        Serial.println("Respawn Set to 30 seconds");
        AudioSelection = "VA0R";
      }
      if (readtxt.toInt() == 904) {
        SetRSPNMode = 4;
        Serial.println("Respawn Set to 45 seconds");
        AudioSelection = "VA0T";
      }
      if (readtxt.toInt() == 905) {
        SetRSPNMode = 5;
        Serial.println("Respawn Set to 60 seconds");
        AudioSelection = "VA0V";
      }
      if (readtxt.toInt() == 906) {
        SetRSPNMode = 6;
        Serial.println("Respawn Set to 90 seconds");
        AudioSelection = "VA0X";
      }
      if (readtxt.toInt() == 907) {
        SetRSPNMode = 7;
        Serial.println("Respawn Set to Ramp 45");
        AudioSelection = "VA0S";
      }
      if (readtxt.toInt() == 908) {
        SetRSPNMode = 8;
        Serial.println("Respawn Set to Ramp 90");
        AudioSelection = "VA0W";
      }
      if (readtxt.toInt() == 909) {
        SetRSPNMode = 9;
        Serial.println("Respawn Set to Manual/Respawn Station");
        AudioSelection = "VA9H";
      }
      // set start delay
      if (readtxt.toInt() == 1001) {
        DelayStart = 1;
        Serial.println("Delay Start Set to Immediate");
        AudioSelection = "VA4T";
      }
      if (readtxt.toInt() == 1002) {
        DelayStart = 2;
        Serial.println("Delay Start Set to 15 seconds");
        AudioSelection = "VA2Q";
      }
      if (readtxt.toInt() == 1003) {
        DelayStart = 3;
        Serial.println("Delay Start Set to 30 seconds");
        AudioSelection = "VA0R";
      }
      if (readtxt.toInt() == 1004) {
        DelayStart = 4;
        Serial.println("Delay Start Set to 45 seconds");
        AudioSelection = "VA0T";
      }
      if (readtxt.toInt() == 1005) {
        DelayStart = 5;
        Serial.println("Delay Start Set to 60 seconds");
        AudioSelection = "VA0V";
      }
      if (readtxt.toInt() == 1006) {
        DelayStart = 6;
        Serial.println("Delay Start Set to 90 seconds");
        AudioSelection = "VA0X";
      }
      if (readtxt.toInt() == 1007) {
        DelayStart = 7;
        Serial.println("Delay Start Set to 5 minutes");
        AudioSelection = "VA2S";
      }
      if (readtxt.toInt() == 1008) {
        DelayStart = 8;
        Serial.println("Delay Start Set to 10 minutes");
        AudioSelection = "VA6H";
      }
      if (readtxt.toInt() == 1009) {
        DelayStart = 9;
        Serial.println("Delay Start Set to 15 minutes");
        AudioSelection = "VA2P";
      }
      // Sync Score Request Recieved
      if (readtxt.toInt() == 1101) {
        SyncScores();
        Serial.println("Request Recieved to Sync Scoring");
        AudioSelection = "VA91";
      }
      // setting gender
      if (readtxt.toInt() == 1200) {
        SetGNDR = 0;
        Serial.println("Gender set to Male");
        AudioSelection = "V3I";
      }
      if (readtxt.toInt() == 1201) {
        SetGNDR = 1;
        Serial.println("Gender set to Female");
        AudioSelection = "VBI";
      }
      // setting friendly fire mode
      if (readtxt.toInt() == 1303) {
        UNLIMITEDAMMO = 3;
        Serial.println("Ammo set to unlimited rounds");
        AudioSelection = "VA6V";
      }
      if (readtxt.toInt() == 1302) {
        UNLIMITEDAMMO = 2;
        Serial.println("Ammo set to unlimited magazies");
        AudioSelection = "VA6V";
      }
      if (readtxt.toInt() == 1301) {
        UNLIMITEDAMMO = 1;
        Serial.println("Ammo set to limited");
        AudioSelection = "VA14";
      }
      // setting Auto ammo replenish mode
      if (readtxt.toInt() == 1401) {
        SetFF = 1;
        Serial.println("Friendly Fire On");
        AudioSelection = "VA32";
      }
      if (readtxt.toInt() == 1400) {
        SetFF = 0;
        Serial.println("Friendly Fire Off");
        AudioSelection = "VA31";
      }
      // Setting Tagger volume
      if (readtxt.toInt() > 1499 && readtxt.toInt() < 1600) {
        SetVol = (readtxt.toInt() - 1499);
        Serial.println("Volume set to" + String(SetVol));
        AudioSelection = "VA9S";
      }
      // enabling game start
      if (readtxt.toInt() == 1601) {
        GAMESTART = true;
        Serial.println("starting game");
      }
      if (readtxt.toInt() == 1600) {
        GAMEOVER = true;
        Serial.println("ending game");
      }
      // enable audio notification for changes
      if (1600 > readtxt.toInt() && readtxt.toInt() > 0) {
        AUDIO = true;
      }
    }
    delay(1000);
  }
}
TaskHandle_t  TaskSerial;
//******************************************************************************************
//******************************************************************************************
//******************************************************************************************
bool connectToServer() {
  bool rsl = false;
  connected = SerialBT.connect(address); // we can use name as well but it is slower
  if (connected) {
    rsl = true;
    Serial.println("Connected Succesfully!");
  } else {
    while (!SerialBT.connected(10000)) {
      Serial.println("Failed to connect. Make sure remote device is available and in range, then restart app.");
    }
  }
  return rsl;
}
void callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
  if (event == ESP_SPP_SRV_OPEN_EVT) {
    Serial.println("Client Connected");
    doConnect = false;
  }
  if (event == ESP_SPP_CLOSE_EVT) {
    Serial.println("Connection Closed");
    doConnect = true;
  }
  if (event == ESP_SPP_DATA_IND_EVT ) {
    notifyCallback();
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting Arduino BLE Client application...");
  SerialLCD.begin(9600, SERIAL_8N1, 16, 17); // setting up serial communication with ESP8266 on pins 16/17 w baud rate of 9600
  delay(5000);
  SerialBT.register_callback(callback);



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

// This is the Arduino main loop function for the BLE Core.
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
      doConnect = false; // stop trying to make the connection.
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

    if (TAGGERUPDATE1) {
      TAGGERUPDATE = false;
      Serial.println("Disabled LCD Data Send");
    }
    if (settingsallowed1 == 3) {
      Serial.println("Team Settings requested");
      delay(250);
      GETTEAM = true;
      settingsallowed1 = 0;
    }
    if (settingsallowed1 == 1) {
      Serial.println("Weapon Slot 0 Requested");
      delay(250);
      GETSLOT0 = true;
      settingsallowed1 = 0;
    }
    if (settingsallowed1 == 2) {
      Serial.println("Weapon Slot 1 Requested");
      delay(250);
      GETSLOT1 = true;
      settingsallowed1 = 0;
    }
    if (settingsallowed > 0) {
      Serial.println("manual settings requested");  // this is triggered if a manual option is required for game settings
      settingsallowed1 = settingsallowed;
    }
    if (RESPAWN) { // checks if respawn was triggered to respawn a player
      respawnplayer(); // respawns player
    }
    if (GAMEOVER) { // checks if something triggered game over (out of lives, objective met)
      gameover(); // runs object to kick player out of game
    }
    if (AUDIO) {
      Audio();
    } else {
      TurnOffAudio = false;
    }
    if (AUDIO1) {
      Audio();
    }
    if (GAMESTART) {
      gameconfigurator();
      delaystart();
      GAMESTART = false;
    }
    if (OutofAmmoA) {
      weaponsettingsA();
      Serial.println("Weapon Slot 0 has been reloaded, disabling reload");
      OutofAmmoA = false;
    }
    if (OutofAmmoB) {
      weaponsettingsB();
      Serial.println("Weapon Slot 1 has been reloaded, disabling reload");
      OutofAmmoB = false;
    }
    // game settings and objective completion checks:
    if ((millis() - GameStartTime) > GameTimer) {
      GAMEOVER = true;
      Serial.println("game time expired");
      GameStartTime = 0;
    }

    //************************************************************************************
  } /*else if (doScan) {
    if (millis() - startScan > 5000) {
      Serial.println("Scanning again");
      BLEDevice::init("");
      BLEDevice::getScan()->start(10, true); // this is just example to start scan after disconnect, most likely there is better way to do it in arduino
      startScan = millis();
    }
  }*/
  delay(500); // delay a second between loops.
} // End of loop
