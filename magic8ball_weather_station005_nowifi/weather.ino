void showWeather() {

  static unsigned long lastUpdate = 0;

  if (millis() - lastUpdate < 250)
    return;

  lastUpdate = millis();

  float tempC = bmp.readTemperature();
  float tempF = tempC * 9.0 / 5.0 + 32.0;
  currentTemperature = tempF;

  float press_hPa = bmp.readPressure() / 100.0;
  float press_inHg = press_hPa * 0.02953;

  updateStats(tempF, press_hPa);

  char timeStr[16] = "-- -- AM";

  if (gps.time.isValid()) {

    // GPS time is UTC
    int year, month, day;
    int hour, minute, second;

    getLocalDateTime(
      year,
      month,
      day,
      hour,
      minute,
      second);

    // Wrap around midnight
    if (hour < 0)
      hour += 24;

    if (hour >= 24)
      hour -= 24;

    bool pm = hour >= 12;

    hour %= 12;

    if (hour == 0)
      hour = 12;

    // Blink colon every second
    char colon = (second & 1) ? ':' : ' ';

    sprintf(
      timeStr,
      "%02d%c%02d %s",
      hour,
      colon,
      minute,
      pm ? "PM" : "AM");
  }

  display.clearDisplay();

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