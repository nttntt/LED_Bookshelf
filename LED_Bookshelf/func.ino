// gLEDBufferに色情報を入れて0~71のブロックごとに描画
void drawByBlock(uint8_t x, uint8_t y)
{
  static uint8_t block[8][12] = {
      {47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36},
      {48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59},
      {99, 99, 5, 4, 3, 2, 1, 0, 99, 99, 61, 60},
      {99, 99, 6, 7, 8, 9, 10, 11, 99, 99, 62, 63},
      {99, 99, 17, 16, 15, 14, 13, 12, 99, 99, 65, 64},
      {99, 99, 18, 19, 20, 21, 22, 23, 99, 99, 66, 67},
      {99, 99, 29, 28, 27, 26, 25, 24, 99, 99, 69, 68},
      {99, 99, 30, 31, 32, 33, 34, 35, 99, 99, 70, 71}};
  if ((0 <= x) && (x < 12) && (0 <= y) && (y < 8) && (block[y][x] < 99))
  {
    for (uint8_t i = 0; i < 17; ++i)
    {
      leds[block[y][x] * 17 + i] = gLEDBuffer;
    }
  }
}
void drawByHalfBlock(uint8_t x, uint8_t y)
{
  static uint8_t block[8][24] = {
      {95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72},
      {96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119},
      {255, 255, 255, 255, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 255, 255, 255, 255, 123, 122, 121, 120},
      {255, 255, 255, 255, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 255, 255, 255, 255, 124, 125, 126, 127},
      {255, 255, 255, 255, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 255, 255, 255, 255, 131, 130, 129, 128},
      {255, 255, 255, 255, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 255, 255, 255, 255, 132, 133, 134, 135},
      {255, 255, 255, 255, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 255, 255, 255, 255, 139, 138, 137, 136},
      {255, 255, 255, 255, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 255, 255, 255, 255, 140, 141, 142, 143}};
  if ((0 <= x) && (x < 25) && (0 <= y) && (y < 8) && (block[y][x] < 255))
  {
    for (uint8_t i = 0; i < 8; ++i)
    {
      leds[block[y][x] * 8 + i + block[y][x] / 2 + block[y][x] % 2] = gLEDBuffer;
    }
  }
}

void drawDot(int16_t x, int16_t y)
{
  if (x < 0 || 201 < x || y < 0 || 7 < y)
  {
    return;
  }
  else if (y == 0)
  {
    leds[612 + 203 - x] = gLEDBuffer;
  }
  else if (y == 1)
  {
    leds[612 + 204 + x] = gLEDBuffer;
  }
  else if ((y == 2 || y == 4 || y == 6) && 33 < x && x < 136)
  {
    leds[(y - 1) * 102 - x + 33] = gLEDBuffer;
  }
  else if ((y == 3 || y == 5 || y == 7) && 33 < x && x < 136)
  {
    leds[(y - 2) * 102 + x - 34] = gLEDBuffer;
  }
  else if ((y == 2 || y == 4 || y == 6) && 169 < x)
  {
    leds[(y - 1) * 34 - x + 170 + 1019] = gLEDBuffer;
  }
  else if ((y == 3 || y == 5 || y == 7) && 169 < x)
  {
    leds[(y - 2) * 34 + x - 170 + 1020] = gLEDBuffer;
  }
}

// gLEDBufferに色情報を入れて0~101個を各段にコピー
void drawToEachRow(uint8_t i)
{
  leds[i] = gLEDBuffer;
  leds[i + NUM_BLOCK * 6] = gLEDBuffer;
  leds[i + NUM_BLOCK * 12] = gLEDBuffer;
  leds[i + NUM_BLOCK * 18] = gLEDBuffer;
  leds[i + NUM_BLOCK * 24] = gLEDBuffer;
  leds[i + NUM_BLOCK * 30] = gLEDBuffer;

  leds[i + NUM_BLOCK * 36] = gLEDBuffer;
  leds[i + NUM_BLOCK * 42] = gLEDBuffer;
  leds[i + NUM_BLOCK * 48] = gLEDBuffer;
  leds[i + NUM_BLOCK * 54] = gLEDBuffer;
  leds[i + NUM_BLOCK * 60] = gLEDBuffer;
  leds[i + NUM_BLOCK * 66] = gLEDBuffer;
}

// gLEDBufferに色情報を入れて中央をメインにコピー
void drawToSameDirection(uint8_t i)
{
  leds[(NUM_BLOCK * 6) - i - 1] = gLEDBuffer;
  leds[(NUM_BLOCK * 6) + i] = gLEDBuffer;
  leds[(NUM_BLOCK * 18) - i - 1] = gLEDBuffer;
  leds[(NUM_BLOCK * 18) + i] = gLEDBuffer;
  leds[(NUM_BLOCK * 30) - i - 1] = gLEDBuffer;
  leds[(NUM_BLOCK * 30) + i] = gLEDBuffer;

  leds[(NUM_BLOCK * 36) + (i * 2 / 3)] = gLEDBuffer;
  leds[(NUM_BLOCK * 46) - i - 1] = gLEDBuffer;
  leds[(NUM_BLOCK * 46) + (i / 3)] = gLEDBuffer;
  leds[(NUM_BLOCK * 50) - (i / 3) - 1] = gLEDBuffer;
  leds[(NUM_BLOCK * 50) + i] = gLEDBuffer;
  leds[(NUM_BLOCK * 60) - (i * 2 / 3) - 1] = gLEDBuffer;
  leds[i + NUM_BLOCK * 60] = gLEDBuffer;
  leds[i + NUM_BLOCK * 66] = gLEDBuffer;
}

// rgb2hsv_approximateにバグがあるので代替
CHSV rgb2hsv_rainbow(CRGB input_color)
{
  CHSV output_color;
  uint8_t maximum = max(input_color.r, max(input_color.g, input_color.b));
  uint8_t minimum = min(input_color.r, min(input_color.g, input_color.b));
  output_color.v = maximum;
  uint8_t delta = maximum - minimum;
  output_color.s = (maximum == 0) ? 0 : (255 * delta / maximum);
  if (maximum == minimum)
  {
    output_color.h = 0;
  }
  else
  {
    if (maximum == input_color.r)
    {
      if (input_color.g >= input_color.b)
      {
        if ((input_color.r - input_color.g) <= (delta / 3))
        {
          output_color.h =
              map((input_color.r - input_color.g), delta / 3, 0, 32, 64);
        }
        else
        {
          output_color.h =
              map((input_color.r - input_color.g), delta, delta / 3, 0, 32);
        }
      }
      else
      {
        output_color.h =
            map((input_color.r - input_color.b), 0, delta, 208, 255);
      }
    }
    else if (maximum == input_color.g)
    {
      if (input_color.r >= input_color.b)
      {
        output_color.h = map((input_color.g - input_color.r), 0, delta, 64, 96);
      }
      else
      {
        if (input_color.g - input_color.b <= delta / 3)
        {
          output_color.h =
              map((input_color.g - input_color.b), delta, delta / 3, 96, 128);
        }
        else
        {
          output_color.h =
              map((input_color.g - input_color.b), delta / 3, delta, 128, 160);
        }
      }
    }
    else if (maximum == input_color.b)
    {
      if (input_color.g > input_color.r)
      {
        output_color.h =
            map((input_color.b - input_color.g), -delta / 3, delta, 128, 160);
      }
      else
      {
        output_color.h =
            map((input_color.b - input_color.r), delta, 0, 160, 208);
      }
    }
  }
  return output_color;
}
