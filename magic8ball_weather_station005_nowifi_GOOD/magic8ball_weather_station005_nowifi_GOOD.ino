#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP280.h>
#include <SPI.h>
#include <SdFat.h>
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>
#include <config.h>

void setup() {

  Serial.begin(115200);

  setCpuFrequencyMhz(80);

  initializeSD();

  loadPrefsFromSD();

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

  setupGPS();
}


void loop() {

  handleButton();
  handleButton2();

  static unsigned long lastDisplayUpdate = 0;

  if (millis() - lastDisplayUpdate >= 200) {

    lastDisplayUpdate = millis();

    switch (currentScreen) {


      case GPS_SCREEN:
        showGPS();
        break;

      case DEVICE_SCREEN:
        showDevice();
        break;

      case WEATHER_SCREEN:
        showWeather();
        break;

      case RIDE_SCREEN:
        showRide();
        break;

      case STATS_SCREEN:
        showStats();
        break;

      case MAGIC8_SCREEN:
        showMagic8Ball();
        break;
    }
  }

  static unsigned long lastSensorUpdate = 0;

  if (millis() - lastSensorUpdate > 5000) {

    lastSensorUpdate = millis();

    currentTemperature =
      bmp.readTemperature() * 9.0 / 5.0 + 29.0;

    float pressure =
      bmp.readPressure() / 100.0;

    updateStats(
      currentTemperature,
      pressure);
  }

  serviceGPS();


  static unsigned long lastLogCheck = 0;

  if (millis() - lastLogCheck >= 100) {

    lastLogCheck = millis();

    updateLogging();
  }

  if (!displaySleeping && millis() - lastActivity > 600000) {

    display.ssd1306_command(SSD1306_DISPLAYOFF);

    displaySleeping = true;
  }
}