/*
* fayalab leaf sample code for leaf-RTC(1307), and display by fayalab-nugget-7seg
* 
* Let's build a cute clock !!!
*
* 2014/07/16 by Welly Lee
* It's inspiration and inherit form http://yehnan.blogspot.tw/2013/01/arduinortcds1307.html by yehnan.
*
* wellylee@fayalab.com
*/
//define libs for RTC DS1307
#include <Wire.h> //I2C transfer
#include <Time.h> //Can get time for DS1307
#include <DS1307RTC.h> //DS1307 library, reading the data in DS1307

//define for 7 seg
#define Numpin 9  // define the start pin for show number, need to link to "A" pin in the fayalab-nugget
#define Digitpin 10   // define the start pin for digits, need to link to "S1" pin in the fayalab-nugget

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
                
void PriNum(int number) //function for show numbers
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

void SetNum(int num,int digitnum) //function for set numbers
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
        
    for(int i=0;i<8;i++)
    {
      if (Num[num][i]==1)
        digitalWrite(pin,HIGH);
      pin--; 
  } 
}

void PinReset()
{
  for(int i=2;i<13;i++)
    digitalWrite(i,LOW);
  //  delay(1);

}                

// function to format string by Yehnan
void pf(const char *fmt, ... ){
  char tmp[128]; // resulting string limited to 128 chars
  va_list args;
  va_start (args, fmt );
  vsnprintf(tmp, 128, fmt, args);
  va_end (args);
  Serial.print(tmp);
}


// function to format string and output to fayalab-nugget 7 segment
void pf7seg(const char *fmt, ... ){
  char tmp[128]; // resulting string limited to 128 chars
  va_list args;
  va_start (args, fmt );
  vsnprintf(tmp, 128, fmt, args);
  va_end (args);
  String tmp2 = tmp;
  PriNum(tmp2.toInt());
  //Serial.print(tmp);
}


void setup()  {
  Serial.begin(9600);
  //1.fayalab-nugget setup
  // Use loop to declare fayalab-nugget pins 
  // You need to modify the start point and endpoint with your hardware wiring
  for(int pin=2;pin<13;pin++)  
    pinMode(pin,OUTPUT);
  PinReset();
  // 2.fayalab-leaf setup
  // Get the time by RTC.get define in DS1307RTC lib
  // This is only place that use DS1307 lib, we use Time lib for other function
  setSyncProvider(RTC.get);
  if(timeStatus() != timeSet)
    Serial.println("Unable to sync with the RTC");
  else
    Serial.println("RTC has set the system time");      
}

void loop()
{
  //Print time every 1 sec
  pf("%d/%d/%d %02d:%02d:%02d\n", year(), month(), day(), hour(), minute(), second()); 
  pf7seg("%02d%02d",hour(), minute());
}

// Setting the time with Serial
// 「y2013」-> set to year 2013
// 「M1」-> set month to Jan.
// 「d2」-> set day to Second
// 「h2」-> set hour to 2 o'clock
// 「m34」-> set the minute
// 「s56」-> set the seconds 
void serialEvent()
{
  int t = Serial.read();
  int v = 0;
  int temp;
  while((temp = Serial.read()) != -1){
    v = v * 10 + (temp - '0');
  }
  
  int y = year();
  int M = month();
  int d = day();
  int h = hour();
  int m = minute();
  int s = second();
  
  switch(t){
    case 'y':
      y = v;
      break;
    case 'M':
      M = v;
      break;
    case 'd':
      d = v;
      break;
    case 'h':
      h = v;
      break;
    case 'm':
      m = v;
      break;
    case 's':
      s = v;
      break;
    default:
      pf("Wrong\n");
      break;
  }
  setTime(h, m, s, d, M, y); 
  // Use the DS1307 lib to set time
  tmElements_t tm;
  tm.Year = y - 1970; 
  tm.Month = M;
  tm.Day = d;
  tm.Hour = h;
  tm.Minute = m;
  tm.Second = s;
  setTime(makeTime(tm));
  RTC.set(makeTime(tm)); 
}
