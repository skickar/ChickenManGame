/* Script to play animations on attached three color LEDs
 *  Contains functions for a "blinkWhite" animation to blink all colors at the same time
 *  BlinkLED function contains variables for which LED to blink and how many times
 *  Goal is to trigger these when the AP is connected to by a new device
 *  Will need to whitelist the MAC of the device that's hopping between devices
 *  Any other device connecting causes a "connected" (white) LED flashing pattern to occur
 *  After that, the team connected can set their team color to claim the device
 *  Then, they can do something else to lock the game piece to their team and fire the laser
 *  
*/

#include <ESP8266WiFi.h>


int ledBlue = D1;
int ledRed = D2;
int ledGreen = D3;
int laserPin = D8; // This one fires the laser
int delayTime = 20; // Adjust the time of delay
int highValue = 90; // Defines the highest brightness for analog values (makes sure the LED's don't get too bringt)
int level = 0;

const char* easyPass[] PROGMEM = {
"password"
"123456789"
"12345678"
"1q2w3e4r"
"sunshine"
"football"
"1234567890"
"computer"
"superman"
"internet"
"iloveyou"
"1qaz2wsx"
"baseball"
"whatever"
"princess"};

const char* numberAdd[] PROGMEM = {
"One"
"Two"
"Three"
"Four"
"Five"
"Six"
"Seven"
"Eight"
"Nine"
"Ten"
"Eleven"
"Twelve"
"Thirteen"
"Fourteen"
"Fifteen"};


void createBird(){
int randNumber = random(0, 14);
char ChickenNumber = char(randNumber);
const char* password = easyPass[randNumber];
const char* ssid = numberAdd[randNumber];
 switch(level){
  case 0:
 Serial.print("Setting soft-AP with ");
  Serial.print(ChickenNumber);
 Serial.println(WiFi.softAP('ChickenEasy'+ssid, password) ? "Ready" : "Failed!");
    // statements
    break;
  case 1:
    // statements
    break;
  case 2:
    // statements
    break;    
  default:
    // statements
    break;
}
}


void blinkWhite(int numbBlinks){
  // This function blinks all of the LED's in the RGB LED array
  for (int i = 0; i < numbBlinks; i ++){
  analogWrite(ledRed, highValue),analogWrite(ledBlue, highValue),analogWrite(ledGreen, highValue);
  digitalWrite(laserPin, HIGH);  // Turn the LED off by making the voltage HIGH
  delay(delayTime);
  analogWrite(ledRed, 0),analogWrite(ledBlue, 0),analogWrite(ledGreen, 0);
  digitalWrite(laserPin, LOW);  // Turn the LED off by making the voltage HIGH
  delay(delayTime);}
}
// This blinks the LED according to which LED you specify and how many blinks
void blinkLed(int currentLed, int numbBlinks){
  for (int i = 0; i < numbBlinks; i ++){
  analogWrite(currentLed, highValue);
  delay(delayTime);
  analogWrite(currentLed, 0);
  delay(delayTime);}
}
// This pulses the laser for zero to full intensity. 
void pulseLaser(){
delay(1000);
for (int i = 0; i < 255; i ++){
  analogWrite(laserPin, i);
  delay(5);}
   analogWrite(laserPin, 0);
delay(1000);
}

void setup() {
  // Set all LED pins to output
  pinMode(ledRed, OUTPUT);
  pinMode(ledBlue, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(laserPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output to turn it the hell off
  digitalWrite(LED_BUILTIN, HIGH);  // onboard LED OFF get the heck out
  randomSeed(analogRead(0));
  createBird();
  }

void loop() { // Testing here
blinkWhite(10);
blinkLed(ledRed, 10);
blinkLed(ledBlue, 10);
blinkLed(ledGreen, 10);
pulseLaser();
}
