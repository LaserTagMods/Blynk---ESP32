/* ESP8266 Laser Tag Points Accumulation and Score Reporting Device
 *  
 *  **************************************************
 *  this sketch is for an esp8266 to work in conjunction with other esp8266s
 *  that are connected to the same network and reporting individual scores.
 *  This device will accumulate and calculate various reportables
 *  ***************************************************
 *  Instruction: 
 *  Ensure the proper network settings are correct and provide power to device
 *  
 *  ****************************************************
 *  Updates and logs: (date - individual - changes)
 *  
 *  5/26/2020 - Initial Code Draft for testing
 *  8/11/2020 - changed score request process to cycle on a timer and send a new player id each time to trigger spaced out data receipts and added player count to use as a way to reduce time spent aquiring scores
 *  8/14/2020 - fixed a few minor bugs on score reporting, added a clear scores process for when a new game starts, scores are reset
 *  
 *  
 */ 
#define BLYNK_PRINT Serial
// Libraries needed to accomodate sketch
#include <ESP8266WiFi.h> // used for setting up wifi comms
#include <BlynkSimpleEsp8266.h> // used to run all necessary blynk objects/functions
// set up bridge widget on V1 of Device 1
// this creates the widget needed to send data to the controller
WidgetBridge bridge1(V1);
//**********************************************************************************
//****************************  UPDATE THIS SECTION!!!!! ***************************
//**********************************************************************************
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon)
char auth[] = "slcCz6BNVlNAFh5XKRBrREydDDfXIEoB";
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Burtek Energy";
char pass[] = "Sunpower2020";
// set the bridge token
BLYNK_CONNECTED() {
  bridge1.setAuthToken("utieZeB7dJLmtGlz2S3KL9M7-icUDuDN"); // Token of the taggers
}
int PlayerCount = 20; // How many Taggers are you playing with? 
                     // they should be Identified as 0-63, 0-9, etc. depending on your count
                     // This is important to speed up syncing scores and not waste time waiting
                     // for non existing guns-players
//**********************************************************************************
//****************************  UPDATE THIS SECTION!!!!! ***************************
//**********************************************************************************
// Variables needed:
int ToTaggers=0;
String tokenStrings[8];
String ScoreTokenStrings[73];
int PlayerDeaths[64];
int PlayerKills[64];
int PlayerObjectives[64];
int TeamKills[4];
int TeamObjectives[4];

String readStr="0";

int ScoreRequestCounter = 0;

bool INGAME = false;
bool SCORESYNC = false;

long gametimer = 0;

// timers for running certain applications periodically with the blynk program
BlynkTimer UpdateClock; // created a timer object called "UpdateClock"
BlynkTimer RequestScores; // created a timer object called "RequestScores"

//******************************************************************
// object used to receive data from each configurator
BLYNK_WRITE(V0) {
    readStr = param.asStr(); // saves incoming data as a temporary string within this object
    Serial.println("printing data received: ");
    Serial.println(readStr);
    char *ptr = strtok((char*)readStr.c_str(), ","); // looks for commas as breaks to split up the string
    int index = 0;
    while (ptr != NULL)
    {
      ScoreTokenStrings[index] = ptr; // saves the individual characters divided by commas as individual strings
      index++;
      ptr = strtok(NULL, ",");  // takes a list of delimiters
    }
    // received a string that looks like this: 
    // (Player ID, token 0), (Player Team, token 1), (Player Objective Score, token 2) (Team scores, tokens 3-8), (player kill counts, tokens 9-72 
    Serial.println("Score Data Recieved from a tagger:");
    
    
    int Deaths=0;
    int Objectives=0;
    int Kills=0;
    int Team=0;
    int Player=0;
    int Data[73];
    int count=0;
    while (count<73) {
      Data[count]=ScoreTokenStrings[count].toInt();
      Serial.println("Converting String character "+String(count)+" to integer: "+String(Data[count]));
      count++;
    }
    Player=Data[0];
    Serial.println("Player ID: " + String(Player));
    Deaths = Data[3] + Data[4] + Data[5] + Data[6] + Data[7] + Data[8]; // added the total team kills to accumulate the number of deaths of this player
    Serial.println("Player Deaths: "+String(Deaths));
    PlayerDeaths[Player] = Deaths; // just converted temporary data to this players death count record
    TeamKills[0] = TeamKills[0] + Data[3]; // accumulating team kill counts
    TeamKills[1] = TeamKills[1] + Data[4];
    TeamKills[2] = TeamKills[2] + Data[5];
    TeamKills[3] = TeamKills[3] + Data[6]; // note, not handling teams 5/6 because they dont actually report/exist
    Serial.println("Team Kill Count Accumulation Complete");
    Serial.println("Red Team Kills: " + String(TeamKills[0]) + "Blue Team Kills: " + String(TeamKills[1]) + "Green Team Kills: " + String(TeamKills[2]) + "Yellow Team Kills: " + String(TeamKills[3]));
    // accumulating player kill counts
    int p = 9; // using for data characters 9-72
    int j = 0; // using for player id status counter
    Serial.println("Accumulating Player kills against current player...");
    while (p < 73) {
      PlayerKills[j] = PlayerKills[j] + Data[p];
      Serial.println("Player " + String(j) + " Killed this player " + String(Data[p]) + " times, Player's new score is " + String(PlayerKills[j]));
      p++;
      j++;
    }
    Team = Data[1]; // setting the player's team for accumulation of player objectives
    PlayerObjectives[Player] = Data[2]; // converted temporary data storage to main memory for score reporting of player objectives
    Serial.println("Player Objectives completed: "+String(Data[2]));
    TeamObjectives[Team] = TeamObjectives[Team] + Data[2]; // added this player's objective score to his team's objective score
    }


