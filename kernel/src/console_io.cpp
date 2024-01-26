#include "console_io.h"

typedef struct {
    uint32_t magic;
    uint32_t version;
    uint32_t headersize;
    uint32_t flags;
    uint32_t numglyph;
    uint32_t bytesperglyph;
    uint32_t height;
    uint32_t width;
    uint8_t glyphs;
} __attribute__((packed)) psf2_t;
extern volatile unsigned char _binary_font_psf_start;

constexpr int FB_SIZE = 1280 * 720;
constexpr int FB_SCANLINE = 1280;

uint32_t __console_cx, __console_cy;
uint32_t __console_mx, __console_my;
uint32_t __console_fg, __console_bg;
uint32_t __console_tabsize;
uint32_t* __console_fb;
psf2_t* __console_font;
bool __console_wrap;

uint32_t __console_px, __console_py;

void termInit(uint32_t* framebuffer)
{
    __console_fb = framebuffer;
    __console_font = (psf2_t*)(&_binary_font_psf_start);
    __console_tabsize = 4;

    __console_fg = 0xFFFFFFFF;
    __console_bg = 0xFF000000;

    __console_mx = 1280 / __console_font->width;
    __console_my = 720 / __console_font->height;

    __console_wrap = false;
}

void termSetTabSize(uint32_t size)
{
    __console_tabsize = size;
}

void termSetTextWrap(bool wrap)
{
    __console_wrap = wrap;
}

void termClear(uint32_t background)
{
    __console_bg = background;
    for(int i = 0; i < FB_SIZE; i++)
    {
        __console_fb[i] = __console_bg;
    }

    __console_cx = 0;
    __console_cy = 0;
}

void termPush() 
{
    uint32_t height = __console_font->height;
    uint32_t line = 1280 * height;

    uint32_t offset = 0;

    for(uint32_t y = 0; y < 720; y++)
    {
        for(uint32_t x = 0; x < 1280; x++)
        {
            uint32_t sy = y + height;
            
            if(sy >= 720)
            {
                __console_fb[offset + x] = __console_bg;
                continue;
            }

            __console_fb[offset + x] = __console_fb[offset + line + x];
        }

        offset += 1280;
    }
}

void termAutoLine()
{
    if(__console_cy < __console_my)
        return;

    __console_cy--;
    
    termPush();
}

void termWrap()
{
    if(!__console_wrap)
        return;

    if(__console_cx < __console_mx)
        return;

    __console_cx = 0;
    __console_cy++;
        
    termAutoLine();
}

void termColor(uint32_t foreground, uint32_t background)
{
    __console_fg = foreground;
    __console_bg = background;
}

void termWrite(const char* str)
{
    char c;

    while((c = *(str++)) != 0)
        termWrite(c);
}

void termWrite(char c)
{
    switch(c)
    {
        default:
            termWrap();

            // Don't bother displaying anything if it's out of bounds.
            if(__console_cx < __console_mx)
            {
                uint32_t w, h;
                w =  __console_font->width;
                h =  __console_font->height;

                uint32_t x, y;
                x = __console_cx * w;
                y = __console_cy * h;

                uint8_t* glyph;
                glyph = (uint8_t*)(__console_font) + (__console_font->headersize + ((c > 0 && c < __console_font->numglyph) ? c : 0) * __console_font->bytesperglyph);

                uint32_t bpl = (w + 7) / 8;

                for(uint32_t sy = 0; sy < h; sy++)
                {
                    uint32_t offset = (y + sy) * 1280 + x;
                    uint32_t mask = 1 << (w - 1);

                    for(uint32_t sx = 0; sx < w; sx++)
                    {
                        __console_fb[offset] = (*glyph & mask) ? __console_fg : __console_bg;
                        
                        mask >>= 1;
                        offset++;
                    }

                    glyph += bpl;
                }
            }

            __console_cx++;
            break;

        case '\t':
            {
                // Calculate how many spaces need to be inserted to reach the tab barrier.
                uint32_t dif = __console_cx % __console_tabsize;

                // If at a tab barrier, set amount needed to the full length of a tab.
                if(dif == 0)
                    dif = __console_tabsize;

                // Insert the calculated amount of spaces.
                while (dif--)
                {
                    termWrite(' ');

                    // Abort if overflowing to new line.
                    if(__console_cx == 0)
                        break;
                }
            }
            break;

        case '\n':
            __console_cy++;
            __console_cx = 0;

            termAutoLine();

            break;

        case '\r':
            __console_cx = 0;
            break;
    }
}

void termWriteLine()
{
    termWrite('\n');
}

void termWriteLine(const char* str)
{
    termWrite(str);
    termWrite('\n');
}
