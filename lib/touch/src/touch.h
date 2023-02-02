#ifndef TOUCH_H
#define TOUCH_H

#include <cstdint>

#define TOUCH_DEFAULT_DELAY 20
#define TOUCH_MULTIPLIER    0.25
#define TOUCH_NUM_SAMPLES   25

class touch {
    public:
        using length_t = enum {
            SHORT_TAP = 0,
            LONG_TAP,
            LONG_HOLD,
            DOUBLE_TAP
        };

        touch(int32_t, int32_t, float, void(*)(length_t));
        
        void calibrate();
        int32_t update();

    private:
        int32_t _ref = 0, _gnd, _pin;
        int32_t _state = 0, _ticks = 0, _time = 0;
        float _thresh;

        void (*_cb)(length_t);

        int32_t read_raw(int num = TOUCH_NUM_SAMPLES);

};

#endif
