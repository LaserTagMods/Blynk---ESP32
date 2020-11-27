# Blynk---ESP32
Blynk Esp32 Configurator
//***************************************************
//************   General      Stuff   ***************
//***************************************************

install esp32 boards to your arduino IDE:
https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/

install esp8266 boards to your arduino IDE:
https://arduino-esp8266.readthedocs.io/en/latest/installing.html

Tip: when installing the JSON resources, you can return in the JSON library references to keep both the 8266, esp32 boards always available from the dropdown board selections

My boards Purchased:
https://www.aliexpress.com/item/32827149250.html?spm=a2g0s.9042311.0.0.6f5d4c4drgvrDP
https://www.aliexpress.com/item/32651747570.html?spm=a2g0s.9042311.0.0.6f5d4c4drgvrDP

Pi purchased/used:
https://www.amazon.ae/CanaKit-Raspberry-Starter-Premium-Black/dp/B07BCC8PK7

//***************************************************
//******** Step 1. Setting up Blynk App *************
//***************************************************
1. I recommend you using a blynk server on your pc or a raspberry pi
   this will make it so you dont have to ever buy energy credits from blynk
   it also is not dependent upon an internet connection
2. go to the github and open up the only jpeg image in the repository main folder: https://github.com/LaserTagMods/Blynk---ESP32/tree/master/Laser%20Tag%20Configurator%20with%20LCD
3. download and install blynk to your tablet/phone of choice
4. create account / login
5. determine if you will be using a local server and get that set up... see next section., otherwise proceed to next step
6. Once you are in blynk and ready to start, you can add a new project by scanning a qr in the upper corner by pressing the qr scann symbol. scan the jpeg and it will automatically upload the app into your server


//***************************************************
//*********   2.  Installing Blynk Server   *********
//***************************************************

https://github.com/blynkkk/blynk-server

0. remove old java stuff! (uninstall any existing java versions)
1. Install java environment
2. verify our ip address
3. run the blynk server!!!
4. setting up designated or static ip addresses with router
5. initial login on blynk server
6. building the app



1. install java environment:
 go to https://github.com/blynkkk/blynk-server#blynk-server
 verify if i have java already! java -version, this is done in command prompt
 download version 8 or 11 of java and install
 check version again, if not right, go to settings and fix the path!
 if you have java already, uninstall the old version and make sure you fix the path... the bin file folder in the java program folders

2. Verifiy Ip Address
	open command prompt
	then type ipconfig hit enter
	identify the network, wired or wireless
		look for ipv4 address... should be something like: 192.168.1.80

3. run blynk server!
	go to web and download it or open my file im giving you! here is the link https://github.com/blynkkk/blynk-server/releases/download/v0.41.12/server-0.41.12-java8.jar
	run the jar file in jav application... through command prompt!
		first get command prompt to the right file folder
		i copied the blynk server file to simplify name for execution
		enter in command prompt:
			"java -jar blynk.jar -dataFolder /server_data"
		login email for blynk is admin@blynk.cc and password is admin
	**** I made it easier for you though, just double click the run blynk server application!

4. setting static ip addresses:
	this process may vary but im using an asus router and the process is all the same for all asus routers.
	connect the device to the wifi network
	use a pc to go to the admin settings of the router, follow the ip address for the "gateway" to router
	log in with admin priveledges, if you dont know them, reset router and use admin/admin as user and password or see the manual
	identify connected clients and enable manual ip address assignment in the LAN settings for each device.

5. Initial login to blynk server
	pull it together now... we need to do two things and in this order!!!!
		first connect to the router or access point.. then run the blynk server.. in that order!
	then you can open the blynk app and login to the server...

Alternatively, install on a raspberry Pi: 
https://github.com/blynkkk/blynk-server

reference raspberry pi install instructions, then set it so it launches automatically by following instructions labeled: 
"Enabling server auto restart on unix-like systems"


//***************************************************
//************2. Preping a Controller ***************
//***************************************************
Use ESP8266 mini or other is used for score tracking and game clock tracking as well
This is a non-essential device but if you want scoring reporting on the blynk app, it is essential


//***************************************************
//************ Preping a Configurator ***************
//***************************************************

Gen 2 and later:
Hardware: ESP32 D1 Mini & ESP8266 D1 Mini

Solder, connect, wire, etc. pins: 
16 to D4, 17 to D3, Vcc to Vcc, Gnd to Gnd

Board Manager:
ESP32: Use Wemos D1 Mini ESP32 board and change partition scheme to "Minimum Spiffs (large apps with OTA)"
ESP8266: Use Lolin(Wemos) D1 R2 and Mini

Before thinking about uploading the software, watch the following tips videos for the programs to make sure you set up the code properly:
ESP32 code: https://youtu.be/S4vJf7yOa5A
ESP 8266 code: https://youtu.be/wFU6d7hrfCg
Also make sure you do the blynk stuff first!!! you need your blynk authentication codes!!!
