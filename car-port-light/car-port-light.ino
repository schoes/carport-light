#include <Adafruit_NeoPixel.h>

#define NEOPIXEL_PIN 6
#define LED 50
#define PIR_PIN 2
#define STATUS_PIN 13
#define LED_DELAY 50


int timeout = LED_DELAY * LED + 100;
boolean motionDetected = false;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
uint32_t warm_white = strip.Color(255, 197, 143);
uint32_t off = strip.Color(0, 0, 0);
void setup() {
  pinMode(STATUS_PIN, OUTPUT);
  pinMode(PIR_PIN, INPUT);

  //set the strip to off
  strip.setBrightness(64);
  strip.begin();
  strip.show();
}

void loop() {
  digitalWrite(STATUS_PIN, digitalRead(PIR_PIN));

  if (digitalRead(PIR_PIN) == HIGH && !motionDetected) {
    motionDetected != motionDetected;
    for (int n = 0; n < LED ; n++) {
      strip.setPixelColor(n, warm_white);
      strip.show();
      delay(LED_DELAY);
    }
  }

  delay(timeout);
  for (int n = LED ; n >= 0 ; n--) {
    strip.setPixelColor(n, off);
    strip.show();
    delay(LED_DELAY);
  }

}
