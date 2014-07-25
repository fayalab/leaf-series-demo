/*
* fayalab leaf sample code for leaf-Hall sensor and a 5v solenoid
*
* It's a latch control by hall sensor
* You can use your magnet to open / lock it !
*
* 2014/07/17 by Welly Lee 
* wellylee@fayalab.com
*/

// set pin numbers:
const int hallPin = 3;
// define Door Latch pin:
int pin_Latch = 13;
// state of hall sensor will change:
int hallState = 0;

// functions area
void latch_open(){
  digitalWrite(pin_Latch, LOW);
}
void latch_lock(){
  digitalWrite(pin_Latch, HIGH);
};

void setup() {
  Serial.begin(9600);
  pinMode(hallPin, INPUT);
  pinMode(pin_Latch, OUTPUT);
  latch_open();
}

void loop(){
  hallState = digitalRead(hallPin);
  Serial.println(hallState);
  //When you take a magnet near hall sensor, the state will be "LOW"
  //Open the door~~
  if(hallState == LOW){
    Serial.println("hall State ON");
    latch_open();
  }
  //Close the door when you leave!
  if(hallState == HIGH){
    Serial.println("hall State OFF");
    latch_lock();
  }
}


