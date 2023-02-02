#include <ssd1306_oled.h>

ssd1306_oled oled = ssd1306_oled(64, 32);

struct {
  String media;
  
  struct {
    int t=0, T=1;
  } progress;
  
  int connstat = false, playstat = false, vrstat = false;
} info;

void oled_update() {
  oled.clear(false);
  oled.set_cursor(0, 8);
  oled.print(info.media.substring(0, 10).c_str());

  info.media = info.media.substring(1, info.media.length()) + info.media[0];

  oled.line(0, 31, 63 * info.progress.t / info.progress.T, 31);

  oled.set_cursor(12, 0);
  
  if(info.connstat < 2) {
    oled.print("-");
  } else if(info.connstat > 3) {
    oled.print("st");
  } else {
    oled.print("con");
  }

  oled.set_cursor(0, 0);

  if(info.playstat == 0) {
    oled.print("o");
  } else if (info.playstat == 1) {
    oled.print("+");
  } else {
    oled.print("-");
  }

  if(info.vrstat) {
    oled.set_cursor(59, 0);
    oled.write('V');
  }

  oled.display();
  
  delay(500);
}
