#include <Arduino.h>
#include "DrawContext_Adafruit_GFX.h"

namespace display
{
    DrawContext_Adafruit_GFX::DrawContext_Adafruit_GFX(Adafruit_GFX *graphics, Clipped *clipped)
        : _graphics(graphics), _clipped(clipped)
    {
    }

    void DrawContext_Adafruit_GFX::draw(const Rect &rc, Color color)
    {
        log_v("rc:%s color:%d", rc.toString().c_str(), color);

        Rect shown = rc.offset(origin()).intersection(extent());
        // Serial.printf("shown %s\n", shown.toString().c_str());
        if (shown.empty())
        {
            return;
        }

        for (const Rect &included : _region)
        {
            Rect clip = included.intersection(shown);
            if (clip.empty())
            {
                continue;
            }

            log_v("clip:%s %d", clip.toString().c_str());
            _graphics->fillRect(clip.x(), clip.y(), clip.width(), clip.height(), color);
        }
    }

    void DrawContext_Adafruit_GFX::draw(const RoundRect &rc, Color color)
    {
        log_v("rc:%s radius:%d, color:%d", rc.toString().c_str(), rc.radius(), color);

        Point o = origin();
        Rect shown = rc.offset(o).intersection(extent());
        // Serial.printf("shown %s\n", shown.toString().c_str());
        if (shown.empty())
        {
            return;
        }

        Rect prior = extent();
        for (const Rect &included : _region)
        {
            Rect clip = included.intersection(shown);
            if (clip.empty())
            {
                continue;
            }

            extent(clip);
            _graphics->fillRoundRect(rc.x() + o.x(), rc.y() + o.y(), rc.width(), rc.height(), rc.radius(), color);
        }
        extent(prior);
    }

    void DrawContext_Adafruit_GFX::draw(Point pt0, Point pt1, Color color)
    {
        Point o = origin();
        int16_t x0 = o.x() + pt0.x();
        int16_t y0 = o.y() + pt0.y();
        int16_t x1 = o.x() + pt1.x();
        int16_t y1 = o.y() + pt1.y();
        int16_t left = x0;
        int16_t top = y0;
        int16_t right = x1 + 1;
        int16_t bottom = y1 + 1;
        if (left >= right)
        {
            left = x1;
            right = x0 + 1;
        }
        if (top >= bottom)
        {
            top = y1;
            bottom = y0 + 1;
        }
        Rect shown = extent().intersection(left, top, right, bottom);

        log_v("pt0:{%d,%d} pt1:{%d,%d} shown:%s",
              pt0.x(), pt0.y(), pt1.x(), pt1.y(),
              shown.toString().c_str());

        if (shown.empty())
        {
            return;
        }

        Rect prior = extent();
        for (const Rect &included : _region)
        {
            Rect clip = included.intersection(shown);
            if (clip.empty())
            {
                continue;
            }

            extent(clip);
            _graphics->drawLine(x0, y0, x1, y1, color);
        }
        extent(prior);
    }

    Rect DrawContext_Adafruit_GFX::measure(const FontPtr &font, const String &text) const
    {
        if (font)
        {
            return font->measure(text);
        }
        // if (font)
        // {
        //     _graphics->setFont(font->glyphs());
        //     _graphics->setTextSize(font->size());
        // }
        // else
        // {
        //     _graphics->setFont(nullptr);
        //     _graphics->setTextSize(1);
        // }

        // int16_t x, y;
        // uint16_t w, h;
        // _graphics->getTextBounds(text.c_str(), 0, 0, &x, &y, &w, &h);
        // if (font)
        // {
        //     Rect cell = font->cell();
        //     y = cell.top();
        //     h = cell.height();
        // }
        // return Rect(Point(x, y), Size(w, h));
    }

    void DrawContext_Adafruit_GFX::draw(const Rect &rc, Color background, Color foreground, const FontPtr &font, const String &text, int alignment)
    {
        Rect frame = rc.offset(origin());
        Rect shown = frame.intersection(extent());
        if (shown.empty())
        {
            return;
        }

        if (font)
        {
            _graphics->setFont(font->glyphs());
            _graphics->setTextSize(font->size());
        }
        else
        {
            _graphics->setFont(nullptr);
            _graphics->setTextSize(1);
        }

        // int16_t x, y;
        // uint16_t w, h;
        // _graphics->getTextBounds(text.c_str(), 0, 0, &x, &y, &w, &h);

        Rect rcText = font->measure(text);

        int16_t ox = frame.left() - rcText.left();
        int16_t oy = frame.top() - rcText.top();

        if (alignment & DrawContext::HORIZONTAL_RIGHT)
        {
            ox += frame.width() - rcText.width();
        }
        else if (alignment & DrawContext::HORIZONTAL_CENTER)
        {
            ox += (frame.width() - rcText.width()) / 2;
        }
        if (alignment & DrawContext::VERTICAL_BOTTOM)
        {
            oy += frame.height() - rcText.height();
        }
        else if (alignment & DrawContext::VERTICAL_CENTER)
        {
            oy += (frame.height() - rcText.height()) / 2;
        }

        Rect prior = extent();
        for (const Rect &included : _region)
        {
            Rect clip = included.intersection(shown);
            if (clip.empty())
            {
                continue;
            }

            extent(clip);
            _graphics->fillRect(clip.x(), clip.y(), clip.width(), clip.height(), background);
            _graphics->setTextColor(foreground, background);
            _graphics->setCursor(ox, oy);
            _graphics->print(text);
        }
        extent(prior);
    }
}
