#include <Arduino.h>
#if defined(__AVR__) || defined(__arm__) || defined(__ARDUINO_ARC__)
	#include <avr/pgmspace.h>
#else
	#include <pgmspace.h>
#endif
#include <ssd1306_oled.h>

#ifndef _BV
#define _BV(x) (1<<x)
#endif

#if defined(__AVR__)
#undef PROGMEM
#define PROGMEM __attribute__((section(".progmem.data")))
#endif

#include "util/font5x7.h"
#include "util/font8x16.h"

#define TOTALFONTS		2

#define swap(a, b) { uint8_t t = a; a = b; b = t; }

const unsigned char * ssd1306_oled::fonts[]={
	font5x7,
	font8x16,
};

static uint8_t start_buf[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x0F, 0x1F, 0x3F, 0x73, 0xe3, 0xc3, 0x83,
	0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xE0, 0x70, 0x30, 0x30, 0x00, 0xFF, 0xFf,
	0x00, 0x00, 0x00, 0x00, 0xc0, 0xE0, 0x70, 0x30, 0x30, 0x30, 0xF0, 0xE0, 0x00, 0x00, 0x00, 0x00,
	0xE0, 0xF0, 0xB0, 0xB0, 0xB0, 0xB0, 0xf0, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0xF0, 0xF8, 0xFc, 0xce, 0xc7, 0xc3, 0xc1,
	0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc3, 0xc7, 0xce, 0xcc, 0xcc, 0xcc, 0xcF, 0xcf,
	0x00, 0x00, 0x00, 0x00, 0x03, 0xc7, 0xce, 0xcc, 0xcc, 0xc0, 0xFF, 0x7F, 0x00, 0x00, 0x00, 0x00,
	0xc7, 0xcF, 0xcd, 0xcd, 0xcd, 0xcd, 0xcd, 0xc4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};


ssd1306_oled::ssd1306_oled(uint8_t w, uint8_t h) {
	width = w;
	height = h;
	pages = (height + 0x7) >> 3;

	// oled_buf = (uint8_t *) calloc(width * pages, sizeof(uint8_t));
	// memset(oled_buf, 0xff, width * pages);
	oled_buf = start_buf;
}

ssd1306_oled::~ssd1306_oled() {
	free(oled_buf);
}


oled_return_t ssd1306_oled::init() {
	set_font(0);
	set_cursor(0,0);

	i2c_init();

	oled_command(DISPLAYOFF);

	oled_command(SETDISPLAYCLOCKDIV);
	oled_command(0x80);

	oled_command(SETMULTIPLEX);
	oled_command(height - 1);

	oled_command(SETDISPLAYOFFSET);
	oled_command(0x0);

	oled_command(SETSTARTLINE | 0x0);

	oled_command(CHARGEPUMP);
	oled_command(0x95);

	oled_command(NORMALDISPLAY);
	oled_command(DISPLAYALLONRESUME);

    oled_command(COMSCANDEC);
    oled_command(SEGREMAP | 0x0);

	oled_command(SETCOMPINS);
	oled_command(0x12);

	oled_command(SETCONTRAST);
	oled_command(0xff);

	oled_command(SETPRECHARGE);
	oled_command(0xF1);

	oled_command(SETVCOMDESELECT);
	oled_command(0x40);

	oled_command(DISPLAYON);
	clear();

	return OLED_SUCCESS;
}

oled_return_t ssd1306_oled::page_addr(uint8_t pg) {
	pg |= 0xb0;
	oled_command(pg);

	return OLED_SUCCESS;
}

oled_return_t ssd1306_oled::col_addr(uint8_t col) {
    col += 32;

	oled_command((0x10 | ((col >> 4) & 0x7)));
	oled_command(0xf & col);

	return OLED_SUCCESS;
}

oled_return_t ssd1306_oled::clear(boolean mode) {
	if (mode) {
		for (uint8_t p = 0; p < pages; p++) {
			page_addr(p);
			col_addr(0x0);
			for (uint8_t n = 0; n < width; n++) oled_data(0);
		}
	}
	else {
		memset(oled_buf, 0, width * pages);
	}

	return OLED_SUCCESS;
}

