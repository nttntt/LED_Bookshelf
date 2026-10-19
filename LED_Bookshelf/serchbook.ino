void appearance(uint8_t reset)
{
  // 指定の本を照らす
  static uint8_t sHue = 30;
  static uint8_t sValue = 0;
  static uint8_t sSeq = 0;
  static int8_t sX = 4;
  static int8_t sY = 3;

  if (reset)
  {
    sHue = 30;
    gBrightness = BRIGHTNESS;
    sSeq = 0;
  }
  sHue += getDeltaY(16);
  chgBrightness();
  if (sSeq < 255)
  {
    sSeq++;
  }
  if (sSeq < 20)
  {
    sValue = sSeq * 10;
    fill_solid(leds, NUM_LEDS, CHSV(sHue, 241, sValue));
  }
  else if (sSeq < 35)
  {
    fadeToBlackBy(leds, NUM_LEDS, 10);
    gLEDBuffer = CHSV(sHue, 241, 190);
    for (int8_t y = sY - 2; y <= sY + 2; ++y)
    {
      for (int8_t x = sX - 2; x <= sX + 2; ++x)
      {
        if ((0 <= x) && (x < 12) && (0 <= y) && (y < 8))
        {
          drawByBlock(x, y);
        }
      }
    }
  }
  else if (sSeq < 50)
  {
    fadeToBlackBy(leds, NUM_LEDS, 10);
    gLEDBuffer = CHSV(sHue, 241, 190);
    for (int8_t y = sY - 1; y <= sY + 1; ++y)
    {
      for (int8_t x = sX - 1; x <= sX + 1; ++x)
      {
        if ((0 <= x) && (x < 12) && (0 <= y) && (y < 8))
        {
          drawByBlock(x, y);
        }
      }
    }
  }
  else if (sSeq < 65)
  {
    fadeToBlackBy(leds, NUM_LEDS, 10);
    gLEDBuffer = CHSV(sHue, 241, 190);
    drawByBlock(sX, sY);
  }
  else if (sSeq < 100)
  {
    fadeToBlackBy(leds, NUM_LEDS, 10);
    leds[145] = CHSV(sHue, 241, 190);
  }
}

void reel(uint8_t reset)
{
  static uint8_t sReel[8][13] = {
      {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
      {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}};

  static uint8_t sSeq = 0;
  static uint8_t sHue = 30;

  if (reset)
  {
    sHue = 30;
    gBrightness = BRIGHTNESS;
    sSeq = 0;
    for (uint8_t y = 0; y < 8; ++y)
    {
      for (uint8_t x = 0; x < 13; ++x)
      {
        sReel[y][x] = 0;
      }
    }
    sReel[0][2] = 1;
    sReel[1][8] = 1;
    sReel[2][5] = 1;
    sReel[3][1] = 1;
    sReel[3][1] = 1;
    sReel[4][7] = 1;
    sReel[5][6] = 1;
    sReel[6][3] = 1;
    sReel[7][11] = 1;
  }

  if (sSeq < 210)
  {
    sSeq++;
  }
  if (sSeq < 180)
  {
  // ブロックごとに描画
  for (int8_t y = 0; y < 8; ++y)
  {
    for (int8_t x = 0; x < 12; ++x)
    {
      if (sReel[y][x] == 0)
      {
        gLEDBuffer = CHSV(sHue, 241, 190);
      }
      else
      {
        gLEDBuffer = CHSV(0, 0, 0);
      }
      drawByBlock(x, y);
    }
  }
    // 配列シフト
    for (int8_t x = 12; x > 0; --x)
    {
      for (int8_t y = 0; y < 8; ++y)
      {
        sReel[y][x] = sReel[y][x - 1];
      }
    }

    for (int8_t y = 0; y < 8; ++y)
    {
      //ローテーション
      sReel[y][0] = sReel[y][12];
      // 暗点増加
      for (int8_t i = 1; i < 11; ++i)
      {
        if (((i * 12) <= sSeq) && (sSeq < ((i + 1) * 12)) && (sReel[y][1] == i))
        {
          sReel[y][0] = i + 1;
        }
      }
    }

    if (100 <= sSeq)
    {
      sReel[0][0] = 12;
    }
    if (106 <= sSeq)
    {
      sReel[7][0] = 12;
    }
    if (106 <= sSeq)
    {
      sReel[2][0] = 12;
    }
    if (112 <= sSeq)
    {
      sReel[6][0] = 12;
    }
    if (118 <= sSeq)
    {
      sReel[4][0] = 12;
    }
    if (124 <= sSeq)
    {
      sReel[1][0] = 12;
    }
    if (136 <= sSeq)
    {
      sReel[3][0] = 12;
    }
  }
  else if (sSeq < 210)
  {
    fadeToBlackBy(leds, NUM_LEDS, 10);
    leds[365] = CHSV(sHue, 241, 190);
  }
}

void lockon(uint8_t reset)
{
  // 指定の本を照らす
  static uint8_t sHue = 30;
  static uint8_t sValue = 0;
  static float sSeq = 0;
  static int16_t sX = 6;
  static int16_t sY = 4;

  if (reset)
  {
    sHue = 30;
    gBrightness = BRIGHTNESS;
    sSeq = 0;
  }
  sHue += getDeltaY(16);
  chgBrightness();

  gLEDBuffer = CRGB(0, 255, 0);
  if (sSeq < 255)
  {
    sSeq++;
  }
  if (sSeq < 105)
  {
    sX = cos(sSeq / 10) * 100 + 100;
    sY = (int)((sSeq / 7)) % 8;
    fadeToBlackBy(leds, NUM_LEDS, 200);
    for (int16_t i = 0; i < 8; ++i)
    {
      gLEDBuffer = CRGB(0, 255, 0);
      drawDot(sX, i);
      // drawDot(sX + 1, i);
    }
    for (int16_t i = 0; i < 204; ++i)
    {
      gLEDBuffer = CRGB(0, 0, 128);
      drawDot(i, sY);
    }
  }
  else if (sSeq < 150)
  {
    sX = cos(10.4) * 100 + 100;
    fadeToBlackBy(leds, NUM_LEDS, 10);
    for (int16_t i = 0; i < 8; ++i)
    {
      drawDot(sX, i);
    }
    /*for (int16_t i = 0; i < 204; ++i)
    {
      gLEDBuffer = CRGB(0, 0, 255);
      drawDot(i, 5);
    }*/
  }
}