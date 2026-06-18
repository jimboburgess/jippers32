void handleButton() {

  bool buttonState =
    digitalRead(BUTTON_PIN);

  if (
    buttonState == LOW && lastButtonState == HIGH && millis() - lastDebounce > 200) {

    // Wake display if sleeping
    if (displaySleeping) {

      display.ssd1306_command(
        SSD1306_DISPLAYON);

      displaySleeping = false;

      lastActivity = millis();
      lastDebounce = millis();
      lastButtonState = buttonState;

      return;
    }

    lastActivity = millis();

    currentScreen =
      (ScreenMode)((currentScreen + 1) % 5);

    lastDebounce =
      millis();
  }

  lastButtonState =
    buttonState;
}
void handleButton2() {

  bool buttonState =
    digitalRead(BUTTON2_PIN);

  if (
    buttonState == LOW && lastButton2State == HIGH && millis() - lastButton2Debounce > 200) {
    // Wake display if sleeping
    if (displaySleeping) {

      display.ssd1306_command(
        SSD1306_DISPLAYON);

      displaySleeping = false;

      lastActivity = millis();
      lastButton2Debounce = millis();
      lastButton2State = buttonState;

      return;
    }
    lastActivity = millis();

    switch (currentScreen) {

      case WEATHER_SCREEN:
        syncTime();
        break;

      case STATS_SCREEN:
        resetStats();
        break;

      case MAGIC8_SCREEN:
        startMagic8Ball();
        break;

      case GPS_SCREEN:
        gpsPage++;
        gpsPage %= 3;

        break;
    }

    lastButton2Debounce = millis();
  }

  lastButton2State = buttonState;
}