oled_return_t ssd1306_oled::invert(boolean inv) {
	if (inv)
		oled_command(INVERTDISPLAY);
	else
		oled_command(NORMALDISPLAY);

	return OLED_SUCCESS;
}

oled_return_t ssd1306_oled::contrast(uint8_t contrast) {
	oled_command(SETCONTRAST);
	oled_command(contrast);

	return OLED_SUCCESS;
}

oled_return_t ssd1306_oled::display(void) {
	for (uint8_t p = 0; p < pages; p++) {
		page_addr(p);
		col_addr(0x0);
		for (uint8_t w = 0; w < width; w++) oled_data(oled_buf[p * width + w]);
	}

	return OLED_SUCCESS;
}

oled_return_t ssd1306_oled::write(uint8_t c) {
	switch (c) {
		case '\n': {
			cursor_y += font_height;
			cursor_x  = 0;

			break;
		}

		case '\r': {

			break;
		}

		default: {
			oled_return_t err = draw_char(cursor_x, cursor_y, c);
			if(err) return OLED_GENERIC_ERROR;

			cursor_x += font_width + 1;
			if ((cursor_x > (width - font_width))) {
				cursor_y += font_height;
				cursor_x = 0;
			}

			break;
		}
	}

	return OLED_SUCCESS;
}

oled_return_t ssd1306_oled::print(const char * s) {
	size_t n = strlen(s);
	oled_return_t err;

  	if (s == NULL) return OLED_ARG_ERROR;
  	while (n--) {
		err = write(*s++);
		if(err) return OLED_GENERIC_ERROR;
	}

	return OLED_SUCCESS;
};

oled_return_t ssd1306_oled::set_cursor(uint8_t x, uint8_t y) {
	cursor_x = x;
	cursor_y = y;

	return OLED_SUCCESS;
}

oled_return_t ssd1306_oled::pixel(uint8_t x, uint8_t y, boolean colour) {
	if ((x < 0) || (x >= width) || (y < 0) || (y >= height)) return OLED_OUT_OF_BOUNDS;

	if (colour)
		oled_buf[x + (y / 8) * width] |= _BV((y % 8));
	else
		oled_buf[x + (y / 8) * width] &= ~_BV((y % 8));

	return OLED_SUCCESS;
}

oled_return_t ssd1306_oled::line(uint8_t xa, uint8_t ya, uint8_t xb, uint8_t yb) {
	boolean dir = abs(yb - ya) > abs(xb - xa);
	uint8_t dx, dy;

	if (dir) {
		swap(xa, ya);
		swap(xb, yb);
	}

	if (xa > xb) {
		swap(xa, xb);
		swap(ya, yb);
	}

	dx = xb - xa;
	dy = abs(yb - ya);

	int8_t err = dx / 2;
	int8_t ystep;

	if (ya < yb) ystep = 1;
	else ystep = -1;

	while(xa < xb) {
		if (dir) pixel(ya, xa);
		else pixel(xa, ya);

		err -= dy;

		if (err < 0) {
			ya += ystep;
			err += dx;
		}

		xa++;
	}

	return OLED_SUCCESS;
}

oled_return_t ssd1306_oled::rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
	uint8_t th;

	line(x, y, x + w, y);
	line(x, y + h - 1, x + w, y + h - 1);

	th = h - 2;

	if (th < 1) return OLED_GENERIC_ERROR;

	line(x, y + 1, x, y + th + 1);
	line(x + w - 1, y + 1, x + w - 1, y + th + 1);

	return OLED_SUCCESS;
}

oled_return_t ssd1306_oled::rect_fill(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
	for (uint8_t n = x; n < x + w; n++) line(n, y, n, y + h);

	return OLED_SUCCESS;
}

oled_return_t ssd1306_oled::circle(uint8_t x0, uint8_t y0, uint8_t radius) {
	int8_t f = 1 - radius;
	int8_t ddF_x = 1;
	int8_t ddF_y = -2 * radius;
	int8_t x = 0;
	int8_t y = radius;

	pixel(x0, y0+radius);
	pixel(x0, y0-radius);
	pixel(x0+radius, y0);
	pixel(x0-radius, y0);

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		pixel(x0 + x, y0 + y);
		pixel(x0 - x, y0 + y);
		pixel(x0 + x, y0 - y);
		pixel(x0 - x, y0 - y);

		pixel(x0 + y, y0 + x);
		pixel(x0 - y, y0 + x);
		pixel(x0 + y, y0 - x);
		pixel(x0 - y, y0 - x);
	}

	return OLED_SUCCESS;
}

