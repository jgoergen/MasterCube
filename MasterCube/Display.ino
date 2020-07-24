#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NEOPIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

int lastDisplay = 0;

void Display_Init() {
  strip.begin();
  for (int i = 0; i < NEOPIXEL_COUNT; i ++) {
    strip.setPixelColor(i, strip.Color(0, 255, 0));
    strip.setPixelColor(i, strip.Color(0, 255, 0));
  }
  strip.show();
  Serial.println(F("Neopixels Started"));
}

void Display_update() {
  if (millis() - lastDisplay > DISPLAY_RATE) {
    lastDisplay = millis();
    strip.show();
  }
}

void Display_Force_Update() {
  strip.show();
}


void Display_Flash(uint8_t r, uint8_t g, uint8_t b, byte count, int pause) {
  for (byte i = 0; i < count; i++)
  {
    Display_SetAll(r, g, b);
    Display_Force_Update();
    delay(pause);
    Display_SetAll(0, 0, 0);
    Display_Force_Update();
    delay(pause);
  }
}

void Display_Clear() {
  Display_SetAll(0, 0, 0);
}

void Display_SetAll(uint8_t r, uint8_t g, uint8_t b) {
  for (int i = 0; i < NEOPIXEL_COUNT; i++) {
    strip.setPixelColor(i, strip.Color(r, g, b));
  }
}

void Display_Set_Direction(byte which) {
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

  Display_Set(which, r, g, b);
}

void Display_Set(byte index, uint8_t r, uint8_t g, uint8_t b) {  
  strip.setPixelColor(index, strip.Color(r, g, b));
}
