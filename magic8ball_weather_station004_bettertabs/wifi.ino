void showTimeSyncStatus(
  bool wifiOK,
  bool timeOK,
  const char* ssid = "") {

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(32, 4);
  display.println("TIME SYNC");

  if (strlen(ssid) > 0) {

    display.setCursor(0, 16);
    display.print("Trying:");

    display.setCursor(0, 26);
    display.println(ssid);
  }

  display.setCursor(12, 42);
  display.print(
    wifiOK ? "[X] " : "[ ] ");
  display.println(
    "WiFi Connected");

  display.setCursor(12, 54);
  display.print(
    timeOK ? "[X] " : "[ ] ");
  display.println(
    "Time Synced");

  display.display();
}


bool connectWiFi() {

  bool connected = false;

  for (int i = 0;
       i < networkCount;
       i++) {

    Serial.print("Trying ");
    Serial.println(
      networks[i].ssid);

    showTimeSyncStatus(
      false,
      false,
      networks[i].ssid);

    WiFi.disconnect(true);
    delay(250);

    WiFi.begin(
      networks[i].ssid,
      networks[i].password);

    unsigned long start =
      millis();

    while (
      WiFi.status() != WL_CONNECTED && millis() - start < 10000) {
      Serial.print("Status after ");
      Serial.print(networks[i].ssid);
      Serial.print(" = ");
      Serial.println(WiFi.status());
      delay(500);
      Serial.println("Moving to next network");
    }

    if (WiFi.status() == WL_CONNECTED) {

      connected = true;

      lastSSID = WiFi.SSID();

      prefs.putString(
        "ssid",
        lastSSID);

      break;
    }
  }

  if (!connected) {

    display.clearDisplay();

    display.setTextSize(1);
    display.setCursor(20, 28);
    display.println("WIFI FAILED");

    display.display();

    delay(3000);

    return false;
  }

  return true;
  lastSSID = WiFi.SSID();

  prefs.putString(
    "ssid",
    lastSSID);
}


void syncTime() {

  showTimeSyncStatus(
    false,
    false);

  WiFi.mode(WIFI_STA);
  Serial.println("syncTime start");
  bool wifiConnected =
    connectWiFi();

  if (!wifiConnected) {

    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);

    return;
  }

  showTimeSyncStatus(
    true,
    false);

  delay(1000);

  configTime(
    gmtOffset_sec,
    daylightOffset_sec,
    ntpServer);

  struct tm timeinfo;

  bool timeSynced = false;

  unsigned long start =
    millis();

  while (
    millis() - start < 4000) {

    if (
      getLocalTime(
        &timeinfo)) {

      timeSynced = true;
      break;
    }

    delay(500);
  }

  showTimeSyncStatus(
    true,
    timeSynced);

  if (timeSynced) {

    delay(2500);

  } else {

    delay(4000);
  }

  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);

  for (int i = 0; i < 12; i++) {

    pressureHistory[i] =
      bmp.readPressure() / 100.0;
  }
}