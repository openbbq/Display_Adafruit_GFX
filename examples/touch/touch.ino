
#include <Arduino.h>
#include <Adafruit_ILI9341.h>
#include <Display_Adafruit_GFX.h>
#include <Display_XPT2046_Touchscreen.h>

#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSansBoldOblique24pt7b.h>

#include <Display.h>
#include <display/ui/Screen.h>
#include <display/ui/Solid.h>

using namespace display;
using namespace display::ui;
#include "Painting.h"

#define DISPLAY_CS 33
#define DISPLAY_RST 27
#define DISPLAY_DC 12
#define TOUCH_CS 13

Clipped_GFX<Adafruit_ILI9341> tft(DISPLAY_CS, DISPLAY_DC, DISPLAY_RST);
XPT2046_Touchscreen ts(TOUCH_CS);

Interface_Adafruit_GFX interface(tft);

void setup()
{
    Serial.begin(115200);
    tft.begin(40000000);
    ts.begin();

    auto touch = interface.device<Display_XPT2046_Touchscreen>(ts);

    touch->calibration(
        TS_Point(4050, 200, 0),  // (0,0)
        TS_Point(4050, 3900, 0), // (240,0)
        TS_Point(580, 200, 0));  // (0,320)

    StylePtr cyan = Style::create(ILI9341_CYAN);
    StylePtr blackGreen = Style::create(ILI9341_BLACK, ILI9341_GREEN);
    StylePtr blackRed = Style::create(ILI9341_BLACK, ILI9341_RED);

    auto screen = interface.begin<Screen>(cyan);

    Rect rc = screen->content();
    int16_t half = rc.height() / 2;
    screen->add<Painting>(blackGreen)->position(rc.indent(4, 4, 4, 2 + half));
    screen->add<Painting>(blackRed)->position(rc.indent(4, 2 + half, 4, 4));
}

void loop()
{
    interface.loop();
}
