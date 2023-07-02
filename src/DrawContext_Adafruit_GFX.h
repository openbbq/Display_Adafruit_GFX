#pragma once

#include <Display.h>
#include <Adafruit_GFX.h>
#include "Clipped_GFX.h"

namespace display
{
    class DrawContext_Adafruit_GFX : public DrawContextBase
    {
    public:
        DrawContext_Adafruit_GFX(Adafruit_GFX *graphics, Clipped *clipped);

        ~DrawContext_Adafruit_GFX()
        {
        }

        // in display coordinates, the current clipped extent
        const Rect &extent() const override { return _clipped->extent(); }
        void extent(const Rect &value) override { _clipped->extent(value); }

        Rect measure(const FontPtr &font, const String &text) const override;

        void draw(const Rect &rc, Color color) override;
        void draw(Point pt0, Point pt1, Color color) override;
        void draw(const RoundRect &rr, Color color) override;
        void draw(const Rect &rc, Color background, Color foreground, const FontPtr &font, const String &text, int alignment) override;

    private:
        Adafruit_GFX *_graphics;
        Clipped *_clipped;
        // Point _origin;
        // Region _region;
    };
}
