#include <Adafruit_NeoPixel.h>
//LED WS2811
#define NEOPIXEL_PIN 6
#define NUM_LEDS 200
// MOTION DETECTION
#define PIR_PIN_ENTRANCE 9
//LIGHT INTENSE DETECTION --> Analog INPUT
#define LIGHT_DETECTION_PIN_ENTRANCE A0
// The defaults
int SHOW_LED_COLOR_TIME_OUT = 2000;
int MIN_BURN_DURATION = 30000;
int LIGHT_INTENSE_BREAKPOINT = 20;
int MAX_BRIGHTNESS = 200;
int lightTurnedOfIndex = 0;
int DEFAULT_SHOW_DELAY = 20;
boolean lightIsTurnedOn = false;
// the LED Strip definition
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, NEOPIXEL_PIN, NEO_BRG);
uint32_t default_color = strip.Color(255, 120, 5);
uint32_t christmas_color_gold = strip.Color(255, 215, 0);
uint32_t christmas_color_red = strip.Color(255, 0, 0);
uint32_t color_black = strip.Color(0, 0, 0);
void setup()
{
  setupLEDStrip();
  pinMode(LIGHT_DETECTION_PIN_ENTRANCE, INPUT);
}

void loop()
{
  while (shouldEnableLight())
  {
    if (!lightIsTurnedOn)
    {
      lightIsTurnedOn = turnOnLight();
    }
  }
  if (lightIsTurnedOn || !shouldEnableLight())
  {
    delay(MIN_BURN_DURATION);
    lightIsTurnedOn = turnOffLight();
  }
   lightIsTurnedOn = turnOffLight();
}
void setupLEDStrip()
{
  strip.begin();
  strip.show();
  turnOnLight();
  delay(SHOW_LED_COLOR_TIME_OUT);
  turnOffLight();
}
boolean shouldEnableLight()
{
  return analogRead(LIGHT_DETECTION_PIN_ENTRANCE) <= LIGHT_INTENSE_BREAKPOINT && digitalRead(PIR_PIN_ENTRANCE) == HIGH;
}
//******TURN ON******
boolean turnOnLight()
{
  Serial.println(lightTurnedOfIndex); 
  for (int n = 0; n < NUM_LEDS; n++)
  {
    turnOnPixel(n, DEFAULT_SHOW_DELAY);
  }
  return true;
}

void turnOnPixel(int index, int showDelay) {
  //strip.setPixelColor(n, default_color);
  if (index % 2 == 0) {
    strip.setPixelColor(index, christmas_color_gold);
  }
  else {
    strip.setPixelColor(index, christmas_color_red);
  }
  strip.show();
  delay(showDelay);
}

//******TURN OFF******
boolean turnOffLight()
{
  for (int n = NUM_LEDS; n >= 0; n--)
  {
    turnOffPixel(n, DEFAULT_SHOW_DELAY);
    if (shouldEnableLight()) {
    lightTurnedOfIndex = n;
      break;
    }
  }
  return false;
}

void turnOffPixel(int index, int showDelay) {
  strip.setPixelColor(index, color_black);
    strip.show();
    delay(showDelay);

}
