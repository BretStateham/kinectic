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

  writePins();

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
      case 'A':
        Serial.println('A');
        Alert(3);
        break;
    }
    writePins();
  }

}
