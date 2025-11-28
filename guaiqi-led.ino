#include <deque>
#include <ArduinoJson.h>
std::deque<String> msgQueue;
#include <WiFi.h>
#include <WebServer.h>
#include <EEPROM.h>
#include "html.h"
#include "homepage.h"
#include <FastLED.h>

#define NUM_LEDS 26         // 灯珠数量
#define DATA_PIN 12         // 数据引脚
#define BRIGHTNESS 200      // 亮度设置 (0-255)
#define EEPROM_SIZE 512     // EEPROM 大小
#define SSID_ADDR 0         // SSID 存储地址
#define PASS_ADDR 64        // 密码存储地址
#define CONFIG_FLAG_ADDR 128// 配置标志地址

const char* ap_ssid = "ESP32-Config";
const char* ap_password = "12345678";

#define CONFIG_PIN 0

IPAddress local_ip(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

WebServer server(80);


CRGB leds[NUM_LEDS];

const long wifi_timeout = 20000;

bool is_setup_mode = false;
String saved_ssid = "";
String saved_password = "";

// ------------------------- 函数声明 --------------------------------
void setupAPMode();
void handleRoot();
void handleSave();
void handleScan();
void handleReset();
void handleNotFound();
// ------------------------------------------------------------------

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
// 非阻塞式LED动画状态机
// ----------------------------
enum LedAnimState {
  IDLE,
  LETTER_RED_UP,
  LETTER_WHITE_FLASH,
  LETTER_OFF,
  ERROR_FLASH_ON,
  ERROR_FLASH_OFF,
  FLASH_ALL_ON,
  FLASH_ALL_OFF,
  NEXT_LETTER,
  END
};

struct LedAnim {
  String msg;
  int pos;
  int v;
  int flashCount;
  LedAnimState state;
  unsigned long lastTime;
  bool errorChar;
};

LedAnim ledAnim = {"", 0, 0, 0, IDLE, 0, false};

void playMessage(String msg) {
  msgQueue.push_back(msg);
}

void startNextMessage() {
  if (msgQueue.empty() || ledAnim.state != IDLE) return;
  String msg = msgQueue.front();
  msgQueue.pop_front();
  Serial.print("收到消息: ");
  Serial.println(msg);
  ledAnim.msg = msg;
  ledAnim.pos = 0;
  ledAnim.v = 0;
  ledAnim.flashCount = 0;
  ledAnim.state = NEXT_LETTER;
  ledAnim.lastTime = millis();
  ledAnim.errorChar = false;
}

void updateLedAnim() {
  if (ledAnim.state == IDLE) {
    startNextMessage();
    return;
  }
  unsigned long now = millis();
  int idx = -1;
  if (ledAnim.pos < ledAnim.msg.length())
    idx = letterToIndex(ledAnim.msg[ledAnim.pos]);
  switch (ledAnim.state) {
    case NEXT_LETTER:
      if (ledAnim.pos >= ledAnim.msg.length()) {
        ledAnim.flashCount = 0;
        ledAnim.state = FLASH_ALL_ON;
        ledAnim.lastTime = now;
        break;
      }
      if (idx >= 0) {
        ledAnim.v = 0;
        ledAnim.state = LETTER_RED_UP;
        ledAnim.lastTime = now;
      } else if (idx == -2) {
        ledAnim.state = ERROR_FLASH_ON;
        ledAnim.lastTime = now;
      } else {
        ledAnim.pos++;
        ledAnim.state = NEXT_LETTER;
      }
      break;
    case LETTER_RED_UP: {
      int realIndex = ledMap[idx];
      leds[realIndex] = CRGB(ledAnim.v, ledAnim.v, ledAnim.v); // 渐变为白色
      FastLED.show();
      if (now - ledAnim.lastTime >= 15) {
        ledAnim.v += 5;
        ledAnim.lastTime = now;
        if (ledAnim.v >= 255) {
          leds[realIndex] = CRGB::White;
          FastLED.show();
          ledAnim.state = LETTER_WHITE_FLASH;
          ledAnim.lastTime = now;
        }
      }
      break;
    }
    case LETTER_WHITE_FLASH: {
      int realIndex = ledMap[idx];
      if (now - ledAnim.lastTime >= 100) { // 保持亮起时间 50ms
        leds[realIndex] = CRGB::Black;
        FastLED.show();
        ledAnim.state = LETTER_OFF;
        ledAnim.lastTime = now;
      }
      break;
    }
    case LETTER_OFF: {
      int realIndex = ledMap[idx];
      if (now - ledAnim.lastTime >= 200) { // 字母间隔时间 200ms
        leds[realIndex] = CRGB::Black;
        FastLED.show();
        ledAnim.pos++;
        ledAnim.state = NEXT_LETTER;
        ledAnim.lastTime = now;
      }
      break;
    }
    case ERROR_FLASH_ON:
      for (int i = 0; i < NUM_LEDS; i++) leds[i] = CRGB(255, 0, 0);
      FastLED.show();
      if (now - ledAnim.lastTime >= 150) {
        ledAnim.state = ERROR_FLASH_OFF;
        ledAnim.lastTime = now;
      }
      break;
    case ERROR_FLASH_OFF:
      for (int i = 0; i < NUM_LEDS; i++) leds[i] = CRGB::Black;
      FastLED.show();
      if (now - ledAnim.lastTime >= 120) {
        ledAnim.pos++;
        ledAnim.state = NEXT_LETTER;
        ledAnim.lastTime = now;
      }
      break;
    case FLASH_ALL_ON:
      for (int i = 0; i < NUM_LEDS; i++) leds[i] = CRGB::White;
      FastLED.show();
      if (now - ledAnim.lastTime >= 180) {
        ledAnim.state = FLASH_ALL_OFF;
        ledAnim.lastTime = now;
      }
      break;
    case FLASH_ALL_OFF:
      for (int i = 0; i < NUM_LEDS; i++) leds[i] = CRGB::Black;
      FastLED.show();
      if (now - ledAnim.lastTime >= 300) { // 闪烁间隔 300ms
        ledAnim.flashCount++;
        if (ledAnim.flashCount < 4) { // 总共闪烁两次（亮灭为一次，2*2=4）
          ledAnim.state = FLASH_ALL_ON;
          ledAnim.lastTime = now;
        } else {
          ledAnim.state = END;
          ledAnim.lastTime = now;
        }
      }
      break;
    case END:
      ledAnim.state = IDLE;
      break;
    default:
      ledAnim.state = IDLE;
      break;
  }
}

void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println("初始化开始...");

  pinMode(CONFIG_PIN, INPUT_PULLUP);
  EEPROM.begin(EEPROM_SIZE);

  // 统一注册全部路由（避免重复调用 server.begin）
  server.on("/", HTTP_GET, handleRoot);
  server.on("/save", HTTP_POST, handleSave);
  server.on("/scan", HTTP_GET, handleScan);
  server.on("/reset", HTTP_POST, handleReset);
  server.onNotFound(handleNotFound);
  server.on("/message", HTTP_POST, []() {
    String msg = server.arg("msg");
    msg.trim();
    msg.toUpperCase();
    if (msg.length() == 0 || msg.length() > 25) {
      server.send(400, "text/plain", "消息内容不合法");
      return;
    }
    for (int i = 0; i < msg.length(); ++i) {
      char c = msg[i];
      if (!(c == ' ' || (c >= 'A' && c <= 'Z'))) {
        server.send(400, "text/plain", "只能输入A-Z字母和空格");
        return;
      }
    }
    playMessage(msg);
    server.send(200, "text/plain", "消息已加入队列");
  });

  server.on("/queue", HTTP_GET, []() {
    DynamicJsonDocument doc(64);
    doc["waiting"] = (int)msgQueue.size();
    String out;
    serializeJson(doc, out);
    server.send(200, "application/json", out);
  });

  bool is_configured = EEPROM.read(CONFIG_FLAG_ADDR) == 1;

  if (is_configured) {
    Serial.println("检测到已保存的WiFi配置，尝试连接...");

    char ssid[64] = {0};
    char password[64] = {0};

    for (int i = 0; i < 64; i++) {
      ssid[i] = EEPROM.read(SSID_ADDR + i);
      password[i] = EEPROM.read(PASS_ADDR + i);
    }

    saved_ssid = String(ssid);
    saved_password = String(password);

    Serial.print("连接WiFi: ");
    Serial.println(saved_ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(saved_ssid.c_str(), saved_password.c_str());

    unsigned long start_time = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - start_time < wifi_timeout) {
      delay(500);
      Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nWiFi连接成功");
      Serial.print("IP地址: ");
      Serial.println(WiFi.localIP());

      is_setup_mode = false;     // ⭐ 必须，加上后才能进入主页
      server.begin();
      Serial.println("HTTP服务器已启动（STA模式）");
      FastLED.addLeds<WS2811, DATA_PIN, GRB>(leds, NUM_LEDS);
      FastLED.setBrightness(BRIGHTNESS);
      FastLED.clear();
      FastLED.show();
      return;
    }

    Serial.println("\n连接失败，进入配网模式");
  } else {
    Serial.println("无配置记录，进入配网模式");
  }

  // 启动AP
  setupAPMode();
}


