#pragma once

#include <Display.h>
#include <Adafruit_GFX.h>
#include "Clipped.h"

namespace display
{
    class Interface_Adafruit_GFX : public BaseInterface
    {
    public:
        Interface_Adafruit_GFX(Adafruit_GFX &graphics, Clipped &clipped) : _graphics(&graphics), _clipped(&clipped) {}

        template <typename TAdafruitGFX>
        Interface_Adafruit_GFX(TAdafruitGFX &graphics) : Interface_Adafruit_GFX(graphics, graphics) {}

        Size size() const override;
        Rect measure(const FontPtr &font, const String &text) const override;

        std::unique_ptr<DrawContext> createDrawContext() override;

    private:
        Adafruit_GFX *_graphics;
        Clipped *_clipped;
    };
}
