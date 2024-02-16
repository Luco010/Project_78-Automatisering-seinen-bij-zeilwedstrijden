#include "GPS.h"

GPS::GPS(){}

void GPS::initialize(Adafruit_GPS GPS) {
  this->GPS = GPS;
  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  this->GPS.begin(9600); // > in my case I use UBLOX 6M: GY-GPS6Mv2
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  this->GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time
  // Set the update rate
  this->GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz
  // Request updates on antenna status, comment out to keep quiet
  this->GPS.sendCommand(PGCMD_ANTENNA);
}
int GPS::updateGPS(){
  //  char c = this->GPS.read();
  //  if ((c) && (GPSECHO))
  //    Serial.write(c);

  // if a sentence is received, we can check the checksum, parse it...
  if (this->GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    //Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false

    if (!this->GPS.parse(this->GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return 1;  // we can fail to parse a sentence in which case we should just wait for another
  }
  return 0;
}
void GPS::getTime(timeStruct &t) {
  t.Year = GPS.year;
  t.Month = GPS.month;
  t.Day = GPS.day;
  t.Hour = GPS.hour;
  t.Minute = GPS.minute;
  t.Second = GPS.seconds;
  t.DayOfWeek = getDayOfWeek(GPS.year, GPS.month, GPS.day);
}

bool GPS::getGPSFix() {
  return this->GPS.fix;
}

int GPS::getGPSQuality() {
  return atoi(this->GPS.fixquality);
}

int GPS::getDayOfWeek(int yr, int mnth, int dy) {
  int m = mnth;
  int d = dy;
  int yy;
  int yyyy = yr;
  int c;
  int mTable;
  int SummedDate;
  int DoW;
  int leap;
  int cTable;

  // Leap Year Calculation
  if((fmod(yyyy,4) == 0 && fmod(yyyy,100) != 0) || (fmod(yyyy,400) == 0))
  { leap = 1; }
  else 
  { leap = 0; }

  // Limit results to year 1900-2299 (to save memory)
  while(yyyy > 2299)
  { yyyy = yyyy - 400; }
  while(yyyy < 1900)
  { yyyy = yyyy + 400; }

  // Calculating century
  c = yyyy/100;

  // Calculating two digit year
  yy = fmod(yyyy, 100);

  // Century value based on Table
  if(c == 19) { cTable = 1; }
  if(c == 20) { cTable = 0; }
  if(c == 21) { cTable = 5; }
  if(c == 22) { cTable = 3; }

  // Jan and Feb calculations affected by leap years
  if(m == 1)
  { if(leap == 1) { mTable = 6; }
    else          { mTable = 0; }}
  if(m == 2)
  { if(leap == 1) { mTable = 2; }
    else          { mTable = 3; }}
  // Other months not affected and have set values
  if(m == 10) { mTable = 0; }
  if(m == 8) { mTable = 2; }
  if(m == 3 || m == 11) { mTable = 3; }
  if(m == 4 || m == 7) { mTable = 6; }
  if(m == 5) { mTable = 1; }
  if(m == 6) { mTable = 4; }
  if(m == 9 || m == 12) { mTable = 5; }

  // Enter the data into the formula
  SummedDate = d + mTable + yy + (yy/4) + cTable;
  
  // Find remainder
  DoW = fmod(SummedDate,7);  

  return DoW;
}
