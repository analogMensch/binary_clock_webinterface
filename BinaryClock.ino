/*
BINARY CLOCK v0.19 (2022/12/09)

- micro controller selection in rows 22,23 or 24,25
- web server port in row 180
- LED type in row 177
- data pin for LEDs in row 37
- daylight saving offset in row 120
- GMT offset in row 121
- date type in row 122
- dark mode auto reset after alarm in row 159
- color for standby (day mode) in row 128
- color for time (day mode) in row 129
- color for week day (day mode) in row 130
- color for date (day mode) in row 131
- color for standby (night mode) in row 132
- color for time (night mode) in row 133
- color for week day (night mode) in row 134
- color for date (night mode) in row 135
*/
#include <Arduino.h>
#include <ESP8266WiFi.h>  //for ESP8266
#include <ESPAsyncTCP.h>  //for ESP8266
//#include <WiFi.h>  //for ESP32
//#include <AsyncTCP.h>  //for ESP32
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <Adafruit_NeoPixel.h>
#include <time.h>
#include <EEPROM.h>
#include "WirelessData.h"
#include "WebInterface.h"

//amount of LEDs
#define NUM_PIXELS 11
//RGB LED chips data out at GPIO4
#define PIN 4

int mode = 0;
int requestTime;
int requestWeek1;
int requestWeek2;
int requestWeek3;
int requestWeek4;
int requestWeek5;
int requestWeek6;
int requestWeek7;
int modeLast;
int columnLeft;
int columnRight;
int changeAnimation = 75;
int loopAnimation = 90;
int loopAnimationDelay = 10;
int loopAnimationEnd = 5;
int ntpYear;
int ntpMonth;
int ntpDay;
int ntpHour;
int ntpMinute;
int ntpSecond;
int ntpWeek;
int monthLEDa;
int monthLEDb;
int monthLEDc;
int monthLEDd;
int monthLEDe;
int monthLEDf;
int dayLEDa;
int dayLEDb;
int dayLEDc;
int dayLEDd;
int dayLEDe;
int dayLEDf;
int weekLEDa;
int weekLEDb;
int weekLEDc;
int weekLEDd;
int weekLEDe;
int hourLEDa;
int hourLEDb;
int hourLEDc;
int hourLEDd;
int hourLEDe;
int minuteLEDa;
int minuteLEDb;
int minuteLEDc;
int minuteLEDd;
int minuteLEDe;
int minuteLEDf;
int secondLEDa;
int secondLEDb;
int secondLEDc;
int secondLEDd;
int secondLEDe;
int secondLEDf;
int leftLEDa;
int leftLEDb;
int leftLEDc;
int leftLEDd;
int leftLEDe;
int rightLEDa;
int rightLEDb;
int rightLEDc;
int rightLEDd;
int rightLEDe;
int rightLEDf;
int alarm1Set;
int alarm1Mode = 0;
int alarm1Time = 0;
int alarm1Week[7] = {0,0,0,0,0,0,0};
int alarm1Left = 0;
int alarm1Right = 0;
int alarm2Set;
int alarm2Mode = 0;
int alarm2Time = 0;
int alarm2Week[7] = {0,0,0,0,0,0,0};
int alarm2Left = 0;
int alarm2Right = 0;
int daylightOffset = 0;
int daylightOffsetValue = 3600;  //0 = countries without daylight saving, 3600 = countries with daylight saving
const int gmtOffset = 3600;  //GMT offset in seconds
const int dateType = 1;  //1 = month on left and day on right side (e.g. january 1st), 2 = day on left and month on right side (e.g. 1st of january)
unsigned long requestColor;
unsigned long timestampSync;
unsigned long pulse;
unsigned long pulseDelay = 1000;
const long colorDark = 0x000000;  //color 0x000000 for dark mode
const long colorStandbyDay = 0x010000;  //color for standby in day mode
const long colorTimeDay = 0x000007;  //color for time in day mode
const long colorWeekDay = 0x000303;  //color for week day in day mode
const long colorDateDay = 0x030003;  //color for date in day mode
const long colorStandbyNight = 0x010000;  //color for standby in night mode
const long colorTimeNight = 0x000001;  //color for time in night mode
const long colorWeekNight = 0x000101;  //color for week day in night mode
const long colorDateNight = 0x010001;  //color for date in night mode
unsigned long colorStandby;
unsigned long colorTime;
unsigned long colorWeek;
unsigned long colorDate;
const long colorWiFiSearch = 0x030000;
const long colorWiFiCheck = 0x030303;
const long colorWiFiFound = 0x000300;
unsigned long alarm1Color = 0x000000;
unsigned long alarm2Color = 0x000000;
const char* modeParameter = "mode";
const char* alarmParameter = "alarm";
const char* timeParameter = "time";
const char* colorParameter = "color";
const char* leftParameter = "hour";
const char* rightParameter = "minute";
const char* weekParameter = "week";
bool boot = false;
bool change = true;
bool timePrint = true;
bool synchronize = false;
bool dayMode = true;
bool nightMode = false;
bool darkMode = false;
bool darkModeReset = false;  //false = stay in dark mode after alarm, true = exit dark mode after alarm
String modeMessage;
String alarmMessage;
String timeMessage;
String colorMessage;
String columnLeftMessage;
String columnRightMessage;
String weekMessage;
String weekMessage1Valid;
String weekMessage2Valid;
String sMode;
String sTime;
String sColor;
String sHour;
String sMinute;
String sWeek;

//objects define
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);

//web server define
AsyncWebServer server(80);

