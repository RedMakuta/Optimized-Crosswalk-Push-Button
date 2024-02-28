#include <Adafruit_NeoPixel.h>

class HapticMotor
{
	int pin;
	long turnOnTime;     // milliseconds of on-time
	long turnOffTime;    // milliseconds of off-time

	// These maintain the current state
	macro state;             		// ledState used to set the LED

  public:
  HapticMotor(int pin, long on, long off) {
	pin = pin;
	pinMode(pin, OUTPUT);     
	  
	turnOnTime = on;
	turnOffTime = off;
  }

  void Update(unsigned long time) {
    time = time % 1000;
    if(time > turnOntime && time <= turnOffTime) {
      Vibrate(true);
    } else {
      Vibrate(false)
    }
  }

  void Vibrate(bool value) {
    if(value == true) {
      digitalWrite(pin, HIGH);
    } else {
     digitalWrite(pin, LOW);
    }
  }

  void Off() {
    Vibrate(false);
  }
};

enum  pattern { NONE, FADE, DIRECTIONAL };

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
    
    uint32_t Color;
    uint16_t TotalSteps;  // total number of steps in the pattern
    uint16_t Index;  // current step within the pattern
    
    void (*OnComplete)();  // Callback on completion of pattern

    // Constructor - calls base-class constructor to initialize strip
    NeoPatterns(uint16_t pixels, uint8_t pin, uint8_t type, void (*callback)())
    :Adafruit_NeoPixel(pixels, pin, type)
    {
        OnComplete = callback;
    }

    // Update the pattern
    void Update() {
        if((millis() - lastUpdate) > Interval) {
            lastUpdate = millis();
            switch(ActivePattern) {
                case FADE:
                    FadeUpdate();
                    break;
                case DIRECTIONAL:
                    DirectionalUpdate();
                    break;
                default:
                    break;
            }
        }
    }

     // Increment the Index and reset at the end
    void Increment() {
        if (Direction == FORWARD) {
           Index++;
           if (Index >= TotalSteps) {
                Index = 0;
                if (OnComplete != NULL) {
                    OnComplete(); // call the comlpetion callback
                }
            }
        }
        else // Direction == REVERSE
        {
            --Index;
            if (Index <= 0) {
                Index = TotalSteps-1;
                if (OnComplete != NULL) {
                    OnComplete(); // call the comlpetion callback
                }
            }
        }
    }

    void ColorFill(uint32_t color) {
      ActivePattern = COLORFILL;
      Color = color;
      fill(color);
      show();
    }

    // nothing to do here
    void ColorFillUpdate(){
      break;
    }

    // Initialize for directional
    // Two lights moving from left to right or right to left
    void Directional(uint32_t color, uint8_t interval, direction dir = FORWARD) {
        ActivePattern = DIRECTIONAL;
        Interval = interval;
        TotalSteps = numPixels() + 1;
        Color = color;
        Index = 0;
        Direction = dir;
    }

    void DirectionalUpdate() {
        setPixelColor(Index, Color);
        if(Direction == FORWARD) {
          setPixelColor(Index - 2, 0, 0, 0, 0);
        } else if(Direction == REVERSE) {
          setPixelColor(Index + 2, 0, 0, 0, 0);
        }
        show();
        Increment();
    }

    // Initialize for fade
    // Fading between a color and a very light version of that color
    void Fade(uint32_t color) {
        ActivePattern = FADE;
        Color = color;
    }

    void FadeUpdate() {
      float phase = (millis() / 1000.0 - 2.0 * 1 / (float)2) * M_PI;
      int brightness = int(pow((sin(phase) + 1.0) * 0.5, GAMMA) * 200 + 0.5) + 55;
      uint32_t currentYellow = pixelsT.Color(brightness, int(brightness*.9), 0);
      fill(currentYellow);
      show();
    }
    
}

#define ACTIVETIME 15000 // Time that the crosswalk is 'green' for
#define WAITINGTIME 2000; // time you have to wait after pushing the button for the crosswalk to go 'green'

// Neopixels
#define NUMPIXELS 8 // number of neopixels in strip

#define BRIGHTNESS 50 // NeoPixel brightness, 0 (min) to 255 (max)
#define GAMMA 2.6

NeoPixel top = NeoPixel(NUMPIXELS, 2, NEOGRBW + NEO_KHZ800);
NeoPixel top = NeoPixel(NUMPIXELS, 3, NEOGRBW + NEO_KHZ800);

uint32_t red = top.Color(255, 0, 0);
uint32_t green = top.Color(0, 255, 0);
uint32_t yellow = top.Color(255, 255, 0);

// Force sensor
#define PINFSR 0
#define FSRACTIVEVALUE 400
int fsrValue;

// Haptic
HapticMotor left(5, 0, 333);
HapticMotor middle(6, 334, 666);
HapticMotor right(7, 667, 1000);

// Noods
#define PINRED 10
#define PINGREEN 11

// Loop stuff
enum mode {OFF, WAITING, ACTIVE};
mode currentMode = OFF;
unsigned long modeStartMillis;

void setup() {
  // Initialize the NeoPixel library.
  top.begin();
  bottom.begin();
  top.setBrightness(BRIGHTNESS);
  bottom.setBrightness(BRIGHTNESS);
  top.clear();
  bottom.clear();
  top.show();
  bottom.show();

  // Noods
  pinMode(PINRED, OUTPUT);
  pinMode(PINGREEN, OUTPUT);

  Serial.begin(9600);
  Serial.println("Starting up!");
}

void loop() {
  fsrValue = analogRead(PINFSR);  
 
  Serial.print("Analog reading = ");
  Serial.println(fsrValue);     // the raw analog reading

  switch(currentMode) {
    case OFF:
      if(fsrValue > FSRACTIVEVALUE) {
        currentMode = WAITING;
        modeStartMillis = millis();
        top.Fade(yellow);
        bottom.Fade(yellow);
        continue;
      }
      // Haptics do nothing
      left.Off();
      middle.Off();
      right.Off();
      // Neopixels do stuff
      top.ColorFill(red);
      bottom.ColorFill(red);
      break;
    case WAITING:
      unsigned long currentMillis = millis();
      unsigned long timeDiff = currentMillis - modeStartMillis;
      if(timeDiff > WAITINGTIME) { // safe to cross
        currentMode = ACTIVE;
        modeStartMillis = currentMillis;
        top.Directional(green, 100);
        bottom.Directional(green, 100);
        continue;
      }
      // Haptics do nothing
      left.Off();
      middle.Off();
      right.Off();
      // NeoPixels do stuff
      top.Update();
      bottom.Update();
      break;
    case ACTIVE:
      unsigned long currentMillis = millis();
      unsigned long timeDiff = currentMillis - modeStartMillis;
      if(timeDiff > ACTIVETIME) { // active time is up, turn off
        currentMode = OFF;
        left.Off();
        middle.Off();
        right.Off();
        top.ColorFill(red);
        bottom.ColorFill(red);
        break;
      }
      left.Update(timeDiff);
      middle.Update(timeDiff);
      right.Update(timeDiff);
      top.Update();
      bottom.Update();
      break;
  }
}