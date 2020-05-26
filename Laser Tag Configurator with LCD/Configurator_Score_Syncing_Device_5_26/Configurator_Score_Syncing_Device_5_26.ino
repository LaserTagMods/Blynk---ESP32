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
char auth[] = "S6xltBO40T-9nhFavpR2Zsn8WXLTCeJx";
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "maxipad";
char pass[] = "9165047812";
// set the bridge token
BLYNK_CONNECTED() {
  bridge1.setAuthToken("nngeMu8Nz6CAjzmPfFR89d31VBoSSRff"); // Token of the taggers
}
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

bool INGAME = false;



// timers for running certain applications periodically with the blynk program
BlynkTimer UpdateClock; // created a timer object called "UpdateClock"

//******************************************************************
// object used to receive data from ESP32 BLE device
BLYNK_WRITE(V0) {
    String readStr = param.asString(); // saves incoming data as a temporary string within this object
    char *ptr = strtok((char*)readStr.c_str(), ","); // looks for commas as breaks to split up the string
    int index = 0;
    while (ptr != NULL)
    {
      ScoreTokenStrings[index] = ptr; // saves the individual characters divided by commas as individual strings
      index++;
      ptr = strtok(NULL, ",");  // takes a list of delimiters
    }
    // create a string that looks like this: 
    // (Player ID, token 0), (Player Team, token 1), (Player Objective Score, token 2) (Team scores, tokens 3-8), (player kill counts, tokens 9-72 
    Serial.println("Score Data Recieved from a tagger");
    
      int Deaths=0;
      int Objectives=0;
      int Kills=0;
      int Team=0;
      int Player=0;
      int Data[64];
      int count=0;
      while (count<73) {
      Data[count]=ScoreTokenStrings[count].toInt();
      Serial.println("Converting String character "+String(count)+" to integer");
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
      TeamKills[3] = TeamKills[3] + Data[6];
      Serial.println("Team Kill Count Accumulation Complete");
      Serial.println("Red Team Kills: " + String(TeamKills[0]) + "Blue Team Kills: " + String(TeamKills[1]) + "Green Team Kills: " + String(TeamKills[2]) + "Yellow Team Kills: " + String(TeamKills[3]));
      // accumulating player kill counts
      int p = 9; // using for data characters 9-72
      int j = 0; // using for player id status counter
      Serial.println("Accumulating Player kills against current player...");
      while (p < 73) {
        PlayerKills[j] = PlayerKills[j] + Data[p];
        Serial.println("Player " + String(j) + "Killed this player " + String(Data[p]) + "times, Player's new score is " + String(PlayerKills[j]));
        p++;
        j++;
      }
      Team = Data[1]; // setting the player's team for accumulation of player objectives
      PlayerObjectives[Player] = Data[2]; // converted temporary data storage to main memory for score reporting of player objectives
      Serial.println("Player Objectives completed: "+String(Data[2]));
      TeamObjectives[Team] = TeamObjectives[Team] + Data[2]; // added this player's objective score to his team's objective score
      // send scores to blynk accumulator device over bridge to be summed and then posted to blynk server/app
      // designating certain pins for score accumulation. 
      // Pins 0-63 are for player kill counts, this sends all player scores
bridge1.virtualWrite(V40, PlayerKills[0]); // sending the value to controller
bridge1.virtualWrite(V41, PlayerKills[1]); // sending the value to controller
bridge1.virtualWrite(V42, PlayerKills[2]); // sending the value to controller
bridge1.virtualWrite(V43, PlayerKills[3]); // sending the value to controller
bridge1.virtualWrite(V44, PlayerKills[4]); // sending the value to controller
bridge1.virtualWrite(V45, PlayerKills[5]); // sending the value to controller
bridge1.virtualWrite(V46, PlayerKills[6]); // sending the value to controller
bridge1.virtualWrite(V47, PlayerKills[7]); // sending the value to controller
bridge1.virtualWrite(V48, PlayerKills[8]); // sending the value to controller
bridge1.virtualWrite(V49, PlayerKills[9]); // sending the value to controller
bridge1.virtualWrite(V50, PlayerKills[10]); // sending the value to controller
bridge1.virtualWrite(V51, PlayerKills[11]); // sending the value to controller
bridge1.virtualWrite(V52, PlayerKills[12]); // sending the value to controller
bridge1.virtualWrite(V53, PlayerKills[13]); // sending the value to controller
bridge1.virtualWrite(V54, PlayerKills[14]); // sending the value to controller
bridge1.virtualWrite(V55, PlayerKills[15]); // sending the value to controller
bridge1.virtualWrite(V56, PlayerKills[16]); // sending the value to controller
bridge1.virtualWrite(V57, PlayerKills[17]); // sending the value to controller
bridge1.virtualWrite(V58, PlayerKills[18]); // sending the value to controller
bridge1.virtualWrite(V59, PlayerKills[19]); // sending the value to controller
bridge1.virtualWrite(V60, PlayerKills[20]); // sending the value to controller
bridge1.virtualWrite(V61, PlayerKills[21]); // sending the value to controller
bridge1.virtualWrite(V62, PlayerKills[22]); // sending the value to controller
bridge1.virtualWrite(V63, PlayerKills[23]); // sending the value to controller
bridge1.virtualWrite(V64, PlayerKills[24]); // sending the value to controller
bridge1.virtualWrite(V65, PlayerKills[25]); // sending the value to controller
bridge1.virtualWrite(V66, PlayerKills[26]); // sending the value to controller
bridge1.virtualWrite(V67, PlayerKills[27]); // sending the value to controller
bridge1.virtualWrite(V68, PlayerKills[28]); // sending the value to controller
bridge1.virtualWrite(V69, PlayerKills[29]); // sending the value to controller
bridge1.virtualWrite(V70, PlayerKills[30]); // sending the value to controller
bridge1.virtualWrite(V71, PlayerKills[31]); // sending the value to controller
bridge1.virtualWrite(V72, PlayerKills[32]); // sending the value to controller
bridge1.virtualWrite(V73, PlayerKills[33]); // sending the value to controller
bridge1.virtualWrite(V74, PlayerKills[34]); // sending the value to controller
bridge1.virtualWrite(V75, PlayerKills[35]); // sending the value to controller
bridge1.virtualWrite(V76, PlayerKills[36]); // sending the value to controller
bridge1.virtualWrite(V77, PlayerKills[37]); // sending the value to controller
bridge1.virtualWrite(V78, PlayerKills[38]); // sending the value to controller
bridge1.virtualWrite(V79, PlayerKills[39]); // sending the value to controller
bridge1.virtualWrite(V80, PlayerKills[40]); // sending the value to controller
bridge1.virtualWrite(V81, PlayerKills[41]); // sending the value to controller
bridge1.virtualWrite(V82, PlayerKills[42]); // sending the value to controller
bridge1.virtualWrite(V83, PlayerKills[43]); // sending the value to controller
bridge1.virtualWrite(V84, PlayerKills[44]); // sending the value to controller
bridge1.virtualWrite(V85, PlayerKills[45]); // sending the value to controller
bridge1.virtualWrite(V86, PlayerKills[46]); // sending the value to controller
bridge1.virtualWrite(V87, PlayerKills[47]); // sending the value to controller
bridge1.virtualWrite(V88, PlayerKills[48]); // sending the value to controller
bridge1.virtualWrite(V89, PlayerKills[49]); // sending the value to controller
bridge1.virtualWrite(V90, PlayerKills[50]); // sending the value to controller
bridge1.virtualWrite(V91, PlayerKills[51]); // sending the value to controller
bridge1.virtualWrite(V92, PlayerKills[52]); // sending the value to controller
bridge1.virtualWrite(V93, PlayerKills[53]); // sending the value to controller
bridge1.virtualWrite(V94, PlayerKills[54]); // sending the value to controller
bridge1.virtualWrite(V95, PlayerKills[55]); // sending the value to controller
bridge1.virtualWrite(V96, PlayerKills[56]); // sending the value to controller
bridge1.virtualWrite(V97, PlayerKills[57]); // sending the value to controller
bridge1.virtualWrite(V98, PlayerKills[58]); // sending the value to controller
bridge1.virtualWrite(V99, PlayerKills[59]); // sending the value to controller
bridge1.virtualWrite(V100, PlayerKills[60]); // sending the value to controller
bridge1.virtualWrite(V101, PlayerKills[61]); // sending the value to controller
bridge1.virtualWrite(V102, PlayerKills[62]); // sending the value to controller
bridge1.virtualWrite(V103, PlayerKills[63]); // sending the value to controller
      // team kill scores (6 pins) are located on pins 64-69
        bridge1.virtualWrite(V104, TeamKills[0]); // sending the value to controller
        bridge1.virtualWrite(V105, TeamKills[0]); // sending the value to controller
        bridge1.virtualWrite(V106, TeamKills[0]); // sending the value to controller
        bridge1.virtualWrite(V107, TeamKills[0]); // sending the value to controller
      // now we need to send objective points for the teams. This will be on pins 70-75
      // we are only sending on one pin because were using the players objective points
      // to accumulate on the right teams pin.
        bridge1.virtualWrite(V108, TeamObjectives[0]); // sending the value to controller
        bridge1.virtualWrite(V109, TeamObjectives[0]); // sending the value to controller
        bridge1.virtualWrite(V110, TeamObjectives[0]); // sending the value to controller
        bridge1.virtualWrite(V111, TeamObjectives[0]); // sending the value to controller
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
      bridge1.virtualWrite(V112, KillsLeader[0]); // sending the index or playr id to controller
      bridge1.virtualWrite(V113, KillsLeader[1]); // sending the index or playr id to controller
      bridge1.virtualWrite(V114, KillsLeader[2]); // sending the index or playr id to controller
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
      bridge1.virtualWrite(V115, ObjectiveLeader[0]); // sending the index or playr id to controller
      bridge1.virtualWrite(V116, ObjectiveLeader[1]); // sending the index or playr id to controller
      bridge1.virtualWrite(V117, ObjectiveLeader[2]); // sending the index or playr id to controller
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
      bridge1.virtualWrite(V118, DeathsLeader[0]); // sending the index or playr id to controller
      bridge1.virtualWrite(V119, DeathsLeader[1]); // sending the index or playr id to controller
      bridge1.virtualWrite(V120, DeathsLeader[2]); // sending the index or playr id to controller
      // now get the player's scores back where they should be:
      PlayerDeaths[DeathsLeader[0]] = LeaderScore[0];
      PlayerDeaths[DeathsLeader[1]] = LeaderScore[1];
      PlayerDeaths[DeathsLeader[2]] = LeaderScore[2];
    }

void GameTimer() {
  
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
  // Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,80), 8080);
  Blynk.begin(auth, ssid, pass);
//**********************************************************************************
//****************************  UPDATE THIS SECTION!!!!! ***************************
//**********************************************************************************

  // timer settings (currently not used)
  UpdateClock.setInterval(1L, GameTimer); // Sending game time updates to blynk app
}

void loop()
{
  Blynk.run();
  if (INGAME) {UpdateClock.run();}
}
