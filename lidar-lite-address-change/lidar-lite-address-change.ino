#include <LIDARLite.h>

/*
This is the main script.

library from https://github.com/garmin/LIDARLite_Arduino_Library
manual from http://static.garmin.com/pumac/LIDAR_Lite_v3_Operation_Manual_and_Technical_Specifications.pdf
*/
static const char lidar_1_enable = 0;
static const char lidar_2_enable = 1;
static const char lidar_3_enable = 2;
static const char lidar_4_enable = 3;
static const char lidar_5_enable = 4;

static const char lidar_1_address = 0x40;
static const char lidar_2_address = 0x42;
static const char lidar_3_address = 0x44;
static const char lidar_4_address = 0x46;
static const char lidar_5_address = 0x48;

LIDARLite lidar_1;
LIDARLite lidar_2;
LIDARLite lidar_3;
LIDARLite lidar_4;
LIDARLite lidar_5;



void setup()
{
    Serial.begin(9600); // Initialize serial connection to display distance readings
	
	//initialize ALL the lidar enable pins and disable all the lidars
	pinMode(lidar_1_enable, OUTPUT);
	pinMode(lidar_2_enable, OUTPUT);
	pinMode(lidar_3_enable, OUTPUT);
	pinMode(lidar_4_enable, OUTPUT);
	pinMode(lidar_5_enable, OUTPUT);
	digitalWrite(lidar_1_enable, LOW);
	digitalWrite(lidar_2_enable, LOW);
	digitalWrite(lidar_3_enable, LOW);
	digitalWrite(lidar_4_enable, LOW);
	digitalWrite(lidar_5_enable, LOW);
	
	//enable lidar 1
	digitalWrite(lidar_1_enable, HIGH);
	lidar_1.begin(0, true); // Set configuration to default and I2C to 400 kHz
	lidar_1.configure(0, );
	
	lidar_2.begin(0, true);
	lidar_3.begin(0, true);
	lidar_4.begin(0, true);
	lidar_5.begin(0, true);
	
    lidarLite.configure(0); // Change this number to try out alternate configurations
}

void loop()
{
    int dist;
  
    // At the beginning of every 100 readings,
    // take a measurement with receiver bias correction
    if ( cal_cnt == 0 ) {
        dist = lidarLite.distance();      // With bias correction
    } else {
        dist = lidarLite.distance(false); // Without bias correction
    }
  
    // Increment reading counter
    cal_cnt++;
    cal_cnt = cal_cnt % 100;
  
    // Display distance
    Serial.print(dist);
    Serial.println(" cm");
  
    delay(10);
}

void change_address(char address, &LIDARLite lidar){
	byte[] serial_number = {0,0}
	lidar.read(0x96, 2, serial_number, 