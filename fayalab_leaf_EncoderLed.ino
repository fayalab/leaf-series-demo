/*
* fayalab leaf sample code for leaf-encoder and leaf-LED
*
* Three mode for encoder 
* 1:control brightness 
* 2:LED blinking with R/G/B 
* 3:turn off the LED 
*
* 2014/07/11 by Welly Lee 
* wellylee@fayalab.com
*/

//First, define the pin depends on your connection
//define pin for fayalab leaf-encoder
const int pin_Push = 2;  
const int pin_A = 3;  
const int pin_B = 4;  
//define pin for fayalab leaf-LED
const int pin_LED_R = 9;  
const int pin_LED_G = 10;  
const int pin_LED_B = 11;

//store the mode of this system
unsigned char push_mode;

unsigned char status_A;
unsigned char status_B;
unsigned char status_A_last=0;

//initial the brightness and how fast the brightness change
int brightness = 100;
int fadelevel = 5;    
unsigned long currentTime;
unsigned long loopTime;

int switch_counter;

void setup()  {
  //declare a serial debug interface, you can use "Serial Monitor" to find the secret behind the code !
  Serial.begin (9600);
  //declare the OUTPUT devices
  pinMode(pin_LED_R, OUTPUT);
  pinMode(pin_LED_G, OUTPUT);
  pinMode(pin_LED_B, OUTPUT);
  
  //for our mode switch, we need to use interrupt here!
  //the pin "0" is mapping to UNO digital pin 2, you can find more information here : http://arduino.cc/en/Reference/attachInterrupt
  attachInterrupt(0, stateChange, CHANGE);
  //declare the INPUT devices
  pinMode(pin_A, INPUT);
  pinMode(pin_B, INPUT);
  
  currentTime = millis();
  loopTime = currentTime; 
  push_mode = 0;
  switch_counter = 0;
} 

//trigger by attachInterrupt
void stateChange(){
  //when you push the encoder controller, the status will change to 0
  //it back to 1 when you release the push controller, so we response only when states 1 > 0 > 1
  //(a mode change will trigger stateChange twice)
  if(switch_counter==1){
    Serial.println ("StateChange!!");  
    delay(500);
    if(push_mode==2){
      push_mode = 0;
    }
    else{
      push_mode++;
    }
    Serial.print ("Switch to mode : ");
    Serial.println (push_mode);
    switch_counter = 0;
  }else{
    switch_counter++;
  }
}
void loop()  {
  // get the current elapsed time
  currentTime = millis();
  if(currentTime >= (loopTime + 5)){
    // 5ms since last check of encoder = 200Hz  
    status_A = digitalRead(pin_A);    // Read encoder pins
    status_B = digitalRead(pin_B);  
    //mode 2 : close the LED
    if(push_mode==2){
      digitalWrite(pin_LED_R, LOW);
      digitalWrite(pin_LED_G, LOW);
      digitalWrite(pin_LED_B, LOW);
    }
    else if(push_mode==1){ //mode 1 : blinking with R/G/B
      digitalWrite(pin_LED_R, HIGH);
      delay(500);
      digitalWrite(pin_LED_R, LOW);
      digitalWrite(pin_LED_G, HIGH);
      delay(500);
      digitalWrite(pin_LED_G, LOW);
      digitalWrite(pin_LED_B, HIGH);
      delay(500);
      digitalWrite(pin_LED_R, LOW);
      digitalWrite(pin_LED_G, LOW);
      digitalWrite(pin_LED_B, LOW);
    }
    else{ //change the brightness of LED
      // Begin to check the encoder state(CW/Counter CW)
      if((!status_A) && (status_A_last)){
        // A has gone from high to low 
        if(status_B) {
          // B is high so clockwise
          if(brightness + fadelevel <= 255) brightness += fadelevel;
        }   
        else {
          // B is low so counter-clockwise
          if(brightness - fadelevel >= 0) brightness -= fadelevel;
        }
      }
      // Store value of A for next time
      status_A_last = status_A;     
      // set the brightness of pin 9:
      analogWrite(pin_LED_R, brightness);
      analogWrite(pin_LED_G, brightness);
      analogWrite(pin_LED_B, brightness);
    }
    loopTime = currentTime;  // Updates loopTime
  }                       
}
