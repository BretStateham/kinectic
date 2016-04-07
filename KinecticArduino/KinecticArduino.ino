/*

  Serial RGB LED Controller
  
  Controls an RGB LED whose R,G,B legs are connected to
  PWM pins 11,10,9 respectively 

*/

const int bellPin = 12;  
const int lightPin = 11; 

bool lightPinState = HIGH;
bool bellPinState = HIGH;


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

void loop() {
  //if there is serial data in the buffer read a byte
  if(Serial.available() > 0){
    int inByte = Serial.read();
    //respond to the values 'l','L','b','B'
    //We don't care about any other values
    if(inByte == 'l') {
      lightPinState = HIGH;
      Serial.println('l');
    }
    if(inByte == 'L') {
      lightPinState = LOW;
      Serial.println('L');
    }
    if(inByte == 'b') {
      bellPinState = HIGH;
      Serial.println('b');
    }
    if(inByte == 'B') {
      bellPinState = LOW;
      Serial.println('B');
    }

    writePins();
  }
}