// object used to que score reporting requests from each gun
BLYNK_WRITE(V127) {
  int b=param.asInt();
  if (b==1) {
    SCORESYNC = true; // enables object to request scores
    INGAME = false; // disables the counter clock
    Serial.println("Commencing Score Sync Process");
    ScoreRequestCounter = 0;
    }
}

BLYNK_WRITE(V16) {
  int b=param.asInt();
  if (b==1) {
    INGAME = true; // enables game counter
    gametimer = 0; // reset game timer
    ClearScores(); // need to reset scores from previous game
  } else {
    INGAME = false; // disables game counter
  }
}

//*****************************************************************************************************************************

void ClearScores() {
  int playercounter = 0;
  while (playercounter < 64) {
    PlayerKills[playercounter] = 0;
    playercounter++;
  }
  int teamcounter = 0;
  while (teamcounter < 4) {
    TeamKills[teamcounter] = 0;
    TeamObjectives[teamcounter] = 0;
    teamcounter++;
  }
  Serial.println("Reset all stored scores from previous game");
}

void updateblynkapp() {    
    // send scores to blynk accumulator device over bridge to be summed and then posted to blynk server/app
    // designating certain pins for score accumulation. 
    // Pins 0-63 are for player kill counts, this sends all player scores
    Serial.println("Sending All Scores to Blynk App");
    Blynk.virtualWrite(V0, PlayerKills[0]); // sending the value to controller
    Blynk.virtualWrite(V1, PlayerKills[1]); // sending the value to controller
    Blynk.virtualWrite(V2, PlayerKills[2]); // sending the value to controller
    Blynk.virtualWrite(V3, PlayerKills[3]); // sending the value to controller
    Blynk.virtualWrite(V4, PlayerKills[4]); // sending the value to controller
    Blynk.virtualWrite(V5, PlayerKills[5]); // sending the value to controller
    Blynk.virtualWrite(V6, PlayerKills[6]); // sending the value to controller
    Blynk.virtualWrite(V7, PlayerKills[7]); // sending the value to controller
    Blynk.virtualWrite(V8, PlayerKills[8]); // sending the value to controller
    Blynk.virtualWrite(V9, PlayerKills[9]); // sending the value to controller
    Blynk.virtualWrite(V10, PlayerKills[10]); // sending the value to controller
    Blynk.virtualWrite(V11, PlayerKills[11]); // sending the value to controller
    Blynk.virtualWrite(V12, PlayerKills[12]); // sending the value to controller
    Blynk.virtualWrite(V13, PlayerKills[13]); // sending the value to controller
    Blynk.virtualWrite(V14, PlayerKills[14]); // sending the value to controller
    Blynk.virtualWrite(V15, PlayerKills[15]); // sending the value to controller
    Blynk.virtualWrite(V16, PlayerKills[16]); // sending the value to controller
    Blynk.virtualWrite(V17, PlayerKills[17]); // sending the value to controller
    Blynk.virtualWrite(V18, PlayerKills[18]); // sending the value to controller
    Blynk.virtualWrite(V19, PlayerKills[19]); // sending the value to controller
    Blynk.virtualWrite(V20, PlayerKills[20]); // sending the value to controller
    Blynk.virtualWrite(V21, PlayerKills[21]); // sending the value to controller
    Blynk.virtualWrite(V22, PlayerKills[22]); // sending the value to controller
    Blynk.virtualWrite(V23, PlayerKills[23]); // sending the value to controller
    Blynk.virtualWrite(V24, PlayerKills[24]); // sending the value to controller
    Blynk.virtualWrite(V25, PlayerKills[25]); // sending the value to controller
    Blynk.virtualWrite(V26, PlayerKills[26]); // sending the value to controller
    Blynk.virtualWrite(V27, PlayerKills[27]); // sending the value to controller
    Blynk.virtualWrite(V28, PlayerKills[28]); // sending the value to controller
    Blynk.virtualWrite(V29, PlayerKills[29]); // sending the value to controller
    Blynk.virtualWrite(V30, PlayerKills[30]); // sending the value to controller
    Blynk.virtualWrite(V31, PlayerKills[31]); // sending the value to controller
    Blynk.virtualWrite(V32, PlayerKills[32]); // sending the value to controller
    Blynk.virtualWrite(V33, PlayerKills[33]); // sending the value to controller
    Blynk.virtualWrite(V34, PlayerKills[34]); // sending the value to controller
    Blynk.virtualWrite(V35, PlayerKills[35]); // sending the value to controller
    Blynk.virtualWrite(V36, PlayerKills[36]); // sending the value to controller
    Blynk.virtualWrite(V37, PlayerKills[37]); // sending the value to controller
    Blynk.virtualWrite(V38, PlayerKills[38]); // sending the value to controller
    Blynk.virtualWrite(V39, PlayerKills[39]); // sending the value to controller
    Blynk.virtualWrite(V40, PlayerKills[40]); // sending the value to controller
    Blynk.virtualWrite(V41, PlayerKills[41]); // sending the value to controller
    Blynk.virtualWrite(V42, PlayerKills[42]); // sending the value to controller
    Blynk.virtualWrite(V43, PlayerKills[43]); // sending the value to controller
    Blynk.virtualWrite(V44, PlayerKills[44]); // sending the value to controller
    Blynk.virtualWrite(V45, PlayerKills[45]); // sending the value to controller
    Blynk.virtualWrite(V46, PlayerKills[46]); // sending the value to controller
    Blynk.virtualWrite(V47, PlayerKills[47]); // sending the value to controller
    Blynk.virtualWrite(V48, PlayerKills[48]); // sending the value to controller
    Blynk.virtualWrite(V49, PlayerKills[49]); // sending the value to controller
    Blynk.virtualWrite(V50, PlayerKills[50]); // sending the value to controller
    Blynk.virtualWrite(V51, PlayerKills[51]); // sending the value to controller
    Blynk.virtualWrite(V52, PlayerKills[52]); // sending the value to controller
    Blynk.virtualWrite(V53, PlayerKills[53]); // sending the value to controller
    Blynk.virtualWrite(V54, PlayerKills[54]); // sending the value to controller
    Blynk.virtualWrite(V55, PlayerKills[55]); // sending the value to controller
    Blynk.virtualWrite(V56, PlayerKills[56]); // sending the value to controller
    Blynk.virtualWrite(V57, PlayerKills[57]); // sending the value to controller
    Blynk.virtualWrite(V58, PlayerKills[58]); // sending the value to controller
    Blynk.virtualWrite(V59, PlayerKills[59]); // sending the value to controller
    Blynk.virtualWrite(V60, PlayerKills[60]); // sending the value to controller
    Blynk.virtualWrite(V61, PlayerKills[61]); // sending the value to controller
    Blynk.virtualWrite(V62, PlayerKills[62]); // sending the value to controller
    Blynk.virtualWrite(V63, PlayerKills[63]); // sending the value to controller
    // team kill scores (6 pins) are located on pins 64-69
    Blynk.virtualWrite(V70, TeamKills[0]); // sending the value to controller Red Kills
    Blynk.virtualWrite(V71, TeamKills[1]); // sending the value to controller Blue Kills
    Blynk.virtualWrite(V72, TeamKills[2]); // sending the value to controller Green Kills
    Blynk.virtualWrite(V73, TeamKills[3]); // sending the value to controller Yellow Kills
    // now we need to send objective points for the teams. This will be on pins 70-75
    // we are only sending on one pin because were using the players objective points
    // to accumulate on the right teams pin.
    Blynk.virtualWrite(V74, TeamObjectives[0]); // sending the value to controller - Red
    Blynk.virtualWrite(V75, TeamObjectives[1]); // sending the value to controller - Blue
    Blynk.virtualWrite(V76, TeamObjectives[2]); // sending the value to controller - Green
    Blynk.virtualWrite(V77, TeamObjectives[3]); // sending the value to controller - Yellow
    // now we calculate and send leader board information
    int KillsLeader[3];
    int ObjectiveLeader[3];
    int DeathsLeader[3];
    int LeaderScore[3];
    // first Leader for Kills:
    int kmax=0; 
    int highest=0;
    for (int k=0; k<64; k++)
    if (PlayerKills[k] > highest) {
    highest = PlayerKills[k];
    kmax = k;
      }
      //  max now contains the largest kills value
      // kmax contains the index to the largest kills value.
      KillsLeader[0] = kmax; // setting the slot 0 for the first place holder
      LeaderScore[0] = highest; // setts the leader score as a temp stored data for now      
      PlayerKills[kmax]=0; // resetting the first place leader's score
      // we do it again for second place
      kmax=0; highest=0; // starting over again but now the highest score is gone
      for (int k=0; k<64; k++)
      if (PlayerKills[k] > highest) {
        highest = PlayerKills[k];
        kmax = k;
      }
      //  max now contains the largest kills value
      // kmax contains the index to the largest kills value.
      KillsLeader[1] = kmax; // setting the slot 1 for the second place holder
      LeaderScore[1] = highest; // setts the leader score as a temp stored data for now      
      PlayerKills[kmax]=0; // resetting the second place leader's score
      // one final time for third place
      kmax=0; highest=0; // starting over again but now the highest score is gone
      for (int k=0; k<64; k++)
      if (PlayerKills[k] > highest) {
        highest = PlayerKills[k];
        kmax = k;
      }
      //  max now contains the largest kills value
      // kmax contains the index to the largest kills value.
      KillsLeader[2] = kmax; // setting the slot 2 for the first place holder
      LeaderScore[2] = highest; // setts the third score as a temp stored data for now      
      PlayerKills[kmax]=0; // resetting the first place leader's score
      // now we send the updates to the blynk app
      Blynk.virtualWrite(V78, KillsLeader[0]); // sending the index or playr id to controller - 1st place
      Blynk.virtualWrite(V79, KillsLeader[1]); // sending the index or playr id to controller - 2nd place
      Blynk.virtualWrite(V80, KillsLeader[2]); // sending the index or playr id to controller - 3rd place
      // now get the player's scores back where they should be:
      PlayerKills[KillsLeader[0]] = LeaderScore[0];
      PlayerKills[KillsLeader[1]] = LeaderScore[1];
      PlayerKills[KillsLeader[2]] = LeaderScore[2];

      // Now Leader for Objectives:
      kmax=0; highest=0;
      for (int k=0; k<64; k++)
      if (PlayerObjectives[k] > highest) {
        highest = PlayerObjectives[k];
        kmax = k;
      }
      //  max now contains the largest kills value
      // kmax contains the index to the largest kills value.
      ObjectiveLeader[0] = kmax; // setting the slot 0 for the first place holder
      LeaderScore[0] = highest; // setts the leader score as a temp stored data for now      
      PlayerObjectives[kmax]=0; // resetting the first place leader's score
      // we do it again for second place
      kmax=0; highest=0; // starting over again but now the highest score is gone
      for (int k=0; k<64; k++)
      if (PlayerObjectives[k] > highest) {
        highest = PlayerObjectives[k];
        kmax = k;
      }
      //  max now contains the largest kills value
      // kmax contains the index to the largest kills value.
      ObjectiveLeader[1] = kmax; // setting the slot 1 for the second place holder
      LeaderScore[1] = highest; // setts the leader score as a temp stored data for now      
      PlayerObjectives[kmax]=0; // resetting the second place leader's score
      // one final time for third place
      kmax=0; highest=0; // starting over again but now the highest score is gone
      for (int k=0; k<64; k++)
      if (PlayerObjectives[k] > highest) {
        highest = PlayerObjectives[k];
        kmax = k;
      }
      //  max now contains the largest kills value
      // kmax contains the index to the largest kills value.
      ObjectiveLeader[2] = kmax; // setting the slot 2 for the first place holder
      LeaderScore[2] = highest; // setts the third score as a temp stored data for now      
      PlayerObjectives[kmax]=0; // resetting the first place leader's score
      // now we send the updates to the blynk app
      Blynk.virtualWrite(V81, ObjectiveLeader[0]); // sending the index or playr id to controller - 1st place
      Blynk.virtualWrite(V82, ObjectiveLeader[1]); // sending the index or playr id to controller - 2nd place
      Blynk.virtualWrite(V83, ObjectiveLeader[2]); // sending the index or playr id to controller - 3rd place
      // now get the player's scores back where they should be:
      PlayerObjectives[ObjectiveLeader[0]] = LeaderScore[0];
      PlayerObjectives[ObjectiveLeader[1]] = LeaderScore[1];
      PlayerObjectives[ObjectiveLeader[2]] = LeaderScore[2];

      // Now Leader for Deaths (this is opposite:
      kmax=0; highest=0;
      for (int k=0; k<64; k++)
      if (PlayerDeaths[k] > highest) {
        highest = PlayerDeaths[k];
        kmax = k;
      }
      //  max now contains the largest kills value
      // kmax contains the index to the largest kills value.
      DeathsLeader[0] = kmax; // setting the slot 0 for the first place holder
      LeaderScore[0] = highest; // setts the leader score as a temp stored data for now      
      PlayerDeaths[kmax]=0; // resetting the first place leader's score
      // we do it again for second place
      kmax=0; highest=0; // starting over again but now the highest score is gone
      for (int k=0; k<64; k++)
      if (PlayerDeaths[k] > highest) {
        highest = PlayerDeaths[k];
        kmax = k;
      }
      //  max now contains the largest kills value
      // kmax contains the index to the largest kills value.
      DeathsLeader[1] = kmax; // setting the slot 1 for the second place holder
      LeaderScore[1] = highest; // setts the leader score as a temp stored data for now      
      PlayerDeaths[kmax]=0; // resetting the second place leader's score
      // one final time for third place
      kmax=0; highest=0; // starting over again but now the highest score is gone
      for (int k=0; k<64; k++)
      if (PlayerDeaths[k] > highest) {
        highest = PlayerDeaths[k];
        kmax = k;
      }
      //  max now contains the largest kills value
      // kmax contains the index to the largest kills value.
      DeathsLeader[2] = kmax; // setting the slot 2 for the first place holder
      LeaderScore[2] = highest; // setts the third score as a temp stored data for now      
      PlayerDeaths[kmax]=0; // resetting the first place leader's score
      // now we send the updates to the blynk app
      Blynk.virtualWrite(V84, DeathsLeader[0]); // sending the index or playr id to controller - 1st place
      Blynk.virtualWrite(V85, DeathsLeader[1]); // sending the index or playr id to controller - 2nd place
      Blynk.virtualWrite(V86, DeathsLeader[2]); // sending the index or playr id to controller - 3d place
      // now get the player's scores back where they should be:
      PlayerDeaths[DeathsLeader[0]] = LeaderScore[0];
      PlayerDeaths[DeathsLeader[1]] = LeaderScore[1];
      PlayerDeaths[DeathsLeader[2]] = LeaderScore[2];
    }

