/**
LED Project

TODO:
-Implement LightPin class
-Implement precise timing in loop() function
-Implement HSV -> RGB and Hue 'autoscroll'
**/


const int SENSOR_PIN = 5;


class LightPin {
  private:
  int PIN; //the pin number
  int intens; //the intensity from 0-255
  public:
  LightPin(int _pin) {
    PIN = _pin;
    intens = 0;
  }

  //set the intensity
  void setIntensity(int _intensity) {
    intens = constrain(_intensity, 0, 255);
  }

  int getIntensity() {
    return intens;
  }

  int getPin() {
    return PIN;
  }


};

const int RED_PIN = 9;
const int GREEN_PIN = 10;
const int BLUE_PIN = 11;

bool debounce = false;


void setup() {
  pinMode(SENSOR_PIN, INPUT);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  digitalWrite(RED_PIN, LOW);

  Serial.begin(300);

}



void loop() {
  int period = 255;


  //read motion sensor pin
  bool sensorReading = digitalRead(SENSOR_PIN);

  //print the state of the sensor pin:
  //THIS IS SUPER SCUFFED LOL I COULDN'T FIGURE OUT HOW TO PRINT A BOOL
  // if (sensorReading == HIGH) {
  //   Serial.write("high");
  // } else {
  //   Serial.write("low");
  // }
  // Serial.write("\n");

  if (sensorReading == HIGH && !debounce) {
    debounce = true;
    digitalWrite(RED_PIN, HIGH);
    Serial.write("Turned on");
    return;
  }

  if (sensorReading == LOW && debounce) {
    debounce = false;
    digitalWrite(RED_PIN, LOW);
    Serial.write("Turned off");
  }

}
