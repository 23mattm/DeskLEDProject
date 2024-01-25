/**
LED Project

TODO:
-Implement LightPin class
-Implement precise timing in loop() function
-Implement HSV -> RGB and Hue 'autoscroll'
**/

#include<string.h>

const int SENSOR_PIN = 5;
//Playing with a constant saturation, should give a more pastel-colored light
const float SATURATION = 0.5;
const float VALUE = 1.0;
//The number of seconds it takes for the hue scroller to complete a full rotation
const float HUE_CYCLE = 5.0;

class LightPin {

  private:

  int PIN;     //the pin number
  int intens;  //the intensity from 0-255

  public:

  LightPin(int _pin) {
    PIN = _pin;
    intens = 0;
    pinMode(PIN, OUTPUT);
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
    //Transform sat and val to decimal
    int H = hue * 360 / 255;
    float S = (float) saturation / 255.0;
    float V = (float) value / 255.0;

    //Calculate some extra variables.
    //Formula from rapidtables.com <-- shitty site :-1:
    float C = V * S;
    float X = C * (float) (1 - abs(((H / 60) % 2) - 1));
    float m = V - C;
    float Rp, Gp, Bp;

    //Assign RGB prime values
    H = 6 * (int) ( H / 360.0);
    if (H == 0) {
      Rp = C;
      Gp = X;
      Bp = 0.0;
    } else if (H == 1) {
      Rp = X;
      Gp = C;
      Bp = 0.0;
    } else if (H == 2) {
      Rp = 0.0;
      Gp = C;
      Bp = X;
    } else if (H == 3) {
      Rp = 0.0;
      Gp = X;
      Bp = C;
    } else if (H == 4) {
      Rp = X;
      Gp = 0.0;
      Bp = C;
    } else if (H == 5) {
      Rp = C;
      Gp = 0.0;
      Bp = X;
    }

    // Assign the RGB values to integers 0-255
    red = (int) ((Rp + m) * 255);
    green = (int) ((Gp + m) * 255);
    blue = (int) ((Bp + m) * 255);
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
    Serial.write("\n--------\n");
    Serial.write("RGB: ");
    Serial.print(red);
    Serial.write(", ");
    Serial.print(green);
    Serial.write(", ");
    Serial.print(blue);
    Serial.write("\n");
    Serial.write("HSV: ");
    Serial.print(hue);
    Serial.write(", ");
    Serial.print(saturation);
    Serial.write(", ");
    Serial.print(value);
    Serial.write("\n--------");

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

  //Begin the serial at 9600 baud.
  Serial.begin(9600);
  Serial.flush();
}

// Debounce for sensor readings
bool debounce = false;
// The main loop for the program
void loop() {

  int period = 255;

  masterColor.setHSV((int) masterHue, (int) (SATURATION * 255.0), (int) (VALUE * 255.0));
  //Update the master hue
  float dt = (float) (millis() - lastHueUpdate) / 1000.0;  // Time since last update in seconds
  masterHue = (int) (masterHue + ((255.0 / HUE_CYCLE) * dt)) % 255;
  lastHueUpdate = millis();

  // Print the master color:
  masterColor.writeOut();

  delay(500);

  //read motion sensor pin
  bool sensorReading = digitalRead(SENSOR_PIN);

  if (sensorReading == HIGH && !debounce) {
    debounce = true;
    Serial.write("Turned on");
    return;
  }

  if (sensorReading == LOW && debounce) {
    debounce = false;
    Serial.write("Turned off");
  }
}