void setup() {
  //serial init
  Serial.begin(9600);

  //color settings
  colorStandby = colorStandbyDay;
  colorTime = colorTimeDay;
  colorWeek = colorWeekDay;
  colorDate = colorDateDay;

  //WS2812B init
  pixels.begin();
  for (int i = 0; i < 11; i++) {
    pixels.setPixelColor(i, colorWiFiCheck);
    pixels.show();
  }
  delay(1000);

  //WiFi mode define
  WiFi.mode(WIFI_STA);
  //WiFi init
  WiFi.begin(ssid, password);
  WiFiConnect();

  //OTA server init
  AsyncElegantOTA.begin(&server);

  //http request without parameters (root page)
  server.on("/", HTTP_GET, [](AsyncWebServerRequest*request){
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", requestWeb);
    response->addHeader("Validation","nochange");
    response->addHeader("Mode", String(mode));
    request->send(response);
  });

  //http request (alarm settings page)
  server.on("/settings", HTTP_GET, [](AsyncWebServerRequest*request){
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", requestWebSettings);
    response->addHeader("Validation","valid");
    response->addHeader("Mode",String(mode));
    response->addHeader("Day",String(dayMode));
    response->addHeader("Night",String(nightMode));
    response->addHeader("Dark",String(darkMode));
    request->send(response);
  });

  //http request (display settings page)
  server.on("/display", HTTP_GET, [](AsyncWebServerRequest*request){
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", requestWebDisplay);
    response->addHeader("Validation","valid");
    response->addHeader("Mode",String(mode));
    request->send(response);
  });

  //http request (alarm settings page)
  server.on("/alarm-r", HTTP_GET, [](AsyncWebServerRequest*request){
    mode = 8;
    alarmFeedback();
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", requestWebAlarm);
    response->addHeader("Validation","valid");
    response->addHeader("Mode",String(mode));
    response->addHeader("Alarm",sMode);
    response->addHeader("Color",sColor);
    response->addHeader("Hour",sHour);
    response->addHeader("Minute",sMinute);
    response->addHeader("Week",sWeek);
    response->addHeader("Time",sTime);
    request->send(response);
    mode = 1;
  });

  //http request (alarm settings page)
  server.on("/alarm-s", HTTP_GET, [](AsyncWebServerRequest*request){
    mode = 9;
    alarmFeedback();
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", requestWebAlarm);
    response->addHeader("Validation","valid");
    response->addHeader("Mode",String(mode));
    response->addHeader("Alarm",sMode);
    response->addHeader("Color",sColor);
    response->addHeader("Hour",sHour);
    response->addHeader("Minute",sMinute);
    response->addHeader("Week",sWeek);
    response->addHeader("Time",sTime);
    request->send(response);
    mode = 1;
  });

  //http GET request with parameters
  server.on(
    "/change", HTTP_GET, [](AsyncWebServerRequest*request) {
      if (request->hasParam(modeParameter)) {
        modeMessage = request->getParam(modeParameter)->value();  //mode
        mode = modeMessage.toInt();
        if (mode <= 11) {  //check for valid mode request
          if (request->hasParam(alarmParameter) && mode == 8) {  //alarm in 0/1 for false/true, only written in mode 8
            alarmMessage = request->getParam(alarmParameter)->value();
            alarm1Set = alarmMessage.toInt();
          }
          if (request->hasParam(alarmParameter) && mode == 9) {  //alarm in 0/1 for false/true, only written in mode 9
            alarmMessage = request->getParam(alarmParameter)->value();
            alarm2Set = alarmMessage.toInt();
          }
          if (request->hasParam(timeParameter)) {  //time in seconds
            timeMessage = request->getParam(timeParameter)->value();
            requestTime = timeMessage.toInt();
          } else {
            if (mode == 8) {
              requestTime = alarm1Time;
            }
            else if (mode == 9) {
              requestTime = alarm2Time;
            } else {
              requestTime = 0;
            }
          }
          if (request->hasParam(colorParameter)) {  //color in HEX
            colorMessage = request->getParam(colorParameter)->value();
            for (int i=0; i<6; i++) {
              char cHexCheck = colorMessage[i];
              if ((cHexCheck < '0' || cHexCheck > '9') && (cHexCheck < 'A' || cHexCheck > 'F') || colorMessage.length() != 6) {  //check for hex value above F und for wrong message length
                goto invalidValueCatch;
              } else {  //write color to unsigned long variable
                char cRequestColor[7];
                colorMessage.toCharArray(cRequestColor,7);
                requestColor = strtoul(cRequestColor, 0, 16);
              }
            }
          } else {
            if (mode == 8) {
              requestColor = alarm1Color;
            }
            else if (mode == 9) {
              requestColor = alarm2Color;
            } else {
              requestColor = 0x000000;
            }
          }
          if (request->hasParam(leftParameter)) {  //left column value up to 31
            columnLeftMessage = request->getParam(leftParameter)->value();
            columnLeft = columnLeftMessage.toInt();
          } else {
            if (mode == 8) {
              columnLeft = alarm1Left;
            }
            else if (mode == 9) {
              columnLeft = alarm2Left;
            } else {
              columnLeft = 0;
            }
          }
          if (request->hasParam(rightParameter)) {  //right column value up to 63
            columnRightMessage = request->getParam(rightParameter)->value();
            columnRight = columnRightMessage.toInt();
          } else {
            if (mode == 8) {
              columnRight = alarm1Right;
            }
            else if (mode == 9) {
              columnRight = alarm2Right;
            } else {
              columnRight = 0;
            }
          }
          if (request->hasParam(weekParameter)) {  //weekday value from 0000000 to 9999999
            weekMessage = request->getParam(weekParameter)->value();
            requestWeek1 = (weekMessage.substring(0,1)).toInt();
            requestWeek2 = (weekMessage.substring(1,2)).toInt();
            requestWeek3 = (weekMessage.substring(2,3)).toInt();
            requestWeek4 = (weekMessage.substring(3,4)).toInt();
            requestWeek5 = (weekMessage.substring(4,5)).toInt();
            requestWeek6 = (weekMessage.substring(5,6)).toInt();
            requestWeek7 = (weekMessage.substring(6,7)).toInt();
          } else {
            if (mode == 8) {
              requestWeek1 = alarm1Week[0];
              requestWeek2 = alarm1Week[1];
              requestWeek3 = alarm1Week[2];
              requestWeek4 = alarm1Week[3];
              requestWeek5 = alarm1Week[4];
              requestWeek6 = alarm1Week[5];
              requestWeek7 = alarm1Week[6];
            }
            else if (mode == 9) {
              requestWeek1 = alarm2Week[0];
              requestWeek2 = alarm2Week[1];
              requestWeek3 = alarm2Week[2];
              requestWeek4 = alarm2Week[3];
              requestWeek5 = alarm2Week[4];
              requestWeek6 = alarm2Week[5];
              requestWeek7 = alarm2Week[6];
            } else {
              requestWeek1 = 0;
              requestWeek2 = 0;
              requestWeek3 = 0;
              requestWeek4 = 0;
              requestWeek5 = 0;
              requestWeek6 = 0;
              requestWeek7 = 0;
            }
          }
          if (mode == 4) {  //check for valid values in mode 4
            if (columnLeft > 31 || columnRight > 63) {
              change = false;
              goto invalidValueCatch;
            } else {
              change = true;
              AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", requestWeb);
              response->addHeader("Validation","valid");
              response->addHeader("Mode",String(mode));
              request->send(response);
            }
          }
          else if (mode == 6) {  //check for valid values in mode 6
            if (dayMode) {
              change = false;
              goto invalidValueCatch;
            } else {
              change = true;
              AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", requestWeb);
              response->addHeader("Validation","valid");
              response->addHeader("Mode",String(mode));
              request->send(response);
            }
          }
          else if (mode == 7) {  //check for valid values in mode 7
            if (nightMode) {
              change = false;
              goto invalidValueCatch;
            }  else {
              change = true;
              AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", requestWeb);
              response->addHeader("Validation","valid");
              response->addHeader("Mode",String(mode));
              request->send(response);
            }
          }
          else if (mode == 8) {  //check for valid values in mode 8
            if (columnLeft <= 23 && columnRight <= 59 && alarm2Set <= 2 && (weekMessage.length() == 0 || weekMessage.length() == 7)) {  //catch only valid values
              change = true;
              alarmSave();
              alarmFeedback();
              weekMessage1Valid = weekMessage;
              AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", requestWeb);
              response->addHeader("Validation","valid");
              response->addHeader("Mode",String(mode));
              response->addHeader("Alarm",sMode);
              response->addHeader("Color",sColor);
              response->addHeader("Hour",sHour);
              response->addHeader("Minute",sMinute);
              response->addHeader("Week",sWeek);
              response->addHeader("Time",sTime);
              request->send(response);
            } else {  //catch one or more invalid values and set them back to the last known valid values
              columnLeft = alarm1Left;
              columnRight = alarm1Right;
              alarm2Set = alarm1Mode;
              weekMessage = weekMessage1Valid;
              goto invalidValueCatch;
            } 
          }
          else if (mode == 9) {  //check for valid values in mode 9
            //check if there is more than one weekday selected
            int dayCount = 0;
            if (requestWeek1 > 0) {
              dayCount++;
            }
            if (requestWeek2 > 0) {
              dayCount++;
            }
            if (requestWeek3 > 0) {
              dayCount++;
            }
            if (requestWeek4 > 0) {
              dayCount++;
            }
            if (requestWeek5 > 0) {
              dayCount++;
            }
            if (requestWeek6 > 0) {
              dayCount++;
            }
            if (requestWeek7 > 0) {
              dayCount++;
            }
            if (columnLeft <= 23 && columnRight <= 59 && alarm2Set <= 2 && (weekMessage.length() == 0 || weekMessage.length() == 7) && dayCount <= 1) {  //catch only valid values
              change = true;
              alarmSave();
              alarmFeedback();
              weekMessage2Valid = weekMessage;
              AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", requestWeb);
              response->addHeader("Validation","valid");
              response->addHeader("Mode",String(mode));
              response->addHeader("Alarm",sMode);
              response->addHeader("Color",sColor);
              response->addHeader("Hour",sHour);
              response->addHeader("Minute",sMinute);
              response->addHeader("Week",sWeek);
              response->addHeader("Time",sTime);
              request->send(response);
            } else {  //catch one or more invalid values and set them back to the last known valid values
              columnLeft = alarm2Left;
              columnRight = alarm2Right;
              alarm2Set = alarm2Mode;
              weekMessage = weekMessage2Valid;
              goto invalidValueCatch;
            } 
          }
          else if (mode == 10) {  //check for valid values in mode 10
            if (!darkMode) {
              change = false;
              goto invalidValueCatch;
            } else {
              change = true;
              AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", requestWeb);
              response->addHeader("Validation","valid");
              response->addHeader("Mode",String(mode));
              request->send(response);
            }
          }
          else if (mode == 11) {  //check for valid values in mode 11
            if (darkMode) {
              change = false;
              goto invalidValueCatch;
            }  else {
              change = true;
              AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", requestWeb);
              response->addHeader("Validation","valid");
              response->addHeader("Mode",String(mode));
              request->send(response);
            }
          } else {
            change = true;
            AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", requestWeb);
            response->addHeader("Validation","valid");
            response->addHeader("Mode",String(mode));
            request->send(response);
          }
        } else {
          invalidValueCatch:
          mode = 1;
          AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", requestWeb);
          response->addHeader("Validation","invalid");
          response->addHeader("Mode",String(mode));
          request->send(response);
        }
      } else {
        AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", requestWeb);
        response->addHeader("Validation","nochange");
        response->addHeader("Mode", String(mode));
        request->send(response);
      }
    });

  //web server init
  server.begin();

  //read alarm setting from eeprom
  EEPROM.begin(512);
  unsigned long alarmColorSub;
  //read settings for alarm 1
  alarm1Set = EEPROM.read(110);
  requestTime = EEPROM.read(120);
  columnLeft = EEPROM.read(130);
  columnRight = EEPROM.read(131);
  for (uint8_t i=0; i<4; i++) {
    alarmColorSub = alarmColorSub * 256;
    alarmColorSub += EEPROM.read(140 + 3 -i);
  }
  for (int i=0; i<7; i++) {
    alarm1Week[i] = EEPROM.read(150+i);
  }
  requestColor = alarmColorSub;
  alarm1Mode = alarm1Set;
  alarm1Time = requestTime;
  alarm1Left = columnLeft;
  alarm1Right = columnRight;
  alarm1Color = requestColor;
  //read settings for alarm 2
  alarm2Set = EEPROM.read(210);
  requestTime = EEPROM.read(220);
  columnLeft = EEPROM.read(230);
  columnRight = EEPROM.read(231);
  for (uint8_t i=0; i<4; i++) {
    alarmColorSub = alarmColorSub * 256;
    alarmColorSub += EEPROM.read(240 + 3 -i);
  }
  for (int i=0; i<7; i++) {
    alarm2Week[i] = EEPROM.read(250+i);
  }
  requestColor = alarmColorSub;
  alarm2Mode = alarm2Set;
  alarm2Time = requestTime;
  alarm2Left = columnLeft;
  alarm2Right = columnRight;
  alarm2Color = requestColor;
  EEPROM.end();

  //wait for NTP connection
  delay(1000);

  //ntp set & init
  Serial.println();
  Serial.println("--> set GMT zone");
  configTime(gmtOffset, daylightOffset, ntpServer);  //initialize NTP without daylight saving offset
  Serial.println();
  Serial.println("--> get time over NTP");
  ntpTime();  //get time for calculating daylight saving offset
  Serial.println();
  Serial.println("--> calculate daylight saving offset");
  daylightOffsetSet();  //calculate daylight saving offset
  Serial.println();
  Serial.println("--> set time including daylight saving offset");
  configTime(gmtOffset, daylightOffset, ntpServer);  //reinitialize NTP with daylight saving offset

  //synchronize time over NTP
  ntpSynchronize();

  //boot done
  boot = true;
}

