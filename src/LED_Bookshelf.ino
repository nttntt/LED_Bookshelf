#define FASTLED_ESP32_I2S true
#include <ArduinoOTA.h>
#include <BLEDevice.h>
#include <ESPmDNS.h>
#include <FastLED.h>
#include <WebServer.h>
#include <WiFi.h>

/* Function Prototype */
void doInitialize(void);
void displayData(void);
void bleTask(void *pvParameters);
void getPeakLevel(void);
void chgBrightness(void);
void chgSensitivity(void);
int16_t getDeltaY(int8_t);
void nextPattern(void);
void flash(uint8_t);
void bpm(uint8_t);
void appearance(uint8_t);
void reel(uint8_t);
void colorpicker(uint8_t);
void solid(uint8_t);
void confetti(uint8_t);
void blur(uint8_t);
void flickerBySound(uint8_t);
void levelmeter(uint8_t);
void moveByJoystick(uint8_t);
void pacifica_loop(uint8_t);
void fire(uint8_t);
void gradation(uint8_t);
void drawByBlock(uint8_t, uint8_t);
void drawByHalfBlock(uint8_t, uint8_t);
void drawDot(int16_t, int16_t);
void drawToEachRow(uint8_t);
void drawToSameDirection(uint8_t);
CHSV rgb2hsv_rainbow(CRGB);
void clock(uint8_t);
void clock0(uint8_t);
void clock1(uint8_t);
void clock2(uint8_t);
void lockon(uint8_t);

/* 基本属性定義  */
#define SPI_SPEED 115200 // SPI通信速度

// ルーター接続情報
#define WIFI_SSID "0856g"
#define WIFI_PASSWORD "nttnttntt"
// Multicast DNS名
#define LEDS_NAME "LED_Bookshelf"
// 時間取得
#define JST 9 * 3600L
#define NTPServer1 "192.168.1.10"
#define NTPServer2 "time.google.com"

// Webサーバーオブジェクト
#define HTTP_PORT 80
WebServer server(HTTP_PORT);

/* LED関連 */

#define DATA_PIN 25
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS 1224
#define NUM_BLOCK 17 // 1ブロックのLED数
#define BRIGHTNESS 255
#define FRAMES_PER_SECOND 100

CRGB leds[NUM_LEDS];
CRGB virtualLeds[NUM_LEDS];
CRGB gLEDBuffer = CHSV(26, 241, 190);

uint8_t gBrightness = BRIGHTNESS;
uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0;                  // rotating "base color" used by many of the patterns

typedef void (*SimplePatternList[])(uint8_t); //, clock1, clock2
SimplePatternList gPatterns = {gradation, solid, pacifica_loop, flash, bpm, confetti, blur, fire, clock, clock0, flickerBySound, appearance, lockon, reel, colorpicker};
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

/* BLE関連 */
#define DEVICE_NAME "LED_Bookshelf" // 対象デバイス名
#define ManufacturerId 0xffff       // 既定のManufacturer ID
const uint8_t scanning_time = 3;    // スキャン時間（秒）
BLEAdvertisedDevice *targetDevice;  // 目的のBLEデバイス
std::string gData = "";
uint8_t gPrevSeq = 255; // 前回のシーケンス番号
int16_t gX = 0;
int16_t gY = 0;
uint8_t gB = 0;

/* マイク関連 */
#define MIC_PIN 32
#define NOISE_LEVEL 10
uint16_t gSoundLevel = 0;  // 計測値（0~4095）
uint16_t gSensitivity = 8; // 感度

/*****************************< Define Callback >*****************************/
// アドバタイジング受信時コールバック
class advertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
  void onResult(BLEAdvertisedDevice advertisedDevice)
  {
    if (advertisedDevice.getName() == DEVICE_NAME)
    {
      BLEDevice::getScan()->stop();
      gData = advertisedDevice.getManufacturerData();
      // Serial.println(advertisedDevice.toString().c_str());
    }
  }
};

/*****************************************************************************
                            Predetermined Sequence
 *****************************************************************************/
