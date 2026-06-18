void updateStats(
  float tempF,
  float pressure) {

  struct tm timeinfo;

  if (!getLocalTime(&timeinfo))
    return;

  if (timeinfo.tm_mday != currentDay) {

    currentDay = timeinfo.tm_mday;

    dailyHigh = tempF;
    dailyLow = tempF;
  }

  if (tempF > dailyHigh)
    dailyHigh = tempF;

  if (tempF < dailyLow)
    dailyLow = tempF;

  prefs.putFloat(
    "high",
    dailyHigh);

  prefs.putFloat(
    "low",
    dailyLow);

  if (
    millis() - lastPressureLog
    > 300000UL) {

    lastPressureLog = millis();

    pressureHistory[pressureIndex] = pressure;

    pressureIndex++;

    pressureIndex %= 12;
    prefs.putBytes(
      "history",
      pressureHistory,
      sizeof(pressureHistory));

    prefs.putInt(
      "pindex",
      pressureIndex);
  }
}
float getPressureChange() {

  int oldest =
    pressureIndex;

  int newest =
    (pressureIndex + 11) % 12;

  return pressureHistory[newest] - pressureHistory[oldest];
}