void loop() {
  if (millis() - pulse >= pulseDelay) {
    //set pulse time
    pulse = millis();
    //check WiFi connection
    while (WiFi.status() != WL_CONNECTED) {
      WiFiConnect();
    }
    //autoreset mode 0
    if (WiFi.status() == WL_CONNECTED && mode == 0) {
      mode = 1;
    }
    //autoreset change
    if (mode == 1) {
      change = false;
    }
    //get time
    ntpTime();
    if (!change) {
      //change summer/winter time
      if (ntpWeek == 7 && ntpDay >=25) {
        if (ntpMonth == 3 && ntpHour == 2 && ntpMinute == 0 && ntpSecond == 0) {
          Serial.println();
          daylightOffsetSet();
          configTime(gmtOffset, daylightOffset, ntpServer);
        }
        if (ntpMonth == 10 && ntpHour == 3 && ntpMinute == 0 && ntpSecond == 0) {
          Serial.println();
          daylightOffsetSet();
          configTime(gmtOffset, daylightOffset, ntpServer);
        }
      }
      //show alarm 1
      if (alarm1Mode > 0 && ntpHour == alarm1Left && ntpMinute == alarm1Right && ntpSecond == 0 && alarm1Week[ntpWeek - 1] > 0) {
        requestTime = alarm1Time;
        requestColor = alarm1Color;
        columnLeft = alarm1Left;
        columnRight = alarm1Right;
        change = true;
        if (alarm1Mode == 1) {  //show time in alarm color
          mode = 4;
        }
        else if (alarm1Mode == 2) {  //show cycle in alarm color
          mode = 5;
        }
      }
      //show alarm 2
      if (alarm2Mode > 0 && ntpHour == alarm2Left && ntpMinute == alarm2Right && ntpSecond == 0 && alarm2Week[ntpWeek - 1] > 0) {
        requestTime = alarm2Time;
        requestColor = alarm2Color;
        columnLeft = alarm2Left;
        columnRight = alarm2Right;
        change = true;
        if (alarm2Mode == 1) {  //show time in alarm color and turn off alarm afterwards
          mode = 4;
          alarm2Mode = 0;
          alarm2Set = 0;
        }
        else if (alarm2Mode == 2) {  //show cycle in alarm color and turn off alarm afterwards
          mode = 5;
          alarm2Mode = 0;
          alarm2Set = 0;
        }
      }
      //show weekday and date for 15 seconds
      if (ntpMinute == 0) {
        Serial.println("!here!");
        if (ntpSecond >= 15 && ntpSecond < 30) {
          mode = 2;
        }
        if (ntpSecond >= 30 && ntpSecond < 45) {
          mode = 3;
        }
        if (ntpSecond >= 45) {
          mode = 1;
        }
      }
      //synchronize clock to milliseconds once a day
      if (ntpHour == 4 && ntpMinute == 29 && ntpSecond == 30) {
        Serial.println();
        ntpSynchronize();
      }
    }
    //change led colors
    led();
  }
  /*if (change) {  //allow to override the manual requests
    led();
  }*/
}

