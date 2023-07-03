#include <Arduino.h>
#include "Font_GFX.h"

namespace display
{
    Font_GFX::Font_GFX(const GFXfont *glyphs, int size) : _glyphs(glyphs), _size(size)
    {
        if (glyphs)
        {
            int mintop = 0;
            int maxbottom = 0;
            GFXglyph *glyph = glyphs->glyph;
            for (int ch = glyphs->first; ch <= glyphs->last; ++ch, ++glyph)
            {
                int top = glyph->yOffset;
                int bottom = top + glyph->height;
                if (mintop > top)
                {
                    mintop = top;
                }
                if (maxbottom < bottom)
                {
                    maxbottom = bottom;
                }
                if (ch == '0')
                {
                    _cellwidth = glyph->xAdvance;
                }
            }
            _glyphtop = mintop;
            _glyphbottom = maxbottom;
            int glyphheight = _glyphbottom - _glyphtop;
            int cellheight = (glyphs->yAdvance + glyphheight) / 2;
            _celltop = _glyphtop - (cellheight - glyphheight) / 2;
            _cellbottom = _celltop + cellheight;

            _glyphtop *= size;
            _glyphbottom *= size;
            _celltop *= size;
            _cellbottom *= size;
        }
    }

    Rect Font_GFX::measure(const String &text) const
    {
        int xAdvance = 0;
        for (const char *psz = text.c_str(); *psz; ++psz)
        {
            int ch = *psz;
            xAdvance += _glyphs->glyph[ch - _glyphs->first].xAdvance * _size;
        }
        return Rect(0, _celltop, xAdvance, _cellbottom);
    }

    void Font_GFX::draw(DrawContext *ctx, const Rect &clipBox, Color background, Color foreground, int16_t x, int16_t y, const String &text) const
    {
        // TODO - this is only called when DrawContext doesn't recognize the font class kind
        // and the font must must draw itself as pixels
    }

} // namespace display
