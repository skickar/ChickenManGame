#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "AP_functions.h"

HTTPClient http;
bool getPassFromSSID(String ssid, const char*& pass);

int totalScores[] = {0,0,0};

void setup(){
  Serial.begin(115200);
  pinMode(D1,OUTPUT);
  pinMode(D2,OUTPUT);
  pinMode(D3,OUTPUT);
}

void loop(){
  //scan for networks
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  int numNetworks = WiFi.scanNetworks();

  // iterate through networks, connecting to chickens to get point data
  // this will also generate handshakes
  for (int i = 0; i < numNetworks; ++i){
    String currentNetwork(WiFi.SSID(i));
    const char* password;
    if (getPassFromSSID(currentNetwork, password)){
      // join and get points
      Serial.print("Connecting to ");
      Serial.print(currentNetwork.c_str());
      Serial.print(" with password ");
      Serial.println(password);
      WiFi.begin(currentNetwork.c_str(), password);

      unsigned long startTime = millis();
      while(WiFi.status() != WL_CONNECTED){
        delay(10);
        unsigned long currentTime = millis();
        if(currentTime - startTime > 5000) return;
      }

      http.begin("http://192.168.4.1/points.html");
      int httpCode = http.GET();
      if (httpCode > 0){
        String payload = http.getString();
        handleIncomingScore(payload);
      }
      
    }
  }
  
}

void handleIncomingScore(String score){
  int max = 0;
  Serial.print("CURRENT TOTAL SCORE: ");
  for(int i = 0; i < 3; ++i){
    totalScores[i] += score.toInt();
    score = score.substring( score.indexOf(',')+1, score.length());
    if (totalScores[max] < totalScores[i])
      max = i;
    Serial.print(totalScores[i]);
    Serial.print(i==2 ? "\r\n" : ", ");
  }

  // set LED of current leading team (none if all scores zero)
  int LEDS[] = {D1,D2,D3};
  for(int i = 0; i < 3; ++i){
    digitalWrite(LEDS[i], LOW);
  }
  if(!digitalRead(LEDS[max]))
    digitalWrite(LEDS[max], HIGH);
}

bool getPassFromSSID(String ssid, const char*& pass){
    int difficulty = -1;
    String seed;

    if(ssid.length() == 15 && ssid.substring(0,12)=="Chicken_Easy"){
      difficulty = 0;
      seed = ssid.substring(13, 17);
    }
    else if(ssid.length() == 17 && ssid.substring(0,14)=="Chicken_Medium"){
      difficulty = 1;
      seed = ssid.substring(15, 19);
    }
    else if(ssid.length() == 15 && ssid.substring(0,12)=="Chicken_Hard"){
      difficulty = 2;
      seed = ssid.substring(13, 17);
    }
    else if(ssid.length() == 17 && ssid.substring(0,15)=="CHICKEN_COOKED_"){
      difficulty = 3;
      seed = '0';
    }

    // no valid match found
    if (difficulty == -1) return false;

    int specific = seed.toInt();
    pass = getPassword(difficulty, specific);
    return true;
}

