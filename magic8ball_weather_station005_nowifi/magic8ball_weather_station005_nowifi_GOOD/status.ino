String getSensorStatus() {

  if (!sensorOK)
    return "PRESSURE FAIL";

  return "ALL SENSORS ACTIVE";
}