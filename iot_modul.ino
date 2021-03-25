#include <Ethernet2.h>
//#define CAYENNE_DEBUG       // Uncomment to show debug messages
#define CAYENNE_PRINT Serial  // Comment this out to disable prints and save space
#include <CayenneMQTTEthernetW5500.h>

///////////pulse///////////////
int PulseSensorPurplePin = A1; // выход Ардуино А0  
int Signal;                 
int Threshold = 550; // значение для данных сенсора, после которого подаётся сигнал            
int key = 2;
int red = 3;
int green = 4;
/////////////ekg////////////////
const int lo_p = 5;
const int lo_m = 6;
#define ekg_output A0
const int buzzer = 7;
int maximum = 0;
int minimum = 1000; 
/////////////////caenne///////////////////////
char username[] = "e8baef70-3512-11ea-b301-fd142d6c1e6c";
char password[] = "67d52d6909897b46b31be06e97016c991b247ca6";
char clientID[] = "8ddfe8e0-8c86-11eb-8779-7d56e82df461";

void setup() {
	Serial.begin(9600);
	Cayenne.begin(username, password, clientID);
//////////////pulse/////////
    pinMode(key, INPUT);
    pinMode(red, OUTPUT);
    pinMode(green, OUTPUT);
////////ekg///////////////
    pinMode(lo_p, INPUT); // Setup for leads off detection LO +
    pinMode(lo_m, INPUT); // Setup for leads off detection LO -
}

void loop() {
	Cayenne.loop();
}

// Default function for sending sensor data at intervals to Cayenne.
// You can also use functions for specific channels, e.g CAYENNE_OUT(1) for sending channel 1 data.
CAYENNE_OUT_DEFAULT()
{
	// Write data to Cayenne here. This example just sends the current uptime in milliseconds on virtual channel 0.
	Cayenne.virtualWrite(0, millis());
 int j = 0;
  int i = 0; 
  int state = 0;
  int state_ekg = 0;
  int a_key = digitalRead(key);
  //Serial.println(a_key);
  if(a_key == 0){
    Serial.println('1');
    Serial.println("working!");
    state = 1;
    a_key = 1;
    delay(1000);
  }
  if(state == 1){
    Cayenne.luxWrite(2, 1);
    while(i < 2500){
    i++;
    digitalWrite(green, LOW);
    digitalWrite(red, HIGH);
    pulse();
    }
    state_ekg = 1;
    }else{
      Cayenne.luxWrite(2, 0);
      digitalWrite(green, HIGH);
      digitalWrite(red, LOW);
      }
Cayenne.luxWrite(2, 0);
digitalWrite(green, HIGH);
digitalWrite(red, LOW);

delay(1000);


if(state_ekg == 1){
  Cayenne.luxWrite(3, 1);
  Serial.println("EKG_READY!");
  delay(1000);
  pisk();
   while(j < 2500){
   ekg(); 
   
  j++;
    }
    Serial.print("MAXIMUM:");
    Serial.println(maximum+200);
    Serial.print("MINIMUM:");
    Serial.println(minimum-200);
  }else{
    Cayenne.luxWrite(3, 0);
    }
Cayenne.luxWrite(3, 0);
  
	//Cayenne.luxWrite(2, 700);
	//Cayenne.virtualWrite(3, 50, TYPE_PROXIMITY, UNIT_CENTIMETER);
}


// Default function for processing actuator commands from the Cayenne Dashboard.
// You can also use functions for specific channels, e.g CAYENNE_IN(1) for channel 1 commands.
CAYENNE_IN_DEFAULT()
{
	CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
	//Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
}

void pulse(){
  int i;
   i++;
   Signal = analogRead(PulseSensorPurplePin); // чтение данных с сенсора 
   Serial.println(Signal);
   Cayenne.luxWrite(5, Signal);
  }
  
void ekg(){
    if((digitalRead(lo_p) == 1)||(digitalRead(lo_m) == 1)){
    
    Serial.println('!');
  }
  else{
    // send the value of analog input 0:
      Serial.println(analogRead(ekg_output));
      Cayenne.luxWrite(4, ekg_output);
  }
  //Wait for a bit to keep serial data from saturating
  if(ekg_output > maximum){
    maximum = analogRead(ekg_output);
    }
  if(ekg_output < minimum){
    minimum = analogRead(ekg_output);
    }
  delay(1);
  }
void pisk(){
  tone(buzzer, 1024, 3000);
  delay(3000);
  noTone(buzzer);
  }
