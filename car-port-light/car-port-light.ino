#include <FastLED.h>
//LED WS2811
#define NEOPIXEL_PIN 6
#define NUM_LEDS 600
#define LED_TYPE WS2811
#define COLOR_ORDER BRG
CRGB leds[NUM_LEDS];
// MOTION DETECTION
#define PIR_PIN_ENTRANCE 9
//LIGHT INTENSE DETECTION --> Analog INPUT
#define LIGHT_DETECTION_PIN_ENTRANCE A0
// https://en.wikipedia.org/wiki/Lux

//#define CHECK_TIME_OUT 300000 // milliseconsd => 5 minutes
int CHECK_TIME_OUT = 3000;
int SAFETY_LED_TIME_OUT = 3000;
int SHOW_LED_COLOR_TIME_OUT = 1000;
int MIN_BURN_DURATION = 30000;
int LIGHT_INTENSE_BREAKPOINT = 1;
int fadeAmount = 5;
int MAX_BRIGHTNESS = 200;
bool lightOn = false;
bool motionDetected = false;
void setup()
{
  Serial.begin(9600);
  setupMotionDetection();
  setupLeds();
  //showLedColor();
}

void loop()
{
  while (digitalRead(PIR_PIN_ENTRANCE) == HIGH)
  {
    if (!lightOn)
    {
      Serial.println("CHECK THE LIGHT OUTSIDE");
      if (LIGHT_INTENSE_BREAKPOINT >= readLightIntensityInLUX())
      {
        Serial.println("GET SOME LIGHT");
        lightOn = enableLight();
      }
    }
  }
  Serial.println("NO MOVEMENTS");
  if (lightOn)
  {
    delay(getBurnDuration());
    lightOn = disableLight();
  }
}

void setupLeds()
{
  Serial.println("==SETUP THE LED STRIP==");
  //SETUP LEDS
  pinMode(NEOPIXEL_PIN, OUTPUT);
  delay(SAFETY_LED_TIME_OUT);
  FastLED.addLeds<LED_TYPE, NEOPIXEL_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.clear();
  FastLED.show();
}

void showLedColor()
{
  enableLight();
  delay(SHOW_LED_COLOR_TIME_OUT);
  disableLight();
}

void setupMotionDetection()
{
  pinMode(LIGHT_DETECTION_PIN_ENTRANCE, INPUT);
  //pinMode(PIR_PIN_ENTRANCE, INPUT);
}

bool enableLight()
{
  Serial.println("Turn on the light");
  for (int fader = 0; fader < MAX_BRIGHTNESS; fader += 5)
  {
    for (int n = 0; n < NUM_LEDS; n++)
    {
      leds[n] = CRGB::Yellow;
      leds[n].maximizeBrightness(fader);
    }
    delay(20);
    FastLED.show();
  }
  return true;
}

bool disableLight()
{
  for (int fader = 0; fader < MAX_BRIGHTNESS; fader += 5)
  {
    for (int n = NUM_LEDS; n >= 0; n--)
    {
      leds[n].fadeToBlackBy(fader);
    }
    delay(20);
    FastLED.show();
  }

  return false;
}

int readLightIntensityInLUX()
{
  int intensity = analogRead(LIGHT_DETECTION_PIN_ENTRANCE);
  Serial.println("intensity");
  Serial.print(intensity);
  Serial.println(" ");
  delay(20);
  float volts = intensity * 0.004887585532746823069403714565; // calculate the voltage
  Serial.print(volts);
  if (volts <= 0)
  {
    return LIGHT_INTENSE_BREAKPOINT;
  } //raw voltage
  int lux = (500 / ((10.72 / (5 - volts)) * volts));
  Serial.print(lux, 5); //lux calculation
  Serial.println(" Lux.");
  delay(20);
  return lux;
}

int getBurnDuration()
{
  return MIN_BURN_DURATION;
}
