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

    deviceInfoPage %= 5;
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
        "Enjoy the taste of");

      display.setCursor(0, 48);
      display.println(
        "Kool! cigarettes");

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
        "Should you give");

      display.setCursor(0, 48);

      display.println(
        "James $5, perhaps?");

      break;

    case 4:
      display.print(" FIX ");

      if (gps.location.age() < 2000)
        display.println(" OK");
      else
        display.println(" OLD");

      display.setCursor(0, 48);
      display.print("SAT ");
      display.println(gps.satellites.value());
      break;
  }

  display.display();
}