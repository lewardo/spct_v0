#ifndef SFE_MICROOLED_H
#define SFE_MICROOLED_H

#include <stdio.h>
#include <Arduino.h>

#if defined(__AVR__) || defined(__arm__) || defined(__ARDUINO_ARC__)
	#include <avr/pgmspace.h>
#else
	#include <pgmspace.h>
#endif

#define I2C_ADDR 0b0111100
#define I2C_COMMAND 0x00
#define I2C_DATA 0x40

#define FONTHEADERSIZE		6

#define SETCONTRAST 		0x81
#define DISPLAYALLONRESUME 	0xA4
#define DISPLAYALLON 		0xA5
#define NORMALDISPLAY 		0xA6
#define INVERTDISPLAY 		0xA7
#define DISPLAYOFF 			0xAE
#define DISPLAYON 			0xAF
#define SETDISPLAYOFFSET 	0xD3
#define SETCOMPINS 			0xDA
#define SETVCOMDESELECT		0xDB
#define SETDISPLAYCLOCKDIV 	0xD5
#define SETPRECHARGE 		0xD9
#define SETMULTIPLEX 		0xA8
#define SETLOWCOLUMN 		0x00
#define SETHIGHCOLUMN 		0x10
#define SETSTARTLINE 		0x40
#define MEMORYMODE 			0x20
#define COMSCANINC 			0xC0
#define COMSCANDEC 			0xC8
#define SEGREMAP 			0xA0
#define CHARGEPUMP 			0x8D
#define EXTERNALVCC 		0x01
#define SWITCHCAPVCC 		0x02

enum oled_return_t {
	OLED_SUCCESS,
	OLED_HW_ERROR,
	OLED_ARG_ERROR,
	OLED_GENERIC_ERROR,
	OLED_OUT_OF_BOUNDS,
};

class ssd1306_oled {
public:
	ssd1306_oled(uint8_t w, uint8_t h);
	~ssd1306_oled();

	oled_return_t init(void);
	oled_return_t write(uint8_t c);
	oled_return_t print(const char * s);

	void oled_command(uint8_t c);
	void oled_data(uint8_t c);
	oled_return_t col_addr(uint8_t col);
	oled_return_t page_addr(uint8_t pg);

	oled_return_t clear(boolean mode = true);
	oled_return_t invert(boolean inv);
	oled_return_t contrast(uint8_t contrast);
	oled_return_t display(void);
	oled_return_t display(uint8_t xa, uint8_t ya, uint8_t xb, uint8_t yb);
	oled_return_t set_cursor(uint8_t x, uint8_t y);
	oled_return_t pixel(uint8_t x, uint8_t y, boolean colour = true);
	oled_return_t line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
	oled_return_t rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
	oled_return_t rect_fill(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
	oled_return_t circle(uint8_t x, uint8_t y, uint8_t radius);
	oled_return_t circle_fill(uint8_t x0, uint8_t y0, uint8_t radius);
	oled_return_t draw_char(uint8_t x, uint8_t y, uint8_t c);
	oled_return_t draw_bmp(uint8_t * bmp);

	oled_return_t set_font(uint8_t type);

	oled_return_t flip_vertical(boolean flip);
	oled_return_t flip_horizontal(boolean flip);

private:
	uint8_t width, height, pages;
	uint8_t * oled_buf;

	uint8_t font_width, font_height, current_font, font_start_char, font_total_char, cursor_x, cursor_y;
	uint16_t font_map_width;
	static const unsigned char *fonts[];

	void i2c_init();
	void i2c_write(byte control, byte data);
};
#endif
