#include <Arduino.h>
#include "Interface_Adafruit_GFX.h"
#include "DrawContext_Adafruit_GFX.h"

namespace display
{
    Size Interface_Adafruit_GFX::size() const
    {
        return Size(_graphics->width(), _graphics->height());
    }

    Rect Interface_Adafruit_GFX::measure(const FontPtr &font, const String &text) const
    {
        return font->measure(text);
    }

    std::unique_ptr<DrawContext> Interface_Adafruit_GFX::createDrawContext()
    {
        return std::unique_ptr<DrawContext>(new DrawContext_Adafruit_GFX(_graphics, _clipped));
    }
}
