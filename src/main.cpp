#include <Arduino.h>
//#include <SPI.h>
#include <mcp_can.h>
#include <TFT_eSPI.h>
//#include <TinyGPS++.h>
#include "parameters.h"

#define Target_framerate 120
#define Frameraterate 1
#define GPIO_right  0
#define GPIO_left   0
#define GPIO_CANH
#define GPIO_CANL
#define Num_if_pgs  3

TFT_eSPI scr = TFT_eSPI();
hw_timer_t *timer = NULL;
hw_timer_t *timer_for_framerate = NULL;
//TinyGPSPlus gps;

long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char msgString[128];                        // Array to store serial string

#define CAN0_INT 6                              // Set INT to pin 2
MCP_CAN CAN0(7);

int PageID = 1;
bool newpage = 1;
int Frame_count = 0;
bool Frameflag1 = 1;
bool Invalid_GPS_data = 0;
bool No_GPS_data_received = 0;

uint16_t RGB888t565(int,int,int);
void Flip_left();
void Flip_right();
void FixedUI();
void showPage0(bool);
void showPage1(bool);
void showPage2(bool);
void Frameclear();
void Mark();

void Displayframe(){
  switch (PageID)
  {
    case 0:
      showPage0(0);
      break;
    case 1:
      showPage1(0);
      break;
    case 2:
      showPage2(0);
      break;
  }
  Frame_count++;
}

void setup() {
  timer = timerBegin(0,80,true);
  timerAttachInterrupt(timer,Displayframe,true);
  timerAlarmWrite(timer,1000000/Target_framerate,true);

  timer_for_framerate = timerBegin(1,80,true);
  timerAttachInterrupt(timer_for_framerate,Frameclear,true);
  timerAlarmWrite(timer_for_framerate,1000000,true);

  
  Serial.begin(115200);

  CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ);
  /*
  // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the masks and filters disabled.
  if(CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515...");
  
  CAN0.setMode(MCP_NORMAL);                     // Set operation mode to normal so the MCP2515 sends acks to received data.

  pinMode(CAN0_INT, INPUT);                            // Configuring pin for /INT input
  
  Serial.println("MCP2515 Library Receive Example...");
  */

  //pinMode(GPIO_right,INPUT);
  //pinMode(GPIO_left,INPUT);
  //attachInterrupt(digitalPinToInterrupt(GPIO_right), Flip_right, FALLING);
  //attachInterrupt(digitalPinToInterrupt(GPIO_left), Flip_left, FALLING);

  scr.init();
  scr.setRotation(3);
  scr.setTextSize(2);
  scr.fillScreen(TFT_BLACK);
  pinMode(05,OUTPUT);
  digitalWrite(05,1);
  scr.fillCircle(240,180,50,RGB888t565(255,117,24));

  timerAlarmEnable(timer);
  timerAlarmEnable(timer_for_framerate);

  INT_V = 1000;
}

void loop() {
  /*
  // 读取串口2的数据，并交给TinyGPS++对象解析
  while (Serial1.available() > 0) {
    if(No_GPS_data_received){
      No_GPS_data_received = 0;
      newpage = 1;
    }

    //scr.fillCircle(470,10,3,TFT_GREEN);
    //else scr.fillCircle(470,10,3,TFT_BLACK);

    if (gps.encode(Serial1.read())) {
      if (gps.location.isValid()) {
        if(Invalid_GPS_data){
          Invalid_GPS_data = 0;
          newpage = 1;
        }
        // 输出纬度和经度
        Lat = gps.location.lat();
        Lon = gps.location.lng();
      } else {
        Invalid_GPS_data = 1;
      }
    }
  }

  // 如果超过5秒没有数据，输出一个提示
  if (millis() > 5000 && gps.charsProcessed() < 10) {
    No_GPS_data_received = 1;
    while (true);
  }*/

  /*
  if(!digitalRead(CAN0_INT))                         // If CAN0_INT pin is low, read receive buffer
  {
    CAN0.readMsgBuf(&rxId, &len, rxBuf);      // Read data: len = data length, buf = data byte(s)
    
    if((rxId & 0x80000000) == 0x80000000)     // Determine if ID is standard (11 bits) or extended (29 bits)
      sprintf(msgString, "Extended ID: 0x%.8lX  DLC: %1d  Data:", (rxId & 0x1FFFFFFF), len);
    else
      sprintf(msgString, "Standard ID: 0x%.3lX       DLC: %1d  Data:", rxId, len);
  
    Serial.print(msgString);
  
    if((rxId & 0x40000000) == 0x40000000){    // Determine if message is a remote request frame.
      sprintf(msgString, " REMOTE REQUEST FRAME");
      Serial.print(msgString);
    } else {
      for(byte i = 0; i<len; i++){
        sprintf(msgString, " 0x%.2X", rxBuf[i]);
        Serial.print(msgString);
      }
    }
        
    Serial.println();
  }
    */
}

uint16_t RGB888t565(int R,int G,int B){
  return (R >> 3) << 11 | (G >> 2) << 5 | (B >> 3);
};

void Flip_left(){
  if(PageID == 0) return;
  PageID--;
  newpage = 1;
};
void Flip_right(){
  if(PageID == Num_if_pgs-1) return;
  PageID++;
  newpage = 1;
};
void FixedUI(){
  scr.setTextDatum(MC_DATUM);
  scr.setTextSize(2);
  scr.drawString("E59",240,10);
  scr.fillCircle(threedots_corX-threedots_distance,threedots_corY,threedots_R,RGB888t565(50,50,50));
  scr.fillCircle(threedots_corX,threedots_corY,threedots_R,RGB888t565(50,50,50));
  scr.fillCircle(threedots_corX+threedots_distance,threedots_corY,threedots_R,RGB888t565(50,50,50));
}
void showPage0(bool torefresh = 0){
  if(torefresh||newpage){
    scr.fillScreen(TFT_BLACK);
    FixedUI();
    scr.fillCircle(threedots_corX-threedots_distance,threedots_corY,threedots_R,TFT_WHITE);
    newpage = 0;
  }
  scr.setTextDatum(MC_DATUM);
  scr.setTextSize(5);
  scr.drawString(int2v(INT_V),240,160);

  scr.setTextSize(2);
  scr.setTextDatum(TL_DATUM);
};
void showPage1(bool torefresh = 0){
  if(torefresh||newpage){
    scr.fillScreen(TFT_BLACK);
    FixedUI();
    scr.fillCircle(threedots_corX,threedots_corY,threedots_R,TFT_WHITE);
    newpage = 0;

  }
};
void showPage2(bool torefresh = 0){
  if(torefresh||newpage){
    scr.fillScreen(TFT_BLACK);
    FixedUI();
    scr.fillCircle(threedots_corX+threedots_distance,threedots_corY,threedots_R,TFT_WHITE);
    newpage = 0;
  }

  scr.setTextDatum(TL_DATUM);
  scr.setTextSize(2);
  if(No_GPS_data_received)  scr.drawString("No GPS data received.",20,30);
  else if(Invalid_GPS_data) scr.drawString("Invalid GPS data.",20,30);
  else {
    scr.drawString(String(Lat),20,100);
    scr.drawString(String(Lon),20,150);}
};

void Frameclear(){
  Frameflag1 = !Frameflag1;
};

void Mark(){
  scr.setTextDatum(TL_DATUM);
  scr.drawString("OK",10,250);
}