void setup()
{
  // 初期化処理をして
  doInitialize();
  // BLE受信のタスク起動
  xTaskCreatePinnedToCore(bleTask, "bleTask", 4096, NULL, 10, NULL, 0);
  xTaskCreatePinnedToCore(htmlTask, "htmlTask", 8192, NULL, 10, NULL, 0);
}

void loop()
{
  boolean autoRotate = false;
  boolean restorePreset = 0;

  if (0 < gB && gB < ARRAY_SIZE(gPatterns) + 1)
  {
    gCurrentPatternNumber = gB - 1;
    gB = 0;
  }
  if (gB == 16)
  {
    restorePreset = 1;
    gB = 0;
  }

  if (gB == 17)
  {
    autoRotate = true;
  }
  else if (gB)
  {
    autoRotate = false;
  }

  if (autoRotate)
  {
    EVERY_N_SECONDS(10)
    {
      nextPattern(); // change patterns periodically
    }
  }

  gPatterns[gCurrentPatternNumber](
      restorePreset); // Call the current pattern function once, updating the
                      // 'leds' array
  restorePreset = 0;

  FastLED.show();

  FastLED.delay(
      1000 / FRAMES_PER_SECOND); // insert a delay to keep the framerate modest
  EVERY_N_MILLISECONDS(20)
  {
    gHue++; // slowly cycle the "base color" through the rainbow
  }
  //  displayData();
}

/*****************************< Other functions >*****************************/
/*  初期化処理  */
void doInitialize()
{
  Serial.begin(SPI_SPEED);

  //  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds,  0, NUM_LEDS);
  FastLED.addLeds<LED_TYPE, DATA_PIN + 0, COLOR_ORDER>(leds, 0, 612);
  FastLED.addLeds<LED_TYPE, DATA_PIN + 1, COLOR_ORDER>(leds, 612, NUM_LEDS - 612);
  FastLED.setBrightness(gBrightness);

  Serial.println("LED Controller start ...");
}

/* 次のパターンに変更 */
void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber =
      (gCurrentPatternNumber + 1) % (ARRAY_SIZE(gPatterns) - 3);
}

/* 受信データを表示する */
void displayData()
{
  Serial.print("No.");
  Serial.print(gPrevSeq);
  Serial.print("  X:");
  Serial.print(gX);
  Serial.print(",  Y:");
  Serial.print(gY);
  Serial.print(",  B:");
  Serial.print(gB);
  Serial.print(",  S:");
  Serial.print(gSensitivity);
  Serial.print(",  M:");
  Serial.println(gCurrentPatternNumber);
}

/*****************************< LED functions >*****************************/

void flash(uint8_t reset)
{
  // チカチカ
  static uint8_t sChanceOfFlash = 32;
  if (reset)
  {
    sChanceOfFlash = 32;
    gBrightness = BRIGHTNESS;
  }
  sChanceOfFlash += getDeltaY(10);
  chgBrightness();
  fadeToBlackBy(leds, NUM_LEDS, 10);
  for (uint16_t i = 0; i < 4; ++i)
  {
    if (random8() < sChanceOfFlash)
    {
      leds[random16(NUM_LEDS)] += CRGB::White;
    }
  }
}
void bpm(uint8_t reset)
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  static uint8_t sBeatsPerMinute = 62;
  if (reset)
  {
    sBeatsPerMinute = 62;
    gBrightness = BRIGHTNESS;
  }
  sBeatsPerMinute -= getDeltaY(5);
  chgBrightness();
  CRGBPalette16 currentPalette = PartyColors_p;
  uint8_t beat = beatsin8(sBeatsPerMinute, 64, 255);
  for (uint16_t i = 0; i < NUM_LEDS; ++i)
  {
    leds[i] = ColorFromPalette(currentPalette, gHue + (i * 2),
                               beat - gHue + (i * 10));
  }
}

void colorpicker(uint8_t reset)
{

  static uint8_t sHue = 26;
  if (reset)
  {
    sHue = 26;
    gBrightness = BRIGHTNESS;
  }
  sHue += getDeltaY(10);
  chgBrightness();
  fill_solid(leds, NUM_LEDS, gLEDBuffer);
}

