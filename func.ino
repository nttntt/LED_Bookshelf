// gLEDBufferに色情報を入れて0~71のブロックごとに描画
void drawByBlock(int8_t x, int8_t y)
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
  leds[(NUM_BLOCK * 6) - i] = gLEDBuffer;
  leds[(NUM_BLOCK * 6) + i] = gLEDBuffer;
  leds[(NUM_BLOCK * 18) - i] = gLEDBuffer;
  leds[(NUM_BLOCK * 18) + i] = gLEDBuffer;
  leds[(NUM_BLOCK * 30) - i] = gLEDBuffer;
  leds[(NUM_BLOCK * 30) + i] = gLEDBuffer;

  leds[(NUM_BLOCK * 36) + (i * 2 / 3)] = gLEDBuffer;
  leds[(NUM_BLOCK * 46) - i] = gLEDBuffer;
  leds[(NUM_BLOCK * 46) + (i / 3)] = gLEDBuffer;
  leds[(NUM_BLOCK * 50) - (i / 3)] = gLEDBuffer;
  leds[(NUM_BLOCK * 50) + i] = gLEDBuffer;
  leds[(NUM_BLOCK * 60) - (i * 2 / 3)] = gLEDBuffer;
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
