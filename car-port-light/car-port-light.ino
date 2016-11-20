#include <FastLED.h>
//LED WS2811
#define NEOPIXEL_PIN 6
#define NUM_LEDS 50
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
// MOTION DETECTION
#define PIR_PIN_ENTRANCE 9
#define PIR_PIN_DRIVE_IN 10
//LIGHT INTENSE DETECTION --> Analog INPUT
#define PIN_LIGHT_DETECTION_ENTRANCE 0
#define PIN_LIGHT_DURATION 1
#define PIN_LIGHT_DETECTION_DRIVE_IN 10
#define LIGHT_INTENSE_BREAKPOINT 200
#define MIN_LIGHT_DURATION 30000

boolean motionDetected = false;
int fadeAmount = 5;
int MAX_BRIGHTNESS = 255;
void setup() {
  //Serial.begin(9600);
  delay(3000);
  //SETUP LEDS
  FastLED.addLeds<LED_TYPE, NEOPIXEL_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.clear();
  FastLED.show();
  //SETUP MOTION DETECTION
  pinMode(PIR_PIN_ENTRANCE, INPUT);
  //pinMode(PIR_PIN_DRIVE_IN, INPUT);
  //SETUP LIGHT DETECTION
  //pinMode(PIN_LIGHT_DETECTION_ENTRANCE, INPUT);
  pinMode(PIN_LIGHT_DETECTION_DRIVE_IN, INPUT);
}

void loop() {
  int lightIntensity = analogRead(PIN_LIGHT_DETECTION_ENTRANCE);
  unsigned long burningDuration = MIN_LIGHT_DURATION;
  if (lightIntensity <= LIGHT_INTENSE_BREAKPOINT) {
    while (digitalRead(PIR_PIN_ENTRANCE) == HIGH) {
      enableLight();
      delay(burningDuration);
    }


    while (digitalRead(PIR_PIN_ENTRANCE) == LOW) {
      disableLight();
    }
  }
}

void enableLight() {
  for (int fader = 0; fader < MAX_BRIGHTNESS ; fader += 5) {
    for (int n = 0; n < NUM_LEDS ; n++) {
      leds[n] = Candle;
      leds[n].maximizeBrightness(fader);
      if (n % 10 == 0) {
        FastLED.show();
      }

    }
    delay(20);
  }
}

void disableLight() {
  for (int fader = 0; fader < 255; fader += 5) {
    for (int n = NUM_LEDS; n >= 0 ; n--) {
      leds[n].fadeToBlackBy(fader);
      if (n % 10 == 0) {
        FastLED.show();
      }

    }
    delay(20);
  }
}

int calculateBurningDuration() {
  unsigned long duration = (analogRead(PIN_LIGHT_DURATION) * 5000);
  Serial.println("READ DURATION SECONDS:");
  Serial.println(duration/1000);
  if (duration < MIN_LIGHT_DURATION) {
    duration =  MIN_LIGHT_DURATION;
  }
  Serial.println("CALCULATED DURATION SECONDS:");
  Serial.println(duration / 1000);
  return duration;

}


