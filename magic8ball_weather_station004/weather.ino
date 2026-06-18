void showWeather() {

  static unsigned long lastUpdate = 0;

  if (millis() - lastUpdate < 1000)
    return;

  lastUpdate = millis();

  float tempC = bmp.readTemperature();
  float tempF = tempC * 9.0 / 5.0 + 32.0;

  float press_hPa = bmp.readPressure() / 100.0;
  float press_inHg = press_hPa * 0.02953;

  updateStats(tempF,press_hPa
);

  struct tm timeinfo;
  char timeStr[16] = "-- -- AM";

  if (getLocalTime(&timeinfo)) {

    int hour = timeinfo.tm_hour;
    bool pm = hour >= 12;

    hour %= 12;

    if (hour == 0)
      hour = 12;

    char colon =
      (timeinfo.tm_sec % 2) ? ':' : ' ';

    sprintf(
      timeStr,
      "%02d%c%02d %s",
      hour,
      colon,
      timeinfo.tm_min,
      pm ? "PM" : "AM"
    );
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);

  display.setCursor(10, 0);
  display.print(timeStr);

  display.setCursor(10, 24);
  display.print(tempF, 1);
  display.print(" F");

  display.setCursor(10, 46);
  display.print(press_inHg, 2);
  display.print("inHg");

  display.display();
}
