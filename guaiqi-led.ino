#include <FastLED.h>

#define NUM_LEDS 26
#define DATA_PIN 12
#define BRIGHTNESS 200

CRGB leds[NUM_LEDS];

// ------------------------------------------------------------------
// 你的走线方式：LED 实际顺序为 Z→A（完全反序）
// A(0) → LED25
// B(1) → LED24
// ...
// Z(25) → LED0
// ------------------------------------------------------------------
int ledMap[26] = {
  25,24,23,22,21,20,19,18,17,16,15,14,13,
  12,11,10,9,8,7,6,5,4,3,2,1,0
};

// ----------------------------
// 字符解析：A～Z → 0～25
// ----------------------------
int letterToIndex(char c) {
  if (c == ' ' || c == '\t' || c == '\n') return -1;

  if (c >= 'A' && c <= 'Z') return c - 'A';
  if (c >= 'a' && c <= 'z') return c - 'a';

  return -2; // 非字母
}

// ----------------------------
// 剧中灯效：大红色渐亮 → 白爆闪 → 熄灭
// ----------------------------
void showLetterEffect(int letterIndex) {
  int realIndex = ledMap[letterIndex];

  // 大红色渐亮（非常慢）
  for (int v = 0; v < 255; v += 5) {    // 更小的步长
    leds[realIndex] = CRGB(v, 0, 0);
    FastLED.show();
    delay(40);                          // 更大的延迟
  }

  // 白爆闪（加长）
  leds[realIndex] = CRGB::White;
  FastLED.show();
  delay(150);

  // 熄灭（稍慢）
  leds[realIndex] = CRGB::Black;
  FastLED.show();
  delay(120);
}

void flashAllTwice() {
  // 闪两次
  for (int t = 0; t < 2; t++) {

    // 全亮白
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::White;
    }
    FastLED.show();
    delay(180);

    // 全灭
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Black;
    }
    FastLED.show();
    delay(180);
  }
}

// ----------------------------
// 错误字符 → 全灯红色爆闪
// ----------------------------
void errorFlash() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(255, 0, 0);   
  }
  FastLED.show();
  delay(150);

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
  delay(120);
}

// ----------------------------
// 播放字符串动画
// ----------------------------
void playMessage(String msg) {
  for (int i = 0; i < msg.length(); i++) {
    int idx = letterToIndex(msg[i]);

    if (idx >= 0) {
      showLetterEffect(idx);
    }
    else if (idx == -2) {
      errorFlash();
    }
    // 空格等忽略
  }

  // 🔥 一段文字播放结束后加这个
  flashAllTwice();
}

void setup() {
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
}

void loop() {
  // 剧中经典对话
  playMessage("RIGHT HERE");
  delay(2000);

  playMessage("RUN");
  delay(2000);

}
