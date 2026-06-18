unsigned long elapsedSeconds =
  (millis() - rideStartTime) / 1000;

int hours = elapsedSeconds / 3600;

int minutes =
  (elapsedSeconds % 3600) / 60;

int seconds =
  elapsedSeconds % 60;


void showRide() {

  display.clearDisplay();

  display.setTextSize(1);

  display.setCursor(0, 0);

  display.println("RIDE STATUS");

  display.drawLine(
    0,
    10,
    127,
    10,
    SSD1306_WHITE);

  unsigned long elapsedSeconds;

  uint32_t displayedPoints;

  if (rideRecording) {

    elapsedSeconds =
      (millis() - rideStartTime) / 1000;

    displayedPoints =
      pointsLogged;

  } else {

    elapsedSeconds =
      rideElapsedTime;

    displayedPoints =
      lastRidePoints;
  }


  int hours =
    elapsedSeconds / 3600;

  int minutes =
    (elapsedSeconds % 3600) / 60;

  int seconds =
    elapsedSeconds % 60;


  display.setCursor(0, 18);

  if (rideRecording)
    display.println("REC");
  else
    display.println("STOPPED");


  display.setCursor(0, 32);

  display.println(filename);


  display.setCursor(0, 46);

  display.printf(
    "%02d:%02d:%02d",
    hours,
    minutes,
    seconds);


  display.setCursor(80, 46);

  display.print(displayedPoints);


  display.display();
}