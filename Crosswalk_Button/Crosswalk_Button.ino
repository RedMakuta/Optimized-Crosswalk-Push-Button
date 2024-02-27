#include <Adafruit_NeoPixel.h>

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

#define PINTOP 2	 // input pin Neopixel is attached to
#define PINBOTTOM 3

#define NUMPIXELS 8 // number of neopixels in strip

// NeoPixel brightness, 0 (min) to 255 (max)
#define BRIGHTNESS 25

Adafruit_NeoPixel pixelsT = Adafruit_NeoPixel(NUMPIXELS, PINTOP, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel pixelsB = Adafruit_NeoPixel(NUMPIXELS, PINBOTTOM, NEO_GRBW + NEO_KHZ800);

uint32_t red = pixelsT.Color(255, 0, 0);
uint32_t green = pixelsT.Color(0, 255, 0);
uint32_t yellow = pixelsT.Color(255, 255, 0);

#define GAMMA 2.6

// Force sensor
int fsrPin = 0;
int fsrValue;

// Haptic
#define HAPTIC1 5
#define HAPTIC2 6
#define HAPTIC3 7

// Noods
#define PINRED 10
#define PINGREEN 11

void setup() {
  // Initialize the NeoPixel library.
  pixelsT.begin();
  pixelsB.begin();
  pixelsT.setBrightness(BRIGHTNESS);
  pixelsB.setBrightness(BRIGHTNESS);
  pixelsT.clear();
  pixelsB.clear();
  pixelsT.show();
  pixelsB.show();

  // Setup the haptic motors
  pinMode(HAPTIC1, OUTPUT);
  pinMode(HAPTIC2, OUTPUT);
  pinMode(HAPTIC3, OUTPUT);

  // Noods
  pinMode(PINRED, OUTPUT);
  pinMode(PINGREEN, OUTPUT);

  Serial.begin(9600);
  Serial.println("Starting up!");
}

void loop() {
  fsrValue = analogRead(fsrPin);  
 
  Serial.print("Analog reading = ");
  Serial.println(fsrValue);     // the raw analog reading
  
  if (fsrValue > 100) {
    buttonPushed();
    for(int thing = 0; thing < 4; thing++) {
      active();
      delay(1000);
    }
  } else {
    //lightWholeStrip(pixelsT, red);
    //lightWholeStrip(pixelsB, red);
    vibrate(HAPTIC1, false);
    vibrate(HAPTIC2, false);
    vibrate(HAPTIC3, false);
    analogWrite(PINGREEN, 0);
    analogWrite(PINRED, 255);
    pixelsT.fill(red);
    pixelsB.fill(red);
    pixelsT.show();
    pixelsB.show();
    //lightGreen(0);
    //lightRed(255);
  }
  delay(25);
}

// After the button is pushed but before the light turns green to cross
void buttonPushed() {
  for(int timestep = 0; timestep < 4000; timestep++) {
    float phase = (millis() / 1000.0 - 2.0 * 1 / (float)2) * M_PI;
    int brightness = int(pow((sin(phase) + 1.0) * 0.5, GAMMA) * 255 + 0.5);
    uint32_t currentYellow = pixelsT.Color(brightness, brightness, 0);
    pixelsT.fill(currentYellow);
    pixelsB.fill(currentYellow);
    pixelsT.show();
    pixelsB.show();
    delay(1);
  }
}

// While the button is on to cross
// each timestep is 100ms
// each haptic is on for 1s -> 10 timesteps
// each neopixel light is on for 1 timestep
void active() {
  analogWrite(PINGREEN, 255);
  analogWrite(PINRED, 0);
  for(int timestep = 0; timestep < 30; timestep++) {
    int activePixel = timestep % 10;
    pixelsT.clear();
    pixelsT.setPixelColor(activePixel, green);
    pixelsT.show();
    pixelsB.clear();
    pixelsB.setPixelColor(activePixel, green);
    pixelsB.show();
    if(timestep > 0 && timestep <= 5) {
      vibrate(HAPTIC1, true);
      vibrate(HAPTIC2, false);
      vibrate(HAPTIC3, false);
    }
    if(timestep > 5 && timestep <= 10) {
      vibrate(HAPTIC1, false);
      vibrate(HAPTIC2, true);
      vibrate(HAPTIC3, false);
    }
    if(timestep > 10 && timestep <= 15) {
      vibrate(HAPTIC1, false);
      vibrate(HAPTIC2, false);
      vibrate(HAPTIC3, true);
    }
    if(timestep > 15 && timestep <= 20) {
      vibrate(HAPTIC1, true);
      vibrate(HAPTIC2, false);
      vibrate(HAPTIC3, false);
    }
    if(timestep > 20 && timestep <= 25) {
      vibrate(HAPTIC1, false);
      vibrate(HAPTIC2, true);
      vibrate(HAPTIC3, false);
    }
    if(timestep >25 && timestep <= 30) {
      vibrate(HAPTIC1, false);
      vibrate(HAPTIC2, false);
      vibrate(HAPTIC3, true);
    }
    delay(100);
  }
  vibrate(HAPTIC1, false);
  vibrate(HAPTIC2, false);
  vibrate(HAPTIC3, false);
}

void vibrate(int pin, bool value) {
  if(value == true) {
    digitalWrite(pin, HIGH);
  } else {
    digitalWrite(pin, LOW);
  }
}

void vibratePattern() {
  vibrate(HAPTIC1, true);
  delay(500);
  vibrate(HAPTIC1, false);
  vibrate(HAPTIC2, true);
  delay(500);
  vibrate(HAPTIC2, false);
  vibrate(HAPTIC3, true);
  delay(500);
  vibrate(HAPTIC3, false);
  delay(500);
}

void neopixelLightWholeStrip(Adafruit_NeoPixel strip, uint32_t color) {
  strip.fill(color);
  strip.show();
}

void neopixelLghtOne(Adafruit_NeoPixel strip, int index, uint32_t color) {
  strip.setPixelColor(index, color);
  strip.show();
}
void neopixelLightOnlyOne(Adafruit_NeoPixel strip, int index, uint32_t color) {
  strip.clear();
  strip.setPixelColor(index, color);
  strip.show();
}