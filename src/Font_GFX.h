#pragma once

#include <Display.h>
#include <display/Kind.h>
#include <display/Font.h>

#include <gfxfont.h>

namespace display
{
    // A font based on Adafruit GFX library GFXfont structure.
    class Font_GFX : public Font
    {
    public:
        Font_GFX(int size = 1) : _glyphs(nullptr), _size(size) {}
        Font_GFX(const GFXfont *glyphs, int size = 1);

        template <typename... _Args>
        static FontPtr create(_Args &&...__args) { return std::make_shared<Font_GFX>(std::forward<_Args>(__args)...); }

        const GFXfont *glyphs() const { return _glyphs; }
        int size() const { return _size; }

        int kind() const override { return Kind::ID<Font_GFX>(); }
        Rect measure(const String &text) const override;
        void draw(DrawContext *ctx, const Rect &clipBox, Color background, Color foreground, int16_t x, int16_t y, const String &text) const override;

    private:
        const GFXfont *_glyphs;
        int _size = 1;

        int _glyphtop = 0;    // highest set pixel (negative)
        int _glyphbottom = 0; // lowest set pixel +1 (positive)
        int _celltop = 0;     // cell top (includes yAdvance padding)
        int _cellbottom = 0;  // cell bottom (includes yAdvance padding)
        int _cellwidth = 0;   // xAdvance of character '0'
    };
} // namespace display
