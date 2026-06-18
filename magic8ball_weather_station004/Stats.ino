void showStats() {

  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);

  // High
  display.setCursor(0, 0);
  display.print("High:");
  display.print(dailyHigh, 1);

  // Low
  display.setCursor(0, 22);
  display.print("Low :");
  display.print(dailyLow, 1);

  // Pressure change
  float pressureChange =
    getPressureChange();

  float deltaInHg =
    pressureChange * 0.02953;

  display.setCursor(0, 44);

  if (deltaInHg > 0)
    display.print("+");

  display.print(deltaInHg, 2);
  display.print("inHg");

  display.display();
}
void resetStats() {

  float tempF =
    bmp.readTemperature() * 9.0 / 5.0 + 32.0;

  dailyHigh = tempF;
  dailyLow = tempF;

  for (int i = 0; i < 12; i++) {
    pressureHistory[i] =
      bmp.readPressure() / 100.0;
  }

  display.clearDisplay();
  display.setCursor(34, 24);
  display.print("RESET");
  display.display();

  delay(1000);
}