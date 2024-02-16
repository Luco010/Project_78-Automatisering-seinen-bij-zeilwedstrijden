// geluidsfragmenten 30, 10, 5, 4, 3, 2, 1 seconden voor

const int GPStx = 14;
const int GPSrx = 15;
const int timezone = +1;
const int encoderPins[] = {19, 18};
const int oneWaySwitch[] =  {2,5};
const int twoWaySwitch[] = {3,4,6,7};
const int button[] = {8,9,10,11,12};
bool buttonInput[5];
bool lastButtonInput[5];
const int switchLight[] = {48, 47, 46, 45};
const int startButtonLight = 44;
const int horn = 49;
const int soundPin = 22;
const int mariphonePin = 23;
const long millisForAnouncement30 = 2500;
const long millisForAnouncement10 = 2500;
const long millisForCountdown = 6500;
unsigned long millisNeeded = 7000;
unsigned long millisLastSound = -millisNeeded;
bool blowHorn = false;
int currentSoundFragment = 0;
int totalSoundFragments = 30;
int targetSoundFragment = 0;
bool playFragment = false;
bool pinout = false;
String flagString[] = {
  "Oranje vlag omhoog",               // 0
  "Oranje vlag omlaag",               // 1
  "Klasse vlag omhoog",               // 2
  "Klasse vlag omlaag",               // 3
  "Papa vlag omhoog",                 // 4
  "Papa vlag omlaag",                 // 5
  "Uniform vlag omhoog",              // 6
  "Uniform vlag omlaag",              // 7
  "Zwarte vlag omlaag",               // 8
  "Zwarte vlag omlaag",               // 9
  "De startprocedure is afgebroken",  // 10
  "De startprocedure is onderbroken", // 11
  "Er is iemand gefinisht",           // 12
  "Er is een overtreding geweest",    // 13
  "De startprocedure is gestart",     // 14
  "De toeterknop is ingedrukt"        // 15
};

#include <Wire.h>
#include <RotaryEncoder.h>
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include "GPS.h"
SoftwareSerial mySerial(GPStx, GPSrx);
Adafruit_GPS TEMPGPS(&mySerial);
#define GPSECHO true

#include <SPI.h>
#include <SD.h>
bool SDInitialized = false;
File logFile;
String dateString;
String logFileName = "Log";
String logFileExtension = ".txt";
#define SD_CS_PIN 53

GPS GPS;
struct timeStruct t;

long encoderLastPos = 0;
long encoderChange = 0;
bool encoderCheck = false;

RotaryEncoder *encoder = nullptr;
void encoderTick() {
  encoder->tick();
}

void setup() {
  Serial.begin(115200);
  while(!Serial);
  Wire.begin(8);
  Wire.onRequest(sendData);
  Wire.onReceive(receiveData);
  GPS.initialize(TEMPGPS); //-------------------------------init GPS
  
  for(int i = 0; i < 2; i++) {//----------------------------begin init switches, buttons and horn
    pinMode(oneWaySwitch[i], INPUT_PULLUP);
  }
  for(int i = 0; i < 4; i++) {
    pinMode(twoWaySwitch[i], INPUT_PULLUP);
  }
  for(int i = 0; i < 5; i++) {
    pinMode(button[i], INPUT_PULLUP);
  }
  for(int i = 0; i < 2; i++) {
    pinMode(encoderPins[i], INPUT_PULLUP);
  }
  for(int i = 0; i < 4; i++) {
    pinMode(switchLight[i], OUTPUT);
  }
  pinMode(startButtonLight, OUTPUT);
  pinMode(horn, OUTPUT);//----------------------------------end init switches, buttons and horn
  pinMode(soundPin, OUTPUT);
//  digitalWrite(soundPin, HIGH);
  pinMode(mariphonePin, OUTPUT);

  encoder = new RotaryEncoder(encoderPins[0], encoderPins[1], RotaryEncoder::LatchMode::TWO03);//---------begin init rotary encoder
  attachInterrupt(digitalPinToInterrupt(encoderPins[0]), encoderTick, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderPins[1]), encoderTick, CHANGE);//--------------------------end init rotary encoder
  
  Serial.println("Initializing SD card");
  if(!SD.begin(SD_CS_PIN)) {
    Serial.println("Initialization failed!");
  } else {
    SDInitialized = true;
    Serial.println("initialization done.");
  }
  
  Serial.println("rebooted");
}

