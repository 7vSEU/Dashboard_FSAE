#include<head.h>
#include<Arduino.h>
#include<SPI.h>
#include<TFT_eSPI.h>
#include<cmath>

#define color TFT_BGR(1,1,1)
#define x0 000
#define x1 479
#define y0 000
#define y1 319

int count = 0;
TFT_eSPI tft = TFT_eSPI();
char velocity[4];

void int2v(int);
void drawcenteredlineR(int,int,int,int,int);
void drawcenteredlineB(int,int,int,int,int);

void setup(){
  //初始化黑屏
  Serial.begin(9600);
  tft.init();
  tft.setRotation(3);
  tft.setTextDatum(4);
  tft.fillScreen(TFT_BLACK);

  //基础UI
  for(int i=0;i<10;i++) tft.drawCircle(240,160,43+i,TFT_BLUE);
  tft.drawRect(0,0,480,320,TFT_BLUE);
  tft.drawLine(20+5*cos(60/180.0),20+5*sin(60/180.0),20+10*cos(60/180.0),20+10*sin(60/180.0),TFT_RED);
}

void loop(){
  tft.setTextSize(4);
  count++;

  //自覆盖部分
  int2v(count);
  tft.drawString(velocity,242,162);

  //阳刷新部分
  drawcenteredlineR(242,162,count/180.0,54,60);

  //阴回填部分
  delay(100);
  drawcenteredlineB(242,162,count/180.0,54,60);

  if(count == 150) count = 0;
  Serial.print(sin(count/180.0));
  Serial.print('\n');
}

void int2v(int num){
  velocity[3] = '\0';
  velocity[2] = (char)(num%10+48);
  velocity[1] = (char)(num%100/10+48);
  velocity[0] = (char)(num/100+48);
};

void drawcenteredlineR(int x,int y,int th,int r0,int r1){
  tft.drawLine(x+r0*cos(th),y+r0*sin(th),x+r1*cos(th),y+r1*sin(th),TFT_RED);
};
void drawcenteredlineB(int x,int y,int th,int r0,int r1){
  tft.drawLine(x+r0*cos(th),y+r0*sin(th),x+r1*cos(th),y+r1*sin(th),TFT_BLACK);
};