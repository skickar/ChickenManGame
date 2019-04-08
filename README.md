The Chicken Man Game
======
A Wi-Fi hacking game for CTF's and hackerspaces to teach cracking WPA/WAP2 - Who will be the Chicken Man?
The purpose of this game is to have an ultra low-cost microcontroller based Wi-Fi hacking game for beginners.
It is designed to be easy to set up, and requires no deauthing to practice.

## Behavior

The Chicken Man game consists of two elements, a game piece and a helper. 

1. The role of the game piece is to create a WPA Wi-Fi access point with a weak password, and host a web interface for players to access. The web interface allows the player to set the LED of the game piece to the color of their team.
2. The role of the helper is to join the access point created by the game piece. The helper knows the password to the Wi-Fi network the game piece creates, and while connecting to each game piece, will generate WPA handshakes for the player to crack.




