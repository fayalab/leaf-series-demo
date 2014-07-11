/*
* fayalab leaf sample code for leaf-encoder and leaf-Speaker
*
* Two mode for encoder with push button 
* 1:Control Speaker frequency
* 2:Play the sample melody 
* 
*
* 2014/07/11 by Welly Lee 
* wellylee@fayalab.com
*/

// define the pair with musical scale and frequency
#define NOTE_    0
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

//First, define the pin depends on your connection
//define pin for fayalab leaf-encoder
const int pin_Push = 2;  
const int pin_A = 3;  
const int pin_B = 4;  

//define pin for fayalab leaf-speaker
const int pin_Speaker = 9;  

unsigned char push_mode;

unsigned char status_A;
unsigned char status_B;
unsigned char status_A_last=0;

//variables for encoder 
unsigned long currentTime;
unsigned long loopTime;
int switch_counter;
int tone_frequency = 10;

// declare melody for play function
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// declare the noteDurations for play function
int noteDurations[] = {
  8, 16, 16, 8, 8, 8, 8, 8
};

// declare the play function to play melody by "Pin"
void play(int Pin, int *melody, int *noteDurations, int num){
  for(int note = 0; note < num; note++){
    int noteDuration = 3000 / noteDurations[note];
    tone(Pin, melody[note], noteDuration);
    delay(noteDuration * 1.30);
  }
}

void setup()  {
  Serial.begin (9600);
  pinMode(pin_Speaker, OUTPUT);
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
    status_A = digitalRead(pin_A);    
    status_B = digitalRead(pin_B);  
    if(push_mode==1){
      play(pin_Speaker,melody, noteDurations, sizeof(melody) / sizeof(int));
    }
    else{
      // Begin to check the encoder state(CW/Counter CW)
      if((!status_A) && (status_A_last)){
        // A has gone from high to low 
	if(status_B) {
          // B is high so clockwise
          if(tone_frequency <= 1000) tone_frequency += 10;
        }
        else {
          // B is low so counter-clockwise
          if(tone_frequency - 100 >= 0) tone_frequency -= 10;               
	}
      }
      status_A_last = status_A;     // Store value of A for next time
      Serial.println (tone_frequency);
      // set the tone_frequency of pin_Speaker:
      for(int i=0; i<50; i++)
      {
        digitalWrite(pin_Speaker,HIGH);
        delayMicroseconds(tone_frequency);
        digitalWrite(pin_Speaker,LOW);
        delayMicroseconds(tone_frequency);
      }
    }
    loopTime = currentTime;  // Updates loopTime
  }                         
}
