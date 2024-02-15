
#define GAMMA 2.6

int greenPin = 5;
int redPin = 3;

void setup() {
  // put your setup code here, to run once:
  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);

}

void loop() {
  // green
  float phase = (millis() / 1000.0 - 2.0 * 1 / (float)2) * M_PI;
  int brightness = int(pow((sin(phase) + 1.0) * 0.5, GAMMA) * 255 + 0.5);
  analogWrite(greenPin, brightness);
  // red
  phase = (millis() / 1000.0 - 2.0 * 2 / (float)2) * M_PI;
  brightness = int(pow((sin(phase) + 1.0) * 0.5, GAMMA) * 255 + 0.5);
  analogWrite(redPin, brightness);
}
