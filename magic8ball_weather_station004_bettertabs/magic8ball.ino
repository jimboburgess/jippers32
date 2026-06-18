struct MagicAnswer {

  const char* line1;
  const char* line2;
};

MagicAnswer answers[] = {

  { "IT IS", "CERTAIN" },
  { "IT IS", "DECIDEDLY SO" },
  { "WITHOUT", "A DOUBT" },
  { "YES", "DEFINITELY" },
  { "YOU MAY", "RELY ON IT" },

  { "AS I SEE IT", "YES" },
  { "MOST", "LIKELY" },
  { "OUTLOOK", "GOOD" },
  { "YES", "" },
  { "SIGNS POINT", "TO YES" },

  { "REPLY HAZY", "TRY AGAIN" },
  { "ASK AGAIN", "LATER" },
  { "BETTER NOT", "TELL YOU NOW" },
  { "CANNOT", "PREDICT NOW" },
  { "CONCENTRATE", "AND ASK AGAIN" },

  { "DON'T COUNT", "ON IT" },
  { "MY REPLY", "IS NO" },
  { "MY SOURCES", "SAY NO" },
  { "OUTLOOK", "NOT SO GOOD" },
  { "VERY", "DOUBTFUL" },

  // Mildly Threatening
  { "BUT YOU WON'T", "LIKE IT" },
  { "MANY CROWS ARE", "WATCHING, PLOTTING" },
  { "BETTER NOT", "OPEN THAT DOOR" },
  { "FAR TOO", "LATE NOW" },
  { "FINAL", "QUESTION" },
  { "DARK SHADOWS", "APPROVE" },
  { "THAT'S HOW", "IT BEGINS" },
  { "CONSIDER THIS", "A WARNING" },
  { "TIME TO", "HIDE" },
  { "YOU HAVE BEEN", "NOTICED" },

  // Bike
  { "THE TAILWINDS", "FAVOR YOU" },
  { "TIME TO CHECK", "TIRE PRESSURE" },
  { "CLIMB NOW", "REGRET LATER" },
  { "THE HILL IS", "UNDEFEATED" },
  { "PEDAL", "HARDER" },
  { "GRAVITY HAS", "PLANS" },
  { "YOUR LEGS", "VOTE NO" },
  { "STRAVA WILL", "BELIEVE YOU" },
  { "ROUTE GETS", "INTERESTING" },
  { "BRING", "WATER" },

  // Cryptic
  { "THE GEESE", "KNOW, YES!" },
  { "ASK AGAIN", "AFTER MOONBLINK" },
  { "THREE SQUIRRELS", "APPROVE OF THIS" },
  { "THE ANSWER", "ESCAPES ME" },
  { "YOU ALREADY", "KNOW IT" },
  { "SIGNS POINT", "SIDEWAYS" },
  { "RACCOONS", "UNSURE" },
  { "REALITY IS", "BUFFERING" },
  { "THE VOID", "SHRUGGED" },
  { "QUESTION", "NOTED, GO ON" },

  // Fantasy
  { "THE DRAGON", "APPROVES" },
  { "WIZARD", "UNAVAILABLE" },
  { "ROLL FOR", "INITIATIVE" },
  { "KOBOLDS ARE", "PLOTTING" },
  { "PROPHECY", "FORMING" },
  { "RUNES SAY", "YES" },
  { "RUNES SAY", "NO" },
  { "GOBLINS ARE", "DIVIDED" },
  { "THE TAVERN", "AGREES" },
  { "THE BARD", "EXAGGERATED" },
};

const int positiveAnswers[] = {
  0, 1, 2, 3, 4,
  5, 6, 7, 8, 9,
  30, 31, 34, 37, 39,
  44, 45, 48, 49, 58
};

const int neutralAnswers[] = {
  10, 11, 12, 13, 14,
  21, 22, 24, 25, 26, 27, 28, 29,
  40, 41, 42, 43, 46, 47,
  51, 52, 54, 57, 59
};

