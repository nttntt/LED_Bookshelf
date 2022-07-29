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
  static uint8_t sReel[9][12] = {
      {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
      {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
      {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
      {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
      {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
      {0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0},
      {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0}};

  static uint8_t sSeq = 0;
  static uint8_t sHue = 30;

  if (reset)
  {
    sHue = 30;
    gBrightness = BRIGHTNESS;
    sSeq = 0;
    for (uint8_t y = 0; y < 8; ++y)
    {
      for (uint8_t x = 0; x < 12; ++x)
      {
        sReel[y][x] = 0;
      }
    }
    sReel[0][2] = 1;
    sReel[0][11] = 1;
    sReel[1][0] = 1;
    sReel[1][8] = 1;
    sReel[2][5] = 1;
    sReel[3][1] = 1;
    sReel[3][10] = 1;
    sReel[4][7] = 1;
    sReel[5][6] = 1;
    sReel[6][3] = 1;
    sReel[6][9] = 1;
    sReel[7][4] = 1;
  }

  if (sSeq < 255)
  {
    sSeq++;
  }

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
      Serial.print(sReel[y][x]);
    }
    Serial.println("");
  }

  // 配列シフト
  for (int8_t y = 8; y > 0; --y)
  {
    for (int8_t x = 0; x < 12; ++x)
    {
      sReel[y][x] = sReel[y - 1][x];
    }
  }

  for (int8_t x = 0; x < 12; ++x)
  {
    //ローテーション
    sReel[0][x] = sReel[8][x];
    // 暗点増加
    if ((20 < sSeq) && (sSeq < 29) && (sReel[1][x] == 1))
    {
      sReel[0][x] = 2;
    }
    if ((40 < sSeq) && (sSeq < 49) && (sReel[1][x] == 2))
    {
      sReel[0][x] = 3;
    }
    if ((60 < sSeq) && (sSeq < 69) && (sReel[1][x] == 3))
    {
      sReel[0][x] = 4;
    }
    if ((80 < sSeq) && (sSeq < 89) && (sReel[1][x] == 4))
    {
      sReel[0][x] = 5;
    }
    if ((100 < sSeq) && (sSeq < 109) && (sReel[1][x] == 5))
    {
      sReel[0][x] = 6;
    }
    if ((120 < sSeq) && (sSeq < 129) && (sReel[1][x] == 6))
    {
      sReel[0][x] = 7;
    }
    if ((140 < sSeq) && (sSeq < 149) && (sReel[1][x] == 7))
    {
      sReel[0][x] = 8;
    }
  }
}