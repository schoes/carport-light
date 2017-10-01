#include <FastLED.h>
//LED WS2811
#define NEOPIXEL_PIN 6
#define NUM_LEDS 600
#define LED_TYPE WS2811
#define COLOR_ORDER BRG
CRGB leds[NUM_LEDS];
// MOTION DETECTION
#define PIR_PIN_ENTRANCE 9
#define PIR_PIN_DRIVE_IN 10
//LIGHT INTENSE DETECTION --> Analog INPUT
#define LIGHT_DETECTION_PIN_ENTRANCE A0
#define LIGHT_DETECTION_PIN_DRIVE_IN A1
// check the real darkenss outside
#define LIGHT_INTENSE_BREAKPOINT 6.0

int fadeAmount = 5;
int MAX_BRIGHTNESS = 255;
unsigned long previousMillis = 0;
unsigned long minLightDuration = 30000;

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
    Serial.println("Ready to turn light on");
    if (digitalRead(PIR_PIN_ENTRANCE) == HIGH) {
      unsigned long currentMillis = millis();
      Serial.println("MOTION DETECTED");
      enableLight();
      delay(getBurnDuration());
      disableLight();
    }
  }
  disableLight();
}

void setupLeds() {
  //SETUP LEDS
  pinMode(NEOPIXEL_PIN, OUTPUT);
  delay(3000);
  FastLED.addLeds<LED_TYPE, NEOPIXEL_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.clear();
  FastLED.show();
  enableLight();
  disableLight();
}

void setupMotionDetection() {
  //SETUP MOTION DETECTION
  pinMode(PIR_PIN_ENTRANCE, INPUT);
}

void setupLightIntenseDetection() {
}


void enableLight() {
  for (int fader = 0; fader < MAX_BRIGHTNESS ; fader += 5) {
    for (int n = 0; n < NUM_LEDS ; n++) {
     // leds[n].setRGB(255, 147, 41);
     leds[n] = CRGB::HotPink;
      leds[n].maximizeBrightness(fader);
    }
    delay(20);
    FastLED.show();
  }
}

void disableLight() {
  for (int fader = 0; fader < 255; fader += 5) {
    for (int n = NUM_LEDS; n >= 0 ; n--) {
      leds[n].fadeToBlackBy(fader);
    }
    delay(20);
    FastLED.show();
  }
}

float readLightIntensity() {
  int intensity = analogRead(LIGHT_DETECTION_PIN_ENTRANCE);
  float volts0 = intensity * 0.004887585532746823069403714565; // calculate the voltage
  Serial.print(volts0);  //raw voltage
  Serial.println(" Volts.");
  float lux = (500 / ((10.72 / (5 - volts0)) * volts0));
  Serial.print(lux, 2); //lux calculation
  Serial.print(" Lux.");
  Serial.println("");
  delay(2000);
  return lux;
}

unsigned long getBurnDuration() {
  return minLightDuration;

}


