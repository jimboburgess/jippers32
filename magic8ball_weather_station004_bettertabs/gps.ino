

HardwareSerial GPSSerial(1);

bool gpsOK = false;
bool gpsTimeValid = false;

int gpsPage = 0;

float averageSpeed = 0.0;

void setupGPS() {

  GPSSerial.begin(
    9600,
    SERIAL_8N1,
    8,
    9);

  Serial.println(
    "GPS started");
}


void serviceGPS() {

  while (GPSSerial.available()) {

    gps.encode(GPSSerial.read());

  }

  if (gps.altitude.isValid()) {

    currentAltitude =
      gps.altitude.feet();

  }

}
void showGPS() {

  switch (gpsPage) {

    case 0:
      showGPSCoordinates();
      break;

    case 1:
      showGPSSpeedAltitude();
      break;

    case 2:
      showGPSDiagnostics();
      break;
  }
}
void showGPSCoordinates() {

  display.clearDisplay();

  display.setTextSize(1);

  if (gps.location.isValid()) {

    display.setCursor(0,0);
    display.println("GPS LOCK");

    display.setCursor(0,18);
    display.println(
      gps.location.lat(),
      4);

    display.setCursor(0,34);
    display.println(
      gps.location.lng(),
      4);

    display.setCursor(0,52);

    display.print("SAT ");
    display.println(
      gps.satellites.value());

  } else {

    display.setCursor(0,0);
    display.println("SEARCHING");

    display.setCursor(0,24);
    display.println("WAITING...");

    display.setCursor(0,52);

    display.print("SATELLITES ");
    display.println(
      gps.satellites.value());
  }

  display.display();
}
void showGPSSpeedAltitude() {

  display.clearDisplay();

  display.setTextSize(2);

  display.setCursor(0,0);

  display.print("SPD ");
  display.println(
    gps.speed.mph(),
    1);

  display.setCursor(0,24);

  display.print("ALT ");
  display.println(
    gps.altitude.feet(),
    0);

  display.setCursor(0,48);

  display.print("AVG ");
  display.println(
    averageSpeed,
    1);

  display.display();
}

void showGPSDiagnostics() {

  display.clearDisplay();

  display.setTextSize(1);

  display.setCursor(0,0);

  display.print("HD ");
  display.println(
    gps.hdop.hdop(),
    1);

  display.setCursor(0,24);

  display.print("SATELLITES ");
  display.println(
    gps.satellites.value());

  display.setCursor(0,48);

  display.print("Last Fix ");

  display.print(
    gps.location.age()/1000);

  display.println("sec");

  display.display();
}

void showGPSTest() {

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);

  if (gps.location.isValid()) {

    display.println(
      "GPS LOCK");

    display.setCursor(0,
      16);

    display.print(
      "SATELLITES ");

    display.println(
      gps.satellites.value());

    display.setCursor(
      0,
      32);

    display.print(
      "LATITUDE ");

    display.println(
      gps.location.lat(),
      4);

    display.setCursor(
      0,
      48);

    display.print(
      "LONGITUDE ");

    display.println(
      gps.location.lng(),
      4);

  } else {

    display.println(
      "SEARCHING");

    display.setCursor(
      0,
      16);

    display.print(
      "SAT ");

    display.println(
      gps.satellites.value());

    display.setCursor(
      0,
      32);

    display.println(
      "WAITING...");
  }

  display.display();
}