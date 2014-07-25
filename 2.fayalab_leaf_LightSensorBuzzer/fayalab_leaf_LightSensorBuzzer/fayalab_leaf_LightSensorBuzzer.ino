/*
* fayalab leaf sample code for leaf-Light sensor and leaf-Buzzer
* 
* Control the Buzzer active frequency by the light sensor value
* 
* 2014/07/11 by Welly Lee 
* wellylee@fayalab.com
*/

// First, define the pin depends on your connection
// define pin for fayalab leaf-Light and leaf-Buzzer
int pin_Lightsensor = A0;   

// select the pin for the LED
int pin_Buzzer = 13;      
// variable to store the value coming from the sensor
int sensorValue = 0;  
// frequency of buzzer active 
int frequency = 0;

void setup() {
  Serial.begin(9600);
  // declare the pin_Buzzer as an OUTPUT:
  pinMode(pin_Buzzer, OUTPUT);
}

void loop() {
  // read the value from the sensor:
  sensorValue = analogRead(pin_Lightsensor);
  //Serial.println(sensorValue);  
  //Active the buzzse 
  digitalWrite(pin_Buzzer, HIGH);  
  //Calculate the frequency of buzzer
  //Here are 2 ways to calculate the frequency, you can try both and see what happen! 
  //1.Use sqrt function
  //frequency = 1023-((sqrt(1023-sensorValue)*70));
  //2.Use map function(recommend)
  frequency = map(sensorValue, 0, 1023, 0, 1500);
  Serial.print("Sensor value: ");
  Serial.println(sensorValue); 
  Serial.print("Frequency : ");
  Serial.println(frequency);  
  if (frequency<=0)
    frequency = 1;
  delay(frequency);  
  // turn off the buzzer        
  digitalWrite(pin_Buzzer, LOW);   
  delay(frequency);                    
}
