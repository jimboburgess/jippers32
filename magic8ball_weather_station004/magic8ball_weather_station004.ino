#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP280.h>
#include <WiFi.h>
#include <time.h>
#include <Preferences.h>
#include <SPI.h>
#include <SdFat.h>

SdFat SD;
File logFile;

#define SDA_PIN 6
#define SCL_PIN 5
#define BUTTON_PIN 4
#define BUTTON2_PIN 7
#define SD_CS 10
#define SD_MOSI 11
#define SD_SCK 12
#define SD_MISO 13

Adafruit_SSD1306 display(128, 64, &Wire, -1);
Adafruit_BMP280 bmp;

bool sensorOK = false;

enum ScreenMode {
  WEATHER_SCREEN,
  STATS_SCREEN,
  DEVICE_SCREEN,
  MAGIC8_SCREEN,
  GPS_SCREEN
};

ScreenMode currentScreen = WEATHER_SCREEN;
//Button Variables
bool lastButtonState = HIGH;
unsigned long lastDebounce = 0;
bool lastButton2State = HIGH;
unsigned long lastButton2Debounce = 0;

// WiFi credentials
struct WiFiNetwork {
  const char* ssid;
  const char* password;
};

WiFiNetwork networks[] = {
  { "Verizon_H4ZMJS", "chalk-hall7-wed" },
  { "Salem Swamp 2.4", "bag12345" },
  { "Edge network", "vjjb1674" }
};

const int networkCount =
  sizeof(networks) / sizeof(networks[0]);

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -5 * 3600;
const int daylightOffset_sec = 3600;

float dailyHigh = -999;
float dailyLow = 999;

int currentDay = -1;
float pressureHistory[12];
int pressureIndex = 0;
unsigned long lastPressureLog = 0;

unsigned long lastActivity = 0;
bool displaySleeping = false;

Preferences prefs;
unsigned long consultationCount = 0;

String lastSSID = "NO WIFI";

unsigned long deviceRotateTimer = 0;
int deviceInfoPage = 0;
//lastActivity = millis();

void setup() {

  Serial.begin(115200);

  setCpuFrequencyMhz(80);
  prefs.begin("weather", false);

  Wire.begin(SDA_PIN, SCL_PIN);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  lastActivity = millis();
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (1)
      ;
  }


  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  sensorOK = bmp.begin(0x76);
  Serial.print("BMP280 = ");
  Serial.println(sensorOK);

  dailyHigh =
    prefs.getFloat(
      "high",
      -999);

  dailyLow =
    prefs.getFloat(
      "low",
      999);

  pressureIndex =
    prefs.getInt(
      "pindex",
      0);

  consultationCount =
    prefs.getULong(
      "consults",
      0);

  lastSSID =
    prefs.getString(
      "ssid",
      "NO WIFI");

  prefs.getBytes(
    "history",
    pressureHistory,
    sizeof(pressureHistory));

  SPI.begin(SD_SCK, SD_MISO, SD_MOSI);

  Serial.println("Initializing SD card...");

  SPI.begin(SD_SCK, SD_MISO, SD_MOSI);

  if (!SD.begin(SD_CS)) {
    Serial.println("SD initialization failed!");
  } else {
    Serial.println("SD card initialized.");
  }
  
  logFile = sd.open("/ride.csv", FILE_WRITE);

  if (logFile) {

    // Header row
    logFile.println(
      "Time,Latitude,Longitude,Speed,Altitude,Temperature");

    logFile.flush();

    Serial.println("Log file opened.");
  }
  setupGPS();

  connectWiFi();

  configTime(
    gmtOffset_sec,
    daylightOffset_sec,
    ntpServer);

  struct tm timeinfo;

  if (getLocalTime(&timeinfo, 10000)) {

    Serial.println("Time synchronized");

  } else {

    Serial.println("Time sync failed");
  }

  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}


void loop() {

  serviceGPS();

  handleButton();

  handleButton2();

  switch (currentScreen) {

    case WEATHER_SCREEN:
      showWeather();
      break;

    case STATS_SCREEN:
      showStats();
      break;

    case GPS_SCREEN:
      showGPS();
      break;

    case DEVICE_SCREEN:
      showDevice();
      break;

    case MAGIC8_SCREEN:
      showMagic8Ball();
      break;
  }

if (gps.location.isValid()) {

    logFile.print(gps.time.hour());
    logFile.print(":");
    logFile.print(gps.time.minute());
    logFile.print(":");
    logFile.print(gps.time.second());

    logFile.print(",");

    logFile.print(gps.location.lat(), 6);
    logFile.print(",");

    logFile.print(gps.location.lng(), 6);
    logFile.print(",");

    logFile.print(gps.speed.mph());
    logFile.print(",");

    logFile.print(currentAltitude);
    logFile.print(",");

    logFile.println(currentTemperature);

    logFile.flush();
}

  if (!displaySleeping && millis() - lastActivity > 30000) {

    display.ssd1306_command(
      SSD1306_DISPLAYOFF);

    displaySleeping = true;
  }

}