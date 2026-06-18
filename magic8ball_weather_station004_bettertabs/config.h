// ======================
// Pins
// ======================

#define SDA_PIN 6
#define SCL_PIN 5
#define BUTTON_PIN 4
#define BUTTON2_PIN 7
#define SD_CS 10
#define SD_MOSI 11
#define SD_SCK 12
#define SD_MISO 13

// ======================
// Objects
// ======================

Adafruit_SSD1306 display(128, 64, &Wire, -1);
Adafruit_BMP280 bmp;

Preferences prefs;

SdFat SD;
File logFile;

TinyGPSPlus gps;

float currentTemperature = 0;

float currentAltitude = 0;


// ======================
// Screen Modes
// ======================

enum ScreenMode {
  WEATHER_SCREEN,
  STATS_SCREEN,
  DEVICE_SCREEN,
  MAGIC8_SCREEN,
  GPS_SCREEN,
  RIDE_SCREEN
};

ScreenMode currentScreen = WEATHER_SCREEN;

// ======================
// Button Variables
// ======================

bool lastButtonState = HIGH;
bool lastButton2State = HIGH;

unsigned long lastDebounce = 0;
unsigned long lastButton2Debounce = 0;


// ======================
// Sensor Variables
// ======================

bool sensorOK = false;


// ======================
// Weather Variables
// ======================

float dailyHigh = -999;
float dailyLow = 999;
int currentDay = -1;
float pressureHistory[12];
int pressureIndex = 0;
unsigned long lastPressureLog = 0;


// ======================
// Display Variables
// ======================

unsigned long lastActivity = 0;
bool displaySleeping = false;


// ======================
// Magic 8 Ball Variables
// ======================

unsigned long consultationCount = 0;


// ======================
// Device Screen Variables
// ======================

unsigned long deviceRotateTimer = 0;
int deviceInfoPage = 0;


// ======================
// WiFi Variables
// ======================

String lastSSID = "NO WIFI";


// ======================
// WiFi credentials
// ======================

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


// ======================
// Time
// ======================

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -5 * 3600;
const int daylightOffset_sec = 3600;

//RIDE Variables
bool rideRecording = false;
unsigned long rideStartTime = 0;
uint32_t pointsLogged = 0;
char filename[20];
// Preserve last ride information
unsigned long rideElapsedTime = 0;
uint32_t lastRidePoints = 0;