void loop() {

  // server 在 AP 或 STA 模式都需要处理请求
  server.handleClient();
  updateLedAnim();

  // 硬件按钮进入配置模式
  if (!is_setup_mode && digitalRead(CONFIG_PIN) == LOW) {
    delay(50);
    while (digitalRead(CONFIG_PIN) == LOW) delay(20);
    Serial.println("按钮触发，进入配网模式");
    setupAPMode();
  }

  delay(1);
}



// ------------------------- AP 配网模式 -------------------------

void setupAPMode() {
  is_setup_mode = true;

  Serial.println("启动 AP 模式...");
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  WiFi.softAP(ap_ssid, ap_password);

  Serial.print("AP SSID: ");
  Serial.println(ap_ssid);
  Serial.print("密码: ");
  Serial.println(ap_password);
  Serial.print("AP IP: ");
  Serial.println(local_ip);

  server.begin();
  Serial.println("HTTP服务器已启动（AP模式）");
}



// ------------------------- HTTP 处理 -------------------------

void handleRoot() {
  if (is_setup_mode || WiFi.getMode() == WIFI_AP) {
    server.send(200, "text/html", html_page);        // 配网页面
  } else {
    server.send(200, "text/html", homepage_html);    // 主页面
  }
}

void handleSave() {
  String new_ssid = server.arg("ssid");
  String new_password = server.arg("password");

  if (new_ssid.length() == 0) {
    server.send(400, "text/plain", "错误：SSID不能为空");
    return;
  }

  Serial.println("保存新的WiFi配置:");
  Serial.println("SSID: " + new_ssid);
  Serial.println("密码: " + new_password);

  for (int i = 0; i < EEPROM_SIZE; i++) EEPROM.write(i, 0);

  for (unsigned int i = 0; i < new_ssid.length() && i < 63; i++)
    EEPROM.write(SSID_ADDR + i, new_ssid[i]);

  for (unsigned int i = 0; i < new_password.length() && i < 63; i++)
    EEPROM.write(PASS_ADDR + i, new_password[i]);

  EEPROM.write(CONFIG_FLAG_ADDR, 1);
  EEPROM.commit();

  server.send(200, "text/html", success_html);

  delay(2000);
  ESP.restart();
}

void handleScan() {
  Serial.println("扫描WiFi...");
  int n = WiFi.scanNetworks();

  String json = "[";
  for (int i = 0; i < n; i++) {
    if (i > 0) json += ",";
    json += "{";
    json += "\"ssid\":\"" + WiFi.SSID(i) + "\",";
    json += "\"rssi\":" + String(WiFi.RSSI(i)) + ",";
    json += "\"encrypted\":" + String(WiFi.encryptionType(i) != WIFI_AUTH_OPEN ? "true" : "false");
    json += "}";
  }
  json += "]";

  server.send(200, "application/json", json);
}

void handleReset() {
  for (int i = 0; i < EEPROM_SIZE; i++) EEPROM.write(i, 0);
  EEPROM.write(CONFIG_FLAG_ADDR, 0);
  EEPROM.commit();

  server.send(200, "text/html", reset_html);

  delay(2000);
  ESP.restart();
}

void handleNotFound() {
  server.send(404, "text/plain", "404: 未找到页面");
}