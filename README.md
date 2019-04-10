The Chicken Man Game
======
The Chicken Man Wi-Fi hacking game is a WPA/WPA2 cracking game for CTF's and hackerspaces.

![Who will be the chicken man?](https://i.imgur.com/WOdqsh2.jpg "WHO WILL BE THE CHICKEN MAN")

The purpose of this game is to have an ultra low-cost microcontroller based Wi-Fi hacking game for beginners.
It is designed to be easy to set up, and requires no deauthing to practice. It has been tested on the D1 mini and NodeMCU.
 
![Who will be the chicken man?](https://previews.123rf.com/images/lisafx/lisafx0911/lisafx091100075/5918355-chicken-man-holding-stolen-cash-and-running-from-a-police-officer-isolated-on-white-.jpg "WHO WILL BE THE CHICKEN MAN")

## Behavior

The Chicken Man game consists of two elements, a game piece and a helper. 

1. The role of the game piece is to create a WPA Wi-Fi access point with a weak password, and host a web interface for players to access. The web interface allows the player to set the LED of the game piece to the color of their team.
2. The role of the helper is to join the access point created by the game piece. The helper knows the password to the Wi-Fi network the game piece creates, and while connecting to each game piece, will generate WPA handshakes for the player to crack.

## Required Hardware

This game requires three elements to play:
* An ESP8266 (reccomended D1 mini or NodeMCU) to serve as the game piece and create the access point, with an RGB LED or three single LED's.
* An ESP8266 to join the network created by the game piece, generating WPA handshakes.
* A Raspberry Pi or other Linux computer with a wireless network adapter capible of packet capture. 


## How to play

* To play, open Wireshark or Airodump-ng and search for the channel the game piece has created an access point on.
* Begin capturing Wi-Fi traffic from the access point, apply filters to show when 4-way handshake is captured from the helper joining the game piece access point. 
* Save the captured handshake as a .CAP file, run aircrack-ng or another cracking program against the game password list to brute-force the password to the access point.
* Try accessing the Wi-Fi access point of the game piece with the cracked password. If it works, scan the network to find the IP address of the game piece webserver
* Open the game piece webserver in a browser, and (if you are the red team) type "set LED red" to turn the LED of the game piece to your team's color
* The game piece will set the LED to your team color and shut down the access point. The AP will start up again with a new password, allowing a player from another team to attempt to crack the new password and set the LED flag to their own color.
* For best results, use several game pieces to keep the game interesting.

## Project Goals
This project has several goals, broken down into behavior for each game element:

#### Game Piece Goals:
1. The game piece creates a soft access point with a password chosen from a short "game password list."
2. A 3 color RGB LED (or three LED's) is connected to the output pins, allowing us to flash red, green, blue, or white.
3. A web interface is started on the game piece, it hosts a web server on port 80 that allows the following commands:
* Set LED red
* Set LED green
* Set LED blue 
4. The LED is off. When a device joins the access point, an LED flashes white (looks more orange). 
5. When a device joins the soft AP with the correct password and sets the LED color, two things happen:
* The LED turns on, set to the color of the team who set the flag. It stays this color unless another team logs in and sets their flag.
* The game piece shuts down the soft AP, choooses a new password from the game password list, and starts the soft AP again with the new password.


#### Helper Goals:
1. The helper starts up and joins the soft AP of any nearby game piece. Doing so creates a WPA handshake for the player to capture.
2. To join the AP, the helper has a few behaviors. First, you can set the number of game pieces. Becaue the name of the access points is sequencial, (ChickenMan1, ChickenMan2, ChickenMan3), the helper will attempt to join all networks from "ChickenMan1" to the number of game pieces you set. If you set 3 game pieces, the helper will try to join "ChickenMan1, ChickenMan2, and ChickenMan3" access points.
3. The helper may not know which password from the "game password list" the game piece is using, espeically if a team has set the flag already and causes the device to create a soft AP with a new password. To fix this, we can ask the helper to do the following if the current password isn't right:
* When joining a game piece, try the first password in the password list. If that password fails, try the next password in the list (up to 15 passwords in the list).
* When the login succeeds, save the name of the network and the password together, and use that pair to join the game piece AP from now on. If another player logs in and the password is changed again, the process repeats itself when the helper finds the password has changed. 


When all is working correctly, the helper joins the AP of each game piece, saving the password associated with each game piece after trying each password stored in the game list. After the helper associates the right password with each network, it goes from network to network, connecting and disconnecting to generate WPA handshakes.

The game is designed to run for 45 minuties after an explanation and demonstration of how it works.






