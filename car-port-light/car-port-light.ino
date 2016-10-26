#include <FastLED.h>
//LED WS2811
#define NEOPIXEL_PIN 6
#define NUM_LEDS 50
#define LED_DELAY 50
#define UPDATES_PER_SECOND 100
#define LED_COUNT_ON 5
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
#define BRIGHTNESS  64
#define TEMPERATURE_1 Candle
CRGB leds[NUM_LEDS];
// MOTION DETECTION
#define PIR_PIN_ENTRANCE 9
#define PIR_PIN_DRIVE_IN 10
//LIGHT INTENSE DETECTION --> Analog INPUT
#define PIN_LIGHT_DETECTION_ENTRANCE 0
#define PIN_LIGHT_DETECTION_DRIVE_IN 10
#define LIGHT_INTENSE_BREAKPOINT 20

int timeout = LED_DELAY * NUM_LEDS + 100;
boolean motionDetected = false;
int brightness = 0;
int fadeAmount = 5;
void setup() {
  Serial.begin(9600);
  delay(3000);
  //SETUP LEDS
  FastLED.addLeds<LED_TYPE, NEOPIXEL_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  //  FastLED.setBrightness(  BRIGHTNESS );
  //  FastLED.setTemperature( TEMPERATURE_1 );
  //SETUP MOTION DETECTION
  pinMode(PIR_PIN_ENTRANCE, INPUT);
  pinMode(PIR_PIN_DRIVE_IN, INPUT);
  //SETUP LIGHT DETECTION
  //pinMode(PIN_LIGHT_DETECTION_ENTRANCE, INPUT);
  pinMode(PIN_LIGHT_DETECTION_DRIVE_IN, INPUT);
}

void loop() {

  //FastLED.delay(1000 / UPDATES_PER_SECOND);
  int lightIntensity = analogRead(PIN_LIGHT_DETECTION_ENTRANCE);
  //Serial.println(lightIntensity);
  if (lightIntensity <= LIGHT_INTENSE_BREAKPOINT) {

    while (digitalRead(PIR_PIN_ENTRANCE) == HIGH) {
      //Serial.println("MOTION DETECTED");
      if (brightness <= 255) {
        brightness += fadeAmount;
        //Serial.println("brigthness:" + brightness);

        for (int n = 0; n < NUM_LEDS ; n++) {
          leds[n] = CRGB::Green;
          leds[n].fadeLightBy(brightness);
        }
        FastLED.show();
        delay(1000);
      }
    }

    while (digitalRead(PIR_PIN_ENTRANCE) == LOW) {
      if (brightness > 0) {
        brightness -= fadeAmount;
        //Serial.println("brigthness:" + brightness);

        for (int n = 0; n < NUM_LEDS ; n++) {
          leds[n] = CRGB::Black;
          leds[n].fadeLightBy(brightness);
        }
        FastLED.show();
        delay(1000);
      }
    }
    //FastLED.clear();
  }
  //if () {

  //motionDetected != motionDetected;


  // delay(20);

  //delay(20);  // This delay sets speed of the fade. I usually do from 5-75 but you can always go higher.

  //}

}