void ntpTime() {
  time_t rawtime;
  struct tm* timeinfo;
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  timestampSync = rawtime;
  ntpYear = timeinfo->tm_year + 1900;
  ntpMonth = timeinfo->tm_mon + 1;
  ntpDay = timeinfo->tm_mday;
  ntpHour = timeinfo->tm_hour;
  ntpMinute = timeinfo->tm_min;
  ntpSecond = timeinfo->tm_sec;
  if (timeinfo->tm_wday == 0) {
    ntpWeek = 7;
  } else {
    ntpWeek = timeinfo->tm_wday;
  }
  if (ntpSecond == 0) {
    timePrint = true;
  }
  if (synchronize && modeLast == mode) {
    if (timePrint) {
      Serial.println();
      Serial.print("weekday/date/time: ");
      Serial.print(ntpWeek);
      Serial.print("_");
      Serial.print(ntpYear);
      Serial.print("-");
      Serial.print(ntpMonth);
      Serial.print("-");
      Serial.print(ntpDay);
      Serial.print("_");
      Serial.print(ntpHour);
      Serial.print("-");
      Serial.print(ntpMinute);
      Serial.print("-");
      Serial.print(ntpSecond);
      timePrint = false;
    }
    else {
      Serial.print(".");
      Serial.print(ntpSecond);
    }
  }
}

void ntpSynchronize() {
  synchronize = false;
  ntpTime();
  unsigned long timestampSyncDone = timestampSync + 1;
  Serial.println();
  Serial.print("--> starting synchronizasion with timestamp ");
  Serial.print(timestampSyncDone);
  while (timestampSync != timestampSyncDone) {
    ntpTime();
    Serial.print(".");
  }
  pulse = millis();
  Serial.println("done");
  synchronize = true;
  timePrint = true;
}

void ntpBinary() {
  int binWeek = sizeof(ntpWeek) * 8;
  int binMonth = sizeof(ntpMonth) * 8;
  int binDay = sizeof(ntpDay) * 8;
  int binHour = sizeof(ntpHour) * 8;
  int binMinute = sizeof(ntpMinute) * 8;
  int iBinWeek;
  int iBinMonth;
  int iBinDay;
  int iBinHour;
  int iBinMinute;
  char cBinWeek[binWeek + 1];
  char cBinMonth[binMonth + 1];
  char cBinDay[binDay + 1];
  char cBinHour[binHour + 1];
  char cBinMinute[binMinute + 1];
  char bufferWeek[5];
  char bufferMonth[6];
  char bufferDay[6];
  char bufferHour[5];
  char bufferMinute[6];
  String sBinWeek;
  String sBinMonth;
  String sBinDay;
  String sBinHour;
  String sBinMinute;
  String sBinWeekPart;
  String sBinMonthPart;
  String sBinDayPart;
  String sBinHourPart;
  String sBinMinutePart;
  String sBinWeekSub;
  String sBinMonthSub;
  String sBinDaySub;
  String sBinHourSub;
  String sBinMinuteSub;
  //buffered string formating
  sprintf(bufferWeek, "%05d", iBinWeek = String(sBinWeek = String(itoa(ntpWeek, cBinWeek, 2))).toInt());
  sBinWeekSub = String(bufferWeek);
  if (dateType == 1) {
    sprintf(bufferMonth, "%05d", iBinMonth = String(sBinMonth = String(itoa(ntpMonth, cBinMonth, 2))).toInt());
    sBinMonthSub = String(bufferMonth);
    sprintf(bufferDay, "%06d", iBinDay = String(sBinDay = String(itoa(ntpDay, cBinDay, 2))).toInt());
    sBinDaySub = String(bufferDay);
  }
  else if (dateType == 2) {
    sprintf(bufferMonth, "%06d", iBinMonth = String(sBinMonth = String(itoa(ntpMonth, cBinMonth, 2))).toInt());
    sBinMonthSub = String(bufferMonth);
    sprintf(bufferDay, "%05d", iBinDay = String(sBinDay = String(itoa(ntpDay, cBinDay, 2))).toInt());
    sBinDaySub = String(bufferDay);
  }
  sprintf(bufferHour, "%05d", iBinHour = String(sBinHour = String(itoa(ntpHour, cBinHour, 2))).toInt());
  sBinHourSub = String(bufferHour);
  sprintf(bufferMinute, "%06d", iBinMinute = String(sBinMinute = String(itoa(ntpMinute, cBinMinute, 2))).toInt());
  sBinMinuteSub = String(bufferMinute);
  //substring week day
  weekLEDa = (sBinWeekPart = sBinWeekSub.substring(4, 5)).toInt();
  weekLEDb = (sBinWeekPart = sBinWeekSub.substring(3, 4)).toInt();
  weekLEDc = (sBinWeekPart = sBinWeekSub.substring(2, 3)).toInt();
  weekLEDd = (sBinWeekPart = sBinWeekSub.substring(1, 2)).toInt();
  weekLEDe = (sBinWeekPart = sBinWeekSub.substring(0, 1)).toInt();
  if (dateType == 1) {
    //substrings month
    monthLEDa = (sBinMonthPart = sBinMonthSub.substring(4, 5)).toInt();
    monthLEDb = (sBinMonthPart = sBinMonthSub.substring(3, 4)).toInt();
    monthLEDc = (sBinMonthPart = sBinMonthSub.substring(2, 3)).toInt();
    monthLEDd = (sBinMonthPart = sBinMonthSub.substring(1, 2)).toInt();
    monthLEDe = (sBinMonthPart = sBinMonthSub.substring(0, 1)).toInt();
    //substrings day
    dayLEDa = (sBinDayPart = sBinDaySub.substring(5, 6)).toInt();
    dayLEDb = (sBinDayPart = sBinDaySub.substring(4, 5)).toInt();
    dayLEDc = (sBinDayPart = sBinDaySub.substring(3, 4)).toInt();
    dayLEDd = (sBinDayPart = sBinDaySub.substring(2, 3)).toInt();
    dayLEDe = (sBinDayPart = sBinDaySub.substring(1, 2)).toInt();
    dayLEDf = (sBinDayPart = sBinDaySub.substring(0, 1)).toInt();
  }
  else if (dateType == 2) {
    //substrings month
    monthLEDa = (sBinMonthPart = sBinMonthSub.substring(5, 6)).toInt();
    monthLEDb = (sBinMonthPart = sBinMonthSub.substring(4, 5)).toInt();
    monthLEDc = (sBinMonthPart = sBinMonthSub.substring(3, 4)).toInt();
    monthLEDd = (sBinMonthPart = sBinMonthSub.substring(2, 3)).toInt();
    monthLEDe = (sBinMonthPart = sBinMonthSub.substring(1, 2)).toInt();
    monthLEDf = (sBinMonthPart = sBinMonthSub.substring(0, 1)).toInt();
    //substrings day
    dayLEDa = (sBinDayPart = sBinDaySub.substring(4, 5)).toInt();
    dayLEDb = (sBinDayPart = sBinDaySub.substring(3, 4)).toInt();
    dayLEDc = (sBinDayPart = sBinDaySub.substring(2, 3)).toInt();
    dayLEDd = (sBinDayPart = sBinDaySub.substring(1, 2)).toInt();
    dayLEDe = (sBinDayPart = sBinDaySub.substring(0, 1)).toInt();
  }
  //substrings hour
  hourLEDa = (sBinHourPart = sBinHourSub.substring(4, 5)).toInt();
  hourLEDb = (sBinHourPart = sBinHourSub.substring(3, 4)).toInt();
  hourLEDc = (sBinHourPart = sBinHourSub.substring(2, 3)).toInt();
  hourLEDd = (sBinHourPart = sBinHourSub.substring(1, 2)).toInt();
  hourLEDe = (sBinHourPart = sBinHourSub.substring(0, 1)).toInt();
  //substrings minute
  minuteLEDa = (sBinMinutePart = sBinMinuteSub.substring(5, 6)).toInt();
  minuteLEDb = (sBinMinutePart = sBinMinuteSub.substring(4, 5)).toInt();
  minuteLEDc = (sBinMinutePart = sBinMinuteSub.substring(3, 4)).toInt();
  minuteLEDd = (sBinMinutePart = sBinMinuteSub.substring(2, 3)).toInt();
  minuteLEDe = (sBinMinutePart = sBinMinuteSub.substring(1, 2)).toInt();
  minuteLEDf = (sBinMinutePart = sBinMinuteSub.substring(0, 1)).toInt();
}

