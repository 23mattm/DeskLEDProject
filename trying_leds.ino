/**
LED Project

TODO:
-Implement LightPin class
-Implement precise timing in loop() function
-Implement HSV -> RGB and Hue 'autoscroll'
**/

#include<string.h>

const int SENSOR_PIN = 5;
//The number of seconds it takes for the hue scroller to complete a full rotation
const float HUE_CYCLE = 5.0;

class LightPin {

  private:

  int PIN;     //the pin number
  int _intensity;

  public:

  LightPin(int _pin) {
    PIN = _pin;
    pinMode(PIN, OUTPUT);
  }

  void on() {
    digitalWrite(PIN, HIGH);
  }

  void off() {
    digitalWrite(PIN, LOW);
  }

  void setIntensity(int intensity) {
    _intensity = intensity;
  }

  int getIntensity() {
    return _intensity;
  }

};

class Color {
  private:
  //Integer, [0,255]
  int red, green, blue;
  //Integer, [0, 255]
  int hue, saturation, value;

  /**
  * Calculate the RGB values for a Color object with HSV
  * values already calculated and assign them(internal)
  */
  void calculateRGB() {
    float r, g, b;
	
    float h = (float) hue / 255;
    float s = (float) saturation / 255;
    float v = (float) value / 255;
    
    int i = floor(h * 6);
    float f = h * 6 - i;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1 - (1 - f) * s);
    
    switch (i % 6) {
      case 0: r = v, g = t, b = p; break;
      case 1: r = q, g = v, b = p; break;
      case 2: r = p, g = v, b = t; break;
      case 3: r = p, g = q, b = v; break;
      case 4: r = t, g = p, b = v; break;
      case 5: r = v, g = p, b = q; break;
    }
    
    red = r * 255;
    green = g * 255;
    blue = b * 255;
  }

  public:

  /**
  * Creates a black Color object
  */
  Color() {
    red = 0;
    green = 0;
    blue = 0;
    //
    hue = 0;
    saturation = 0;
    value = 0;
  }

  /**
  * Creates a color object from a given HSV
  * @param _hue A float between 0 and 1
  * @param _sat A float between 0 and 1
  * @param _val A float between 0 and 1
  */
  static Color fromHSV(float _hue, float _sat, float _val) {
    Color col = Color();
    col.setHSV(
      (int) (_hue * 255), // hue as an int
      (int) (_sat * 255), // sat as an int
      (int) (_val * 255)  // val as an int
    );
    return col;
  }

  /**
  * Set the HSV for the object
  */
  void setHSV(int _hue, int _sat, int _val) {
    hue = _hue;
    saturation = _sat;
    value = _val;
    calculateRGB();
  }

  int getRed() {
    return red;
  }

  int getGreen() {
    return green;
  }

  int getBlue() {
    return blue;
  }

  int getHue() {
    return hue;
  }

  int getSaturation() {
    return saturation;
  }

  int getValue() {
    return value;
  }

  /**
  * Writes the color's properties to the Serial monitor
  */
  void writeOut() {
    Serial.println("\n--------");
    Serial.print("RGB: ");
    Serial.print(red);
    Serial.print(", ");
    Serial.print(green);
    Serial.print(", ");
    Serial.println(blue);
    Serial.print("HSV: ");
    Serial.print(hue);
    Serial.print(", ");
    Serial.print(saturation);
    Serial.print(", ");
    Serial.print(value);
    Serial.print("\n--------");
    Serial.flush();
  }
  
};

// Declare the RGB pins
LightPin redPin = NULL;
LightPin greenPin = NULL;
LightPin bluePin = NULL;


//The color object that will control the color of the strip
Color masterColor;
float masterHue = 0.0;
int lastHueUpdate = millis();

void setup() {
  //Setup the sensor pin to read in
  pinMode(SENSOR_PIN, INPUT);

  //Initialize the RGB pins. This sets the pinMode to OUTPUT and the intensity to 0
  redPin = LightPin(9);
  greenPin = LightPin(10);
  bluePin = LightPin(11);

  //Initialize the masterColor object
  masterColor = Color();

  //Begin the serial.
  Serial.begin(9600);
  Serial.flush();
}

// Debounce for sensor readings
bool debounce = false;

// The main loop for the program
void loop() {

  //read motion sensor pin
  bool sensorReading = digitalRead(SENSOR_PIN);

  if (sensorReading == HIGH && !debounce) {
    debounce = true;
    Serial.println("Turned on");
  } else if (sensorReading == LOW && debounce) {
    debounce = false;
    Serial.println("Turned off");
  }

  // The lights should be on if this is true
  if (sensorReading == HIGH && debounce) {

      int period = 255;

      masterColor.setHSV((int) masterHue, 255, 255);

      //Update the master hue
      float dt = (float) (millis() - lastHueUpdate) / 1000.0;  // Time since last update in seconds
      masterHue = (int) (masterHue + ((255.0 / HUE_CYCLE) * dt)) % 255;
      lastHueUpdate = millis();

      redPin.setIntensity(masterColor.getRed());
      greenPin.setIntensity(masterColor.getGreen());
      bluePin.setIntensity(masterColor.getBlue());

      //I'm so sorry for the below implementation.

      LightPin pins[3] = {redPin, greenPin, bluePin};
      redPin.on(); greenPin.on(); bluePin.on();
      int pinsOnTimestamp = micros();
      int pinsOn = 3;
      while (pinsOn > 0) {
        int deltaMicroSeconds = micros() - pinsOnTimestamp;
        if (redPin.getIntensity() >= deltaMicroSeconds) {redPin.off(); pinsOn -= 1;}
        if (greenPin.getIntensity() >= deltaMicroSeconds) {greenPin.off(); pinsOn -= 1;}
        if (bluePin.getIntensity() >= deltaMicroSeconds) {bluePin.off(); pinsOn -= 1;}
      }


      // Print the master color:
      // masterColor.writeOut();
  }

}
