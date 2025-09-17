#include <Adafruit_LiquidCrystal.h>
Adafruit_LiquidCrystal lcd(0);
int mode = 0;                               // mode of the wellness lamp 0=off, 1=therapy, 2=lamp, 3=alarm
unsigned long endMillis;                    // Stores the start time of the current mode
unsigned long duration = 45;                // Stores how long the user wants to have a therapy session for in minutes
bool durationSet = false;                   // Stores if the user has entered their desired duration or not
const unsigned long minuteMS = 60000;       // 1 minute = 60 seconds * 1000 ms

int r = 0;                                  // The intensity of red in lamp mode
int g = 0;                                  // The intensity of green in lamp mode
int b = 0;                                  // The intensity of blue in lamp mode
int brightness = 127;                       // The overall brightness of the lamp from 0-255
int curColour = 0;                          // The current colour selected out of the list of 15
int colours[15][3] = {                      // A list of 15 colours to cycle through during lamp mode
  {253, 244, 220}, {255, 255, 255},{227, 228, 237},
  {50, 89, 255}, {157, 175, 255}, {50, 255, 255},
  {70, 255, 49}, {167, 255, 158}, {255, 238, 51},
  {255, 146, 49}, {255, 243, 205}, {255, 146, 51},
  {171, 50, 255}, {255, 52, 52}, {255, 205, 243}};

bool modeButtonDown = false;                // Stores if the mode button is currently down
bool enterButtonDown = false;               // Stores if the enter button is currently down
bool upButtonDown = false;                  // Stores if the up buttom is currently down
bool downButtonDown = false;                // Stores if the down buttom is currently down
bool leftButtonDown = false;                // Stores if the left buttom is currently down
bool rightButtonDown = false;               // Stores if the right buttom is currently down

bool modeButtonPressed = false;             // Stores if the mode button is pressed
bool enterButtonPressed = false;            // Stores if the enter button is pressed
bool upButtonPressed = false;               // Stores if the up buttom is pressed
bool downButtonPressed = false;             // Stores if the down buttom is pressed
bool leftButtonPressed = false;             // Stores if the left buttom is pressed
bool rightButtonPressed = false;            // Stores if the right buttom is pressed

// Variables to track time
bool timeSet = false;                       // Track if the time has been set at the beginning
int curInterval = 0;                        // The time interval the user is currently setting 
int hours = 0;                              // The current hour
int minutes = 0;                            // The current minute
int seconds = 0;                            // The current second
unsigned long nextSecond = 1000;            // The millisecond value of the next second
unsigned long resetMillis = 0;              // For periodic realignment

// Alarm start and end time
int startHours = 7;                         // The sunrise time's hours
int startMinutes = 0;                       // The sunrise time's minutes
int endHours = 21;                          // The sunset time's hours
int endMinutes = 0;                         // The sunset time's minutes
int alarmScreen = 0;                        // The selected screen of the alarm setting: 0 - current time; 1 - sunrise selection; 2 - sunset selection



// tell the Arduino which pins are doing what
void setup()
{
  pinMode(12, INPUT); // button to change mode
  pinMode(9, OUTPUT); // wellness LED Blue NMOS signal
  pinMode(10, OUTPUT);// wellness LED Green NMOS signal
  pinMode(11, OUTPUT);// wellness LED Red NMOS signal
  
  pinMode(13, INPUT); //enter button
  pinMode(7, INPUT); //left
  pinMode(6, INPUT); //up
  pinMode(5, INPUT); //down
  pinMode(4, INPUT); //right

  setupLcd(); // setup lcd
}

