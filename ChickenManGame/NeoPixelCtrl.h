#ifndef CHICKEN_PIXEL
#define CHICKEN_PIXEL

#include <Adafruit_NeoPixel.h>
#include "config.h"

// https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library-use

Adafruit_NeoPixel _CHICKEN_STRIP(NEOPIXEL_LED_COUNT,NEOPIXEL_LED_PIN,NEO_RGB+NEO_KHZ800);

class chicken_neopixel {
	public:
		chicken_neopixel(){
			if (setup_done)
				return;
			setup_done = true;
			_CHICKEN_STRIP.begin();
			last_red = NEOPIXEL_LED_COUNT/2;
		}

		// just shove in the points of each team here
		void red_blue_score(int red, int blue){
			unsigned int currentTime = millis();
			if ((currentTime - lastRedBlueUpdate)<100) return;
			lastRedBlueUpdate = currentTime;


			// figure out how many red lights should be on
			int num_red = NEOPIXEL_LED_COUNT/2;
			if(red+blue)
				num_red = (red*NEOPIXEL_LED_COUNT)/(red+blue);
			if(num_red!=last_red)
				 last_red += (num_red > last_red ? 1 : -1);

			//turn em on
			_CHICKEN_STRIP.fill(_CHICKEN_STRIP.Color(0,255,0), 0, last_red);
			_CHICKEN_STRIP.fill(_CHICKEN_STRIP.Color(0,0,255), last_red);
			_CHICKEN_STRIP.show();
		}

		// do some cool animation for winning team
		void win_sequence(int red, int blue){
			if(red==blue){
				meteorRain(255,255,255,10, 64, true, 30);
				while (true) RunningLights(0xff,0xff,0xff,50);
			}
			else{
				meteorRain(0,red>blue?255:0,red>blue?0:255,10, 64, true, 30);
				while(true) RunningLights(0,red>blue?255:0,red>blue?0:255,50);
			}
		}

	private:
		int last_red;
		unsigned long lastRedBlueUpdate;
		static bool setup_done;

		void meteorRain(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay) {
		  _CHICKEN_STRIP.clear();

		  for(int i = 0; i < NEOPIXEL_LED_COUNT+NEOPIXEL_LED_COUNT; i++) {


			// fade brightness all LEDs one step
			for(int j=0; j<NEOPIXEL_LED_COUNT; j++) {
			  if( (!meteorRandomDecay) || (random(10)>5) ) {
				fadeToBlack(j, meteorTrailDecay );
			  }
			}

			// draw meteor
			for(int j = 0; j < meteorSize; j++) {
			  if( ( i-j <NEOPIXEL_LED_COUNT) && (i-j>=0) ) {
				_CHICKEN_STRIP.setPixelColor(i-j, red, green, blue);
			  }
			}

			_CHICKEN_STRIP.show();
			delay(SpeedDelay);
		  }
		}
		void fadeToBlack(int ledNo, byte fadeValue) {
		 #ifdef ADAFRUIT_NEOPIXEL_H
			// NeoPixel
			uint32_t oldColor;
			uint8_t r, g, b;
			int value;

			oldColor = _CHICKEN_STRIP.getPixelColor(ledNo);
			r = (oldColor & 0x00ff0000UL) >> 16;
			g = (oldColor & 0x0000ff00UL) >> 8;
			b = (oldColor & 0x000000ffUL);
			r=(r<=10)? 0 : (int) r-(r*fadeValue/256);
			g=(g<=10)? 0 : (int) g-(g*fadeValue/256);
			b=(b<=10)? 0 : (int) b-(b*fadeValue/256);

			_CHICKEN_STRIP.setPixelColor(ledNo, r,g,b);
		 #endif
		 #ifndef ADAFRUIT_NEOPIXEL_H
		   // FastLED
		   leds[ledNo].fadeToBlackBy( fadeValue );
		 #endif
		}
		void RunningLights(byte red, byte green, byte blue, int WaveDelay) {
		  int Position=0;

		  for(int j=0; j<NEOPIXEL_LED_COUNT*2; j++)
		  {
			  Position++; // = 0; //Position + Rate;
			  for(int i=0; i<NEOPIXEL_LED_COUNT; i++) {
				// sine wave, 3 offset waves make a rainbow!
				//float level = sin(i+Position) * 127 + 128;
				//setPixel(i,level,0,0);
				//float level = sin(i+Position) * 127 + 128;
				_CHICKEN_STRIP.setPixelColor(i,((sin(i+Position) * 127 + 128)/255)*red,((sin(i+Position) * 127 + 128)/255)*green,((sin(i+Position) * 127 + 128)/255)*blue);
			  }

			  _CHICKEN_STRIP.show();
			  delay(WaveDelay);
		  }
		}

};

bool chicken_neopixel::setup_done = false;


#endif

