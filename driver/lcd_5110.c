/* This file based on source code of LCD5110 for STM32F4
** https://stm32f4-discovery.net/pcd8544-nokia-33105110-lcd-stm32f429-discovery-library/
** and has modified to run on linux OS
*/

#include "lcd_5110.h"

unsigned char LCD_Buffer[LCD_BUFFER_SIZE];
unsigned char LCD_UpdateXmin = 0;
unsigned char LCD_UpdateXmax = 0;
unsigned char LCD_UpdateYmin = 0;
unsigned char LCD_UpdateYmax = 0;
unsigned char LCD_x;
unsigned char LCD_y;

const uint8_t LCD_Font5x7 [97][LCD_CHAR5x7_WIDTH] = {
	{ 0x00, 0x00, 0x00, 0x00, 0x00 },   // sp
	{ 0x00, 0x00, 0x2f, 0x00, 0x00 },   // !
	{ 0x00, 0x07, 0x00, 0x07, 0x00 },   // "
	{ 0x14, 0x7f, 0x14, 0x7f, 0x14 },   // #
	{ 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   // $
	{ 0x32, 0x34, 0x08, 0x16, 0x26 },   // %
	{ 0x36, 0x49, 0x55, 0x22, 0x50 },   // &
	{ 0x00, 0x05, 0x03, 0x00, 0x00 },   // '
	{ 0x00, 0x1c, 0x22, 0x41, 0x00 },   // (
	{ 0x00, 0x41, 0x22, 0x1c, 0x00 },   // )
	{ 0x14, 0x08, 0x3E, 0x08, 0x14 },   // *
	{ 0x08, 0x08, 0x3E, 0x08, 0x08 },   // +
	{ 0x00, 0x00, 0x50, 0x30, 0x00 },   // ,
	{ 0x10, 0x10, 0x10, 0x10, 0x10 },   // -
	{ 0x00, 0x60, 0x60, 0x00, 0x00 },   // .
	{ 0x20, 0x10, 0x08, 0x04, 0x02 },   // /
	{ 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0
	{ 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1
	{ 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2
	{ 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3
	{ 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4
	{ 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5
	{ 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6
	{ 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7
	{ 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8
	{ 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9
	{ 0x00, 0x36, 0x36, 0x00, 0x00 },   // :
	{ 0x00, 0x56, 0x36, 0x00, 0x00 },   // ;
	{ 0x08, 0x14, 0x22, 0x41, 0x00 },   // <
	{ 0x14, 0x14, 0x14, 0x14, 0x14 },   // =
        { 0x00, 0x41, 0x22, 0x14, 0x08 },   // >
        { 0x02, 0x01, 0x51, 0x09, 0x06 },   // ?
        { 0x32, 0x49, 0x59, 0x51, 0x3E },   // @
        { 0x7E, 0x11, 0x11, 0x11, 0x7E },   // A
        { 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B
        { 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C
        { 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D
        { 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E
        { 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F
        { 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G
        { 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H
        { 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I
        { 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J
        { 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K
        { 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L
        { 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M
        { 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N
        { 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O
        { 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P
        { 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q
        { 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R
        { 0x46, 0x49, 0x49, 0x49, 0x31 },   // S
        { 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T
        { 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U
        { 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V
        { 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W
        { 0x63, 0x14, 0x08, 0x14, 0x63 },   // X
        { 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y
        { 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z
        { 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [
        { 0x55, 0x2A, 0x55, 0x2A, 0x55 },   // 55
        { 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ]
        { 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^
	{ 0x40, 0x40, 0x40, 0x40, 0x40 },   // _
	{ 0x00, 0x01, 0x02, 0x04, 0x00 },   // '
	{ 0x20, 0x54, 0x54, 0x54, 0x78 },   // a
	{ 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b
	{ 0x38, 0x44, 0x44, 0x44, 0x20 },   // c
	{ 0x38, 0x44, 0x44, 0x48, 0x7F },   // d
	{ 0x38, 0x54, 0x54, 0x54, 0x18 },   // e
	{ 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f
	{ 0x0C, 0x52, 0x52, 0x52, 0x3E },   // g
	{ 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h
	{ 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i
	{ 0x20, 0x40, 0x44, 0x3D, 0x00 },   // j
	{ 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k
	{ 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l
	{ 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m
	{ 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n
	{ 0x38, 0x44, 0x44, 0x44, 0x38 },   // o
	{ 0x7C, 0x14, 0x14, 0x14, 0x08 },   // p
        { 0x08, 0x14, 0x14, 0x18, 0x7C },   // q
        { 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r
        { 0x48, 0x54, 0x54, 0x54, 0x20 },   // s
        { 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t
        { 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u
        { 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v
        { 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w
        { 0x44, 0x28, 0x10, 0x28, 0x44 },   // x
        { 0x0C, 0x50, 0x50, 0x50, 0x3C },   // y
        { 0x44, 0x64, 0x54, 0x4C, 0x44 },   // z
        { 0x00, 0x7F, 0x3E, 0x1C, 0x08 },   // > Filled         123
        { 0x08, 0x1C, 0x3E, 0x7F, 0x00 },   // < Filled         124
        { 0x08, 0x7C, 0x7E, 0x7C, 0x08 },   // Arrow up         125
        { 0x10, 0x3E, 0x7E, 0x3E, 0x10 },   // Arrow down       126
        { 0x3E, 0x3E, 0x3E, 0x3E, 0x3E },   // Stop             127
        { 0x00, 0x7F, 0x3E, 0x1C, 0x08 }    // Play             128
};

const uint8_t LCD_Font3x5[106][3] = {
        { 0x00, 0x00, 0x00 },   // sp - 32
        { 0x00, 0x17, 0x00 },   // ! - 33
        { 0x03, 0x00, 0x03 },   // " - 34
        { 0x1F, 0x0A, 0x1F },   // # - 35
        { 0x0A, 0x1F, 0x05 },   // $
        { 0x09, 0x04, 0x12 },   // %
        { 0x0F, 0x17, 0x1C },   // &
        { 0x00, 0x03, 0x00 },   // '
        { 0x00, 0x0E, 0x11 },   // ( - 40
        { 0x11, 0x0E, 0x00 },   // )
        { 0x05, 0x02, 0x05 },   // *
        { 0x04, 0x0E, 0x04 },   // +
        { 0x10, 0x08, 0x00 },   // ,
        { 0x04, 0x04, 0x04 },   // - - 45
        { 0x00, 0x10, 0x00 },   // .
        { 0x08, 0x04, 0x02 },   // /
        { 0x1F, 0x11, 0x1F },   // 0
        { 0x12, 0x1F, 0x10 },   // 1
        { 0x1D, 0x15, 0x17 },   // 2 - 50
        { 0x11, 0x15, 0x1F },   // 3
        { 0x07, 0x04, 0x1F },   // 4
        { 0x17, 0x15, 0x1D },   // 5
        { 0x1F, 0x15, 0x1D },   // 6
        { 0x01, 0x01, 0x1F },   // 7 - 55
        { 0x1F, 0x15, 0x1F },   // 8
        { 0x17, 0x15, 0x1F },   // 9 - 57
        { 0x00, 0x0A, 0x00 },   // :
        { 0x10, 0x0A, 0x00 },   // ;
        { 0x04, 0x0A, 0x11 },   // < - 60
        { 0x0A, 0x0A, 0x0A },   // =
        { 0x11, 0x0A, 0x04 },   // >
        { 0x01, 0x15, 0x03 },   // ?
        { 0x0E, 0x15, 0x16 },   // @
        { 0x1E, 0x05, 0x1E },   // A - 65
        { 0x1F, 0x15, 0x0A },   // B
        { 0x0E, 0x11, 0x11 },   // C
        { 0x1F, 0x11, 0x0E },   // D
        { 0x1F, 0x15, 0x15 },   // E
        { 0x1F, 0x05, 0x05 },   // F - 70
        { 0x0E, 0x15, 0x1D },   // G
        { 0x1F, 0x04, 0x1F },   // H
        { 0x11, 0x1F, 0x11 },   // I
        { 0x08, 0x10, 0x0F },   // J
        { 0x1F, 0x04, 0x1B },   // K - 75
        { 0x1F, 0x10, 0x10 },   // L
        { 0x1F, 0x06, 0x1F },   // M
        { 0x1F, 0x0E, 0x1F },   // N
        { 0x0E, 0x11, 0x0E },   // O
        { 0x1F, 0x05, 0x02 },   // P - 80
        { 0x0E, 0x11, 0x1E },   // Q
        { 0x1F, 0x0D, 0x16 },   // R
        { 0x12, 0x15, 0x09 },   // S
        { 0x01, 0x1F, 0x01 },   // T
        { 0x0F, 0x10, 0x0F },   // U - 85
        { 0x07, 0x18, 0x07 },   // V
        { 0x1F, 0x0C, 0x1F },   // W
        { 0x1B, 0x04, 0x1B },   // X
        { 0x03, 0x1C, 0x03 },   // Y
        { 0x19, 0x15, 0x13 },   // Z - 90
        { 0x1F, 0x11, 0x00 },   // [
        { 0x02, 0x04, 0x08 },   // 55 - backspace - 92
        { 0x00, 0x11, 0x1F },   // ]
        { 0x02, 0x01, 0x02 },   // ^
        { 0x10, 0x10, 0x10 },   // _ - 95
        { 0x01, 0x02, 0x00 },   // `
        { 0x1A, 0x16, 0x1C },   // a
        { 0x1F, 0x12, 0x0C },   // b
        { 0x0C, 0x12, 0x12 },   // c
        { 0x0C, 0x12, 0x1F },   // d - 100
        { 0x0C, 0x1A, 0x16 },   // e
        { 0x04, 0x1E, 0x05 },   // f
        { 0x06, 0x15, 0x0F },   // g
        { 0x1F, 0x02, 0x1C },   // h
        { 0x00, 0x1D, 0x00 },   // i - 105
        { 0x10, 0x10, 0x0D },   // j
        { 0x1F, 0x0C, 0x12 },   // k
        { 0x11, 0x1F, 0x10 },   // l
        { 0x1E, 0x0E, 0x1E },   // m
        { 0x1E, 0x02, 0x1C },   // n - 110
        { 0x0C, 0x12, 0x0C },   // o
        { 0x1E, 0x0A, 0x04 },   // p
        { 0x04, 0x0A, 0x1E },   // q
        { 0x1C, 0x02, 0x02 },   // r
        { 0x14, 0x1E, 0x0A },   // s - 115
	{ 0x02, 0x1F, 0x12 },   // t
	{ 0x0E, 0x10, 0x1E },   // u
	{ 0x0E, 0x10, 0x0E },   // v
	{ 0x1E, 0x1C, 0x1E },   // w
	{ 0x12, 0x0C, 0x12 },   // x - 120
	{ 0x02, 0x14, 0x1E },   // y
	{ 0x1A, 0x1E, 0x16 },   // z
	{ 0x04, 0x1B, 0x11 },   // {
	{ 0x00, 0x1F, 0x00 },   // |
	{ 0x11, 0x1B, 0x04 },   // }
	{ 0x04, 0x06, 0x02 },   // ~
	{ 0x1F, 0x1F, 0x1F },   // delete
};

void gpio_set_pin(int gpio, Pin_State_t state)
{
	gpio_direction_output(gpio, 1);
	gpio_set_value(gpio, state);
}

int LCD_init_IO(void)
{
	int ret;

	if (!gpio_is_valid(LCD_RST_PIN)) {
		pr_err("gpio pin %d is not available\n", LCD_RST_PIN);
		return -ENODEV;
	}

	ret = gpio_request(LCD_RST_PIN, "reset pin");
	if (ret) {
		pr_err("can not request gpio %d\n", LCD_RST_PIN);
		return ret;
	}

	if (!gpio_is_valid(LCD_DC_PIN)) {
		pr_err("gpio pin %d is not avaiable\n", LCD_DC_PIN);
		return -ENODEV;
	}

	ret = gpio_request(LCD_DC_PIN, "DC pin");
	if (ret) {
		pr_err("Can not request gpio pin %d\n", LCD_DC_PIN);
		return ret;
	}

	if (!gpio_is_valid(LCD_CLK)) {
		pr_err("gpio pin %d is not available\n", LCD_CLK);
		return -ENODEV;
	}
	ret = gpio_request(LCD_CLK, "CLK pin");
	if (ret) {
		pr_err("Can not request gpio pin %d\n", LCD_CLK);
		return ret;
	}

	if (!gpio_is_valid(LCD_DIN)) {
		pr_err("gpio pin %d is not available\n", LCD_DIN);
		return -ENODEV;
	}
	ret = gpio_request(LCD_DIN, "Data pin");
	if (ret) {
		pr_err("Can not request gpio pin %d\n", LCD_DIN);
		return ret;
	}


	/*Reset pin HIGH*/
	gpio_set_pin(LCD_RST_PIN, PIN_HIGH);

	return ret;
}

void spi_send(unsigned char data)
{
	int i;

	for (i = 0; i < 8; i++) {
		gpio_set_pin(LCD_CLK, PIN_LOW);
		if (data & 0x80) {
			gpio_set_pin(LCD_DIN, PIN_HIGH);
		} else {
			gpio_set_pin(LCD_DIN, PIN_LOW);
		}
		ndelay(10);
		gpio_set_pin(LCD_CLK, PIN_HIGH);
		data <<= 1;
		ndelay(10);
	}
}

void LCD_free_IO(void)
{
	gpio_free(LCD_RST_PIN);
	gpio_free(LCD_DC_PIN);
	gpio_free(LCD_CLK);
	gpio_free(LCD_DIN);
}

void LCD_send(unsigned char data)
{
	spi_send(data);
}

void LCD_Pin(LCD_Pin_t pin, LCD_State_t state)
{
	switch (pin) {
	case LCD_Pin_DC:
		if (state != LCD_State_Low) {
			gpio_set_pin(LCD_DC_PIN, PIN_HIGH);
		} else {
			gpio_set_pin(LCD_DC_PIN, PIN_LOW);
		}
		break;
	case LCD_Pin_RST:
		if (state != LCD_State_Low) {
			gpio_set_pin(LCD_RST_PIN, PIN_HIGH);
		} else {
			gpio_set_pin(LCD_RST_PIN, PIN_LOW);
		}
		break;
	default: break;
	}
}

void LCD_Write(LCD_WriteType_t type, unsigned char data)
{
	switch (type) {
	case LCD_DATA:
		LCD_Pin(LCD_Pin_DC, LCD_State_High);
		break;
	case LCD_COMMAND:
		LCD_Pin(LCD_Pin_DC, LCD_State_Low);
		break;
	default: break;
	}

	LCD_send(data);
}

void LCD_Home(void)
{
	LCD_Write(LCD_COMMAND, LCD_SETXADDR | 0);
	LCD_Write(LCD_COMMAND, LCD_SETYADDR | 0);
}

void LCD_GotoXY(unsigned char x, unsigned char y)
{
	LCD_x = x;
	LCD_y = y;
}

void LCD_UpdateArea(unsigned char xMin, unsigned char yMin, unsigned char xMax,
			unsigned char yMax)
{
	if (xMin < LCD_UpdateXmin)
		LCD_UpdateXmin = xMin;
	if (xMax > LCD_UpdateXmax)
		LCD_UpdateXmax = xMax;
	if (yMin < LCD_UpdateYmin)
		LCD_UpdateYmin = yMin;
	if (yMax > LCD_UpdateYmax)
		LCD_UpdateYmax = yMax;
}

void LCD_Refresh(void)
{
	unsigned char i, j;

	for (i = 0; i < 6; i++) {
		if (LCD_UpdateYmin > ((i + 1) * 8))
			continue;
		if ((i * 8) > LCD_UpdateYmax)
			break;

		LCD_Write(LCD_COMMAND, LCD_SETYADDR | i);
		LCD_Write(LCD_COMMAND, LCD_SETXADDR | LCD_UpdateXmin);

		for (j = LCD_UpdateXmin; j <= LCD_UpdateXmax; j++) {
			LCD_Write(LCD_DATA, LCD_Buffer[i * LCD_WIDTH + j]);
		}
	}

	LCD_UpdateXmin = LCD_WIDTH - 1;
	LCD_UpdateXmax = 0;
	LCD_UpdateYmin = LCD_HEIGHT - 1;
	LCD_UpdateYmax = 0;
}

void LCD_Clear(void)
{
	LCD_Home();
	memset(LCD_Buffer, 0, LCD_BUFFER_SIZE);
	LCD_GotoXY(0, 0);
	LCD_UpdateArea(0, 0, LCD_WIDTH - 1, LCD_HEIGHT -1);
	LCD_Refresh();
}

void LCD_Init(unsigned char contrast)
{
	LCD_init_IO();

	/*Reset*/
	LCD_Pin(LCD_Pin_RST, LCD_State_Low);
	msleep(10);
	LCD_Pin(LCD_Pin_RST, LCD_State_High);

	/*Go in extended mode*/
	LCD_Write(LCD_COMMAND, LCD_FUNCTIONSET | LCD_EXTENDEDINSTRUCTION);

	/*LCD bias select*/
	LCD_Write(LCD_COMMAND, LCD_SETBIAS | 0x4);

	/*set VOP*/
	if (contrast > 0x7F)
		contrast = 0x7F;
	LCD_Write(LCD_COMMAND, LCD_SETVOP | contrast);

	/*normal mode*/
	LCD_Write(LCD_COMMAND, LCD_FUNCTIONSET);

	/*Set display to Normal*/
	LCD_Write(LCD_COMMAND, LCD_DISPLAYNORMAL | LCD_DISPLAYCONTROL);

	/*Set cursor to home position*/
	LCD_Home();

	/*Normal display*/
	LCD_Write(LCD_COMMAND, LCD_DISPLAYCONTROL | LCD_DISPLAYNORMAL);

	/*Clear display*/
	LCD_Clear();
}

void LCD_SetContrast(unsigned char contrast)
{
	/*Go in extended mode*/
	LCD_Write(LCD_COMMAND, LCD_FUNCTIONSET | LCD_EXTENDEDINSTRUCTION);

	/*set VOP*/
	if (contrast > 0x7F)
		contrast = 0x7F;

	LCD_Write(LCD_COMMAND, LCD_SETVOP | contrast);

	/*normal mode*/
	LCD_Write(LCD_COMMAND, LCD_FUNCTIONSET);
}

void LCD_DrawPixel(unsigned char x, unsigned char y, LCD_Pixel_t pixel)
{
	if (x >= LCD_WIDTH)
		return;
	if (y >= LCD_HEIGHT)
		return;

	if (pixel != LCD_Pixel_Clear)
		LCD_Buffer[x + (y / 8) * LCD_WIDTH] |= 1 << (y % 8);
	else
		LCD_Buffer[x + (y / 8) * LCD_WIDTH] &= ~(1 << (y % 8));

	LCD_UpdateArea(x, y, x, y);
}

void LCD_Invert(LCD_Invert_t invert)
{
	if (invert != LCD_Invert_No)
		LCD_Write(LCD_COMMAND, LCD_DISPLAYCONTROL | LCD_DISPLAYINVERTED);
	else
		LCD_Write(LCD_COMMAND, LCD_DISPLAYCONTROL | LCD_DISPLAYNORMAL);
}

void LCD_Putc(char c, LCD_Pixel_t color, LCD_FontSize_t size)
{
	unsigned char c_height, c_width, i, b, j;

	if (size == LCD_FontSize_3x5) {
		c_width = LCD_CHAR3x5_WIDTH;
		c_height = LCD_CHAR3x5_HEIGHT;
	} else {
		c_width = LCD_CHAR5x7_WIDTH;
		c_height = LCD_CHAR5x7_HEIGHT;
	}

	if ((LCD_x + c_width) > LCD_WIDTH) {
		LCD_y += c_height;
		LCD_x = 0;
	}

	for (i = 0; i < c_width; i++) {
		if (c < 32) {
		} else if (size == LCD_FontSize_3x5) {
			b = LCD_Font3x5[c - 32][i];
		} else {
			b = LCD_Font5x7[c - 32][i];
		}

		if (b == 0x00 && (c != 0 && c != 32))
			continue;

		for (j = 0; j < c_height; j++) {
			if (color == LCD_Pixel_Set)
				LCD_DrawPixel(LCD_x, (LCD_y + j),
				((b >> j) & 1) ? LCD_Pixel_Set:LCD_Pixel_Clear);
			else
				LCD_DrawPixel(LCD_x, (LCD_y +j),
				((b >> j) & 1) ? LCD_Pixel_Clear:LCD_Pixel_Set);
		}
		LCD_x++;
	}
	LCD_x++;
}

void LCD_Puts(char *s, LCD_Pixel_t color, LCD_FontSize_t size)
{
	while (*s) {
		LCD_Putc(*s++, color, size);
	}
}

void LCD_DrawLine(unsigned char x0, unsigned char y0, unsigned char x1,
			 unsigned char y1, LCD_Pixel_t color)
{
	short dx, dy;
	short temp;

	if (x0 > x1) {
		temp = x1;
		x1 = x0;
		x0 = temp;
	}

	if (y0 > y1) {
		temp = y1;
		y1 = y0;
		y0 = temp;
	}

	dx = x1 - x0;
	dy = y1 - y0;

	if (dx == 0) {
		do {
			LCD_DrawPixel(x0, y0, color);
			y0++;
		} while (y1 >= y0);
		return;
	}

	if (dy == 0) {
		do {
			LCD_DrawPixel(x0, y0, color);
			x0++;
		} while (x1 >= x0);
		return;
	}

	if (dx > dy) {
		temp = 2 * dy - dx;
		while (x0 != x1) {
			LCD_DrawPixel(x0, y0, color);
			x0++;
			if (temp > 0) {
				y0++;
				temp += 2 * dy - 2 * dx;
			} else {
				temp += 2 * dy;
			}
		}
		LCD_DrawPixel(x0, y0, color);
	} else {
		temp = 2 * dx - dy;
		while (y0 != y1) {
			LCD_DrawPixel(x0, y0, color);
			y0++;
			if (temp > 0) {
				x0++;
				temp += 2 * dy - 2 * dx;
			} else {
				temp += 2 * dy;
			}
		}
		LCD_DrawPixel(x0, y0, color);
	}
}

void LCD_DrawRectangle(unsigned char x0, unsigned char y0, unsigned char x1,
			unsigned char y1, LCD_Pixel_t color)
{
	LCD_DrawLine(x0, y0, x1, y0, color);
	LCD_DrawLine(x0, y0, x0, y1, color);
	LCD_DrawLine(x1, y0, x1, y1, color);
	LCD_DrawLine(x0, y1, x1, y1, color);
}

void LCD_DrawFilledRectangle(unsigned char x0, unsigned char y0, unsigned char x1,
				unsigned char y1, LCD_Pixel_t color)
{
	for (; y0 < y1; y0++) {
		LCD_DrawLine(x0, y0, x1, y0, color);
	}
}

void LCD_DrawCircle(char x0, char y0, char r, LCD_Pixel_t color)
{
	int f = 1 - r;
	int ddF_x = 1;
	int ddF_y = -2 * r;
	int x = 0;
	int y = r;

	LCD_DrawPixel(x0, y0 + r, color);
	LCD_DrawPixel(x0, y0 - r, color);
	LCD_DrawPixel(x0 + r, y0, color);
	LCD_DrawPixel(x0 - r, y0, color);

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		LCD_DrawPixel(x0 + x, y0 + y, color);
		LCD_DrawPixel(x0 - x, y0 + y, color);
		LCD_DrawPixel(x0 + x, y0 - y, color);
		LCD_DrawPixel(x0 - x, y0 - y, color);

		LCD_DrawPixel(x0 + y, y0 + x, color);
		LCD_DrawPixel(x0 - y, y0 + x, color);
		LCD_DrawPixel(x0 + y, y0 - x, color);
		LCD_DrawPixel(x0 - y, y0 - x, color);
	}
}

void LCD_DrawFilledCircle(char x0, char y0, char r, LCD_Pixel_t color)
{
	int f = 1 - r;
	int ddF_x = 1;
	int ddF_y = -2 * r;
	int x = 0;
	int y = r;

	LCD_DrawPixel(x0, y0 + r, color);
	LCD_DrawPixel(x0, y0 - r, color);
	LCD_DrawPixel(x0 + r, y0, color);
	LCD_DrawPixel(x0 - r, y0, color);
	LCD_DrawLine(x0 - r, y0, x0 + r, y0, color);

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		LCD_DrawLine(x0 - x, y0 + y, x0 + x, y0 + y, color);
		LCD_DrawLine(x0 + x, y0 - y, x0 - x, y0 - y, color);

		LCD_DrawLine(x0 + y, y0 + x, x0 - y, y0 + x, color);
		LCD_DrawLine(x0 + y, y0 - x, x0 - y, y0 - x, color);
	}
}