// Main loop
void loop() {
  // Determine which buttons have been pressed
  readButtons();

  // Set the time when starting the arduino up first
  if (!timeSet) {
    // Flash currently selected time interval
    // Determine if flash should hide or not
    bool flash;
    if (millis() / 500 % 2 == 0) flash = false;
    else flash = true;

    // Check if arrow buttons were pressed
    if (upButtonPressed) {
      // Update the interval of time the user is currently changing
      switch (curInterval) {
        case 0:   // Hours
          hours++;
          // Overflow protection
          if (hours > 23) hours = 0;
          break;

        case 1:   // Minutes
          minutes++;
          // Overflow protection
          if (minutes > 59) minutes = 0;
          break;

        case 2:   // Seconds
          seconds++;
          // Overflow protection
          if (seconds > 59) seconds = 0;
          break;
      }
    }
    if (downButtonPressed) {
      // Update the interval of time the user is currently changing
      switch (curInterval) {
        case 0:   // Hours
          hours--;
          // Underflow protection
          if (hours < 0) hours = 23;
          break;

        case 1:   // Minutes
          minutes--;
          // Underflow protection
          if (minutes < 0) minutes = 59;
          break;

        case 2:   // Seconds
          seconds--;
          // Underflow protection
          if (seconds < 0) seconds = 59;
          break;
      }
    }
    if (leftButtonPressed) {
      // Move current interval to left
      curInterval--;
      if (curInterval < 0) curInterval = 2;
    }
    if (rightButtonPressed) {
      // Move current interval to right
      curInterval++;
      if (curInterval > 2) curInterval = 0;
    }

    // Display current time
    lcd.setCursor(4,1);
    if (curInterval == 0 && flash) lcd.print("  ");
    else {
      if (hours < 10) lcd.print("0");
      lcd.print(hours);
    }
    lcd.print(":");
    if (curInterval == 1 && flash) lcd.print("  ");
    else {
      if (minutes < 10) lcd.print("0");
      lcd.print(minutes);
    }
    lcd.print(":");
    if (curInterval == 2 && flash) lcd.print("  ");
    else {
      if (seconds < 10) lcd.print("0");
      lcd.print(seconds);
    }

    // End time screen if user presses enter button
    if (enterButtonPressed) {
      // End time selection screen
      timeSet = true;
      clear();
      // Start next second cycle
      nextSecond = millis() + 1000;
    }
  }

  // If time has already been set
  else {
    unsigned long currentMillis = millis(); // Set current time to millis

    // If a second has passed, update the clock
    if (currentMillis >= nextSecond) {
      // Set nextSecond value to the next second
      nextSecond += 1000;

      // Update the clock
      seconds++;
      // Rollback detection
      if (seconds > 59) {
        seconds = 0;
        minutes++;

        if (minutes > 59) {
          minutes = 0;
          hours++;

          if (hours > 23) {
            hours = 0;
          }
        }
      }
    }
  
    // Update mode if mode button is pressed
    if (modeButtonPressed) {
      // Change mode
      mode++;
      mode %= 4;
      // Turn off lights if mode is 0 or 1
      if (mode == 0 || mode == 1) setLights(0);
      else setLights(brightness);
      // Clear screen
      clear();
      // Unset duration if therapy mode switched early
      durationSet = false;
    }

    switch (mode) {
      case 0:   // Off
        clear();
        break;

      case 1:   // Therapy Mode
        // If duration hasn't been set yet
        if (!durationSet) {
          // Ask the user how long they want to have a therapy session for
          lcd.setCursor(2,0);
          lcd.print("Set Duration");

          // Get any input from user
          // Up & Down - Change duration of session
          if (upButtonPressed) {
            duration += 5;
            if (duration > 95) duration = 95;
          }
          if (downButtonPressed) {
            duration -= 5;
            if (duration < 5) duration = 5;
          }

          // Show current time selected
          lcd.setCursor(5, 1);
          if (duration < 10) lcd.print("0");
          lcd.print(duration);
          lcd.print(" min");

          // Check if user enters desired duration
          if (enterButtonPressed) {
            // Hide prompt
            clear();
            // Start countdown
            durationSet = true;
            endMillis = currentMillis + (duration * 60 * 1000);
            // Turn on LEDs
            curColour = 1;
            // Set brightness
            // If shorter than recommended session set lights to max brightness
            if (duration < 20) brightness = 255;
            // Calculate recommended brightness
            else brightness = 20 * 255 / duration;
            setLights(brightness);
          }
        }
        // If duration has been set
        else {
          // Show current mode
          lcd.setCursor(2, 0);
          lcd.print("Therapy Mode");

          // Calculate minutes left
          unsigned long minLeft = (endMillis - currentMillis) / minuteMS;
          // Calculate seconds left
          unsigned long secLeft = ((endMillis - currentMillis) % minuteMS) / 1000;

          // Show length of time left
          lcd.setCursor(3, 1);
          if (minLeft < 10) lcd.print("0");
          lcd.print(minLeft);
          lcd.print(":");
          if (secLeft < 10) lcd.print("0");
          lcd.print(secLeft);
          lcd.print(" left");

          // Fade out if less than a minute remains
          if (minLeft == 0)
            setLights(brightness * secLeft / 60);

          // Check if time has ended
          if (currentMillis >= endMillis) {
            // Turn off lights
            setLights(0);
            // Change mode to off
            mode = 0;
            durationSet = false;
          }
        }
        
        break;

      case 2:   // Lamp Mode
        // Tell user which mode they're in
        lcd.setCursor(3, 0);
        lcd.print("Light Mode");

        // Display the time on the LCD
        lcd.setCursor(4, 1);  // Move cursor to the second line
        if (hours < 10) lcd.print("0"); // Add leading zero if necessary
        lcd.print(hours);
        lcd.print(":");
        if (minutes < 10) lcd.print("0"); // Add leading zero if necessary
        lcd.print(minutes);
        lcd.print(":");
        if (seconds < 10) lcd.print("0");  // Add leading zero if necessary
        lcd.print(seconds);

        // Read any inputs from the user and change lights if input given
        // Right & Left - Change colour from colour list
        if (rightButtonPressed) {
          curColour++;
          if (curColour > 14) curColour = 0;
          setLights(brightness);
        }
        if (leftButtonPressed) {
          curColour--;
          if (curColour < 0) curColour = 14;
          setLights(brightness);
        }
        // Up & Down - Change brightness
        if (upButtonPressed && brightness < 255) {
          brightness += 32;
          setLights(brightness);
        }
        if (downButtonPressed && brightness > 31) {
          brightness -= 32;
          setLights(brightness);
        }
        break;

      case 3:   // Alarm Mode
        // Set brightness of LEDs
        // Calculate current, start, and end time in minutes
        int curTime = hours * 60 + minutes;
        int startTime = startHours * 60 + startMinutes;
        int endTime = endHours * 60 + endMinutes;

        // Turn off lights at night
        if ((curTime < startTime) || (curTime > endTime)) setLights(0);
        // Max brightness during the day
        else if ((curTime >= startTime + 30) || (curTime <= endTime - 30)) setLights(255);
        // Calculate brightness for fade in/out
        // If fade in
        else if (curTime > startTime && curTime < startTime + 30) setLights(255 * (curTime - startTime) / 30);
        // If fade out
        else if (curTime > endTime - 30 && curTime < endTime) setLights(255 * (endTime - curTime) / 30);

        // Check for user input
        if (enterButtonPressed) {
          // Change screen on LCD
          alarmScreen++;
          alarmScreen %= 3;
          // Clear current screen
          clear();
        }
        // Increase time
        if (upButtonPressed && alarmScreen != 0) {
          // Increase sunrise
          if (alarmScreen == 1) {
            startMinutes += 15;
            if (startMinutes > 59) {
              startMinutes = 0;
              startHours++;
              if (startHours > 23)
                startHours = 0;
            }
          }
          // Increase sunset
          else {
            endMinutes += 15;
            if (endMinutes > 59) {
              endMinutes = 0;
              endHours++;
              if (endHours > 23)
                endHours = 0;
            }
          }
        }
        // Decrease time
        if (downButtonPressed && alarmScreen != 0) {
          // Increase sunrise
          if (alarmScreen == 1) {
            startMinutes -= 15;
            if (startMinutes < 0) {
              startMinutes = 45;
              startHours--;
              if (startHours < 0)
                startHours = 23;
            }
          }
          // Increase sunset
          else {
            endMinutes -= 15;
            if (endMinutes < 0) {
              endMinutes = 45;
              endHours--;
              if (endHours < 0)
                endHours = 23;
            }
          }
        }

        // Display text on LCD
        // Show current time for screen 0
        if (alarmScreen == 0) {
          // Tell user which mode they're in
          lcd.setCursor(3, 0);
          lcd.print("Alarm Mode");

          lcd.setCursor(4, 1);  // Move cursor to the second line
          if (hours < 10) lcd.print("0"); // Add leading zero if necessary
          lcd.print(hours);
          lcd.print(":");
          if (minutes < 10) lcd.print("0"); // Add leading zero if necessary
          lcd.print(minutes);
          lcd.print(":");
          if (seconds < 10) lcd.print("0");  // Add leading zero if necessary
          lcd.print(seconds);
        }
        // Set sunrise time for screen 1
        else if (alarmScreen == 1) {
          // Top row
          lcd.setCursor(0,0);
          lcd.print("Set Sunrise Time");
          // Bottom Row
          lcd.setCursor(4,1);
          if (startHours < 10) lcd.print("0"); // Add leading zero if necessary
          lcd.print(startHours);
          lcd.print(":");
          if (startMinutes < 10) lcd.print("0"); // Add leading zero if necessary
          lcd.print(startMinutes);
          lcd.print(":00");
        }
        else if (alarmScreen == 2) {
          // Top row
          lcd.setCursor(0,0);
          lcd.print("Set Sunset Time");
          // Bottom Row
          lcd.setCursor(4,1);
          if (endHours < 10) lcd.print("0"); // Add leading zero if necessary
          lcd.print(endHours);
          lcd.print(":");
          if (endMinutes < 10) lcd.print("0"); // Add leading zero if necessary
          lcd.print(endMinutes);
          lcd.print(":00");
        }
        break;
    }
  }

  // Set a short delay for simulation to speed up
  delay(10);
}



