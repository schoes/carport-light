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
    delay(MIN_BURN_DURATION);
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
    //strip.setPixelColor(n, default_color);
    if (n % 2 == 0) {
      strip.setPixelColor(n, christmas_color_gold);
    }
    else {
      strip.setPixelColor(n, christmas_color_red);
    }
    strip.show();
    delay(20);
  }
  lightOn = true;
}

void disableLight()
{
  for (int n = NUM_LEDS; n >= 0; n--)
  {
    strip.setPixelColor(n, color_black);
    strip.show();
    delay(20);
    if(digitalRead(PIR_PIN_ENTRANCE) == HIGH){
      break;
    }
  }
  lightOn = false;
}
