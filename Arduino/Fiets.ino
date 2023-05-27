// --------------------
  // Steer
  // --------------------
const int steerPin_CLK = 5;
const int steerPin_DT = 4;
int steerPosition = 0;
int steerPin_CLK_State;
int steerPin_CLK_OldState;

// --------------------
  // Steer Button
  // --------------------
const int steerPin_SW = 3;
unsigned long steerButtonPress = 0;

// --------------------
  // Wheel
  // --------------------
const int WHEEL_DIAMETER = 9; // please enter the wheel diameter in inches (or centimeters if USE_MPH is set to false)
const int RPM_SAMPLE_PERIOD = 1; // RPM is calculated based on the last 10 seconds of data
int previousMagValue = 0;
int previousRingState = 0;
const int wheelPin = 2; // Initialize a pin for the Hall Sensor.

// --------------------
// Ring
// --------------------
const int ringPin = 6;

void setup () {
  // --------------------
  // Steer
  // --------------------
  pinMode (steerPin_CLK, INPUT);
  pinMode (steerPin_DT, INPUT);

  // --------------------
  // Steer Button
  // --------------------
  pinMode (steerPin_SW, INPUT_PULLUP);

  // --------------------
  // Wheel
  // --------------------
  pinMode (wheelPin, INPUT); // Set the Hall Sensor pin as an input pin.

// --------------------
  // Ring
  // --------------------
  pinMode (ringPin, INPUT);

  Serial.begin(9600);
  steerPin_CLK_OldState = digitalRead(steerPin_CLK);
}

void loop () {
  // --------------------
  // Steer
  // --------------------
  steerPin_CLK_State = digitalRead(steerPin_CLK);

  if (steerPin_CLK_State != steerPin_CLK_OldState && steerPin_CLK_State == 1) {
    if (digitalRead(steerPin_DT) != steerPin_CLK_State) {
      steerPosition++;
    } else {
      steerPosition--;
    }

    if (steerPosition < -5) {
      steerPosition = -5;
    }
    if (steerPosition > 5) {
      steerPosition = 5;
    }
  }

  steerPin_CLK_OldState = steerPin_CLK_State;

  // --------------------
  // Steer Button
  // --------------------
  int steerButtonState = digitalRead(steerPin_SW);

  if (steerButtonState == LOW) {
    if (millis() - steerButtonPress > 50) {
      steerPosition = 0;
    }
    steerButtonPress = millis();
  }

  // --------------------
  // Wheel
  // --------------------
  //wheel();
  static bool firstTime = true; // The first datapoint is not used because previousMagValue has not been assigned a meaningful value
    int thisMagValue = 0;
    int wheelValue = 0;
    static int rpm = 0;
    static long mph = 0;
    static long kph = 0;
    static long cmpm = 0;
    static float magnetCount = 0.00; // represents how many times a magnet has been recorded
    static unsigned long timeOld = 0; // represents the amount of seconds since the last minute, goes to 0 after it hits user defined const RPM_SAMPLE_PERIOD

    //int previousMagValue = thisMagValue; // The value of the Hall Sensor right now
    thisMagValue = digitalRead(wheelPin);// The value of the Hall Sensor right now

    if(!firstTime) {

        if (previousMagValue != thisMagValue) {
            ++magnetCount;
            previousMagValue = thisMagValue;
        }

        //Serial.println(magnetCount);
        // Every RPM_SAMPLE_PERIOD seconds we reset the magnetData array to 0 
        if(floor((millis() - timeOld) / 1000) == RPM_SAMPLE_PERIOD) {
            timeOld = millis();
            magnetCount = 0;
        }
        
        if (abs((((float)(millis() - timeOld) / 1000) / 60)) > 0.01) {
            rpm = (magnetCount/2) / (((float)(millis()-timeOld)/1000)/60);
        }
        delay(15);

        cmpm = ((WHEEL_DIAMETER * PI)* rpm); // 1666.67 is the conversion factor between cm/minute and km/hour
        kph = cmpm / 1666.67;
        wheelValue = (cmpm / 1666.67) * 2;

        if (kph > 9) {
          kph = 9;
        }
    }

    firstTime = false;
  int steerValue = steerPosition + 15;

  // --------------------
  // Ring
  // --------------------
  int currentRingState = 0;
  int ringValue = 0;

  currentRingState = digitalRead(ringPin);

  if (previousRingState != currentRingState) {
    ringValue = 1;
    if(floor((millis() - timeOld) / 1000) == RPM_SAMPLE_PERIOD) {
      previousRingState = currentRingState;
      ringValue = 0;
    }
  }

  Serial.print(steerValue);
  Serial.print(wheelValue);
  Serial.println(ringValue);

  delay(2);
}