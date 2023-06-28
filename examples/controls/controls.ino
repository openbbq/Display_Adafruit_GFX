
#include <Arduino.h>
#include <Adafruit_ILI9341.h>

#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSans12pt7b.h>

#include <Display_Adafruit_GFX.h>
#include <Display_XPT2046_Touchscreen.h>

#include <display/ui/Screen.h>
#include <display/ui/CheckBox.h>

using namespace display;
using namespace display::ui;

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
    tft.begin();
    ts.begin();

    auto touch = interface.device<Display_XPT2046_Touchscreen>(ts);

    touch->calibration(
        TS_Point(4050, 200, 0),  // (0,0)
        TS_Point(4050, 3900, 0), // (240,0)
        TS_Point(580, 200, 0));  // (0,320)

    StylePtr normal = Style::create(ILI9341_WHITE, ILI9341_BLACK, Font::create(&FreeSans18pt7b));
    StylePtr toggled = Style::create(ILI9341_WHITE, ILI9341_BLUE);
    StylePtr pressed = Style::create(ILI9341_LIGHTGREY, ILI9341_BLACK);

    auto screen = interface.begin<Screen>(normal);

    auto chkSample = screen->add<CheckBox>(normal, pressed, toggled);
    chkSample->position(Rect(Point(40, 40), Size(48, 48)));
}

void loop()
{
    interface.loop();
}
