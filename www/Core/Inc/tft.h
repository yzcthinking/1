#ifndef TFT_H
#define TFT_H

#include "main.h"

void TFT_Init(void);
void TFT_Clear(uint16_t color);
void TFT_PutChar(char character);
void TFT_DrawChar(uint16_t x, uint16_t y, char character,
                  uint16_t foreground, uint16_t background);
void TFT_WriteText(uint16_t x, uint16_t y, const char *text,
                   uint16_t foreground, uint16_t background);
void TFT_WriteLargeCode(const char *code, uint16_t foreground, uint16_t background);

#endif
