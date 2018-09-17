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
#define LIGHT_INTENSE_BREAKPOINT 200.0
#define CHECK_TIME_OUT 300000 // milliseconsd => 5 minutes
#define SAFETY_LED_TIME_OUT 3000
#define SHOW_LED_COLOR_TIME_OUT 1000

int fadeAmount = 5;
int MAX_BRIGHTNESS = 200;
unsigned long previousMillis = 0;
unsigned long minLightDuration = 30000;
float lightIntensity = 1000;
bool lightOn = false;
void setup()
{
  // enable loggin
  //Serial.begin(9600);
  setupLeds();
  setupMotionDetection();
}

void loop()
{
  disableLight();
  delay(CHECK_TIME_OUT);
  while (readLightIntensityInLUX() <= LIGHT_INTENSE_BREAKPOINT)
  {
    Serial.println("==GET INTO ACTIVE STATE==");
    if (!lightOn)
    {
      if (digitalRead(PIR_PIN_ENTRANCE) == HIGH)
      {
        Serial.println("==MOTION DETECTED==");
        lightOn = enableLight();
        delay(getBurnDuration());
      }
    }
    else
    {
      Serial.println("==NO MORE MOTION DETECTED==");
      lightOn = disableLight();
    }
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
  enableLight();
  delay(SHOW_LED_COLOR_TIME_OUT);
  disableLight();
}

void setupMotionDetection()
{
  Serial.println("==SETUP THE MOTION DETECTION==");
  //SETUP MOTION DETECTION
  pinMode(PIR_PIN_ENTRANCE, INPUT);
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
  Serial.println("Will disable the light");
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

float readLightIntensityInLUX()
{
  int intensity = analogRead(LIGHT_DETECTION_PIN_ENTRANCE);
  float volts0 = intensity * 0.004887585532746823069403714565; // calculate the voltage
  Serial.print(volts0);
  if (volts0 <= 0)
  {
    return LIGHT_INTENSE_BREAKPOINT;
  } //raw voltage
  float lux = (500 / ((10.72 / (5 - volts0)) * volts0));
  Serial.print(lux, 2); //lux calculation
  Serial.print(" Lux.");
  Serial.println("");
  delay(2000);
  return lux;
}

unsigned long getBurnDuration()
{
  return minLightDuration;
}
