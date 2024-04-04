#include <Adafruit_NeoPixel.h>

class HapticMotor {
	int Pin;
	long turnOnTime;     // milliseconds of on-time
	long turnOffTime;    // milliseconds of off-time

	// These maintain the current state
	//macro state;             		// ledState used to set the LED

  public:
  HapticMotor(int pin, long on, long off) {
	  Pin = pin;
	  pinMode(pin, OUTPUT);     
	  
	  turnOnTime = on;
	  turnOffTime = off;
  }

  void Update(unsigned long time) {
    time = time % 1000;
    if(time > turnOnTime && time <= turnOffTime) {
      digitalWrite(Pin, HIGH);
      Vibrate(true);
    } else {
      digitalWrite(Pin, LOW);
      Vibrate(false);
    }
  }

  void Vibrate(bool value) {
    if(value == true) {
      digitalWrite(Pin, HIGH);
    } else {
     digitalWrite(Pin, LOW);
    }
  }

  void Off() {
    Vibrate(false);
  }
};

// Neopixels
#define NUMPIXELS 16 // number of neopixels in strip

#define BRIGHTNESS 50 // NeoPixel brightness, 0 (min) to 255 (max)
#define GAMMA 2.6

enum  pattern { NONE, FADE, DIRECTIONAL1, DIRECTIONAL2, COLORFILL, COUNTDOWN};

enum  direction { FORWARD, REVERSE };

// NeoPixel Class - derived from the Adafruit_NeoPixel class
// Derived from Bill Earl's NeoPattern
class NeoPixel : public Adafruit_NeoPixel {
    public:

    // Member Variables:  
    pattern  ActivePattern;  // which pattern is running
    direction Direction;     // direction to run the pattern
    
    unsigned long Interval;   // milliseconds between updates
    unsigned long lastUpdate; // last update of position
    
    uint32_t ActiveColor;
    uint16_t TotalSteps;  // total number of steps in the pattern
    uint16_t Index;  // current step within the pattern

    // Constructor - calls base-class constructor to initialize strip
    NeoPixel(uint16_t pixels, uint8_t pin, uint8_t type)
    :Adafruit_NeoPixel(pixels, pin, type) {
        
    }

    // Update the pattern
    void Update() {
        if((millis() - lastUpdate) > Interval) {
            lastUpdate = millis();
            switch(ActivePattern) {
                case FADE:
                    FadeUpdate();
                    break;
                case DIRECTIONAL1:
                    Directional1Update();
                    break;
                case DIRECTIONAL2:
                    Directional2Update();
                    break;
                case COUNTDOWN:
                    CountdownUpdate();
                default:
                    break;
            }
        }
    }

     // Increment the Index and reset at the end
    void Increment() {
        //if (Direction == FORWARD) {
           Index++;
           if (Index >= TotalSteps) {
                Index = 0;
            }
        //}
        //else // Direction == REVERSE
        //{
        //    --Index;
        //    if (Index <= 0) {
        //        Index = TotalSteps-1;
        //    }
        //}
    }

    void ColorFill(uint32_t color) {
      ActivePattern = COLORFILL;
      ActiveColor = color;
      fill(color);
      show();
    }

    // nothing to do here
    void ColorFillUpdate(){
      
    }

    void Directional1(uint32_t color, uint8_t interval, direction dir = FORWARD) {
      ActivePattern = DIRECTIONAL1;
        Interval = interval;
        TotalSteps = numPixels();
        ActiveColor = color;
        Direction = dir;
        Index = 0;
        clear();
    }

    void Directional1Update() {
      if(Direction == FORWARD) {
        clear();
        setPixelColor(Index, ActiveColor);
      } else if(Direction == REVERSE) {
        clear();
        setPixelColor(numPixels() - Index - 1, ActiveColor);
      }
      show();
      Increment();
    }

    // Initialize for directional
    // Two lights moving from left to right or right to left
    void Directional2(uint32_t color, uint8_t interval, direction dir = FORWARD) {
        ActivePattern = DIRECTIONAL2;
        Interval = interval;
        TotalSteps = numPixels() + 1;
        ActiveColor = color;
        Direction = dir;
        Index = 0;
        clear();
    }

    void Directional2Update() {
        if(Direction == FORWARD) {
          setPixelColor(Index, ActiveColor);
          setPixelColor(Index - 2, 0, 0, 0, 0); // turn off the one that's two before it
          if(Index == 0) { // if we've looped back to the start, turn off the last one
            setPixelColor(numPixels()-1, 0, 0, 0, 0);
          }
        } else if(Direction == REVERSE) {
          int currentPixel = numPixels() - Index - 1;
          setPixelColor(currentPixel, ActiveColor);
          setPixelColor(currentPixel + 2, 0, 0, 0, 0); // turn off the one 2 to the right
          if(Index == 0) { // if we're at the very right, turn off the one to the very left
            setPixelColor(0, 0, 0, 0, 0);
          }
        }
        show();
        Increment();
    }

    // Initialize for fade
    // Fading between a color and a very light version of that color
    void Fade(uint32_t color) {
        ActivePattern = FADE;
        ActiveColor = color;
    }

