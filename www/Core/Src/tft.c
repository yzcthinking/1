#include "tft.h"

#define TFT_WIDTH       320U
#define TFT_HEIGHT      240U
#define TFT_CHAR_WIDTH  6U
#define TFT_CHAR_HEIGHT 8U
#define TFT_BLACK       0x0000U
#define TFT_WHITE       0xFFFFU

#define TFT_CS_PORT GPIOB
#define TFT_CS_PIN  GPIO_PIN_12
#define TFT_SCK_PORT GPIOB
#define TFT_SCK_PIN  GPIO_PIN_13
#define TFT_MOSI_PORT GPIOB
#define TFT_MOSI_PIN  GPIO_PIN_15
#define TFT_DC_PORT GPIOC
#define TFT_DC_PIN  GPIO_PIN_5
#define TFT_BL_PORT GPIOB
#define TFT_BL_PIN  GPIO_PIN_1

static uint16_t cursor_x;
static uint16_t cursor_y;

static void tft_write_byte(uint8_t value)
{
  for (uint8_t bit = 0U; bit < 8U; ++bit)
  {
    HAL_GPIO_WritePin(TFT_SCK_PORT, TFT_SCK_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(TFT_MOSI_PORT, TFT_MOSI_PIN,
                     (value & 0x80U) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(TFT_SCK_PORT, TFT_SCK_PIN, GPIO_PIN_SET);
    value <<= 1;
  }
  HAL_GPIO_WritePin(TFT_SCK_PORT, TFT_SCK_PIN, GPIO_PIN_RESET);
}

static void tft_command(uint8_t command)
{
  HAL_GPIO_WritePin(TFT_CS_PORT, TFT_CS_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(TFT_DC_PORT, TFT_DC_PIN, GPIO_PIN_RESET);
  tft_write_byte(command);
  HAL_GPIO_WritePin(TFT_CS_PORT, TFT_CS_PIN, GPIO_PIN_SET);
}

static void tft_data(const uint8_t *data, uint32_t length)
{
  HAL_GPIO_WritePin(TFT_CS_PORT, TFT_CS_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(TFT_DC_PORT, TFT_DC_PIN, GPIO_PIN_SET);
  while (length-- != 0U)
  {
    tft_write_byte(*data++);
  }
  HAL_GPIO_WritePin(TFT_CS_PORT, TFT_CS_PIN, GPIO_PIN_SET);
}

static void tft_data_byte(uint8_t value)
{
  tft_data(&value, 1U);
}

static void tft_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
  uint8_t data[4];
  tft_command(0x2AU);
  data[0] = (uint8_t)(x0 >> 8); data[1] = (uint8_t)x0;
  data[2] = (uint8_t)(x1 >> 8); data[3] = (uint8_t)x1;
  tft_data(data, 4U);
  tft_command(0x2BU);
  data[0] = (uint8_t)(y0 >> 8); data[1] = (uint8_t)y0;
  data[2] = (uint8_t)(y1 >> 8); data[3] = (uint8_t)y1;
  tft_data(data, 4U);
  tft_command(0x2CU);
}

static const uint8_t *glyph(char character)
{
  static uint8_t result[5];
  static const uint8_t digits[10][5] = {
    {0x3E,0x51,0x49,0x45,0x3E},{0x00,0x42,0x7F,0x40,0x00},
    {0x42,0x61,0x51,0x49,0x46},{0x21,0x41,0x45,0x4B,0x31},
    {0x18,0x14,0x12,0x7F,0x10},{0x27,0x45,0x45,0x45,0x39},
    {0x3C,0x4A,0x49,0x49,0x30},{0x01,0x71,0x09,0x05,0x03},
    {0x36,0x49,0x49,0x49,0x36},{0x06,0x49,0x49,0x29,0x1E}
  };
  static const uint8_t letters[26][5] = {
    {0x7E,0x11,0x11,0x11,0x7E},{0x7F,0x49,0x49,0x49,0x36},
    {0x3E,0x41,0x41,0x41,0x22},{0x7F,0x41,0x41,0x22,0x1C},
    {0x7F,0x49,0x49,0x49,0x41},{0x7F,0x09,0x09,0x09,0x01},
    {0x3E,0x41,0x49,0x49,0x7A},{0x7F,0x08,0x08,0x08,0x7F},
    {0x00,0x41,0x7F,0x41,0x00},{0x20,0x40,0x41,0x3F,0x01},
    {0x7F,0x08,0x14,0x22,0x41},{0x7F,0x40,0x40,0x40,0x40},
    {0x7F,0x02,0x0C,0x02,0x7F},{0x7F,0x04,0x08,0x10,0x7F},
    {0x3E,0x41,0x41,0x41,0x3E},{0x7F,0x09,0x09,0x09,0x06},
    {0x3E,0x41,0x51,0x21,0x5E},{0x7F,0x09,0x19,0x29,0x46},
    {0x46,0x49,0x49,0x49,0x31},{0x01,0x01,0x7F,0x01,0x01},
    {0x3F,0x40,0x40,0x40,0x3F},{0x1F,0x20,0x40,0x20,0x1F},
    {0x7F,0x20,0x18,0x20,0x7F},{0x63,0x14,0x08,0x14,0x63},
    {0x07,0x08,0x70,0x08,0x07},{0x61,0x51,0x49,0x45,0x43}
  };
  if (character >= 'a' && character <= 'z') character -= ('a' - 'A');
  if (character >= '0' && character <= '9') return digits[character - '0'];
  if (character >= 'A' && character <= 'Z') return letters[character - 'A'];
  result[0] = result[1] = result[2] = result[3] = result[4] = 0U;
  if (character == '-') result[1] = result[2] = result[3] = 0x08U;
  else if (character == '.') result[2] = 0x60U;
  else if (character == ':') { result[1] = result[3] = 0x36U; }
  else if (character == ' ') { return result; }
  else if (character == '>') { result[1] = 0x08U; result[2] = 0x14U; result[3] = 0x22U; }
  else if (character == '<') { result[1] = 0x22U; result[2] = 0x14U; result[3] = 0x08U; }
  else { result[0] = result[1] = result[2] = result[3] = result[4] = 0x7FU; }
  return result;
}

void TFT_Init(void)
{
  HAL_GPIO_WritePin(TFT_CS_PORT, TFT_CS_PIN, GPIO_PIN_SET);
  HAL_GPIO_WritePin(TFT_BL_PORT, TFT_BL_PIN, GPIO_PIN_SET);
  tft_command(0x01U); HAL_Delay(5U);
  tft_command(0x11U); HAL_Delay(120U);
  tft_command(0x3AU); tft_data_byte(0x55U);
  tft_command(0x36U); tft_data_byte(0x28U);
  tft_command(0x29U); HAL_Delay(20U);
  TFT_Clear(TFT_BLACK);
}

void TFT_Clear(uint16_t color)
{
  uint8_t data[2] = {(uint8_t)(color >> 8), (uint8_t)color};
  tft_set_window(0U, 0U, TFT_WIDTH - 1U, TFT_HEIGHT - 1U);
  HAL_GPIO_WritePin(TFT_CS_PORT, TFT_CS_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(TFT_DC_PORT, TFT_DC_PIN, GPIO_PIN_SET);
  for (uint32_t pixel = 0U; pixel < TFT_WIDTH * TFT_HEIGHT; ++pixel) tft_data(data, 2U);
  HAL_GPIO_WritePin(TFT_CS_PORT, TFT_CS_PIN, GPIO_PIN_SET);
  cursor_x = 0U; cursor_y = 0U;
}

void TFT_DrawChar(uint16_t x, uint16_t y, char character, uint16_t foreground, uint16_t background)
{
  uint8_t pixels[2];
  const uint8_t *bitmap = glyph(character);
  tft_set_window(x, y, x + 5U, y + 7U);
  HAL_GPIO_WritePin(TFT_CS_PORT, TFT_CS_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(TFT_DC_PORT, TFT_DC_PIN, GPIO_PIN_SET);
  pixels[0] = (uint8_t)(foreground >> 8); pixels[1] = (uint8_t)foreground;
  for (uint8_t row = 0U; row < 8U; ++row)
  {
    for (uint8_t column = 0U; column < 6U; ++column)
    {
      if (column < 5U && (bitmap[column] & (1U << row)) != 0U)
        tft_data(pixels, 2U);
      else
      {
        pixels[0] = (uint8_t)(background >> 8); pixels[1] = (uint8_t)background;
        tft_data(pixels, 2U);
        pixels[0] = (uint8_t)(foreground >> 8); pixels[1] = (uint8_t)foreground;
      }
    }
  }
  HAL_GPIO_WritePin(TFT_CS_PORT, TFT_CS_PIN, GPIO_PIN_SET);
}

void TFT_PutChar(char character)
{
  if (character == '\f') { TFT_Clear(TFT_BLACK); return; }
  if (character == '\r') { cursor_x = 0U; return; }
  if (character == '\n') { cursor_x = 0U; cursor_y += TFT_CHAR_HEIGHT; }
  else if (character == '\b') { if (cursor_x >= TFT_CHAR_WIDTH) cursor_x -= TFT_CHAR_WIDTH; }
  else if (character >= 0x20 && character <= 0x7EU)
  {
    TFT_DrawChar(cursor_x, cursor_y, character, TFT_WHITE, TFT_BLACK);
    cursor_x += TFT_CHAR_WIDTH;
  }
  if (cursor_x + TFT_CHAR_WIDTH > TFT_WIDTH) { cursor_x = 0U; cursor_y += TFT_CHAR_HEIGHT; }
  if (cursor_y + TFT_CHAR_HEIGHT > TFT_HEIGHT) { TFT_Clear(TFT_BLACK); }
}

void TFT_WriteText(uint16_t x, uint16_t y, const char *text, uint16_t foreground, uint16_t background)
{
  while (*text != '\0')
  {
    TFT_DrawChar(x, y, *text++, foreground, background);
    x += TFT_CHAR_WIDTH;
    if (x + TFT_CHAR_WIDTH > TFT_WIDTH) { x = 0U; y += TFT_CHAR_HEIGHT; }
  }
}

static void TFT_DrawScaledChar(uint16_t x, uint16_t y, char character,
                               uint16_t foreground, uint16_t background)
{
  const uint8_t *bitmap = glyph(character);
  uint8_t data[2];

  tft_set_window(x, y, x + 41U, y + 79U);
  HAL_GPIO_WritePin(TFT_CS_PORT, TFT_CS_PIN, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(TFT_DC_PORT, TFT_DC_PIN, GPIO_PIN_SET);
  for (uint8_t row = 0U; row < 8U; ++row) {
    for (uint8_t row_scale = 0U; row_scale < 10U; ++row_scale) {
      for (uint8_t column = 0U; column < 6U; ++column) {
        uint16_t color = (column < 5U && (bitmap[column] & (1U << row)) != 0U)
                       ? foreground : background;
        data[0] = (uint8_t)(color >> 8);
        data[1] = (uint8_t)color;
        for (uint8_t column_scale = 0U; column_scale < 7U; ++column_scale) {
          tft_data(data, 2U);
        }
      }
    }
  }
  HAL_GPIO_WritePin(TFT_CS_PORT, TFT_CS_PIN, GPIO_PIN_SET);
}

void TFT_WriteLargeCode(const char *code, uint16_t foreground, uint16_t background)
{
  const uint16_t first_line_x = 34U;
  const uint16_t second_line_x = 13U;

  TFT_Clear(background);
  for (uint8_t index = 0U; index < 7U; ++index) {
    TFT_DrawScaledChar(first_line_x + (uint16_t)index * 42U, 35U,
                       code[index], foreground, background);
  }
  for (uint8_t index = 7U; index < 15U; ++index) {
    TFT_DrawScaledChar(second_line_x + (uint16_t)(index - 7U) * 42U, 125U,
                       code[index], foreground, background);
  }
}
