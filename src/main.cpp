#include <Arduino.h>
#include<TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();
int led_pin[7]={6,8,11,12,13,18,19};

#define COLOR565(r, g, b) ((((r) >> 3) << 11) | (((g) >> 2) << 5) | ((b) >> 3))

void setup() {
  // put your setup code here, to run once:
  
  tft.init();
  //delay(500);
  tft.setRotation(1);
  tft.setTextSize(2);
  tft.fillScreen(TFT_BLACK);
  //tft.drawPixel(319,0,TFT_BLUE);uint8_t r = 255;
  tft.setTextColor(COLOR565(140,170,255));
  String str = String(COLOR565(0,0,1));
  tft.drawString("E24",120,5);
  //tft.drawString(str,120,100);
  /*
 for(int i=0;i<7;i++){
  pinMode(led_pin[i], OUTPUT);

 }
 */
}

void loop() {
  // put your main code here, to run repeatedly:
  /*
  for(int i=0;i<7;i++)
  {
  digitalWrite(led_pin[i], HIGH);}
  // 等待一段时间
  delay(500);

  for(int i=0;i<7;i++)
  {
  // 关闭 LED
  digitalWrite(led_pin[i], LOW);
  // 等待一段时间
  }
  delay(500);
  */

}