    void FadeUpdate() {
      float phase = (millis() / 1000.0 - 2.0 * 1 / (float)2) * M_PI;
      int brightness = int(pow((sin(phase) + 1.0) * 0.5, GAMMA) * 235 + 0.5) + 20;
      uint32_t currentYellow = Color(brightness, int(brightness*.45), 0);
      fill(currentYellow);
      show();
    }

    void Countdown(uint32_t color, uint8_t interval, direction dir = FORWARD) {
      ActivePattern = COUNTDOWN;
      Interval = interval;
      TotalSteps = numPixels();
      ActiveColor = color;
      Direction = dir;
      Index = 0;
      lastUpdate = millis();
      fill(color);
    }

    void CountdownUpdate() {
        if(Direction == FORWARD){ // turn off the numPixels-Index -1  light
          setPixelColor(numPixels() - Index -1, 0, 0, 0, 0);
        } else if(Direction == REVERSE) {
          setPixelColor(Index, 0, 0, 0, 0);
        }
      show();
      Increment();
    }
};

class Buzzer {

  int Pin;
	long turnOnTime = 150;     // milliseconds of on-time
	long turnOffTime = 1000;    // milliseconds of off-time
  int Hz;
  unsigned long previousBuzz;

  public:
  Buzzer(int pin, int hz) {
	  Pin = pin;
	  pinMode(pin, OUTPUT);     
    Hz = hz;
  }

  void Update(unsigned long time) {
    if(time - previousBuzz >= turnOnTime + turnOffTime) {
      tone(Pin, Hz, turnOnTime);
      previousBuzz = time;
    }
  }
};

#define ACTIVETIME 15000 // Time that the crosswalk is 'green' for
#define WAITINGTIME 5000 // time you have to wait after pushing the button for the crosswalk to go 'green'

NeoPixel top(NUMPIXELS, 2, NEO_GRBW + NEO_KHZ800);
NeoPixel bottom(NUMPIXELS, 3, NEO_GRBW + NEO_KHZ800);

uint32_t red = top.Color(255, 0, 0);
uint32_t green = top.Color(0, 255, 0);
uint32_t yellow = top.Color(255, 255, 0);
uint32_t black = top.Color(0, 0, 0);

// Force sensor
#define PINFSR 0
#define FSRACTIVEVALUE 200
int fsrValue;

// Haptic
HapticMotor left(7, 50, 300);
HapticMotor middle(6, 400, 650);
HapticMotor right(5, 700, 950);

// Noods
#define PINRED 10
#define PINGREEN 11

// Buzzer
Buzzer speaker(8, 400);

// Loop stuff
enum mode {OFF, WAITING, ACTIVE};
mode currentMode = OFF;
unsigned long modeStartMillis;

void setup() {
  // Initialize the NeoPixel library.
  top.begin();
  bottom.begin();
  top.setBrightness(75);
  bottom.setBrightness(75);
  top.clear();
  bottom.clear();
  top.show();
  bottom.show();

  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  // Noods
  pinMode(PINRED, OUTPUT);
  pinMode(PINGREEN, OUTPUT);

  modeStartMillis = millis();

  Serial.begin(9600);
  Serial.println("Starting up!");
}

void loop() {
  fsrValue = analogRead(PINFSR);  

  unsigned long currentMillis = millis();
  unsigned long timeDiff = currentMillis - modeStartMillis;
 
  //Serial.print("Analog reading = ");
  //Serial.println(fsrValue);

  speaker.Update(currentMillis);

  switch(currentMode) {
    case OFF:
      if(fsrValue > FSRACTIVEVALUE) {
        Serial.println("Button pressed");
        currentMode = WAITING;
        modeStartMillis = millis();
        top.Fade(yellow);
        bottom.Fade(yellow);
        break;
      }
      // Haptics do nothing
      left.Off();
      middle.Off();
      right.Off();
      // Neopixels do stuff
      top.ColorFill(red);
      bottom.ColorFill(red);
      // Noods
      analogWrite(PINGREEN, 0);
      analogWrite(PINRED, 255);
      break;
    case WAITING:
      if (timeDiff > WAITINGTIME) { // safe to cross
        Serial.println("Activating");
        currentMode = ACTIVE;
        modeStartMillis = currentMillis;
        top.Directional1(green, 100);
        bottom.Countdown(green, 2000, REVERSE);
        break;
      }
      // Haptics do nothing
      left.Off();
      middle.Off();
      right.Off();
      // NeoPixels do stuff
      top.Update();
      bottom.Update();
      // Noods
      analogWrite(PINGREEN, 0);
      analogWrite(PINRED, 255);
      break;
    case ACTIVE:
      if(timeDiff > ACTIVETIME) { // active time is up, turn off
        Serial.println("Turning off");
        currentMode = OFF;
        left.Off();
        middle.Off();
        right.Off();
        top.ColorFill(red);
        bottom.ColorFill(red);
        break;
      }
      // Haptics
      left.Update(timeDiff);
      middle.Update(timeDiff);
      right.Update(timeDiff);
      // Neopixels
      top.Update();
      bottom.Update();
      // noods
      analogWrite(PINGREEN, 255);
      analogWrite(PINRED, 0);
      break;
  }
}