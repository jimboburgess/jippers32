bool isLeapYear(int year) {
  return ((year % 4 == 0 && year % 100 != 0)
          || (year % 400 == 0));
}


int daysInMonth(int year, int month) {
  switch (month) {
    case 4:
    case 6:
    case 9:
    case 11:
      return 30;

    case 2:
      return isLeapYear(year) ? 29 : 28;

    default:
      return 31;
  }
}


bool isDST(int year, int month, int day) {
  // Jan, Feb, Dec are standard time
  if (month < 3 || month > 11)
    return false;

  // Apr-Oct are daylight time
  if (month > 3 && month < 11)
    return true;

  int y = year;
  int m = month;

  if (m < 3) {
    m += 12;
    y--;
  }

  int K = y % 100;
  int J = y / 100;

  int h =
    (1 + (13 * (m + 1)) / 5
     + K
     + K / 4
     + J / 4
     + 5 * J)
    % 7;

  // Convert Zeller result to Sunday=0
  int firstDay = (h + 6) % 7;

  // Second Sunday in March
  int secondSunday = 14 - firstDay;

  // First Sunday in November
  int firstSunday = 7 - firstDay;

  if (month == 3)
    return day >= secondSunday;

  return day < firstSunday;
}


int getUTCOffset() {
  if (!gps.date.isValid())
    return -5;  // default to EST

  return isDST(
           gps.date.year(),
           gps.date.month(),
           gps.date.day())
           ? -4
           : -5;
}


void getLocalDateTime(
  int &year,
  int &month,
  int &day,
  int &hour,
  int &minute,
  int &second) {
  year = gps.date.year();
  month = gps.date.month();
  day = gps.date.day();

  hour = gps.time.hour() + getUTCOffset();
  minute = gps.time.minute();
  second = gps.time.second();

  // Previous day
  if (hour < 0) {
    hour += 24;

    day--;

    if (day < 1) {
      month--;

      if (month < 1) {
        month = 12;
        year--;
      }

      day = daysInMonth(year, month);
    }
  }

  // Next day
  if (hour >= 24) {
    hour -= 24;

    day++;

    if (day > daysInMonth(year, month)) {
      day = 1;

      month++;

      if (month > 12) {
        month = 1;
        year++;
      }
    }
  }
}