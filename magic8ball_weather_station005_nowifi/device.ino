void showDevice() {

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Rotate bottom section every 5 seconds
  if (
    millis() - deviceRotateTimer
    > 5000) {

    deviceRotateTimer =
      millis();

    deviceInfoPage++;

    deviceInfoPage %= 4;
  }

  // Top section
  display.setCursor(0, 0);
  display.print("BATTERY");

  display.setCursor(70, 0);
  display.print(readBatteryVoltage(), 2);
  display.println("V");

  display.drawLine(
    0, 10,
    127, 10,
    SSD1306_WHITE);

  // Middle section
  unsigned long seconds =
    millis() / 1000;

  int days =
    seconds / 86400;

  int hours =
    (seconds % 86400) / 3600;

  display.setCursor(0, 16);
  display.print("UPTIME");

  display.setCursor(70, 16);
  display.print(days);
  display.print("D ");
  display.print(hours);
  display.print("H");

  display.drawLine(
    0, 26,
    127, 26,
    SSD1306_WHITE);

  // Bottom section
  display.setCursor(0, 34);

  switch (deviceInfoPage) {

    case 0:

      display.println(
        "Eat more peanut butter!");

      display.setCursor(0, 48);
      display.println(
        "Hello Guv!");

      break;

    case 1:

      display.println(
        "CONSULTATIONS");

      display.setCursor(0, 48);

      display.println(
        consultationCount);

      break;

    case 2:

      display.println(
        "SENSORS");

      display.setCursor(0, 48);

      display.println(
        getSensorStatus());

      break;

    case 3:

      display.println(
        "CLOCK");

      display.setCursor(0, 48);

      display.println(
        "TIME OK");

      break;
  }

  display.display();
}