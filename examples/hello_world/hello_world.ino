
#include <Arduino.h>
#include <Display_Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#include <display/ui/Screen.h>
#include <display/ui/Label.h>
#include <display/ui/Solid.h>

#include <Font_GFX.h>
#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSans12pt7b.h>

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

    StylePtr blue18pt = Style::create(
        ILI9341_BLUE,
        ILI9341_WHITE,
        Font_GFX::create(&FreeSans18pt7b));

    StylePtr black12pt = Style::create(
        ILI9341_BLACK,
        ILI9341_GREEN,
        Font_GFX::create(&FreeSans12pt7b));

    StylePtr yellow = Style::create(ILI9341_YELLOW);
    Size padding(3, 3);

    auto screen = interface.begin<Screen>(blue18pt);

    screen->addTop<Label>(blue18pt, "Hello, world!", DrawContext::CENTER);
    screen->addTop<Solid>(yellow, padding);

    screen->addBottom<Label>(blue18pt, "left text", DrawContext::LEFT);
    screen->addBottom<Label>(blue18pt, "right text", DrawContext::RIGHT);
    screen->addBottom<Solid>(yellow, padding);

    screen->addFill<Label>(black12pt, "hello_world.ino", DrawContext::CENTER);
}

void loop()
{
    interface.loop();
}
