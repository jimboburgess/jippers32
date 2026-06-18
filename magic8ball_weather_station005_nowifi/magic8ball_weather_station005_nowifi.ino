#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP280.h>
#include <Preferences.h>
#include <SPI.h>
#include <SdFat.h>
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>
#include <config.h>

void setup() {

  Serial.begin(115200);

  setCpuFrequencyMhz(240);

  prefs.begin("weather", false);

  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(400000);

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


  dailyHigh = prefs.getFloat("high", -999);

  dailyLow = prefs.getFloat("low", 999);

  pressureIndex = prefs.getInt("pindex", 0);

  consultationCount =
    prefs.getULong("consults", 0);

  prefs.getBytes(
    "history",
    pressureHistory,
    sizeof(pressureHistory));


  initializeSD();

  setupGPS();
}


void loop() {

  if (digitalRead(BUTTON_PIN) == LOW)
  Serial.println("BUTTON1");

if (digitalRead(BUTTON2_PIN) == LOW)
  Serial.println("BUTTON2");

  serviceGPS();

  handleButton();
  handleButton2();

  static unsigned long lastDisplayUpdate = 0;

  if (millis() - lastDisplayUpdate >= 200) {

    lastDisplayUpdate = millis();

    switch (currentScreen) {

      case WEATHER_SCREEN:
        showWeather();
        break;

      case RIDE_SCREEN:
        showRide();
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

  }  // <-- closes the if

  updateLogging();

  if (!displaySleeping && millis() - lastActivity > 60000) {

    display.ssd1306_command(SSD1306_DISPLAYOFF);

    displaySleeping = true;
  }

}  // <-- closes loop()