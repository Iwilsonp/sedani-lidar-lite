#include "LIDARLite_expanded.h"

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
	
	while (!Serial){
		digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
		delay(20);
	}
	
	//initialize ALL the lidar enable pins and disable all the lidars
	pinMode(lidar_1_enable, OUTPUT);
	pinMode(lidar_2_enable, OUTPUT);
	
	digitalWrite(lidar_1_enable, LOW);
	digitalWrite(lidar_2_enable, LOW);
	
	setup_lidar(lidar_1_address, lidar_1_enable, lidar_1);
	lidar_1.scanI2CBus();
	/*
	setup_lidar(lidar_2_address, lidar_2_enable, lidar_2);
	lidar_1.scanI2CBus();
	*/
}

int cal_cnt = 0;

void loop()
{
    int dist_1;
	//int dist_2;
  
    // At the beginning of every 100 readings,
    // take a measurement with receiver bias correction
	Serial.println("Reading");
    if ( cal_cnt == 0 ) {
        dist_1 = lidar_1.distance(false, lidar_1_address);      // With bias correction
		//dist_2 = lidar_2.distance();
    } else {
        dist_1 = lidar_1.distance(false, lidar_1_address); // Without bias correction
    }   //dist_2 = lidar_2.distance(false);
  
    // Increment reading counter
    cal_cnt++;
    cal_cnt = cal_cnt % 100;
  
    // Display distance
	Serial.print("Lidar 1: ");
    Serial.println(dist_1);
	/*
	Serial.print(" cm. Lidar 2: ");
	Serial.print(dist_2);
    Serial.println(" cm");
	*/
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(200);
}

void setup_lidar(char new_address, char lidar_enable, LIDARLite LIDAR){
	digitalWrite(lidar_enable, HIGH);
	delay(100); //let LIDARlite turn on
	Serial.print("Beginning Lidar on ");
	Serial.println(new_address);
	LIDAR.begin(0, true);  // Set configuration to default and I2C to 400 kHz
	Serial.println("Configuring Lidar");
	LIDAR.configure(0);
	Serial.println("Changing LIDAR address");
	change_address(new_address, LIDAR);
}


//changes LIDARlite address following procedure outlined in manual pg 5
void change_address(char address, LIDARLite lidar){
	byte serial_number[2];
	Serial.println("Reading Serial number");
	lidar.read(0x96, 2, serial_number, true, lidar_default_address);
	Serial.println("Got Serial number ");
	lidar.write(0x18, serial_number[0]);
	Serial.println("Written first byte");
	lidar.write(0x19, serial_number[1]);
	Serial.println("Written second byte");
	lidar.write(0x1a, address);
	Serial.println("Written address");
	lidar.write(0x1e, 0x08);
	Serial.println("Overwritten 0x1e");
}
	