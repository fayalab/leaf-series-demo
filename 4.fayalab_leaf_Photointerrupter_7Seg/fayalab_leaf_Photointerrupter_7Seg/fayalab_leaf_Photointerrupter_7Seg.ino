/*
* fayalab leaf sample code for leaf-photo interrupter, fayalab-nugget-7seg, and servo
*
* The 7 segment will display the count of the photo interrupter which active by the servo
* We ues the servo as a small annoying robot that test the photo interrupter for us !
*
* 2014/07/17 by Welly Lee 
* wellylee@fayalab.com
*/

//setup the requirement of servo motor
#include <Servo.h>
Servo myServo;

// declare pins
const int pin_Servo=0;
const int pin_Photointerrupter = 13;  

//the status of photointerrupter sensor
int StatusPI = 0;
int pre_StatusPI =1;

// the number that will display with fayalab-nugget
int counter = 0;
// store in/out status
int in_out = 0;
// the angle for servo
int angle = 0;
// the rotation direction of servo
boolean rotate_direction = 0;

//variable and functions for fayalab-nugget-7seg
#define Numpin 9  // define the start pin for show number, need to link to "A" pin in the fayalab-nugget
#define Digitpin 10  // define the start pin for digits, need to link to "S1" pin in the fayalab-nugget

int digit[4][3]={{0,0,0},  //units digit
                 {1,0,0},  //tens digit
                 {0,1,0},  //hundreds digit
                 {1,1,0}}; //thousands digit

int Num[10][8]={{1,1,1,1,1,1,0,0},  //0
                {0,1,1,0,0,0,0,0},  //1
                {1,1,0,1,1,0,1,0},  //2
                {1,1,1,1,0,0,1,0},  //3
                {0,1,1,0,0,1,1,0},  //4
                {1,0,1,1,0,1,1,0},  //5
                {1,0,1,1,1,1,1,0},  //6
                {1,1,1,0,0,0,0,0},  //7
                {1,1,1,1,1,1,1,0},  //8
                {1,1,1,1,0,1,1,0}};  //9

void PriNum(int number) //function for show number
{
  int temp;
  temp=number%10;
  SetNum(temp,0);
  delay(1);
  
  temp=(number/10)%10;
  SetNum(temp,1);
  delay(1);
  
  temp=(number/100)%10;
  SetNum(temp,2);
  delay(1);
   
  temp=number/1000;    
  SetNum(temp,3);
  delay(1);
}
                
//function for setting the number
void SetNum(int num,int digitnum) 
{
  PinReset();
  int pin=Numpin;
  int pin2=Digitpin;
  for(int j=0;j<3;j++)
  {
    if(digit[digitnum][j]==1)
      digitalWrite(pin2,HIGH);
    else
      digitalWrite(pin2,LOW);
    pin2++;
  }   
  for(int i=0;i<8;i++){
    if (Num[num][i]==1)
      digitalWrite(pin,HIGH);
    pin--; 
  } 
}

void PinReset()
{
  for(int i=2;i<13;i++)
    digitalWrite(i,LOW);
}    

void setup(){
  //setup servo
  myServo.attach(pin_Servo);
  pinMode(pin_Photointerrupter,INPUT);
  //use loop to declare fayalab-nugget pins 
  //you need to modify the start point and endpoint with your config
  for(int pin=2;pin<13;pin++)  
    pinMode(pin,OUTPUT);
  PinReset();
}

void loop(){
  //Change the direction when angle = 180
  if(angle >= 180){
    rotate_direction = 1;
  }else if(angle <=0){
    rotate_direction = 0;
  }
  if(rotate_direction)
    angle--;
  else
    angle++;
  myServo.write(angle);
  //determine how many times the servo arms wave.
  StatusPI = digitalRead(pin_Photointerrupter);
  if(StatusPI != pre_StatusPI){
    in_out++;
    if(in_out==2){
      //Serial.println(in_out);
      counter++;
      in_out = 0;
    }
  }
  //Print the number with 7 segment
  PriNum(counter);
  pre_StatusPI = StatusPI;
}