void solid(uint8_t reset)
{
  // 普通の光
  static uint8_t sHue = 44;
  if (reset)
  {
    sHue = 44;
    gBrightness = BRIGHTNESS;
  }
  sHue += getDeltaY(10);
  chgBrightness();
  fill_solid(leds, NUM_LEDS, CHSV(sHue, 255, 255));
}
void confetti(uint8_t reset)
{
  static uint8_t sHue = 24;
  if (reset)
  {
    sHue = 24;
    gBrightness = BRIGHTNESS;
  }
  sHue += getDeltaY(10);
  chgBrightness();
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy(leds, NUM_LEDS, 5);
  uint8_t pos = random16(NUM_LEDS / 10);
  uint8_t hue = random8(64);
  for (uint16_t i = 0; i < 10; ++i)
  {
    leds[pos * 10 + i] += CHSV(sHue + hue, 200, 255);
  }
}

void gradation(uint8_t reset)
{
  static uint8_t sPattern = 2;
  static uint8_t sDeltaHue = 7;
  if (reset)
  {
    sPattern = 4;
    sDeltaHue = 7;
  }
  if (gX || gY || gB)
  {
    sPattern = (sPattern + getDeltaY(1) + 8) % 8; // 0~7(Rotateあり）
    chgBrightness();
  }
  CRGBPalette16 currentPalette;
  switch (sPattern)
  {
  case 0:
    currentPalette = CloudColors_p;
    break; // blue and white
  case 1:
    currentPalette = LavaColors_p;
    break; // orange, red, black and yellow),
  case 2:
    currentPalette = OceanColors_p;
    break; // blue, cyan and white
  case 3:
    currentPalette = ForestColors_p;
    break; // greens and blues
  case 4:
    currentPalette = RainbowColors_p;
    break; // standard rainbow animation
  case 5:
    currentPalette = RainbowStripeColors_p;
    break; // single colour, black space, next colour and so forth
  case 6:
    currentPalette = PartyColors_p;
    break; // red, yellow, orange, purple and blue
  case 7:
    currentPalette = HeatColors_p;
    break; // red, orange, yellow and white
  }
  for (uint16_t i = 0; i < NUM_LEDS; ++i)
  {
    leds[i] = ColorFromPalette(currentPalette, gHue + i * sDeltaHue, 255);
  }
}

void fire(uint8_t reset)
{
  static uint8_t sHue = 0;
  if (reset)
  {
    sHue = 0;
    gBrightness = BRIGHTNESS;
  }
  sHue = sHue + getDeltaY(10);
  chgBrightness();

  CRGBPalette16 currentPalette = HeatColors_p;
  int16_t a = millis();
  // 128の仮想LEDで絵作りして部屋の各壁に投影
  for (uint16_t i = 0; i < 102; ++i)
  {
    uint8_t noise = inoise8(0, i * 60 - a, a / 3);
    uint8_t index = qsub8(noise, i * 2);
    // Set the LED color from the palette
    gLEDBuffer = ColorFromPalette(currentPalette, index, 255);
    //  白の時は変換しない
    if ((gLEDBuffer.r != gLEDBuffer.g) || (gLEDBuffer.r != gLEDBuffer.b))
    {
      CHSV tempColor = rgb2hsv_rainbow(gLEDBuffer);
      tempColor.hue = (tempColor.hue + sHue) % 256;
      gLEDBuffer = tempColor;
    }
    drawToSameDirection(i);
  }
}

void blur(uint8_t reset)
{
  if (reset)
  {
    //    sHue = 0;
    gBrightness = BRIGHTNESS;
  }
  chgBrightness();

  uint8_t sinBeat = beatsin8(30, 0, NUM_BLOCK * 6 - 1, 0, 0);
  uint8_t sinBeat2 = beatsin8(30, 0, NUM_BLOCK * 6 - 1, 0, 85);
  uint8_t sinBeat3 = beatsin8(30, 0, NUM_BLOCK * 6 - 1, 0, 170);
  gLEDBuffer = CRGB::Green;
  drawToSameDirection(sinBeat);
  gLEDBuffer = CRGB::Blue;
  drawToSameDirection(sinBeat2);
  gLEDBuffer = CRGB::Red;
  drawToSameDirection(sinBeat3);

  EVERY_N_MILLISECONDS(1) { blur1d(leds, NUM_LEDS, 50); }
}

