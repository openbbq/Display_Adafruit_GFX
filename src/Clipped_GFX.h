#pragma once

#include <display/Coords.h>
#include <display/Style.h>

namespace display
{
    class Clipped
    {
    public:
        const Rect &extent() const { return _extent; }
        void extent(const Rect &value) { _extent = value; }

    private:
        Rect _extent;
    };

    template <typename TGFX>
    class Clipped_GFX : public TGFX, public Clipped
    {
    public:
        using TGFX::TGFX; // inherits constructors

        void writePixel(int16_t x, int16_t y, uint16_t color) override
        {
            if (extent().contains(Point(x, y)))
            {
                TGFX::writePixel(x, y, color);
            }
        }

        void writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override
        {
            Rect rc = extent().intersection(x, y, x + w, y + h);
            if (!rc.empty())
            {
                TGFX::writeFillRect(rc.x(), rc.y(), rc.width(), rc.height(), color);
            }
        }

        void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) override
        {
            Rect rc = extent().intersection(x, y, x + w, y + 1);
            if (!rc.empty())
            {
                TGFX::writeFastHLine(rc.x(), rc.y(), rc.width(), color);
            }
        }
        void writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) override
        {
            Rect rc = extent().intersection(x, y, x + 1, y + h);
            if (!rc.empty())
            {
                TGFX::writeFastVLine(rc.x(), rc.y(), rc.height(), color);
            }
        }
    };
}
