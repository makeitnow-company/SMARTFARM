/*
 * Blynk 기본 설정
 */
#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL3IhvBKiT"
#define BLYNK_DEVICE_NAME "SMAR FARM"
#define BLYNK_AUTH_TOKEN "PW-PS0iZ5GaNENoMeo2IC1vctprzso9h"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "KT_GiGA_2G_makeitnow"; // ESP32가 연결할 WIFI ID
char pass[] = "makeitnow1";           // ESP32가 연결할 WIFI PW

/*
 * OLED 디스플레이모듈 사용
 */
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/*
 * 식물LED
 */
#include <Adafruit_NeoPixel.h>
#define PIN       19
#define NUMPIXELS 9
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

/*
 * DHT11 온습도 센서
 */
#include <DHT.h>
#define DHTPIN 18
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

/*
 * 토양 수분 센서, 조도센서
 */
#define SOIL 36 //GPIO 36번에 토양수분센서
#define CDS 39  //GPIO 39번에 조도센서
#define BUTTON 23 //GPIO 23번에 LED제어 버튼

/*
 * 사용 변수 선언
 */
float h,t;      //습도, 온도
int soil_value, cds_value; //토양센서, 조도센서
bool led_state; //LED 상태 값

BlynkTimer timer;

void soilEvent(){
  soil_value = analogRead(SOIL);
  soil_value = map(soil_value, 1300, 3100, 100, 0);
  Blynk.virtualWrite(V2, soil_value);
}

void dhtEvent(){
  h = dht.readHumidity();
  t = dht.readTemperature();
  Blynk.virtualWrite(V0, h);
  Blynk.virtualWrite(V1, t);
}

void cdsEvent(){
  cds_value = analogRead(CDS);
  Blynk.virtualWrite(V4, cds_value/1000);
}

void showDisplay(){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE); 
  display.setCursor(0, 0);
  display.print("HUMI:");
  display.print(h);
  display.setCursor(0, 21);
  display.print("TEMP:");
  display.print(t);
  display.setCursor(0, 42);
  display.print("SOIL:");
  display.print(soil_value);
  display.display();
}

BLYNK_WRITE(V3){
  led_state = param.asInt(); //V3핀의 상태를 서버로부터 가져오는 코드
}

void rgbControl(){
  if(digitalRead(BUTTON)){
    led_state = !led_state;
    delay(30);
    Blynk.virtualWrite(V3, led_state);
  }
  
  pixels.clear();
  if(led_state){
    for(int i=0; i<NUMPIXELS; i++){
      if(i%2){
        pixels.setPixelColor(i, pixels.Color(255,0,0));
      }else{
        pixels.setPixelColor(i, pixels.Color(0,0,255));
      }
    }
  }else{
    for(int i=0; i<NUMPIXELS; i++){
      pixels.setPixelColor(i, pixels.Color(0,0,0));
    }
  }
  pixels.show();
}


void setup() {
  Serial.begin(9600);
  pinMode(BUTTON,INPUT);
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // 무한루프
  }
  dht.begin();  //온습도 센서 초기화
  pixels.begin(); //LED 초기화
  timer.setInterval(300L, soilEvent); //0.3초 간격으로 토양센서 데이터 읽기
  timer.setInterval(1000L, dhtEvent); //1초 간격으로 온습도 데이터 읽기
  timer.setInterval(500L, cdsEvent);  //0.5초 간격으로 조도 데이터 읽기
  timer.setInterval(300L, rgbControl);//0.3초 간격으로 LED 버튼체크
  timer.setInterval(300L, showDisplay);//0.3초 간격으로 디스플레이 업데이트
  Blynk.begin(auth, ssid, pass);  //BLYNK 초기화
}

void loop() {
  Blynk.run();
  timer.run();
}