oled_return_t ssd1306_oled::circle_fill(uint8_t x_pos, uint8_t y_pos, uint8_t radius) {
	int8_t f = 1 - radius;
	int8_t ddF_x = 1;
	int8_t ddF_y = -2 * radius;
	int8_t x = 0;
	int8_t y = radius;

	for (uint8_t i = y_pos - radius; i <= y_pos + radius; i++) pixel(x_pos, i);

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x ++;
		ddF_x += 2;
		f += ddF_x;

		for (uint8_t p = y_pos - y; p <= y_pos + y; p++) {
			pixel(x_pos + x, p);
			pixel(x_pos - x, p);
		}
		for (uint8_t p = y_pos - x; p <= y_pos + x; p++) {
			pixel(x_pos + y, p);
			pixel(x_pos - y, p);
		}
	}

	return OLED_SUCCESS;
}

oled_return_t ssd1306_oled::set_font(uint8_t type) {
	if ((type >= TOTALFONTS) || (type < 0)) return OLED_OUT_OF_BOUNDS;

	current_font = type;
	font_width = pgm_read_byte(fonts[current_font] + 0);
	font_height = pgm_read_byte(fonts[current_font] + 1);
	font_start_char = pgm_read_byte(fonts[current_font] + 2);
	font_total_char = pgm_read_byte(fonts[current_font] + 3);
	font_map_width = (pgm_read_byte(fonts[current_font ] + 4) * 100) + pgm_read_byte(fonts[current_font] + 5); // two bytes values into integer 16

	return OLED_SUCCESS;
}

oled_return_t ssd1306_oled::draw_char(uint8_t x, uint8_t y, uint8_t c) {
	uint8_t page_span,row, offset_c;
	uint8_t w, p, data;
	uint16_t page_per_char, bmp_col_pos, bmp_row_pos, bmp_start_pos;

	if ((c < font_start_char) || (c > (font_start_char + font_total_char - 1)))	return OLED_OUT_OF_BOUNDS;

	offset_c = c - font_start_char;

	page_span = font_height / 8;
	if (page_span <= 1) page_span = 1;

	if (page_span == 1) {
		for (w = 0; w < font_width + 1; w++) {
			if (w == font_width) data = 0;
			else data = pgm_read_byte(fonts[current_font] + FONTHEADERSIZE + (offset_c * font_width) + w);

			for (p = 0; p < 8; p++) {
				if (data & 0x1) pixel(x + w, y + p);
				else pixel(x + w, y + p, false);

				data >>= 1;
			}
		}

		return OLED_SUCCESS;
	}

	page_per_char = font_map_width / font_width;
	bmp_col_pos = offset_c % page_per_char;
	bmp_row_pos = static_cast<int>(offset_c / page_per_char);
	bmp_start_pos = (bmp_row_pos * font_map_width * (font_height / 8)) + (bmp_col_pos * font_width) ;

	for (row = 0; row < page_span; row++) {
		for (w = 0; w < font_width; w++) {
			data = pgm_read_byte(fonts[current_font] + FONTHEADERSIZE + (bmp_start_pos + w + (row * font_map_width)));
			for (p = 0; p < 8; p++) {
				if (data & 0x1) pixel(x + w, y + p + (row * 8));
				else pixel(x + w, y + p + (row * 8), false);

				data >>= 1;
			}
		}
	}

	return OLED_SUCCESS;
}

oled_return_t ssd1306_oled::flip_vertical(boolean flip) {
	if (flip) oled_command(COMSCANINC);
	else oled_command(COMSCANDEC);

	return OLED_SUCCESS;
}

oled_return_t ssd1306_oled::flip_horizontal(boolean flip) {
	if (flip) oled_command(SEGREMAP | 0x0);
	else oled_command(SEGREMAP | 0x1);

	return OLED_SUCCESS;
}

oled_return_t ssd1306_oled::draw_bmp(uint8_t * bmp) {
	for (uint16_t i = 0; i < (width * pages); i++)
		oled_buf[i] = bmp[i];

	return OLED_SUCCESS;
}
