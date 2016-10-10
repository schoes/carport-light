 #include <FastLED.h>
//LED WS2811
#define NEOPIXEL_PIN 6
#define NUM_LEDS 300
#define LED_DELAY 50
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
#define BRIGHTNESS  64
#define TEMPERATURE_1 Candle
CRGB leds[NUM_LEDS];
// MOTION DETECTION
#define PIR_PIN_ENTRANCE 2
#define PIR_PIN_DRIVE_IN 2
//LIGHT INTENSE DETECTION
#define PIN_LIGHT_DETECTION_ENTRANCE 9
#define PIN_LIGHT_DETECTION_DRIVE_IN 10
#define LIGHT_INTENSE_BREAKPOINT 70



int timeout = LED_DELAY * NUM_LEDS + 100;
boolean motionDetected = false;
void setup() {
  delay(3000);
  //SETUP LEDS
  FastLED.addLeds<LED_TYPE, NEOPIXEL_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  FastLED.setTemperature( TEMPERATURE_1 );
  //SETUP MOTION DETECTION
  pinMode(PIR_PIN_ENTRANCE, INPUT);
  pinMode(PIR_PIN_DRIVE_IN, INPUT);
  //SETUP LIGHT DETECTION
  pinMode(PIN_LIGHT_DETECTION_ENTRANCE,INPUT);
  pinMode(PIN_LIGHT_DETECTION_DRIVE_IN,INPUT);
}

void loop() {
  if (digitalRead(PIR_PIN_ENTRANCE) == HIGH && !motionDetected) {
    motionDetected != motionDetected;
    for (int n = 0; n < NUM_LEDS ; n++) {
      if(n%60 == 0){
        FastLED.show();
        delay(LED_DELAY);
      }  
    }
  }

}
