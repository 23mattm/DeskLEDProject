/**
LED Project

TODO:
-Implement precise timing in loop() function
-Implement HSV -> RGB and Hue 'autoscroll'
**/

const int SENSOR_PIN = 5;


void setup() {
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);

  pinMode(SENSOR_PIN, INPUT);

  Serial.begin(9600);
}


/**
 * Turns on all three light pins for white light
 */
void turnOn() {
  digitalWrite(9, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
}

/**
 * Turns off all three light pins for no light
 */
void turnOff() {
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
}


void loop() {

  if (digitalRead(5) == 1) {
    turnOn();
  } else {
    turnOff();
  }
  
}