void requestBinary() {
  int binLeft = sizeof(ntpHour) * 8;
  int binRight = sizeof(ntpMinute) * 8;
  int iBinLeft;
  int iBinRight;
  char cBinLeft[binLeft + 1];
  char cBinRight[binRight + 1];
  char bufferLeft[5];
  char bufferRight[6];
  String sBinLeft;
  String sBinRight;
  String sBinLeftPart;
  String sBinRightPart;
  //buffered string formating
  sprintf(bufferLeft, "%05d", iBinLeft = String(sBinLeft = String(itoa(columnLeft, cBinLeft, 2))).toInt());
  String sBinLeftSub = String(bufferLeft);
  sprintf(bufferRight, "%06d", iBinRight = String(sBinRight = String(itoa(columnRight, cBinRight, 2))).toInt());
  String sBinRightSub = String(bufferRight);
  //substring left column
  leftLEDa = (sBinLeftPart = sBinLeftSub.substring(4, 5)).toInt();
  leftLEDb = (sBinLeftPart = sBinLeftSub.substring(3, 4)).toInt();
  leftLEDc = (sBinLeftPart = sBinLeftSub.substring(2, 3)).toInt();
  leftLEDd = (sBinLeftPart = sBinLeftSub.substring(1, 2)).toInt();
  leftLEDe = (sBinLeftPart = sBinLeftSub.substring(0, 1)).toInt();
  //substrings right column
  rightLEDa = (sBinRightPart = sBinRightSub.substring(5, 6)).toInt();
  rightLEDb = (sBinRightPart = sBinRightSub.substring(4, 5)).toInt();
  rightLEDc = (sBinRightPart = sBinRightSub.substring(3, 4)).toInt();
  rightLEDd = (sBinRightPart = sBinRightSub.substring(2, 3)).toInt();
  rightLEDe = (sBinRightPart = sBinRightSub.substring(1, 2)).toInt();
  rightLEDf = (sBinRightPart = sBinRightSub.substring(0, 1)).toInt();
}

void alarmSave() {
  if (mode == 8) {
    //write alarm mode to variable
    alarm1Mode = alarm1Set;
    //write alarm time to variable
    alarm1Time = requestTime;
    //write alarm clock time to variables
    alarm1Left = columnLeft;
    alarm1Right = columnRight;
    //write alarm week day setting to array
    alarm1Week[0] = requestWeek1;
    alarm1Week[1] = requestWeek2;
    alarm1Week[2] = requestWeek3;
    alarm1Week[3] = requestWeek4;
    alarm1Week[4] = requestWeek5;
    alarm1Week[5] = requestWeek6;
    alarm1Week[6] = requestWeek7;
    //write alarm color to variable
    alarm1Color = requestColor;
    //save alarm settings to eeprom
    EEPROM.begin(512);
    EEPROM.write(110, alarm1Mode);
    EEPROM.write(120, alarm1Time);
    EEPROM.write(130, alarm1Left);
    EEPROM.write(131, alarm1Right);
    unsigned long alarmColorSub = alarm1Color;
    for (uint8_t i =0; i<4; i++) {
      EEPROM.write(140 + i, alarmColorSub & 255);
      alarmColorSub = alarmColorSub / 256;
    }
    for (int i=0; i<7; i++) {
      EEPROM.write(150+i, alarm1Week[i]);
    }
    EEPROM.commit();
    EEPROM.end();
  }
  else if (mode == 9) {
    //write alarm mode to variable
    alarm2Mode = alarm2Set;
    //write alarm time to variable
    alarm2Time = requestTime;
    //write alarm clock time to variables
    alarm2Left = columnLeft;
    alarm2Right = columnRight;
    //write alarm week day setting to array
    alarm2Week[0] = requestWeek1;
    alarm2Week[1] = requestWeek2;
    alarm2Week[2] = requestWeek3;
    alarm2Week[3] = requestWeek4;
    alarm2Week[4] = requestWeek5;
    alarm2Week[5] = requestWeek6;
    alarm2Week[6] = requestWeek7;
    //write alarm color to variable
    alarm2Color = requestColor;
    //save alarm settings to eeprom
    EEPROM.begin(512);
    EEPROM.write(210, alarm2Mode);
    EEPROM.write(220, alarm2Time);
    EEPROM.write(230, alarm2Left);
    EEPROM.write(231, alarm2Right);
    unsigned long alarmColorSub = alarm2Color;
    for (uint8_t i =0; i<4; i++) {
      EEPROM.write(240 + i, alarmColorSub & 255);
      alarmColorSub = alarmColorSub / 256;
    }
    for (int i=0; i<7; i++) {
      EEPROM.write(250+i, alarm2Week[i]);
    }
    EEPROM.commit();
    EEPROM.end();
  }
}

void alarmFeedback() {
  if (mode == 8) {
    //format strings for web ui
    sMode = String(alarm1Mode);
    char bufferColor[6];
    char bufferHour[2];
    char bufferMinute[2];
    sTime = String(alarm1Time);
    sprintf(bufferColor, "%06x", (alarm1Color));
    sColor = bufferColor;
    sprintf(bufferHour, "%02d", (alarm1Left));
    sHour = bufferHour;
    sprintf(bufferMinute, "%02d", (alarm1Right));
    sMinute = bufferMinute;
    sWeek = String(alarm1Week[0]) + String(alarm1Week[1]) + String(alarm1Week[2]) + String(alarm1Week[3]) + String(alarm1Week[4]) + String(alarm1Week[5]) + String(alarm1Week[6]);
  }
  else if (mode == 9) {
    //format strings for web ui
    sMode = String(alarm2Mode);
    char bufferColor[6];
    char bufferHour[2];
    char bufferMinute[2];
    sTime = String(alarm2Time);
    sprintf(bufferColor, "%06x", (alarm2Color));
    sColor = bufferColor;
    sprintf(bufferHour, "%02d", (alarm2Left));
    sHour = bufferHour;
    sprintf(bufferMinute, "%02d", (alarm2Right));
    sMinute = bufferMinute;
    sWeek = String(alarm2Week[0]) + String(alarm2Week[1]) + String(alarm2Week[2]) + String(alarm2Week[3]) + String(alarm2Week[4]) + String(alarm2Week[5]) + String(alarm2Week[6]);
  }
}

