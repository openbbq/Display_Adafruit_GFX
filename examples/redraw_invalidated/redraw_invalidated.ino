
#include <Arduino.h>
#include <Display_Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSansBoldOblique24pt7b.h>

#include <display/ui/Screen.h>
#include <display/ui/Label.h>
#include <display/ui/Background.h>

using namespace display;
using namespace display::ui;

#define DISPLAY_CS 33
#define DISPLAY_RST 27
#define DISPLAY_DC 12

Clipped_GFX<Adafruit_ILI9341> tft(DISPLAY_CS, DISPLAY_DC, DISPLAY_RST);

Interface_Adafruit_GFX interface(tft);

WindowPtr box1;
WindowPtr box2;

WindowPtr addBox(WindowPtr w, StylePtr b, StylePtr s)
{
    Size sz = s->font()->measure("00000").size();

    WindowPtr box = w->add<Background>(b);
    box->position(Rect(Point(0, 0), sz + Size(4, 4)));

    WindowPtr text = box->add<Label>(s, "xxx", DrawContext::CENTER);
    text->position(Rect(Point(2, 2), sz));

    return box;
}

void setup()
{
    Serial.begin(115200);
    tft.begin();

    StylePtr cyan = Style::create(ILI9341_CYAN);
    StylePtr red = Style::create(ILI9341_RED);
    StylePtr black12pt = Style::create(ILI9341_BLACK, ILI9341_GREEN, Font::create(&FreeSans12pt7b));
    StylePtr black24pt = Style::create(ILI9341_BLACK, ILI9341_BLUE, Font::create(&FreeSansBoldOblique24pt7b));
    StylePtr yellow12pt = Style::create(ILI9341_YELLOW, ILI9341_BLACK, Font::create(&FreeSans12pt7b));

    auto screen = interface.begin<Screen>(cyan);

    // this window draws under moving boxes
    screen->add<Label>(yellow12pt, "below", DrawContext::CENTER)->position(Rect(30, 120, 210, 160));

    // these windows are moved in loop, which invalidates their interface positions
    // the invalidation causes only those old and new extents to be drawn
    box1 = addBox(screen, red, black12pt);
    box2 = addBox(screen, red, black24pt);

    // this window draws over moving boxes
    screen->add<Label>(yellow12pt, "above", DrawContext::CENTER)->position(Rect(30, 50, 210, 90));
}

void move(WindowPtr ptr)
{
    int m = millis() * 60 / 1000;
    Size sz = (interface.size() - ptr->size()) * 2;
    int x = m % sz.width();
    int y = m % sz.height();
    if (x > sz.width() / 2)
    {
        x = sz.width() - x - 1;
    }
    if (y > sz.height() / 2)
    {
        y = sz.height() - y - 1;
    }
    ptr->position(Rect(Point(x, y), ptr->size()));
}

void loop()
{
    // tft.fillScreen(ILI9341_MAGENTA);

    move(box1);
    move(box2);

    interface.loop();
}
