
#define GAMMA 2.6


#define red 11
#define green 10

int greenPin = 10;
int redPin = 11;

void setup() {
  // put your setup code here, to run once:
  pinMode(green, OUTPUT);
  pinMode(red, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // green
  float phase = (millis() / 1000.0 - 2.0 * 1 / (float)2) * M_PI;
  int brightness = int(pow((sin(phase) + 1.0) * 0.5, GAMMA) * 255 + 0.5);
  analogWrite(green, brightness);
  // red
  phase = (millis() / 1000.0 - 2.0 * 2 / (float)2) * M_PI;
  brightness = int(pow((sin(phase) + 1.0) * 0.5, GAMMA) * 255 + 0.5);
  analogWrite(red, brightness);
  Serial.println(brightness);
}
