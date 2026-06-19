void updateStats(
  float tempF,
  float pressure) {

  Serial.println("updateStats()");

  // Initialize highs and lows the first time
  if (currentDay == -1) {

    currentDay = 0;

    dailyHigh = tempF;
    dailyLow = tempF;
  }

  if (tempF > dailyHigh)
    dailyHigh = tempF;

  if (tempF < dailyLow)
    dailyLow = tempF;

  if (millis() - lastPressureLog > 300000UL) {

    Serial.println("Saving pressure sample");

    lastPressureLog = millis();

    pressureHistory[pressureIndex] = pressure;

    pressureIndex++;

    pressureIndex %= 12;

    savePrefsToSD();
  }
}

float getPressureChange() {

  int oldest = pressureIndex;
  int newest = (pressureIndex + 11) % 12;

  float change =
    pressureHistory[newest] -
    pressureHistory[oldest];

  return change;
}