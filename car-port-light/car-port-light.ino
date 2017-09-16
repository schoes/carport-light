#include <FastLED.h>
//LED WS2811
#define NEOPIXEL_PIN 6
#define NUM_LEDS 600
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
// check the real darkenss outside
#define LIGHT_INTENSE_BREAKPOINT 200
#define MIN_LIGHT_DURATION 30000

boolean motionDetected = false;
int fadeAmount = 5;
int MAX_BRIGHTNESS = 255;
unsigned long previousMillis = 0;

void setup() {
  // enable loggin
  Serial.begin(9600);
  setupLeds();
  setupMotionDetection();
  setupLightIntenseDetection();
}

void loop() {
  int lightIntensity = readLightIntensity();

  while (lightIntensity <= LIGHT_INTENSE_BREAKPOINT) {
    unsigned long currentMillis = millis();
    Serial.println("Ready to turn light on");
    if (digitalRead(PIR_PIN_ENTRANCE) == HIGH) {
      Serial.println("MOTION DETECTED");
      unsigned long burningDuration = getBurnDuration();
      if (currentMillis - previousMillis >= burningDuration) {
        enableLight();
      }
      else {
        disableLight();
      }
    }
    else {
      disableLight();
    }
  }
}

void setupLeds() {
  //SETUP LEDS
  delay(3000);
  FastLED.addLeds<LED_TYPE, NEOPIXEL_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.clear();
  FastLED.show();
}

void setupMotionDetection() {
  //SETUP MOTION DETECTION
  pinMode(PIR_PIN_ENTRANCE, INPUT);
  pinMode(PIR_PIN_DRIVE_IN, INPUT);
}

void setupLightIntenseDetection() {
  //SETUP LIGHT DETECTION
  pinMode(PIN_LIGHT_DETECTION_ENTRANCE, INPUT);
  pinMode(PIN_LIGHT_DETECTION_DRIVE_IN, INPUT);
}


unsigned int enableLight() {
  unsigned int burningDuration = getBurnDuration();
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
  return burningDuration;
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

int readLightIntensity() {
  int intensity = analogRead(PIN_LIGHT_DETECTION_ENTRANCE);
  Serial.println("Intensity");
  Serial.println(intensity);
  return intensity;
}

unsigned long getBurnDuration() {
  unsigned long duration = MIN_LIGHT_DURATION;
  return duration;

}


