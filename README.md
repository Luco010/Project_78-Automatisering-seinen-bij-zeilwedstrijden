# Project_78-Automatisering-seinen-bij-zeilwedstrijden

## branches
- **main:** Bevat meest recente code.
- **oud:** Bevat code van de vorige ontwillel groep.

## folders

### epaper_arduino
Bevat de code voor de arduino om de inputs en outputs van de hardware te regelen.

**Dependencies:**
- Wire.h
- SoftwareSerial.h
- TinyGPS++.h

### epaper_main:
Bevat de code voor de esp32 / M5Paper om de applicatie state te regelen het scherm te weergeven.

**Dependencies:**
- M5EPD.h
- RTClib.h

### AutoDick_platformio
Bevat test code om de batterij status uit te lezen.

### dashboard
Bevat mock-up code voor het dashboard.