void led() {
  if (mode == 1) {  //time
    if (modeLast != mode) {
      pulseDelay = 2000;
      Serial.println();
      Serial.print("--> mode 1");
      timePrint = true;
      for (int i = 0; i < 11; i++) {
        pixels.setPixelColor(i, colorTime);
        delay(changeAnimation);
        pixels.show();
      }
    } else {
      pulseDelay = 1000;
    }
    ntpBinary();
    if (hourLEDa == 1) {
      pixels.setPixelColor(0, colorTime);
    } else {
      pixels.setPixelColor(0, colorStandby);
    }
    if (modeLast != mode) { delay(changeAnimation); }
    pixels.show();
    if (hourLEDb == 1) {
      pixels.setPixelColor(1, colorTime);
    } else {
      pixels.setPixelColor(1, colorStandby);
    }
    if (modeLast != mode) { delay(changeAnimation); }
    pixels.show();
    if (hourLEDc == 1) {
      pixels.setPixelColor(2, colorTime);
    } else {
      pixels.setPixelColor(2, colorStandby);
    }
    if (modeLast != mode) { delay(changeAnimation); }
    pixels.show();
    if (hourLEDd == 1) {
      pixels.setPixelColor(3, colorTime);
    } else {
      pixels.setPixelColor(3, colorStandby);
    }
    if (modeLast != mode) { delay(changeAnimation); }
    pixels.show();
    if (hourLEDe == 1) {
      pixels.setPixelColor(4, colorTime);
    } else {
      pixels.setPixelColor(4, colorStandby);
    }
    if (modeLast != mode) { delay(changeAnimation); }
    pixels.show();
    if (minuteLEDf == 1) {
      pixels.setPixelColor(5, colorTime);
    } else {
      pixels.setPixelColor(5, colorStandby);
    }
    if (modeLast != mode) { delay(changeAnimation); }
    pixels.show();
    if (minuteLEDe == 1) {
      pixels.setPixelColor(6, colorTime);
    } else {
      pixels.setPixelColor(6, colorStandby);
    }
    if (modeLast != mode) { delay(changeAnimation); }
    pixels.show();
    if (minuteLEDd == 1) {
      pixels.setPixelColor(7, colorTime);
    } else {
      pixels.setPixelColor(7, colorStandby);
    }
    if (modeLast != mode) { delay(changeAnimation); }
    pixels.show();
    if (minuteLEDc == 1) {
      pixels.setPixelColor(8, colorTime);
    } else {
      pixels.setPixelColor(8, colorStandby);
    }
    if (modeLast != mode) { delay(changeAnimation); }
    pixels.show();
    if (minuteLEDb == 1) {
      pixels.setPixelColor(9, colorTime);
    } else {
      pixels.setPixelColor(9, colorStandby);
    }
    if (modeLast != mode) { delay(changeAnimation); }
    pixels.show();
    if (minuteLEDa == 1) {
      pixels.setPixelColor(10, colorTime);
    } else {
      pixels.setPixelColor(10, colorStandby);
    }
    if (modeLast != mode) { delay(changeAnimation); }
    pixels.show();
    modeLast = mode;
  }
  if (mode == 2) {  //week day
    Serial.println();
    Serial.print("--> mode 2");
    timePrint = true;
    if (change) {
      if (requestTime < 5) {  //mode 1 fallback time in request mode
        pulseDelay = 5000;
      } else {
        pulseDelay = requestTime * 1000;
      }
    } else {
      pulseDelay = 15000;  //mode 1 fallback time in automatic mode
    }
    for (int i = 0; i < 11; i++) {
      pixels.setPixelColor(i, colorWeek);
      delay(changeAnimation);
      pixels.show();
    }
    ntpBinary();
    if (weekLEDa == 1) {
      pixels.setPixelColor(0, colorWeek);
    } else {
      pixels.setPixelColor(0, colorStandby);
    }
    delay(changeAnimation);
    pixels.show();
    if (weekLEDb == 1) {
      pixels.setPixelColor(1, colorWeek);
    } else {
      pixels.setPixelColor(1, colorStandby);
    }
    delay(changeAnimation);
    pixels.show();
    if (weekLEDc == 1) {
      pixels.setPixelColor(2, colorWeek);
    } else {
      pixels.setPixelColor(2, colorStandby);
    }
    delay(changeAnimation);
    pixels.show();
    if (weekLEDd == 1) {
      pixels.setPixelColor(3, colorWeek);
    } else {
      pixels.setPixelColor(3, colorStandby);
    }
    delay(changeAnimation);
    pixels.show();
    if (weekLEDe == 1) {
      pixels.setPixelColor(4, colorWeek);
    } else {
      pixels.setPixelColor(4, colorStandby);
    }
    delay(changeAnimation);
    pixels.show();
    for (int i = 5; i < 11; i++) {
      pixels.setPixelColor(i, colorStandby);
      delay(changeAnimation);
      pixels.show();
    }
    change = false;
    modeLast = mode;
    mode = 1;
  }
  if (mode == 3) {  //date
    Serial.println();
    Serial.print("--> mode 3");
    timePrint = true;
    if (change) {
      if (requestTime < 5) {  //mode 1 fallback time in request mode
        pulseDelay = 5000;
      } else {
        pulseDelay = requestTime * 1000;
      }
    } else {
      pulseDelay = 15000;  //mode 1 fallback time in automatic mode
    }
    for (int i = 0; i < 11; i++) {
      pixels.setPixelColor(i, colorDate);
      delay(changeAnimation);
      pixels.show();
    }
    ntpBinary();
    if (dateType == 1) {
      if (monthLEDa == 1) {
        pixels.setPixelColor(0, colorDate);
      } else {
        pixels.setPixelColor(0, colorStandby);
      }
      delay(changeAnimation);
      pixels.show();
      if (monthLEDb == 1) {
        pixels.setPixelColor(1, colorDate);
      } else {
        pixels.setPixelColor(1, colorStandby);
      }
      delay(changeAnimation);
      pixels.show();
      if (monthLEDc == 1) {
        pixels.setPixelColor(2, colorDate);
      } else {
        pixels.setPixelColor(2, colorStandby);
      }
      delay(changeAnimation);
      pixels.show();
      if (monthLEDd == 1) {
        pixels.setPixelColor(3, colorDate);
      } else {
        pixels.setPixelColor(3, colorStandby);
      }
      delay(changeAnimation);
      pixels.show();
      if (monthLEDe == 1) {
        pixels.setPixelColor(4, colorDate);
      } else {
        pixels.setPixelColor(4, colorStandby);
      }
      delay(changeAnimation);
      pixels.show();
      if (dayLEDf == 1) {
        pixels.setPixelColor(5, colorDate);
      } else {
        pixels.setPixelColor(5, colorStandby);
      }
      delay(changeAnimation);
      pixels.show();
      if (dayLEDe == 1) {
        pixels.setPixelColor(6, colorDate);
      } else {
        pixels.setPixelColor(6, colorStandby);
      }
      delay(changeAnimation);
      pixels.show();
      if (dayLEDd == 1) {
        pixels.setPixelColor(7, colorDate);
      } else {
        pixels.setPixelColor(7, colorStandby);
      }
      delay(changeAnimation);
      pixels.show();
      if (dayLEDc == 1) {
        pixels.setPixelColor(8, colorDate);
      } else {
        pixels.setPixelColor(8, colorStandby);
      }
      delay(changeAnimation);
      pixels.show();
      if (dayLEDb == 1) {
        pixels.setPixelColor(9, colorDate);
      } else {
        pixels.setPixelColor(9, colorStandby);
      }
      delay(changeAnimation);
      pixels.show();
      if (dayLEDa == 1) {
        pixels.setPixelColor(10, colorDate);
      } else {
        pixels.setPixelColor(10, colorStandby);
      }
      delay(changeAnimation);
      pixels.show();
    }
    else if (dateType == 2) {
      if (dayLEDa == 1) {
        pixels.setPixelColor(0, colorDate);
      } else {
        pixels.setPixelColor(0, colorStandby);
      }
      delay(changeAnimation);
      pixels.show();
      if (dayLEDb == 1) {
        pixels.setPixelColor(1, colorDate);
      } else {
        pixels.setPixelColor(1, colorStandby);
      }
      delay(changeAnimation);
      pixels.show();
      if (dayLEDc == 1) {
        pixels.setPixelColor(2, colorDate);
      } else {
        pixels.setPixelColor(2, colorStandby);
      }
      delay(changeAnimation);
      pixels.show();
      if (dayLEDd == 1) {
        pixels.setPixelColor(3, colorDate);
      } else {
        pixels.setPixelColor(3, colorStandby);
      }
      delay(changeAnimation);
      pixels.show();
      if (dayLEDe == 1) {
        pixels.setPixelColor(4, colorDate);
      } else {
        pixels.setPixelColor(4, colorStandby);
      }
      delay(changeAnimation);
      pixels.show();
      if (monthLEDf == 1) {
        pixels.setPixelColor(5, colorDate);
      } else {
        pixels.setPixelColor(5, colorStandby);
      }
      delay(changeAnimation);
      pixels.show();
      if (monthLEDe == 1) {
        pixels.setPixelColor(6, colorDate);
      } else {
        pixels.setPixelColor(6, colorStandby);
      }
      delay(changeAnimation);
      pixels.show();
      if (monthLEDd == 1) {
        pixels.setPixelColor(7, colorDate);
      } else {
        pixels.setPixelColor(7, colorStandby);
      }
      delay(changeAnimation);
      pixels.show();
      if (monthLEDc == 1) {
        pixels.setPixelColor(8, colorDate);
      } else {
        pixels.setPixelColor(8, colorStandby);
      }
      delay(changeAnimation);
      pixels.show();
      if (monthLEDb == 1) {
        pixels.setPixelColor(9, colorDate);
      } else {
        pixels.setPixelColor(9, colorStandby);
      }
      delay(changeAnimation);
      pixels.show();
      if (monthLEDa == 1) {
        pixels.setPixelColor(10, colorDate);
      } else {
        pixels.setPixelColor(10, colorStandby);
      }
      delay(changeAnimation);
      pixels.show();
    }
    change = false;
    modeLast = mode;
    mode = 1;
  }
  if (mode == 4) {  //request special function
    if (change) {
      Serial.println();
      Serial.print("--> mode 4");
      timePrint = true;
      if (dayMode) {
        colorStandby = colorStandbyDay;
      }
      else if (nightMode) {
        colorStandby = colorStandbyNight;
      }
      if (requestTime < 5) {  //mode 1 fallback time in request mode
        pulseDelay = 5000;
      } else {
        pulseDelay = requestTime * 1000;
      }
      for (int i = 0; i < 11; i++) {
        pixels.setPixelColor(i, requestColor);
        delay(changeAnimation);
        pixels.show();
      }
      requestBinary();
      if (leftLEDa == 1) {
        pixels.setPixelColor(0, requestColor);
      } else {
        pixels.setPixelColor(0, colorStandby);
      }
      delay(changeAnimation);
      pixels.show();
      if (leftLEDb == 1) {
        pixels.setPixelColor(1, requestColor);
      } else {
        pixels.setPixelColor(1, colorStandby);
      }
      delay(changeAnimation);
      pixels.show();
      if (leftLEDc == 1) {
        pixels.setPixelColor(2, requestColor);
      } else {
        pixels.setPixelColor(2, colorStandby);
      }
      delay(changeAnimation);
      pixels.show();
      if (leftLEDd == 1) {
        pixels.setPixelColor(3, requestColor);
      } else {
        pixels.setPixelColor(3, colorStandby);
      }
      delay(changeAnimation);
      pixels.show();
      if (leftLEDe == 1) {
        pixels.setPixelColor(4, requestColor);
      } else {
        pixels.setPixelColor(4, colorStandby);
      }
      delay(changeAnimation);
      pixels.show();
      if (rightLEDf == 1) {
        pixels.setPixelColor(5, requestColor);
      } else {
        pixels.setPixelColor(5, colorStandby);
      }
      delay(changeAnimation);
      pixels.show();
      if (rightLEDe == 1) {
        pixels.setPixelColor(6, requestColor);
      } else {
        pixels.setPixelColor(6, colorStandby);
      }
      delay(changeAnimation);
      pixels.show();
      if (rightLEDd == 1) {
        pixels.setPixelColor(7, requestColor);
      } else {
        pixels.setPixelColor(7, colorStandby);
      }
      delay(changeAnimation);
      pixels.show();
      if (rightLEDc == 1) {
        pixels.setPixelColor(8, requestColor);
      } else {
        pixels.setPixelColor(8, colorStandby);
      }
      delay(changeAnimation);
      pixels.show();
      if (rightLEDb == 1) {
        pixels.setPixelColor(9, requestColor);
      } else {
        pixels.setPixelColor(9, colorStandby);
      }
      delay(changeAnimation);
      pixels.show();
      if (rightLEDa == 1) {
        pixels.setPixelColor(10, requestColor);
      } else {
        pixels.setPixelColor(10, colorStandby);
      }
      delay(changeAnimation);
      pixels.show();
      change = false;
      modeLast = mode;
      if (darkMode && !darkModeReset) {
        colorStandby = colorDark;
      }
      if (darkMode && darkModeReset) {
        change = true;
        mode = 10;
      } else {
        mode = 1;
      } 
      
    }
    else {
      mode = 1;  //go back to mode 1 if parameters for mode 4 have been invalid
    }
  }
  if (mode == 5) {  //request cycling function
    Serial.println();
    Serial.print("--> mode 5");
    timePrint = true;
    if (dayMode) {
      colorStandby = colorStandbyDay;
    }
    else if (nightMode) {
      colorStandby = colorStandbyNight;
    }
    int loopCount;
    if ((requestTime - loopAnimationEnd) < 2) {  //mode 1 fallback time in request mode
      loopCount = 1;
      pulseDelay = 7000;
    } else {
      if ((requestTime - loopAnimationEnd) % 2 == 0) {
        loopCount = (requestTime - loopAnimationEnd) / 2;
        pulseDelay = requestTime * 1000;
      }
      else {
        loopCount = ((requestTime - loopAnimationEnd) + 1) / 2;
        pulseDelay = (requestTime + 1) * 1000;
      }
    }
    for (int i = 0; i < loopCount; i++) {
      for (int j = 0; j < 11; j++) {
        pixels.setPixelColor(j, requestColor);
        delay(loopAnimation);
        pixels.show();
      }
      delay(loopAnimationDelay);
      if (i < (loopCount - 1)) {  //stay in requested color
        for (int j = 0; j < 11; j++) {
          pixels.setPixelColor(j, colorStandby);
          delay(loopAnimation);
          pixels.show();
        }
        delay(loopAnimationDelay);
      }
    }
    change = false;
    modeLast = mode;
    if (darkMode && !darkModeReset) {
      colorStandby = colorDark;
    }
    if (darkMode && darkModeReset) {
      change = true;
      mode = 10;
    } else {
      mode = 1;
    } 
  }
  if (mode == 6) {  //request day mode;
    if (change) {
      Serial.println();
      Serial.print("--> mode 6");
      timePrint = true;
      if (darkMode) {
        colorStandby = colorDark;
        colorTime = colorDark;
        colorWeek = colorDark;
        colorDate = colorDark;
      } else {
        colorStandby = colorStandbyDay;
        colorTime = colorTimeDay;
        colorWeek = colorWeekDay;
        colorDate = colorDateDay;
      }
      dayMode = true;
      nightMode = false;
      change = false;
      modeLast = mode;  //mode for animation, 1 for switchover
      mode = 1;
    }
    else {
      mode = 1;
    }
  }
  if (mode == 7) {  //request night mode;
    if (change) {
      Serial.println();
      Serial.print("--> mode 7");
      timePrint = true;
      if (darkMode) {
        colorStandby = colorDark;
        colorTime = colorDark;
        colorWeek = colorDark;
        colorDate = colorDark;
      } else {
        colorStandby = colorStandbyNight;
        colorTime = colorTimeNight;
        colorWeek = colorWeekNight;
        colorDate = colorDateNight;
      }
      dayMode = false;
      nightMode = true;
      change = false;
      modeLast = mode;  //mode for animation, 1 for switchover
      mode = 1;
    }
    else {
      mode = 1;
    }
  }
  if (mode == 8) {  //request alarm function
    if (change) {
      Serial.println();
      Serial.print("--> mode 8");
      timePrint = true;
      //format strings for serial console
      String sMode;
      char bufferColor[6];
      char bufferHour[2];
      char bufferMinute[2];
      if (alarm1Mode == 0) {
        sMode = "off";
      }
      else if (alarm1Mode == 1) {
        sMode = "time";
      }
      else if (alarm1Mode == 2) {
        sMode = "cycle";
      }
      String sTime = String(alarm1Time);
      sprintf(bufferColor, "%06x", (alarm1Color));
      String sColor = bufferColor;
      sprintf(bufferHour, "%02d", (alarm1Left));
      String sHour = bufferHour;
      sprintf(bufferMinute, "%02d", (alarm1Right));
      String sMinute = bufferMinute;
      String sWeek = String(alarm1Week[0]) + String(alarm1Week[1]) + String(alarm1Week[2]) + String(alarm1Week[3]) + String(alarm1Week[4]) + String(alarm1Week[5]) + String(alarm1Week[6]);
      String alarmSettings = sMode + ",0x" + sColor + "," + sHour + ":" + sMinute + "," + sWeek + "," + sTime + "sec";
      //serial console output
      Serial.println();
      Serial.print("alarm settings (mode/time/color/time/alarm time): ");
      Serial.println(alarmSettings);
      Serial.print("--> mode 1");
      change = false;
      mode = 1;
    }
    else {
      mode = 1;  //go back to mode 1 if parameters for mode 4 have been invalid
    }
  }
  if (mode == 9) {  //request alarm function
    if (change) {
      Serial.println();
      Serial.print("--> mode 9");
      timePrint = true;
      //format strings for serial console
      String sMode;
      char bufferColor[6];
      char bufferHour[2];
      char bufferMinute[2];
      if (alarm1Mode == 0) {
        sMode = "off";
      }
      else if (alarm1Mode == 1) {
        sMode = "time";
      }
      else if (alarm1Mode == 2) {
        sMode = "cycle";
      }
      String sTime = String(alarm2Time);
      sprintf(bufferColor, "%06x", (alarm2Color));
      String sColor = bufferColor;
      sprintf(bufferHour, "%02d", (alarm2Left));
      String sHour = bufferHour;
      sprintf(bufferMinute, "%02d", (alarm2Right));
      String sMinute = bufferMinute;
      String sWeek = String(alarm2Week[0]) + String(alarm2Week[1]) + String(alarm2Week[2]) + String(alarm2Week[3]) + String(alarm2Week[4]) + String(alarm2Week[5]) + String(alarm2Week[6]);
      String alarmSettings = sMode + ",0x" + sColor + "," + sHour + ":" + sMinute + "," + sWeek + "," + sTime + "sec";
      //serial console output
      Serial.println();
      Serial.print("alarm settings (mode/time/color/time/alarm time): ");
      Serial.println(alarmSettings);
      Serial.print("--> mode 1");
      change = false;
      mode = 1;
    }
    else {
      mode = 1;  //go back to mode 1 if parameters for mode 4 have been invalid
    }
  }
  if (mode == 10) {  //request to exit from dark mode;
    if (change) {
      Serial.println();
      Serial.print("--> mode 10");
      timePrint = true;
      if (dayMode) {
        colorStandby = colorStandbyDay;
        colorTime = colorTimeDay;
        colorWeek = colorWeekDay;
        colorDate = colorDateDay;
      }
      else if (nightMode) {
        colorStandby = colorStandbyNight;
        colorTime = colorTimeNight;
        colorWeek = colorWeekNight;
        colorDate = colorDateNight;
      }
      darkMode = false;
      change = false;
      modeLast = mode;  //mode for animation, 1 for switchover
      mode = 1;
    }
    else {
      mode = 1;
    }
  }
  if (mode == 11) {  //request to enter into dark mode;
    if (change) {
      Serial.println();
      Serial.print("--> mode 11");
      timePrint = true;
      colorStandby = colorDark;
      colorTime = colorDark;
      colorWeek = colorDark;
      colorDate = colorDark;
      darkMode = true;
      change = false;
      modeLast = mode;  //mode for animation, 1 for switchover
      mode = 1;
    }
    else {
      mode = 1;
    }
  }
}

