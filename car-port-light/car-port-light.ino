#include <Adafruit_NeoPixel.h>
//LED WS2811
#define NEOPIXEL_PIN 6
#define NUM_LEDS 200
// MOTION DETECTION
#define PIR_PIN_ENTRANCE 9
//LIGHT INTENSE DETECTION --> Analog INPUT
#define LIGHT_DETECTION_PIN_ENTRANCE A0
int SHOW_LED_COLOR_TIME_OUT = 2000;
int MIN_BURN_DURATION = 30000;
int LIGHT_INTENSE_BREAKPOINT = 20;
int MAX_BRIGHTNESS = 200;
int lightDisaledIndex = 0;
boolean lightOn = false;
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
    if (!lightOn)
    {
      enableLight();
    }
  }
  if (lightOn)
  {
    delay(MIN_BURN_DURATION);
    disableLight();
  }
}
void setupLEDStrip()
{
  strip.begin();
  strip.show();
  enableLight();
  delay(SHOW_LED_COLOR_TIME_OUT);
  disableLight();
}
boolean shouldEnableLight()
{
  return (analogRead(LIGHT_DETECTION_PIN_ENTRANCE) <= LIGHT_INTENSE_BREAKPOINT) && (digitalRead(PIR_PIN_ENTRANCE) == HIGH);
}

void enableLight()
{
  for (int n = lightDisaledIndex; n < NUM_LEDS; n++)
  {
    setStripColor(n);
  }
  lightOn = true;
}

void setStripColor(int index) {
  //strip.setPixelColor(n, default_color);
  if (index % 2 == 0) {
    strip.setPixelColor(index, christmas_color_gold);
  }
  else {
    strip.setPixelColor(index, christmas_color_red);
  }
  strip.show();
  delay(20);
}

void disableLight()
{
  for (int n = NUM_LEDS; n >= 0; n--)
  {
    strip.setPixelColor(n, color_black);
    strip.show();
    delay(20);
    if (digitalRead(PIR_PIN_ENTRANCE) == HIGH) {
      lightDisaledIndex = n;
      break;
    }
  }
  lightDisaledIndex = 0;
  lightOn = false;
}
