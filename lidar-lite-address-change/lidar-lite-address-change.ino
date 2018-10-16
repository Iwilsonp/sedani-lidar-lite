#include <LIDARLite.h>

/*
This ia program to test changing the lidar address.

library from https://github.com/garmin/LIDARLite_Arduino_Library
manual from https://static.garmin.com/pumac/LIDAR_Lite_v3_Operation_Manual_and_Technical_Specifications.pdf
*/
static const char lidar_1_enable = 0;
static const char lidar_2_enable = 1;

static const char lidar_default_address = 0x62;

static const char lidar_1_address = 0x40;
static const char lidar_2_address = 0x42;

LIDARLite lidar_1;
LIDARLite lidar_2;

void change_address(char address, LIDARLite lidar);
void setup_lidar(char new_address, char lidar_enable, LIDARLite LIDAR);

void setup()
{
    Serial.begin(115200); // Initialize serial connection to display distance readings
	pinMode(LED_BUILTIN, OUTPUT);
	
	//initialize ALL the lidar enable pins and disable all the lidars
	pinMode(lidar_1_enable, OUTPUT);
	pinMode(lidar_2_enable, OUTPUT);
	
	digitalWrite(lidar_1_enable, LOW);
	digitalWrite(lidar_2_enable, LOW);
	
	setup_lidar(lidar_1_address, lidar_1_enable, lidar_1);
	setup_lidar(lidar_2_address, lidar_2_enable, lidar_2);
}

int cal_cnt = 0;

void loop()
{
    int dist_1;
	int dist_2;
  
    // At the beginning of every 100 readings,
    // take a measurement with receiver bias correction
    if ( cal_cnt == 0 ) {
        dist_1 = lidar_1.distance();      // With bias correction
		dist_2 = lidar_2.distance();
    } else {
        dist_1 = lidar_1.distance(false); // Without bias correction
    }   dist_2 = lidar_2.distance(false);
  
    // Increment reading counter
    cal_cnt++;
    cal_cnt = cal_cnt % 100;
  
    // Display distance
	Serial.print("Lidar 1: ");
    Serial.print(dist_1);
	Serial.print(" cm. Lidar 2: ");
	Serial.print(dist_2);
    Serial.println(" cm");
  
    delay(10);
}

void setup_lidar(char new_address, char lidar_enable, LIDARLite LIDAR){
	digitalWrite(lidar_enable, HIGH);
	delay(30); //let LIDARlite turn on
	LIDAR.begin(0, true);  // Set configuration to default and I2C to 400 kHz
	LIDAR.configure(0);
	change_address(new_address, LIDAR);
}


//changes LIDARlite address following procedure outlined in manual pg 5
void change_address(char address, LIDARLite lidar){
	byte serial_number[2];
	lidar.read(0x96, 2, serial_number, true, lidar_default_address);
	lidar.write(0x18, serial_number[0]);
	lidar.write(0x19, serial_number[1]);
	lidar.write(0x1a, address);
	lidar.write(0x1e, 0x08);
}
	