void WiFiConnect() {
  for (int i = 0; i < 11; i++) {
    pixels.setPixelColor(i, colorWiFiCheck);
    delay(changeAnimation);
    pixels.show();
  }
  for (int i = 0; i < 11; i++) {
    pixels.setPixelColor(i, colorWiFiSearch);
    delay(changeAnimation);
    pixels.show();
  }
  int connectCountBlink = 0;
  int connectCountSum = 0;
  Serial.println();
  Serial.println();
  Serial.print("--> connecting to ");
  Serial.print(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    connectCountBlink++;
    pixels.setPixelColor(connectCountBlink - 2, colorWiFiCheck);
    pixels.show();
    delay(1000);
    Serial.print(".");
    if (connectCountBlink > 5) {
      connectCountSum++;
      if (connectCountSum < 7) {
        pixels.setPixelColor(11 - connectCountSum, colorWiFiCheck);
        pixels.show();
      } else {
        pixels.setPixelColor(17 - connectCountSum, colorWiFiSearch);
        pixels.show();
      }
      connectCountBlink = 1;
      for (int i = 0; i < 5; i++) {
        pixels.setPixelColor(i, colorWiFiSearch);
        pixels.show();
      }
    }
    if (connectCountSum == 12) {
      Serial.println("failed");
      Serial.println("--> restart MCU now");
      delay(1000);
      ESP.restart();
    }
    mode = 0;
  }
  Serial.print("connected with IP ");
  Serial.println(WiFi.localIP());
  modeLast = mode;
  mode = 1;
  for (int i = 0; i < 11; i++) {
    pixels.setPixelColor(i, colorWiFiFound);
    delay(changeAnimation);
    pixels.show();
  }
  //synchronize milliseconds to timestamp (except for first boot run)
  if (boot){
    ntpSynchronize();
  }
}

