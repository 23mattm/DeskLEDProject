const int SENSOR_PIN = 5;

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
  //read motion sensor pin
  bool sensorReading = digitalRead(SENSOR_PIN);

  //print the state of the sensor pin:
  //THIS IS SUPER SCUFFED LOL I COULDN'T FIGURE OUT HOW TO PRINT A BOOL
  if (sensorReading == HIGH) {
    Serial.write("high");
  } else {
    Serial.write("low");
  }
  Serial.write("\n");

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