void GameTimer() { // runs when a game is started
  gametimer++;
  Blynk.virtualWrite(V120, gametimer);
  Serial.println("running game timer and added one second to the clock");
}

void RequestingScores() {
  bridge1.virtualWrite(V11, ScoreRequestCounter); // sending the Player Score Request number to all players/taggers
  if (ScoreRequestCounter < PlayerCount) {
    Serial.println("Sent Request for Score to Player "+String(ScoreRequestCounter)+" out of "+String(PlayerCount));
    ScoreRequestCounter++;
  } else {
    SCORESYNC = false; // disables the score requesting object until a score is reported back from a player
    ScoreRequestCounter = 0;
    updateblynkapp();
    Serial.println("All Scores Requested, Closing Score Request Process, Resetting Counter");
  }
}
//****************************************************************
//*****************************************************************************************
//*****************************************************************************************

void setup()
{
   // put your setup code here, to run once:
  // Debug console
  Serial.begin(115200);

//**********************************************************************************
//****************************  UPDATE THIS SECTION!!!!! ***************************
//**********************************************************************************
  Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,117), 8080);
  // Blynk.begin(auth, ssid, pass);
//**********************************************************************************
//****************************  UPDATE THIS SECTION!!!!! ***************************
//**********************************************************************************

  // timer settings (currently not used)
  UpdateClock.setInterval(1000L, GameTimer); // Sending game time updates to blynk app
  RequestScores.setInterval(100L, RequestingScores); // requesting player scores via bridge
}

void loop()
{
  Blynk.run();
  if (INGAME) {UpdateClock.run();}
  if (SCORESYNC) {RequestScores.run();}
}