// Set the values of the outputs based on the current colour & brightness selected
void setLights(int setBrightness)
{
    analogWrite(11, colours[curColour][0] * setBrightness / 255); // Red value
    analogWrite(10, colours[curColour][1] * setBrightness / 255); // Green Value
    analogWrite(9, colours[curColour][2] * setBrightness / 255);  // Blue Value
}

void readButtons() {
  // Mode button
  if (!modeButtonDown && digitalRead(12)) {
    modeButtonDown = true;
    modeButtonPressed = true; // Set variable to true so that any effects only happen once per button press
  }
  else if (modeButtonDown && !digitalRead(12)) {
    // Button is released
    modeButtonDown = false;
    modeButtonPressed = false;
  }
  else
    modeButtonPressed = false;

  // Enter button
  if (!enterButtonDown && digitalRead(13)) {
    enterButtonDown = true;
    enterButtonPressed = true;
  }
  else if (enterButtonDown && !digitalRead(13)) {
    enterButtonDown = false;
    enterButtonPressed = false;
  }
  else
    enterButtonPressed = false;

  // Up button
  if (!upButtonDown && digitalRead(6)) {
    upButtonDown = true;
    upButtonPressed = true;
  }
  else if (upButtonDown && !digitalRead(6)) {
    upButtonDown = false;
    upButtonPressed = false;
  }
  else
    upButtonPressed = false;

  // Down button
  if (!downButtonDown && digitalRead(5)) {
    downButtonDown = true;
    downButtonPressed = true;
  }
  else if (downButtonDown && !digitalRead(5)) {
    downButtonDown = false;
    downButtonPressed = false;
  }
  else
    downButtonPressed = false;

  // Left button
  if (!leftButtonDown && digitalRead(7)) {
    leftButtonDown = true;
    leftButtonPressed = true;
  }
  else if (leftButtonDown && !digitalRead(7)) {
    leftButtonDown = false;
    leftButtonPressed = false;
  }
  else
    leftButtonPressed = false;

  // Right button
  if (!rightButtonDown && digitalRead(4)) {
    rightButtonDown = true;
    rightButtonPressed = true;
  }
  else if (rightButtonDown && !digitalRead(4)) {
    rightButtonDown = false;
    rightButtonPressed = false;
  }
  else
    rightButtonPressed = false;
}

void setupLcd() // Initialize time selection
{
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Set Current Time");
}

void clear() {  // Clear the screen
  // Clear row 0
  lcd.setCursor(0,0);
  // Fill the row with spaces
  for (int i = 0; i < 16; i++)
    lcd.print(" ");

  // Clear row 1
  lcd.setCursor(0,1);
  for (int i = 0; i < 16; i++)
    lcd.print(" ");
}
