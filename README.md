The Chicken Man Game
======
A Wi-Fi hacking game for CTF's and hackerspaces to teach cracking WPA/WAP2 - Who will be the Chicken Man?
The purpose of this game is to have an ultra low-cost microcontroller based Wi-Fi hacking game for beginners.
It is designed to be easy to set up, and requires no deauthing to practice.

## Behavior

The Chicken Man game consists of two elements, a game piece and a helper. 

1. The role of the game piece is to create a WPA Wi-Fi access point with a weak password, and host a web interface for players to access. The web interface allows the player to set the LED of the game piece to the color of their team.
2. The role of the helper is to join the access point created by the game piece. The helper knows the password to the Wi-Fi network the game piece creates, and while connecting to each game piece, will generate WPA handshakes for the player to crack.

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
1. The helper starts up and joins the soft AP of any nearby game piece. 




