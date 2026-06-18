// ======================
// SD Card Logging
// ======================

unsigned long lastLogTime = 0;
unsigned long lastFlushTime = 0;

// ======================
// Initialize SD Card
// ======================

void initializeSD() {

  Serial.println("Initializing SD card...");

  SPI.begin(
    SD_SCK,
    SD_MISO,
    SD_MOSI);

  if (!SD.begin(SD_CS)) {

    Serial.println("SD initialization failed!");
    return;
  }

  Serial.println("SD card initialized.");
}
void startRide() {

  if (rideRecording)
    return;

  int rideNumber = 1;

  while (true) {

    sprintf(
      filename,
      "/ride%04d.csv",
      rideNumber);

    if (!SD.exists(filename))
      break;

    rideNumber++;
  }

  logFile = SD.open(
    filename,
    FILE_WRITE);

  if (!logFile) {

    Serial.println("Couldn't open ride file");
    return;
  }

  logFile.println(
    "Time,Latitude,Longitude,SpeedMPH,AltitudeFt,TemperatureF");

  logFile.flush();

  rideRecording = true;

  rideStartTime = millis();

  pointsLogged = 0;

  Serial.print("Started ");

  Serial.println(filename);
}
// ======================
// Log Data Once Per Second
// ======================

void updateLogging() {

  if (!rideRecording)
  return;
  // Log once per second

  if (millis() - lastLogTime < 1000)
    return;

  lastLogTime = millis();

  if (!gps.location.isValid())
    return;

  logFile.print(gps.time.hour());
  logFile.print(":");
  logFile.print(gps.time.minute());
  logFile.print(":");
  logFile.print(gps.time.second());

  logFile.print(",");

  logFile.print(
    gps.location.lat(),
    6);

  logFile.print(",");

  logFile.print(
    gps.location.lng(),
    6);

  logFile.print(",");

  logFile.print(
    gps.speed.mph());

  logFile.print(",");

  logFile.print(currentAltitude);

  logFile.print(",");

  logFile.println(currentTemperature);

  pointsLogged++;

  // Flush every 10 seconds

  if (millis() - lastFlushTime > 10000) {

    logFile.flush();

    lastFlushTime = millis();
  }
}
void closeLogFile() {

  if (!rideRecording)
    return;

  rideElapsedTime =
    (millis() - rideStartTime) / 1000;

  lastRidePoints = pointsLogged;

  logFile.flush();

  logFile.close();

  rideRecording = false;

  Serial.println("Ride stopped");
}
