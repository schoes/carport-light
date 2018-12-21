#include <Adafruit_NeoPixel.h>
//LED WS2811
#define NEOPIXEL_PIN 6
#define NUM_LEDS 200
Adafruit_NeoPixel strip;
// MOTION DETECTION
#define PIR_PIN_ENTRANCE 9
//LIGHT INTENSE DETECTION --> Analog INPUT
#define LIGHT_DETECTION_PIN_ENTRANCE A0
int SAFETY_LED_TIME_OUT = 3000;
int SHOW_LED_COLOR_TIME_OUT = 2000;
int MIN_BURN_DURATION = 30000;
int LIGHT_INTENSE_BREAKPOINT = 20;
int fadeAmount = 5;
int MAX_BRIGHTNESS = 200;
boolean lightOn = false;
bool motionDetected = false;
void setup()
{

  //Serial.begin(9600);
  setupMotionDetection();
  setupLeds();
  showLedColor();
}

void loop()
{
  while (digitalRead(PIR_PIN_ENTRANCE) == HIGH)
  {
    if (!lightOn)
    {
      Serial.println("CHECK THE LIGHT OUTSIDE");
      if (shouldEnableLight())
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
  strip = Adafruit_NeoPixel(NUM_LEDS, NEOPIXEL_PIN, NEO_BRG);
  strip.begin();
  strip.show();
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
}

bool enableLight()
{
  Serial.println("Turn on the light");
  for (int n = 0; n < NUM_LEDS; n++)
  {
    strip.setPixelColor(n, 255, 120, 5);
    strip.show();
    delay(50);
  }
}
return true;
}

bool disableLight()
{
  for (int fader = 0; fader < MAX_BRIGHTNESS; fader += 5)
  {
    for (int n = NUM_LEDS; n >= 0; n--)
    {
      leds.setPixelColor(n, 0, 0, 0);
      strip.show();
      delay(50);
    }
  }

  return false;
}

bool shouldEnableLight()
{
  int intensity = analogRead(LIGHT_DETECTION_PIN_ENTRANCE);
  Serial.print("intensity:");
  Serial.print(intensity);
  Serial.println(" ");
  return intensity <= LIGHT_INTENSE_BREAKPOINT;
}

int getBurnDuration()
{
  return MIN_BURN_DURATION;
}
