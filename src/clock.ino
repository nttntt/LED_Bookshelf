uint8_t gFont[10][51] = {
    {2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2,
     2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2,
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},

    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},

    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2,
     2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},

    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2,
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2,
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},

    {2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2,
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},

    {2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2,
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},

    {2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
     2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2,
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},

    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},

    {2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2,
     2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2,
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},

    {2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2,
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1}};

void clock(uint8_t reset)
{
  struct tm timeInfo; //時刻を格納するオブジェクト
  static uint8_t prev_s = 0;
  getLocalTime(&timeInfo); // tmオブジェクトのtimeInfoに現在時刻を入れ込む

  uint8_t s = timeInfo.tm_sec;
  if (prev_s != s)
  {
    prev_s = s;
    uint16_t y = timeInfo.tm_year + 1900;
    uint8_t mo = timeInfo.tm_mon + 1;
    uint8_t d = timeInfo.tm_mday;
    uint8_t h = timeInfo.tm_hour;
    uint8_t m = timeInfo.tm_min;

    displayNumber(h / 10, 0);
    displayNumber(h % 10, 1);
    displayNumber(m / 10, 2);
    displayNumber(m % 10, 3);
    displayNumber(s / 10, 4);
    displayNumber(s % 10, 5);
    displayNumber((y / 10) % 10, 6);
    displayNumber(y % 10, 7);
    displayNumber(mo / 10, 8);
    displayNumber(mo % 10, 9);
    displayNumber(d / 10, 10);
    displayNumber(d % 10, 11);
  }
}

void displayNumber(uint8_t number, uint8_t position)
{

  for (int8_t x = 0; x < 17; ++x)
  {
    if (position < 6)
    {
      leds[(05 - position) * 17 + (16 - x)].setRGB(0, 0, gFont[number][0 * 17 + x] * gFont[number][0 * 17 + x] * 63);
      leds[(06 + position) * 17 + x].setRGB(0, 0, gFont[number][1 * 17 + x] * gFont[number][1 * 17 + x] * 63);
      leds[(17 - position) * 17 + (16 - x)].setRGB(0, 0, gFont[number][2 * 17 + x] * gFont[number][2 * 17 + x] * 63);
    }
    else if (position < 12)
    {
      leds[(12 + position) * 17 + x].setRGB(0, gFont[number][0 * 17 + x] * gFont[number][0 * 17 + x] * 63, 0);
      leds[(35 - position) * 17 + (16 - x)].setRGB(0, gFont[number][1 * 17 + x] * gFont[number][1 * 17 + x] * 63, 0);
      leds[(24 + position) * 17 + x].setRGB(0, gFont[number][2 * 17 + x] * gFont[number][2 * 17 + x] * 63, 0);
    }
  }
}