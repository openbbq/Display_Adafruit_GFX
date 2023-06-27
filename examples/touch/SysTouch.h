#pragma once

#include <Display.h>
#include <XPT2046_Touchscreen.h>

class SysTouch
{
public:
    SysTouch(int8_t CS, int8_t IRQ = 255);

    void configure(display::Size size, TS_Point origin, TS_Point horizontal, TS_Point vertical);

    void connect(display::Touch *touch) { _touch = touch; }

    bool begin();
    bool loop();

private:
    XPT2046_Touchscreen _ts;
    display::Touch *_touch = nullptr;
    bool _down = false;

    float _a11, _a12, _a13, _a21, _a22, _a23;
};
