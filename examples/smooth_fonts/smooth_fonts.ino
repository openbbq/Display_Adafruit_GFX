
#include <Arduino.h>
#include <Display_Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#include <Display_FreeTypeOutline.h>
#include <Outlines/Roboto-Light-12.h>
#include <Outlines/Roboto-Light-18.h>

#include <Font_GFX.h>
#include <Fonts/FreeSans18pt7b.h>

#include <display/ui/Screen.h>
#include <display/ui/Label.h>
#include <display/ui/Solid.h>
#include <display/Font_FTO.h>

using namespace display;
using namespace display::ui;

#define DISPLAY_CS 33
#define DISPLAY_RST 27
#define DISPLAY_DC 12

Clipped_GFX<Adafruit_ILI9341> tft(DISPLAY_CS, DISPLAY_DC, DISPLAY_RST);

Interface_Adafruit_GFX interface(tft);

void setup()
{
    Serial.begin(115200);
    tft.begin();

    StylePtr blue18pt1 = Style::create(
        ILI9341_BLUE,
        ILI9341_WHITE,
        Font_FTO::create(&roboto::light18::font, 18));

    StylePtr blue18pt2 = Style::create(
        ILI9341_BLUE,
        ILI9341_WHITE,
        Font_GFX::create(&FreeSans18pt7b));

    StylePtr black12pt = Style::create(
        ILI9341_BLACK,
        ILI9341_GREEN,
        Font_FTO::create(&roboto::light12::font, 12));

    StylePtr yellow = Style::create(ILI9341_YELLOW);
    Size padding(3, 3);

    auto screen = interface.begin<Screen>(blue18pt1);

    screen->addTop<Label>(blue18pt1, "Hello, world!", DrawContext::CENTER);
    screen->addTop<Label>(blue18pt2, "Hello, world!", DrawContext::CENTER);
    screen->addTop<Solid>(yellow, padding);

    screen->addBottom<Label>(blue18pt1, "left text", DrawContext::LEFT);
    screen->addBottom<Label>(blue18pt2, "left text", DrawContext::LEFT);
    screen->addBottom<Label>(blue18pt1, "right text", DrawContext::RIGHT);
    screen->addBottom<Label>(blue18pt2, "right text", DrawContext::RIGHT);
    screen->addBottom<Solid>(yellow, padding);

    screen->addFill<Label>(black12pt, "hello_world.ino", DrawContext::CENTER);
}

void loop()
{
    interface.loop();
}
