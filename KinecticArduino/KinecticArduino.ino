#include <Adafruit_WS2801.h>
#include "SPI.h" // Comment out this line if using Trinket or Gemma
#ifdef __AVR_ATtiny85__
 #include <avr/power.h>
#endif

/*

KinecticArduino

By 
Bret Stateham
Bret.Stateham@microsoft.com

Allows the Arduino to be controlled by a PC running the Kinectic Application

This version of the code assumes the Arduino is connected to a Sain-Smart 16 Channel Relay Module
These boards default to having the relays ON (closed) when it is supplied w 12v.  

To get a relay to open (turn off) the arduino needs to set the pin associated with that realy to HIGH.

So:

Arduino Pin State | Relay State
----------------- | -----------
HIGH              | OFF (Open)
LOW               | ON (Closed)

That is inversed from what you might expect.  For that reason, and to keep the code readable, 
I am defining two constants, RELAYON and RELAYOFF that will map to the appropriate PIN states

Protocol:

The communications protocol is pretty simple. The Arduino listens for four possible commands

l = turn light off
L = turn light ON
b = turn bell off
B = turn bell ON
A = Alert (Fire one sequence of bell rings) 

If it receives a command it recognizes, it responsds with that same command

*/

uint8_t dataPin  = 2;    // Yellow wire on Adafruit Pixels
uint8_t clockPin = 3;    // Green wire on Adafruit Pixels

// Set the first variable to the NUMBER of pixels. 20 = 20 pixels in a row
Adafruit_WS2801 strip = Adafruit_WS2801(20, dataPin, clockPin);

#define RELAYON  0  //Arduino pin needs to be LOW (0) to turn the relay on
#define RELAYOFF 1  //Arduino pin needs to be HIGH (1) to turn the relay off

// Define which LED pins are connected to the bel and siren light
const int bellPin = 12;  
const int lightPin = 11; 

// Track the light pin states
bool lightPinState = RELAYOFF;
bool bellPinState = RELAYOFF;


void setup() {
  //initiate serial communication with the host
  Serial.begin(9600);
  
  //Initialize the LED Pins as outputs:
  pinMode(lightPin,OUTPUT);
  pinMode(bellPin,OUTPUT);

  InitLedStrip();

  writePins();

}

void InitLedStrip(){
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
    clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
  #endif

  strip.begin();

  // Update LED contents, to start they are all 'off'
  strip.show();
}

void writePins(){
  digitalWrite(lightPin,lightPinState);
  digitalWrite(bellPin,bellPinState);
}

void Alert(int times){
  int ringsPerTime = 3;
  for(int t = 0; t < times; t++){
    for(int r = 0; r < ringsPerTime; r++){
      digitalWrite(bellPin,RELAYON);
      delay(100);
      digitalWrite(bellPin,RELAYOFF);
      delay(250);
    }
    if(times > 0){
      delay(250);
    }
  }
}

void loop() {
  //if there is serial data in the buffer read a byte
  if(Serial.available() > 0){
    int inByte = Serial.read();
    //respond to the values 'l','L','b','B'
    //We don't care about any other values
    switch(inByte)
    {
      case '0':
        Serial.println('0');
        lightPinState = RELAYOFF;
        bellPinState = RELAYOFF;
        colorWipe(Color(0,0,0),2);
        break;
      case 'l':
        Serial.println('l');
        lightPinState = RELAYOFF;
        break;
      case 'L':
        Serial.println('L');
        lightPinState = RELAYON;
        break;
      case 'b':
        Serial.println('b');
        bellPinState = RELAYOFF;
        break;
      case 'B':
        Serial.println('B');
        bellPinState = RELAYON;
        break;
      case 's':
        Serial.println('s');
        colorWipe(Color(0,0,0),0);
        break;
      case 'S':
        Serial.println('S');
        colorWipe(Color(255,0,0),10);
        colorWipe(Color(0,255,0),10);
        colorWipe(Color(0,0,255),10);
        colorWipe(Color(255,255,255),10);
        break;
      case 'A':
        Serial.println('A');
        Alert(3);
        break;
    }
    writePins();
  }
}

// Methods used by the Adafruit LED Strip:
void rainbow(uint8_t wait) {
  int i, j;
   
  for (j=0; j < 256; j++) {     // 3 cycles of all 256 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel( (i + j) % 255));
    }  
    strip.show();   // write all the pixels out
    delay(wait);
  }
}

// Slightly different, this one makes the rainbow wheel equally distributed 
// along the chain
void rainbowCycle(uint8_t wait) {
  int i, j;
  
  for (j=0; j < 256 * 5; j++) {     // 5 cycles of all 25 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      // tricky math! we use each pixel as a fraction of the full 96-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 96 is to make the wheel cycle around
      strip.setPixelColor(i, Wheel( ((i * 256 / strip.numPixels()) + j) % 256) );
    }  
    strip.show();   // write all the pixels out
    delay(wait);
  }
}

// fill the dots one after the other with said color
// good for testing purposes
void colorWipe(uint32_t c, uint8_t wait) {
  int i;
  
  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

/* Helper functions */

// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

//Input a value 0 to 255 to get a color value.
//The colours are a transition r - g -b - back to r
uint32_t Wheel(byte WheelPos)
{
  if (WheelPos < 85) {
   return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
   WheelPos -= 85;
   return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170; 
   return Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
