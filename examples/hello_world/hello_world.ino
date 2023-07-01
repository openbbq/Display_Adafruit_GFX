
#include <Arduino.h>
#include <Display_Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#include <Display_Outline_Render.h>

#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSans12pt7b.h>

#include <display/ui/Screen.h>
#include <display/ui/Label.h>
#include <display/ui/Solid.h>

using namespace display;
using namespace display::ui;

#define DISPLAY_CS 33
#define DISPLAY_RST 27
#define DISPLAY_DC 12

Clipped_GFX<Adafruit_ILI9341> tft(DISPLAY_CS, DISPLAY_DC, DISPLAY_RST);

Interface_Adafruit_GFX interface(tft);

#define STANDALONE_
#define FT_CONFIG_STANDARD_LIBRARY_H <stdlib.h>
#include <ftmisc.h>
#include <ftimage.h>
extern const FT_Raster_Funcs ft_standard_raster;
extern const FT_Raster_Funcs ft_grays_raster;

#include "example.h"

void setup()
{
    // FT_Library library;
    // FT_Error error = FT_Init_FreeType(&library);

    Serial.begin(115200);
    tft.begin();

    StylePtr blue18pt = Style::create(
        ILI9341_BLUE,
        ILI9341_WHITE,
        Font::create(&FreeSans18pt7b));

    StylePtr black12pt = Style::create(
        ILI9341_BLACK,
        ILI9341_GREEN,
        Font::create(&FreeSans12pt7b));

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

void *ft_alloc(FT_Memory memory, long size)
{
    log_v("size:%d", size);
    void *block = malloc(size);
    log_v("block:%d", block);
    return block;
}

void *ft_realloc(FT_Memory memory, long cur_size, long new_size, void *block)
{
    log_v("cur_size:%d new_size:%d block:%d", cur_size, new_size, block);
    return realloc(block, new_size);
}

void ft_free(FT_Memory memory, void *block)
{
    log_v("block:%d", block);
    free(block);
}

FT_MemoryRec_ ft_memory = {nullptr, ft_alloc, ft_free, ft_realloc};

typedef uint16_t U16;
#define Swap16(u16) ((U16)(((U16)(u16) >> 8) | \
                           ((U16)(u16) << 8)))

#define ILI9341_COLOR(r, g, b) ((((uint16_t)b) >> 3) |            \
                                ((((uint16_t)g) << 3) & 0x07E0) | \
                                ((((uint16_t)r) << 8) & 0xf800))

void span_writer(int y,
                 int count,
                 const FT_Span *spans,
                 void *user)
{
    Point *pt = (Point *)user;
    log_i("y:%d spans:%d", y, count);

    for (int i = 0; i != count; ++i)
    {
        log_i("x:%d w:%d", spans[i].x, spans[i].len);
        tft.drawFastHLine(
            pt->x() + spans[i].x, pt->y() - y,
            spans[i].len,
            ILI9341_COLOR(spans[i].coverage, spans[i].coverage, spans[i].coverage));
    }
}

void loop()
{
    interface.loop();

    FT_Raster raster;
    FT_Error error = ft_grays_raster.raster_new(&ft_memory, &raster);
    log_i("raster_new: %d", error);

    Point pt;
    FT_Raster_Params params = {0};
    params.user = &pt;
    params.flags = FT_RASTER_FLAG_AA | FT_RASTER_FLAG_DIRECT;
    params.gray_spans = span_writer;
    params.clip_box.xMin = 0;
    params.clip_box.yMin = 0;
    params.clip_box.xMax = interface.size().width();
    params.clip_box.yMax = interface.size().height();

    pt = Point(50, 80);
    params.source = &cp_967_e834;
    error = ft_grays_raster.raster_render(raster, &params);
    log_i("raster_render: %d", error);

    pt = Point(90, 80);
    params.source = &cp_968_e835;
    error = ft_grays_raster.raster_render(raster, &params);
    log_i("raster_render: %d", error);

    pt = Point(50, 120);
    params.source = &cp_1337_e9f6;
    error = ft_grays_raster.raster_render(raster, &params);
    log_i("raster_render: %d", error);

    pt = Point(150, 120);
    params.source = &cp_1336_e9f5;
    error = ft_grays_raster.raster_render(raster, &params);
    log_i("raster_render: %d", error);

    ft_grays_raster.raster_done(raster);
}
