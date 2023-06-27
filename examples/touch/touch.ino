
#include <Arduino.h>
#include <Display_Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSansBoldOblique24pt7b.h>

#include <Display.h>
#include <display/ui/Screen.h>
#include <display/ui/Solid.h>

#include "SysTouch.h"

using namespace display;
using namespace display::ui;

#define DISPLAY_CS 33
#define DISPLAY_RST 27
#define DISPLAY_DC 12
#define TOUCH_CS 12

Clipped_GFX<Adafruit_ILI9341> tft(DISPLAY_CS, DISPLAY_DC, DISPLAY_RST);
SysTouch touch(TOUCH_CS);

Interface_Adafruit_GFX interface(tft);

class Painting : public Solid
{
public:
    using Solid::Solid; // inherit constructors

    bool touchDownHandler(TouchContext *ctx) override
    {
        if (ctx->containsCurrent())
        {
            log_i("starting line");
            _lines.push_back(std::vector<Point>());
            _prior = ctx->current();
            touchCapture(true);
            return true;
        }
        return false;
    }

    bool touchMoveHandler(TouchContext *ctx) override
    {
        if (touchCapture())
        {
            if (ctx->containsCurrent())
            {
                Point pt = ctx->current();
                log_i("adding point {%d,%d}", pt.x(), pt.y());
                _lines.rbegin()->push_back(pt);
                invalidate(Rect(
                    _min(pt.x(), _prior.x()),
                    _min(pt.y(), _prior.y()),
                    _max(pt.x(), _prior.x()) + 1,
                    _max(pt.y(), _prior.y()) + 1));
                _prior = ctx->current();
                return true;
            }
            touchRelease();
        }
        return false;
    }

    bool touchUpHandler(TouchContext *ctx) override
    {
        if (touchCapture())
        {
            touchRelease();
            return true;
        }
        return false;
    }

    void drawHandler(DrawContext *ctx) override
    {
        Rect rc = content();
        ctx->draw(rc, style()->background());
        for (const auto &line : _lines)
        {
            bool first = true;
            Point prior;
            for (Point pt : line)
            {
                if (first)
                {
                    first = false;
                }
                else
                {
                    ctx->draw(prior, pt, style()->foreground());
                }
                prior = pt;
            }
        }
        ctx->exclude(rc);
    }

private:
    std::vector<std::vector<Point>> _lines;
    Point _prior;
};

void setup()
{
    Serial.begin(115200);

    touch.connect(&interface);
    touch.configure(
        interface.size(),
        TS_Point(4050, 200, 0),  // (0,0)
        TS_Point(4050, 3900, 0), // (240,0)
        TS_Point(580, 200, 0));  // (0,320)

    tft.begin(40000000);

    touch.begin();

    StylePtr cyan = Style::create(ILI9341_CYAN);
    StylePtr blackGreen = Style::create(ILI9341_BLACK, ILI9341_GREEN);
    StylePtr blackRed = Style::create(ILI9341_BLACK, ILI9341_RED);

    auto screen = interface.begin<Screen>(cyan);

    Rect rc = screen->content();
    int16_t half = rc.height() / 2;
    screen->add<Painting>(blackGreen)->position(rc.indent(3, 3, 3, 3 + half));
    screen->add<Painting>(blackRed)->position(rc.indent(3, 3 + half, 3, 3));
}

void loop()
{
    log_d("---");
    touch.loop();
    interface.loop();
}
