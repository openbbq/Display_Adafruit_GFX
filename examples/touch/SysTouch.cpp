#include <Arduino.h>
#include "SysTouch.h"

using namespace display;

SysTouch::SysTouch(int8_t CS, int8_t IRQ) : _ts(CS, IRQ)
{
}

// https://www.wolframalpha.com/input?i2d=true&i=inv%5C%2840%29%7B%7B1%2C0%2Co%5C%2840%29x%5C%2841%29%7D%2C%7B0%2C1%2Co%5C%2840%29y%5C%2841%29%7D%2C%7B0%2C0%2C1%7D%7D.%7B%7Bh%5C%2840%29x%5C%2841%29%2Cv%5C%2840%29x%5C%2841%29%2C0%7D%2C%7Bh%5C%2840%29y%5C%2841%29%2Cv%5C%2840%29y%5C%2841%29%2C0%7D%2C%7B0%2C0%2C1%7D%7D.%7B%7BDivide%5B1%2Cs%5C%2840%29x%5C%2841%29%5D%2C0%2C0%7D%2C%7B0%2CDivide%5B1%2Cs%5C%2840%29y%5C%2841%29%5D%2C0%7D%2C%7B0%2C0%2C1%7D%7D%5C%2841%29

void SysTouch::configure(Size size, TS_Point o, TS_Point h, TS_Point v)
{
    float sx = size.width();
    float sy = size.height();

    // horizontal and vertical vector are relative to origin
    float ox = o.x;
    float oy = o.y;
    float hx = h.x - o.x;
    float hy = h.y - o.y;
    float vx = v.x - o.x;
    float vy = v.y - o.y;

    // Inverse[{{1, 0, o[x]}, {0, 1, o[y]}, {0, 0, 1}} . {{h[x], v[x], 0}, {h[y], v[y], 0}, {0, 0, 1}} . {{1/s[x], 0, 0}, {0, 1/s[y], 0}, {0, 0, 1}}]

    float hxvy_hyvx = hx * vy - hy * vx;
    float hyvx_hxvy = hy * vx - hx * vy;
    float oxvy_oyvx = ox * vy - oy * vx;
    float oyhx_oxhy = oy * hy - ox * hy;

    _a11 = sx * vy / hxvy_hyvx;
    _a12 = sx * vx / hyvx_hxvy;
    _a13 = sx * oxvy_oyvx / hyvx_hxvy;
    _a21 = sy * hy / hyvx_hxvy;
    _a22 = sy * hx / hxvy_hyvx;
    _a23 = sy * oyhx_oxhy / hyvx_hxvy;
}

bool SysTouch::begin()
{
    return _ts.begin();
}

bool SysTouch::loop()
{
    if (!_touch)
    {
        return false;
    }

    TS_Point input = _ts.getPoint();

    float x = _a11 * input.x + _a12 * input.y + _a13;
    float y = _a21 * input.x + _a22 * input.y + _a23;

    Point pt(x, y);
    if (!_down)
    {
        if (input.z > 400)
        {
            log_d("touchDown raw:{%d,%d} point:{%d,%d}", input.x, input.y, pt.x(), pt.y());
            _down = true;
            _touch->touchDown(pt);
        }
    }
    if (_down)
    {
        log_d("touchMove raw:{%d,%d} point:{%d,%d}", input.x, input.y, pt.x(), pt.y());
        _touch->touchMove(pt);
        if (input.z < 250)
        {
            _down = false;
            log_d("touchUp raw:{%d,%d} point:{%d,%d}", input.x, input.y, pt.x(), pt.y());
            _touch->touchUp(pt);
        }
    }
    return true;
}
