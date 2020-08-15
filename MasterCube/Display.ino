#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

int lastDisplay = 0;
float targetR[] = {0,0,0,0,0,0};
float targetG[] = {0,0,0,0,0,0};
float targetB[] = {0,0,0,0,0,0};
float actualR[] = {0,0,0,0,0,0};
float actualG[] = {0,0,0,0,0,0};
float actualB[] = {0,0,0,0,0,0};
byte motionType = 0;
float updateR = 0;
float updateG = 0;
float updateB = 0;

void Display_Init() 
{
  strip.begin();
  for (int i = 0; i < NEOPIXEL_COUNT; i ++) 
  {
    strip.setPixelColor(i, strip.Color(0, 255, 0));
    strip.setPixelColor(i, strip.Color(0, 255, 0));
  }
  strip.show();
  Serial.println(F("Neopixels Started"));
}

void Display_SetColor(byte index, byte r, byte g, byte b)
{
  targetR[index] = r;
  targetG[index] = g;
  targetB[index] = b;
}

void Display_Update() {

  // animate to colors
  for (byte i = 0; i < NEOPIXEL_COUNT; i ++)
  {
    updateR = actualR[i] = Display_MoveColorTowards(actualR[i], targetR[i], COLOR_CHANGE_SPEED);
    updateG = actualG[i] = Display_MoveColorTowards(actualG[i], targetG[i], COLOR_CHANGE_SPEED);
    updateB = actualB[i] = Display_MoveColorTowards(actualB[i], targetB[i], COLOR_CHANGE_SPEED);

    if ((updateR < 1.0f && updateG < 1.0f && updateB < 1.0f) || (updateR > 254.0f && updateG > 254.0f && updateB > 254.0f)) 
    {
      // don't mess with full black or full white
    }
    else
    {
      switch(motionType) 
      {
        case 0:
          break;
    
        case 1:
          updateR += cos((millis() * MOTION_SPEED) + float(i)) * 40.0f;
          updateG += sin((millis() * MOTION_SPEED) + float(i)) * 40.0f;
          updateB += cos((millis() * MOTION_SPEED) + float(i)) * 40.0f;
          break;
      }
    }
    
    strip.setPixelColor(
      i, 
      min(255, max(0, int(updateR))), 
      min(255, max(0, int(updateG))), 
      min(255, max(0, int(updateB))));
  }
  
  if (millis() - lastDisplay > DISPLAY_RATE) 
  {
    lastDisplay = millis();
    strip.show();
  }
}

void Display_SetMotion(byte val)
{
  motionType = val;
}

void Display_Force_Update() 
{
  strip.show();
}

float Display_MoveColorTowards(float current, float target, float changeSpeed) {

  if (current < target)
    return current += changeSpeed;
  else if (current > target)
    return current -= changeSpeed;

  return target;
}

void Display_Flash(uint8_t r, uint8_t g, uint8_t b, byte count, int pause) 
{
  Display_SetAllImmediate(0, 0, 0);
  delay(pause);
    
  for (byte i = 0; i < count; i++)
  { 
    Display_SetAllImmediate(r, g, b);
    delay(pause);
    Display_SetAllImmediate(0, 0, 0);
    delay(pause);
  }
}

void Display_Clear() 
{
  Display_SetAll(0, 0, 0);
}

void Display_SetAll(uint8_t r, uint8_t g, uint8_t b) 
{
  for (int i = 0; i < NEOPIXEL_COUNT; i++) {
    Display_SetColor(i, r, g, b);
  }
}

void Display_SetAllImmediate(uint8_t r, uint8_t g, uint8_t b) 
{
  for (int i = 0; i < NEOPIXEL_COUNT; i++) {
    Display_SetColor(i, r, g, b);
    strip.setPixelColor(i, strip.Color(r, g, b));
  }
  strip.show();
}

void Display_Set_Direction(byte which) 
{
  byte r = 0;
  byte g = 0;
  byte b = 0;

  switch(which) {
    case TOP:
      r = TOP_R;
      g = TOP_G;
      b = TOP_B;
      break;

    case LEFT:
      r = LEFT_R;
      g = LEFT_G;
      b = LEFT_B;
      break;

    case RIGHT:
      r = RIGHT_R;
      g = RIGHT_G;
      b = RIGHT_B;
      break;

    case FRONT:
      r = FRONT_R;
      g = FRONT_G;
      b = FRONT_B;
      break;

    case BACK:
      r = BACK_R;
      g = BACK_G;
      b = BACK_B;
      break;

    case BOTTOM:
      r = BOTTOM_R;
      g = BOTTOM_G;
      b = BOTTOM_B;
      break;
  }

  Display_SetColor(which, r, g, b);
}
