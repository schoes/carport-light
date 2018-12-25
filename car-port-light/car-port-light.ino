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
bool lightOn = false;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, NEOPIXEL_PIN, NEO_BRG);

void setup()
{
  setupLEDStrip();
  pinMode(LIGHT_DETECTION_PIN_ENTRANCE, INPUT);
}

void loop()
{
  while (digitalRead(PIR_PIN_ENTRANCE) == HIGH)
  {
    if (!lightOn)
    {
      if (shouldEnableLight())
      {
        enableLight();
      }
    }
  }
  if (lightOn)
  {
    delay(getBurnDuration());
    disableLight();
  }
}
void setupLEDStrip()
{
  strip.begin();
  //strip.setBrightness(64);
  strip.show();
  enableLight();
  delay(SHOW_LED_COLOR_TIME_OUT);
  disableLight();
}
bool shouldEnableLight()
{
  int intensity = analogRead(LIGHT_DETECTION_PIN_ENTRANCE);
  return intensity <= LIGHT_INTENSE_BREAKPOINT;
}

void enableLight()
{
  for (int n = 0; n < NUM_LEDS; n++)
  {
    strip.setPixelColor(n, 255, 120, 5);
    strip.show();
    delay(30);
  }
  lightOn = true;
}

void disableLight()
{
  for (int n = NUM_LEDS; n >= 0; n--)
  {
    strip.setPixelColor(n, 0, 0, 0);
    strip.show();
    delay(30);
  }
  lightOn = false;
}

int getBurnDuration()
{
  return MIN_BURN_DURATION;
}