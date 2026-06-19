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


// ======================
// Load Saved Weather Data
// ======================

void loadPrefsFromSD() {

  File file = SD.open("/prefs.dat");

  if (!file) {

    Serial.println("No prefs file found.");

    dailyHigh = -999;
    dailyLow = 999;
    pressureIndex = 0;
    consultationCount = 0;

    memset(
      pressureHistory,
      0,
      sizeof(pressureHistory));

    return;
  }


  file.read(
    (uint8_t *)&saved,
    sizeof(saved));

  file.close();

  dailyHigh = saved.dailyHigh;
  dailyLow = saved.dailyLow;

  if (currentDay == -1) {

    currentDay = 0;

  }
  pressureIndex = saved.pressureIndex;
  consultationCount = saved.consultationCount;

  memcpy(
    pressureHistory,
    saved.pressureHistory,
    sizeof(pressureHistory));

  Serial.println("Loaded pressure history:");

  for (int i = 0; i < 12; i++) {

    Serial.print(i);
    Serial.print(": ");
    Serial.println(pressureHistory[i]);
  }

  Serial.println("Preferences loaded.");
}

// ======================
// Save Weather Data
// ======================

void savePrefsToSD() {

  saved.dailyHigh = dailyHigh;
  saved.dailyLow = dailyLow;
  saved.pressureIndex = pressureIndex;
  saved.consultationCount = consultationCount;

  memcpy(
    saved.pressureHistory,
    pressureHistory,
    sizeof(pressureHistory));

  File file = SD.open("/prefs.dat", O_WRITE | O_CREAT | O_TRUNC);

  if (!file) {

    Serial.println("Couldn't save prefs.");
    return;
  }

  Serial.println("Saving pressure history:");

  for (int i = 0; i < 12; i++) {

    Serial.print(i);
    Serial.print(": ");
    Serial.println(saved.pressureHistory[i]);
  }
  file.write(
    (uint8_t *)&saved,
    sizeof(saved));

  file.close();

  Serial.println("Preferences saved.");
}

// ======================
// Start Ride Logging
// ======================

void startRide() {

  if (rideRecording)
    return;

  if (!gps.date.isValid() || !gps.time.isValid()) {

    Serial.println("GPS time not valid");

    strcpy(
      filename,
      "/NOFIX.csv");

  } else {

    int year, month, day;
    int hour, minute, second;

    getLocalDateTime(
      year,
      month,
      day,
      hour,
      minute,
      second);

    sprintf(
      filename,
      "/%04d%02d%02d_%02d%02d%02d.csv",
      year,
      month,
      day,
      hour,
      minute,
      second);
  }

  logFile = SD.open(
    filename,
    FILE_WRITE);

  if (!logFile) {

    Serial.println(
      "Couldn't open ride file");

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

  if (millis() - lastLogTime < 1000)
    return;

  lastLogTime = millis();

  if (!gps.location.isValid())
    return;

  logFile.print(gps.time.hour());
  logFile.print(":");
  logFile.print(gps.time.minute());
  logFile.print(":");
  logFile.println(gps.time.second());

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

  logFile.print(
    currentAltitude);

  logFile.print(",");

  logFile.println(
    currentTemperature);

  pointsLogged++;

  if (millis() - lastFlushTime > 10000) {

    logFile.flush();

    lastFlushTime = millis();
  }
}

// ======================
// Stop Ride Logging
// ======================

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