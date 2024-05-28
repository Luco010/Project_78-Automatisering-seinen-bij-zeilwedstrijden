#include <Arduino.h>
#include "lib/GPS.h"
#include <Wire.h>

#define PINMP3 8

#define PINAfbreken 9
#define PINOvertreding 10
#define PINToeter 11
#define PINFinish 12
#define PINStart 13

#define PINSingle A0
#define PINSequenceA A1
#define PINSequenceB A2
#define PINFlag A3
#define PINModeA 5
#define PINModeB 6

const int BUTTONS[] {PINStart, PINOvertreding, PINToeter, PINFinish, PINAfbreken};
const int SWITCHES[] {PINModeB, PINModeB, PINFlag, PINSequenceB, PINSequenceA, PINSingle};

SoftwareSerial ss(4,3);
TinyGPSPlus tinyGPS;
GPS gps(tinyGPS, ss); 

void sendData() {
	byte switches = B00000000;
	byte buttons = B00000000;

	for (size_t i = 0; i < (sizeof(SWITCHES) / sizeof(SWITCHES[0])); i++)
		switches |= (digitalRead(SWITCHES[i]) << i);

	for (size_t i = 0; i < (sizeof(BUTTONS) / sizeof(BUTTONS[0])); i++)
		buttons |= (digitalRead(BUTTONS[i]) << i);

	static struct timeStruct t;
	gps.getTime(t);

	Wire.write(switches);
	Wire.write(buttons);
	Wire.write(0);
	Wire.write(1);
	Wire.write(t.Day);
	Wire.write(t.Month);	
	Wire.write(t.Year);
	Wire.write(t.Hour);
	Wire.write(t.Minute);
	Wire.write(t.Second);
	Wire.write(1); // GPS fix?
}

void receiveData(int dataC){
	while (Wire.available() > 0)
	{
		Serial.print(Wire.read());
	}
	Serial.println(" <- some info from epaper");
}

void playNext(){
	digitalWrite(PINMP3, 1);
	delay(100);
	digitalWrite(PINMP3, 0);
	delay(100);
	digitalWrite(PINMP3, 1);
}

void setup() {
	Serial.begin(115200);
	ss.begin(9600);

	Wire.begin(8);
  	Wire.onRequest(sendData);
	Wire.onReceive(receiveData);

	pinMode(PINMP3, OUTPUT);
	for (size_t i = 0; i < (sizeof(BUTTONS) / sizeof(BUTTONS[0])); i++)
		pinMode(BUTTONS[i], INPUT);

	Serial.println("hello?");
}

void loop() {
	gps.updateGPS();

	if (digitalRead(PINToeter)){
		Serial.println("PLAY");
		playNext();
	}

	// timeStruct time;
	// gps.getTime(time);
	// Serial.println(time.Minute);
}