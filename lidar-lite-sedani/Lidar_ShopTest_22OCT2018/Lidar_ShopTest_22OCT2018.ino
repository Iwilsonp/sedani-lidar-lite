#include <Wire.h>
#include <LIDARLite.h>

//Define pin array to be scanned for valid lidar scanners
int pinArray[] = {2,3,4,5,6,7,8,9};
int unitCounter = 0;

struct LIDAR
{
    int address;
    int pin;
    LIDARLite myLidarLite;
};

LIDAR lidarUnits[8];      //Array of lidarUnits to store configuration data


void change_address(char address, LIDARLite lidar);
void setup_lidar(char new_address, char lidar_enable, LIDARLite LIDAR);

void setup() 
{
  Wire.begin();
  Serial.begin(9600);   //Start serial communications
  while(!Serial){
    delay(1);
  }
  for(int i=0; i < 8; i++) // Setup enable pins
  {
    pinMode(pinArray[i],OUTPUT);
    digitalWrite(pinArray[i],0);
  }

  delay(100);

  
  int defAddress = 0x62;    //default address
  
  //Sequentially Enable the enable pins to detect active Lidar sensors
  for(int i=0; i < 8; i++) 
  {
    digitalWrite(pinArray[i],1); //enable the enable pin
    delay(100);                   //Wait 20ms to enable comms to Lidar unit
        
    Wire.beginTransmission(defAddress);  //Attempt to communicate to device
    delay(10);
    int error = Wire.endTransmission();      //Comm error
    if(error == 0)                       //Communications attempt successful
    {
       
       lidarUnits[i].pin = pinArray[i]; //Set pin number
       lidarUnits[i].address = defAddress + unitCounter*2;  //Set device address
       Serial.print("Device found on pin ");
       Serial.println(lidarUnits[i].pin);
       //Write address to LIDAR
       change_address(lidarUnits[i].address);
       unitCounter += 1;
    }
    else
    {
      Serial.print("No device found on ");
      Serial.println(pinArray[i]);
    }
    
    digitalWrite(pinArray[i],0); //Disable the enable pin
  }

  //Enable all lidars
  for(int i=0; i < 8; i++)
  {
    digitalWrite(pinArray[i],1);
  }

  delay(100);
  myLidarLite.begin(0, true);
  myLidarLite.configure(0);
  Serial.print(unitCounter);
  Serial.println(" devices detected and readdressed");
}

void loop() 
{
    //Loop through each Lidar unit and read distance data
    for(int i = 0; i < unitCounter; i++)
    {        
       //Serial.println(myLidarLite.distance(false, lidarUnits[i].address));
       Serial.print("reading lidar ");
       Serial.print(i);
       Serial.print(" on address ");
       Serial.println(lidarUnits[i].address);
       Serial.println(myLidarLite.distance(false, lidarUnits[i].address));
       delay(1000);
    }
}



//changes LIDARlite address following procedure outlined in manual pg 5
void change_address(char address){
  byte serial_number[2];
  byte lidar_default_address = 0x62;
  Serial.println("Reading Serial number");
  myLidarLite.read(0x96, 2, serial_number, false, lidar_default_address);
  Serial.println("Got Serial number ");
  myLidarLite.write(0x18, serial_number[0]);
  Serial.println("Written first byte");
  myLidarLite.write(0x19, serial_number[1]);
  Serial.println("Written second byte");
  myLidarLite.write(0x1a, address);
  Serial.println("Written address");
  myLidarLite.write(0x1e, 0x08);
  Serial.println("Overwritten 0x1e");
}
