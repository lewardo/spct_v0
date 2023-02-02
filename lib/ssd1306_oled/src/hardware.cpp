 #include <ssd1306_oled.h>
#include <Wire.h>

#define I2C_FREQ 400000L

void ssd1306_oled::i2c_init() {
	Wire.begin();
}

void ssd1306_oled::i2c_write(byte dc, byte data) {
	Wire.beginTransmission(I2C_ADDR);
	Wire.write(dc);
	Wire.write(data);
	Wire.endTransmission();
}

void ssd1306_oled::oled_command(uint8_t c) {
		i2c_write(I2C_COMMAND, c);
}

void ssd1306_oled::oled_data(uint8_t c) {
		i2c_write(I2C_DATA, c);
}
