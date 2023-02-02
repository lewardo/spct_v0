#include "Arduino.h"
#include "touch.h"

#define CURRENT_TOUCH_BIT 0
#define PREV_TOUCH_BIT    1
#define ONE_TAP_BIT 2

touch::touch(int32_t pin, int32_t gnd, float thresh, void(*cb)(touch::length_t)):
    _pin(pin),
    _gnd(gnd),
    _thresh(thresh),
    _cb(cb) {};


void touch::calibrate() {
    pinMode(_gnd, INPUT_ANALOG);
    pinMode(_pin, INPUT_ANALOG);

    digitalWrite(_gnd, 0);

    delay(250);

    _ref = 0;

    for(int i = 0; i < 50; ++i) {
        delay(5);
        _ref += read_raw() * 0.02;
    }
}

int32_t touch::update() {
    float max = 0, min = read_raw();

    for(int i = 0; i < 15; ++i) {
        float val = read_raw();
        if(val > max) max = val;
        if(val < min) min = val;
        delay(5);
    }

    _state &= ~(1 << CURRENT_TOUCH_BIT);
    _state |= (max - min > _thresh) << CURRENT_TOUCH_BIT;

    if(millis() - _time > 200 && _state & (1 << ONE_TAP_BIT)) {
        _state &= ~(1 << ONE_TAP_BIT);

        if(_ticks < 350) _cb(SHORT_TAP);
        else if (_ticks < 1500) _cb(LONG_TAP);
        else _cb(LONG_HOLD);
    }

    switch(_state & 0b11) {
        case 0b01:  
            _ticks = millis();
            
            break;

        case 0b10:
            if(_state & (1 << ONE_TAP_BIT)) {
                _state &= ~(1 << ONE_TAP_BIT);

                _cb(DOUBLE_TAP);
            } else {
                _time = millis();
                _ticks = _time - _ticks;

                _state |=(1 << ONE_TAP_BIT);
            }

            break;
    }

    _state &= ~(1 << PREV_TOUCH_BIT);
    _state |= !!(_state & (1 << CURRENT_TOUCH_BIT)) << PREV_TOUCH_BIT;

    return max - min;
}

int32_t touch::read_raw(int num) {
    int32_t total = 0;

    while (num-- > 0) {
        delayMicroseconds(TOUCH_DEFAULT_DELAY);
        total += analogRead(_pin);
    }

    return total / TOUCH_NUM_SAMPLES;
}