const int negativeAnswers[] = {
  15, 16, 17, 18, 19,
  20, 23,
  32, 33, 35, 36, 38,
  50, 53, 55, 56
};
const int answerCount =
  sizeof(answers) / sizeof(answers[0]);

enum MagicState {

  MAGIC_IDLE,
  MAGIC_SHAKING,
  MAGIC_REVEAL,
  MAGIC_DONE

};

MagicState magicState = MAGIC_IDLE;

int selectedAnswer = 0;

unsigned long stateStart = 0;

uint8_t revealChars = 0;

void drawNoise() {

  for (int i = 0; i < 350; i++) {

    if (random(2)) {

      display.drawPixel(
        random(128),
        random(64),
        SSD1306_WHITE);
    }
  }
}


void startMagic8Ball() {

  consultationCount++;

  prefs.putULong(
    "consults",
    consultationCount);
  float change =
    getPressureChange();

  Serial.print("Pressure change: ");
  Serial.println(change, 2);

  int moodRoll = random(100);

  if (change > 0.03) {

    // Rising pressure = optimistic

    if (moodRoll < 60) {

      selectedAnswer =
        positiveAnswers[random(
          sizeof(positiveAnswers) / sizeof(positiveAnswers[0]))];

    } else if (moodRoll < 85) {

      selectedAnswer =
        neutralAnswers[random(
          sizeof(neutralAnswers) / sizeof(neutralAnswers[0]))];

    } else {

      selectedAnswer =
        negativeAnswers[random(
          sizeof(negativeAnswers) / sizeof(negativeAnswers[0]))];
    }

  } else if (change < -0.03) {

    // Falling pressure = pessimistic

    if (moodRoll < 60) {

      selectedAnswer =
        negativeAnswers[random(
          sizeof(negativeAnswers) / sizeof(negativeAnswers[0]))];

    } else if (moodRoll < 85) {

      selectedAnswer =
        neutralAnswers[random(
          sizeof(neutralAnswers) / sizeof(neutralAnswers[0]))];

    } else {

      selectedAnswer =
        positiveAnswers[random(
          sizeof(positiveAnswers) / sizeof(positiveAnswers[0]))];
    }

  } else {

    selectedAnswer =
      random(answerCount);
  }

  magicState = MAGIC_SHAKING;

  stateStart = millis();

  revealChars = 0;
}

void showMagic8Ball() {

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(22, 0);
  display.println("MAGIC 8 BALL");

  switch (magicState) {

    case MAGIC_IDLE:

      display.setCursor(20, 28);
      display.println("BLUE BUTTON");

      break;

    case MAGIC_SHAKING:

      drawNoise();

      display.setCursor(18, 54);
      display.println("CONSULTING...");

      if (millis() - stateStart > 3000) {

        magicState = MAGIC_REVEAL;

        stateStart = millis();

        revealChars = 0;
      }

      break;

    case MAGIC_REVEAL:

      if (millis() - stateStart > 200) {

        revealChars++;

        stateStart = millis();
      }

      {
        String line1 =
          answers[selectedAnswer].line1;

        String line2 =
          answers[selectedAnswer].line2;

        int len1 = line1.length();

        display.setCursor(30, 22);

        if (revealChars <= len1) {

          display.print(
            line1.substring(
              0,
              revealChars));

        } else {

          display.print(line1);

          display.setCursor(20, 38);

          int charsOnLine2 =
            revealChars - len1;

          if (charsOnLine2 > line2.length())
            charsOnLine2 = line2.length();

          display.print(
            line2.substring(
              0,
              charsOnLine2));
        }

        if (
          revealChars > len1 + line2.length()) {

          magicState = MAGIC_DONE;
        }
      }

      break;

    case MAGIC_DONE:

      display.setCursor(30, 22);
      display.println(
        answers[selectedAnswer].line1);

      display.setCursor(20, 38);
      display.println(
        answers[selectedAnswer].line2);

      break;
  }

  display.display();
}