void flickerBySound(uint8_t reset)
{
  //音で揺らぐ光
  static uint8_t sHue = 24;
  if (reset)
  {
    sHue = 24;
    gSensitivity = 8;
  }
  sHue += getDeltaY(10);
  chgSensitivity();

  uint16_t t;
  int16_t noise;
  uint16_t brightness;

  getPeakLevel();
  //  brightness = constrain(gSoundLevel * 8 / gSensitivity, 0, 63);  //
  //  マイク入力と感度から0~63くらいになる数を算出
  // Serial.println(brightness);
  t = millis() / 5;
  for (uint16_t i = 0; i < NUM_LEDS; ++i)
  {
    noise = inoise8(i * 10, t) / 4 - 32; // -32~+31のばらつきを持たせて
    leds[i].setHSV(sHue + noise, 200, brightness + noise + 160);
  }
}
void levelmeter(uint8_t reset)
{
  // レベルメーター
  static uint8_t sHue = 96;
  static uint8_t sMax = 0;
  static uint32_t previousTime = 0;

  if (reset)
  {
    sHue = 96;
    gSensitivity = 8;
  }
  sHue += getDeltaY(10);
  chgSensitivity();

  // マイク入力と感度から0~127になる数を算出
  getPeakLevel();
  uint16_t numTurnOn = constrain(gSoundLevel * 8 / gSensitivity, 0, 127);
  // ピークマーカー
  if (sMax < numTurnOn)
  {
    sMax = numTurnOn;
  }
  else if (sMax > 0)
  {
    sMax--;
  }

  // 描画
  // 100msに1回更新
  if ((millis() - previousTime) < 100)
  {
    return;
  }
  previousTime = millis();

  uint8_t hue = sHue;

  for (uint16_t i = 0; i < 102; ++i)
  {
    if (i == 72)
      hue -= 96; // よく見るレベルメーターのように60%以上から色を変える
    if (i < numTurnOn)
    { // 点灯
      gLEDBuffer.setHSV(hue, 255, 255);
    }
    else
    {
      gLEDBuffer.setHSV(hue, 0, 0);
    }
    if ((i % 8) > 4)
    { // 8個につき3個消灯して黒線を入れる
      gLEDBuffer.setHSV(hue, 0, 0);
    }
    if (sMax < i && i < (sMax + 5))
    { // ピークマーカー
      gLEDBuffer.setHSV(hue, 0, 255);
    }
    drawToSameDirection(i);
  }
}

void moveByJoystick(uint8_t reset)
{
  // Joystickで色と光を動かす
  static int16_t lastLED = 0;
  int8_t color = 10;
  fadeToBlackBy(leds, NUM_LEDS, 50);
  // ある程度ジョイスティックを動かしたら
  if (gX < -1000 || 1000 < gX || gY < -1000 || 1000 < gY)
  {
    // 部屋のLEDの配置に合わせてジョイスティックの入力値から移動先LEDを算出
    int16_t destinationLED = (atan2(-gY, -gX) / 6.2832 + 0.5) * NUM_LEDS;
    // 相対距離に変換して（最短方向になるように考慮）
    int16_t relativeLED =
        ((int16_t)(destinationLED - lastLED + NUM_LEDS * 1.5) % NUM_LEDS) -
        NUM_LEDS / 2;
    // 相対距離の1/10を1回の移動量に（小数点以下は切り上げ・負の場合小数点以下は切り下げと同じになるよう）
    relativeLED =
        ((0 < relativeLED) - (relativeLED < 0)) * ceil(abs(relativeLED) / 10);
    // 移動中は色を変える
    color = 255;
    // オーバーフローやマイナスにならないように考慮して移動量を加算
    lastLED = (lastLED + relativeLED + NUM_LEDS) % NUM_LEDS;
  }

  int16_t workLED = lastLED;
  for (int i = 0; i < (NUM_LEDS / 16); ++i)
  {
    leds[workLED].setHSV(gHue, color, 255);
    workLED++;
    if (workLED == NUM_LEDS)
    {
      workLED = 0;
    }
  }
}

