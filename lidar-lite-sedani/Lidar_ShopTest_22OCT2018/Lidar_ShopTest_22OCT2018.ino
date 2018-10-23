#include <Wire.h>
#include <LIDARLite.h>

//Define pin array to be scanned for valid lidar scanners
int pinArray[] = {2,3,4,5,6,7,8,9};
int unitCounter = 0;

LIDARLite myLidarLite;

struct LIDAR
{
    int address;
    int pin;
};

LIDAR lidarUnits[8];      //Array of lidarUnits to store configuration data


void setup() 
{
  
  Serial.begin(9600);   //Start serial communications
  
  for(int i=0; i < sizeof(pinArray); i++) // Setup enable pins
  {
    pinMode(pinArray[i],OUTPUT);
    digitalWrite(pinArray[i],0);
  }

  delay(20);

  
  int defAddress = 0x62;    //default address
  
  //Sequentially Enable the enable pins to detect active Lidar sensors
  int len = sizeof(pinArray);
  for(int i=0; i < len; i++) 
  {
    digitalWrite(pinArray[i],1); //enable the enable pin
    delay(20);                   //Wait 20ms to enable comms to Lidar unit
        
    Wire.beginTransmission(defAddress);  //Attempt to communicate to device
    int error = Wire.endTransmission();      //Comm error
    if(error == 0)                       //Communications attempt successful
    {
       ++unitCounter;
       lidarUnits[i].pin = pinArray[i]; //Set pin number
       lidarUnits[i].address = defAddress + unitCounter;  //Set device address
       Serial.print("Device found on pin ");
       Serial.println(lidarUnits[i].pin);
     
       //Write address to LIDAR    
    }
    
    digitalWrite(pinArray[i],0); //Disable the enable pin
  }

  //Enable all lidars
  for(int i=0; i < sizeof(pinArray); i++)
  {
    digitalWrite(pinArray[i],1);
  }

  delay(20);
  
}

void loop() 
{
  
    //Loop through each Lidar unit and read distance data
    for(int i = 0; i < unitCounter; i++)
    {        
       Serial.println(myLidarLite.distance(false, lidarUnits[i].address));
       delay(1000);
    }
}