void daylightOffsetSet() {
  Serial.println();
  Serial.println("--> set daylight saving offset");
  if (ntpMonth <= 2 || ntpMonth >= 11) {  //force winter time bewtween november and february
    daylightOffset = 0;
  }
  else if (ntpMonth >=4 && ntpMonth <= 9) {  //force summer time between april and september
    daylightOffset = daylightOffsetValue;
  }
  else if (ntpMonth == 3) {  //if month is march
    if (ntpDay < 25) {  //if it's 24th of march or lower
      daylightOffset = 0;
    }
    else {  //if it's 25th of march or higher
      int remainingDaysMar = 31 - ntpDay;  //calculate remaining days of month
      Serial.println(remainingDaysMar);
      if (ntpWeek == 7) {  //check for sunday
        if (ntpHour >=2) {  //check for time
          daylightOffset = daylightOffsetValue;
        }
      }
      else if (ntpWeek + remainingDaysMar >=7) {  //check for incoming sunday
        daylightOffset = 0;
      }
      else {  //if no incoming sunday
        daylightOffset = daylightOffsetValue;
      }
    }
  }
  else if (ntpMonth == 10) {  //if month is october
    if (ntpDay < 25) {  //if it's 24th or october or lower
      daylightOffset = daylightOffsetValue;
    }
    else {  //if it's 25th of october or higher
      int remainingDaysOct = 31 - ntpDay;  //calculate remaining days of month
      if (ntpWeek == 7) {  //check for sunday
        if (ntpHour >=2) {  //check for time
          daylightOffset = 0;
        }
      }
      else if (ntpWeek + remainingDaysOct >=7) {  //check for incoming sunday
        daylightOffset = daylightOffsetValue;
      }
      else {  //if no incoming sunday
        daylightOffset = 0;
      }
    }
  }
  timePrint = true;
}