void sendData(int howMany) {
  int switches = 
    digitalRead(oneWaySwitch[0]) * 32 + 
    !digitalRead(twoWaySwitch[0]) * 16 + 
    digitalRead(twoWaySwitch[1]) * 8 + 
    digitalRead(oneWaySwitch[1]) * 4 +
    !digitalRead(twoWaySwitch[2]) * 2 + 
    digitalRead(twoWaySwitch[3]) * 1;
  int buttons = 
    !digitalRead(button[0]) * 16 +
    !digitalRead(button[1]) * 8 +
    !digitalRead(button[2]) * 4 + 
    !digitalRead(button[3]) * 2 + 
    !digitalRead(button[4]) * 1;
  setEncoderChange();
  GPS.getTime(t);
  Wire.write(switches);
  Wire.write(buttons);
  Wire.write(encoderChange);
  Wire.write(SDInitialized);
  Wire.write(t.Day);
  Wire.write(t.Month);
  Wire.write(t.Year);
  Wire.write(t.Hour);
  Wire.write(t.Minute);
  Wire.write(t.Second);
  Wire.write(GPS.getGPSFix());
//  char Buffer[32];
//  sprintf(Buffer,"%02d %02d  %02d/%02d/%04d %02d:%02d:%02d", encoderChange, encoderLastPos, t.Day, t.Month, t.Year, t.Hour, t.Minute, t.Second);
//  Serial.println(Buffer);
}

String getTimeString(int hour_, int min_, int sec_) {
  char timeBuffer[32];
  sprintf(timeBuffer, "%02d:%02d:%02d", hour_, min_, sec_);
  return timeBuffer;
}

void receiveData(int howMany) {
  for(int i = 0; i < 5; i++) {
    lastButtonInput[i] = buttonInput[i];
    buttonInput[i] = !digitalRead(button[i]);
  }
  int n = 0;
//  Serial.println(howMany);
/*
 * howMany = 7 for the lights, horn and the sound fragment
 * howMany = ? for the logging of files
 */
  while(Wire.available() > 5) {
    if(n < sizeof(switchLight) / sizeof(switchLight[0])) {
      digitalWrite(switchLight[n], Wire.read());
    } else if(n == sizeof(switchLight) / sizeof(switchLight[0])) {
      digitalWrite(startButtonLight, Wire.read());
    } else if(n == sizeof(switchLight) / sizeof(switchLight[0]) + 1) {
      blowHorn = Wire.read();
    } else {
      int temp = Wire.read();
      if(temp != 99){
        targetSoundFragment = temp;
        playFragment = true;
        millisNeeded = 2500;
        if(temp % 3 == 0) millisNeeded = millisForAnouncement30;
        if(temp % 3 == 1) millisNeeded = millisForAnouncement10;
        if(temp % 3 == 2) millisNeeded = millisForCountdown;
      }
    }
    n++;
  }
  int dataInput[Wire.available() - 1];
  n = 0;
  while(Wire.available()) {
    dataInput[n] = Wire.read();
    n++;
  }
  Serial.println(dataInput[0]);
  if(dataInput[0] == 1) {
    String tempString = getTimeString(dataInput[1], dataInput[2], dataInput[3]);
    dateString = tempString.substring(0, 2) + "-" + tempString.substring(3,5);
    Serial.println("making/opening file: " + logFileName + dateString + logFileExtension);
    logFile = SD.open(logFileName + dateString + logFileExtension, FILE_WRITE);
    if(logFile){
      logFile.println("Machine gestart");
      logFile.close();
    }
    else Serial.println("error making file");
  } else if(dataInput[0] == 2) {
    String logDataReceived = getTimeString(dataInput[1], dataInput[2], dataInput[3]) +  " - " + flagString[dataInput[4]];
    logFile = SD.open(logFileName + dateString + logFileExtension, FILE_WRITE);
    if(logFile) {
      Serial.println("writing to file: " + logDataReceived);
      logFile.println(logDataReceived);
      logFile.close();
    } else Serial.println("error writing to file");
  }
}

void setEncoderChange() {
  encoderChange = 0;
  long encoderPos = encoder->getPosition() / 2;
  encoderChange = encoderPos - encoderLastPos;
  encoderLastPos = encoderPos;
}

void loop(){ 
  String inputString = "";
  if(Serial.available()){
    delay(100);
    while(Serial.available()) {
      inputString += (char)Serial.read();
    }
    targetSoundFragment = inputString.toInt();
    playFragment = true;
  }
  if(playFragment && targetSoundFragment < totalSoundFragments) {
    digitalWrite(soundPin, HIGH);
    delay(100);
    digitalWrite(soundPin, LOW);
    delay(100);
    currentSoundFragment = (currentSoundFragment + 1) % totalSoundFragments;
//    Serial.println(currentSoundFragment);
    if(currentSoundFragment == targetSoundFragment) {
      playFragment = false;
      millisLastSound = millis();
    }
  }
  digitalWrite(mariphonePin, millis() - millisLastSound < millisNeeded);
  digitalWrite(horn, !digitalRead(button[2]) || blowHorn);\
  
//  static long millisResetSD;
//  if(millis() - 5000 > millisResetSD) {
//    millisResetSD = millis();
//    SDInitialized = SD.begin(SD_CS_PIN);
//    Serial.println(SDInitialized);
//  }
}