/****************************< MIC functions >****************************
  #define SAMPLING_RATE 50    // ミリセカンド
  void getPeakLevel() {
  static uint32_t sStartMillis = millis();
  static uint16_t sSignalMax = 1700;
  static uint16_t sSignalMin = 1900;
  static uint16_t micin = 1800;

  if (millis() - sStartMillis < SAMPLING_RATE) {
    micin = analogRead(MIC_PIN);
    if ((0 <= micin) && ( micin <= 4095)) {
      if (micin > sSignalMax) {
        sSignalMax = micin;
      } else if (micin < sSignalMin) {
        sSignalMin = micin;
      }
    }
  } else {
    gSoundLevel = abs(sSignalMax - sSignalMin);
    gSoundLevel = constrain(gSoundLevel - NOISE_LEVEL, 0, 4095);
    sSignalMax = 1700;
    sSignalMin = 1900;
    sStartMillis = millis();
  }
  }*/
void getPeakLevel()
{
  static int32_t sAverageLevel = 1800;
  uint16_t micin = analogRead(MIC_PIN);

  sAverageLevel = (sAverageLevel * 19 + micin) / 20;
  //  gSoundLevel = (gSoundLevel + abs(micin - sAverageLevel)) / 2;
  gSoundLevel = abs(micin - sAverageLevel);
  gSoundLevel = constrain(gSoundLevel - NOISE_LEVEL, 0, 4095);
  //  Serial.print("sAverageLevel:");
  //  Serial.print(sAverageLevel - 1500);
  //  Serial.print(" gSoundLevel:");
  //  Serial.print(gSoundLevel);
  //  Serial.print(" micin:");
  //  Serial.println(micin - 1500);
}

/****************************< Joystick functions >****************************/
// JoystickのX軸で明るさ調整
void chgBrightness()
{
  if (gX)
  {
    // -2000~2000を-32~32に変換
    int8_t delta = gX * 32 / 2000;
    gBrightness = constrain(gBrightness + delta, 10, 255);
    FastLED.setBrightness(dim8_lin(gBrightness));
    gX = 0;
  }
}

void chgSensitivity()
{
  if (gX < -1000 && gSensitivity > 1)
  {
    gSensitivity = gSensitivity - 1;
  }
  else if (gX > 1000 && gSensitivity < 12)
  {
    gSensitivity = gSensitivity + 1;
  }
  gX = 0;
}

int16_t getDeltaY(int8_t range)
{
  int16_t result = 0;
  if (gY < -100 || 100 < gY)
  {
    result = gY * range / 2000;
  }
  gY = 0;
  return result;
}

/*****************************< BLE Task functions
 * >*****************************/
/* マルチタスクでBLE待ち受け */
void bleTask(void *pvParameters)
{
  BLEDevice::init("");                      // BLEデバイスを作成する
  BLEScan *pBLEScan = BLEDevice::getScan(); // Scanオブジェクトを取得して、
  pBLEScan->setAdvertisedDeviceCallbacks(
      new advertisedDeviceCallbacks()); //コールバックを設定
  pBLEScan->setActiveScan(false);       // パッシブスキャンに設定する
  while (true)
  {
    pBLEScan->start(0); // スキャン

    //  uint8_t manuCode  = gData[1] << 8 | gData[0];
    uint8_t seqNumber = gData[2];

    if (seqNumber !=
        gPrevSeq)
    { // シーケンス番号が更新されていたら受信データを取り出す
      gPrevSeq = seqNumber;
      gX = gData[4] << 8 | gData[3];
      gY = gData[6] << 8 | gData[5];
      gB = gData[7];
    }
    delay(1);
    // displayData();
  }
}
