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
void setup()
{
  // enable loggin
  //Serial.begin(9600);
  setupLeds();
  setupMotionDetection();
}

void loop()
{
  delay(CHECK_TIME_OUT);
  lightIntensity = readLightIntensityInLUX();
  bool lightOn = false;
  while (lightIntensity <= LIGHT_INTENSE_BREAKPOINT)
  {
    Serial.println("Ready to turn light on");
    if (digitalRead(PIR_PIN_ENTRANCE) == HIGH)
    {
      //unsigned long currentMillis = millis();
      Serial.println("MOTION DETECTED");
      if (!lightOn)
      {
        Serial.println("Turn on the light");
        enableLight();
        lightOn = true;
      }
    }
    else
    {
      Serial.println("NO MORE MOTION DETECTED");
      delay(getBurnDuration());
      Serial.println("Will disable the light");
      disableLight();
      lightIntensity = readLightIntensityInLUX();
      lightOn = false;
    }
  }

  disableLight();
}

void setupLeds()
{
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
  //SETUP MOTION DETECTION
  pinMode(PIR_PIN_ENTRANCE, INPUT);
}

void enableLight()
{
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
}

void disableLight()
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
