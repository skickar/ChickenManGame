      ____ _  _ _ ____ _  _ ____ _  _    _  _ ____ _  _    ____ ____ _  _ ____ 
      |    |__| | |    |_/  |___ |\ |    |\/| |__| |\ |    | __ |__| |\/| |___ 
      |___ |  | | |___ | \_ |___ | \|    |  | |  | | \|    |__] |  | |  | |___ 
                                                                         

                              ,~.
                           ,-'__ `-,
                          {,-'  `. }              ,')
                         ,( a )   `-.__         ,',')~,
                        <=.) (         `-.__,==' ' ' '}
                          (   )                      /)
                           `-'\   ,                    ) 
                               |  \        `~.        /
                               \   `._        \      / 
                                \     `._____,'    ,'
                                 `-.             ,'
                                    `-._     _,-'
                                        77jj'
                                       //_||
                                    __//--'/`          
                                  ,--'/`  '
                                  


The Chicken Man Wi-Fi hacking game is a WPA/WPA2 cracking game for CTF's and hackerspaces. It is designed to use cheap hardware to help beginners practice Wi-Fi hacking and requires no deauthing.

![Who will be the chicken man?](https://i.imgur.com/WOdqsh2.jpg "WHO WILL BE THE CHICKEN MAN")

The purpose of this project is to use ultra low-cost microcontrollers to create a Wi-Fi hacking game for beginners.
It is designed to be easy to set up, and requires no deauthing to practice. It has been tested on the D1 mini and NodeMCU.
 

                      /'._     _,                      /'._     _,
                      \   ;__.'  }                     \   ;__.'  }
                  (`-._;-" _.--.}'                (`-._;-" _.--.}'
                  /_'    /`    _}                /_'    /`    _}
                    `.   \_._.;                    `.   \_._.;
                      '-.__ /                       '-.__ /
                       _/  `\                        _/  `\
                      ^`   ^`                       ^`   ^`
## How Wi-Fi Hacking Works

There are several ways to get the password to a WPA2 Wi-Fi network. These include:
* Cracking the WPS Setup Pin
* Tricking the user into giving it to you
* Capturing a handshake and cracking it

We'll be focusing on the last method, known as WPA2 handshake cracking. To crack a Wi-Fi password, you must listen in on the traffic of a device connecting to the network you are targeting. We'll be using two ESP8266 microcontrollers to set this up. One ESP8266 will create a WPA2 encrypted Wi-Fi access point, and the second ESP8266 will join the access point with the correct password, generating a handshake that a hacker can capture.

Once we capture the handshake from one ESP8266 connecting to the other, we can crack it with a good password list and a program like Aircrack-ng, Pyrit, or Hashcat.

The process looks like this:
1) ESP8266 A creates a Wi-Fi network with password "password123456"
2) ESP8266 B joins the Wi-Fi network over and over, creating handshakes for the hacker to capture
3) The hacker captures a handshake while listening in with a tool like airodump-ng and saves the .cap file
4) The hacker runs aircrack-ng with a wordlist containing "password123456", and cracks the network password


## How The Game Works
To play the game, you need two game pieces: The Chicken Man, and one (or several) Chickens.

CHICKENS:
Each Chicken creates a WPA2 encrypted Wi-Fi network containing the word "Chicken" and the level of password difficulty. "Chicken_Easy_01" would be set with a very weak, easy to guess password, while "Chicken_Hard_09" would be set with one that is more difficult. The goal of the hacker is to join the Chicken's Wi-Fi network, go to the web portal at 192.168.4.1, and set the LED the color of their team (red, green, or blue)

CHICKEN MAN:
The Chicken Man knows the password to every possible chicken that can be created, and will join the Wi-Fi network of any Chicken automatically. A hacker can listen in when the Chicken Man joins the network, save the handshake, and try to crack it to gain access to the Wi-Fi network. If the hacker succeeds and sets the Chicken's LED to their team color, the Chicken man saves the number of points awarded from each captured Chicken and displays the winning team's color.

                          ,.
                         (\(\)
         ,_              ;  o >
          {`-.          /  (_) 
          `={\`-._____/`   |
           `-{ /    -=`\   |
            `={  -= = _/   /
               `\  .-'   /`
                {`-,__.'===,_
                //`        `\\
               //
              `\=
              
To win: 
* Follow the Chicken Man as he visits his Chickens, and save the captured handshake
* Crack the captured handshake with a password list
* Join the Wi-Fi network of the Chicken and navigate to the web interface
* Click on your team color to set your team's flag 
* Every second your flag is set on a Chicken, your team gets points
* The higher the difficulty of the Chicken's password, the more points you get when you capture it

## Required Hardware

This game requires two elements to play:
* Two ESP8266 (reccomended D1 mini or NodeMCU) to serve as the game piece and create the access point, with an RGB LED or three single LED's.
* A Raspberry Pi or other Linux computer with a wireless network adapter capible of packet capture. Players join this computer over SSH to play the game.


## How to play

                                    .=""=.
                                   / _  _ \
                                  |  d  b  |
                                  \   /\   /             ,
                                 ,/'-=\/=-'\,    |\   /\/ \/|   ,_
                                / /        \ \   ; \/`     '; , \_',
                               | /          \ |   \        / 
                               \/ \        / \/    '.    .'    /`.
                                   '.    .'          `~~` , /\ `"`
                                   _|`~~`|_              .  `"
                                   /|\  /|\


* To play, open Wireshark or Airodump-ng and search for the channel number that the Chicken has created it's Wi-Fi network on.
* Begin capturing Wi-Fi traffic on that channel, apply filters to eliminate other traffic, and wait for the 4-way handshake to be captured from the Chicken Man joining the Chicken's network. 
* Save the captured handshake as a .CAP file, run aircrack-ng or another cracking program against the a password list to brute-force the password to the access point.
* Try accessing the Wi-Fi access point of the Chicken with the cracked password. If it works, scan the network to find the IP address of the Chicken's webserver (should be 192.168.4.1).
* Open the Chicken's webserver in a browser, and (if you are the red team) click the "Red Team" button to turn the LED of the Chicken to your team's color.

![Who will be the chicken man?](https://i.imgur.com/CWArhK1.jpg "Set your team color!")

* The Chicken will set the LED to your team color and shut down the access point. The Chicken will start up again with a new password with higher difficulty, allowing a player from another team to attempt to crack the new password and set the LED flag to their own color.
* For best results, use several game pieces to keep the game interesting.

              .".".".
            (`       `)               _.-=-.
             '._.--.-;             .-`  -'  '.
            .-'`.o )  \           /  .-_.--'  `\
           `;---) \    ;         /  / ;' _-_.-' `
             `;"`  ;    \        ; .  .'   _-' \
              (    )    |        |  / .-.-'    -`
               '-.-'     \       | .' ` '.-'-\`
                /_./\_.|\_\      ;  ' .'-'.-.
                /         '-._    \` /  _;-,
               |         .-=-.;-._ \  -'-,
               \        /      `";`-`,-"`)
                \       \     '-- `\.\
                 '.      '._ '-- '--'/
                   `-._     `'----'`;
                       `"""--.____,/
                              \\  \
                              // /`
                          ___// /__
                        (`(`(---"-`)

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






