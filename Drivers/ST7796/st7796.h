#ifndef LCD_H
#define LCD_H

#include "stm32h7xx_hal.h"

// 引脚宏定义（根据实际接线修改）
#define LCD_RES_PIN    GPIO_PIN_4
#define LCD_RES_PORT   GPIOA
#define LCD_DC_PIN     GPIO_PIN_3
#define LCD_DC_PORT    GPIOA
#define LCD_CS_PIN     GPIO_PIN_2
#define LCD_CS_PORT    GPIOA

// 屏幕分辨率（根据实际屏幕修改）
#define LCD_WIDTH      480
#define LCD_HEIGHT     320

/* 颜色格式 RGB565 */
#define RED     0xF800
#define GREEN   0x07E0
#define BLUE    0x001F
#define WHITE   0xFFFF
#define BLACK   0x0000
#define ORANGE  0xFCA0
#define GRAY	0b0100000111001000

// 外部声明SPI句柄，避免直接引用spi.h
extern SPI_HandleTypeDef hspi1;

// 命令/数据宏
#define LCD_CMD        HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_RESET)
#define LCD_DATA       HAL_GPIO_WritePin(LCD_DC_PORT, LCD_DC_PIN, GPIO_PIN_SET)

// 函数声明
void LCD_Init(void);
void LCD_WriteCmd(uint8_t cmd);
void LCD_WriteData(uint8_t data);
void LCD_SetWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_Fill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void LCD_FillScreen(uint16_t color);

// 图形绘制函数
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color);
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color, uint8_t fill);
void LCD_DrawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color, uint8_t fill);

// 字符绘制函数
void LCD_DrawChar(uint16_t x, uint16_t y, uint8_t c, uint16_t color, uint16_t bg_color, uint8_t size);
void LCD_DrawString(uint16_t x, uint16_t y, const char *str, uint16_t color, uint16_t bg_color, uint8_t size);
void LCD_DrawCharHex(uint16_t x, uint16_t y, uint8_t c, uint16_t color, uint16_t bg_color, uint8_t size);


#endif
