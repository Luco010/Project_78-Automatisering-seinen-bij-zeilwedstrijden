#include <Adafruit_GPS.h>

struct timeStruct {
  byte Year = 2021;
  byte Month = 12;
  byte Day = 3;
  byte Hour = 11;
  byte Minute = 41;
  byte Second = 50;
  byte DayOfWeek = 5;
};


class GPS {
  public:
    GPS();
    void initialize(Adafruit_GPS GPS);
    int updateGPS();
    void getTime(timeStruct &t);
    void setTime(timeStruct &t);
    bool getGPSFix();
    int getGPSQuality();
    int getDayOfWeek(int yr, int mnth, int dy);
  private:
    struct timeStruct t;
    Adafruit_GPS GPS;
};
