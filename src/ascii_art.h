#ifndef ASCII_ART_H
#define ASCII_ART_H

#ifdef ASCII_ART_IMPLEMENTATION
#define NOTHING_IMPLEMENTATION
#endif // ASCII_ART_IMPLEMENTATION 
#include "nothing.h"

typedef struct {
    char *symbols;
    float aspect_ratio;
    size_t size;
} ASCII_FONT;

void generate_ascii_art(String_Builder *buffer, int art_quality_level, ASCII_FONT art_font, unsigned char *img_pixels, int img_width, int img_height, int img_channels);

const ASCII_FONT default_ascii_fonts[] = {
    (ASCII_FONT){
        .symbols = " .,:;i1tfLCG08@",
        .aspect_ratio = 0.5,
        .size = 15,
    },
    (ASCII_FONT){
        .symbols = " `.-':_,^=;><+!rc#1#z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$Bg0MNWQ%&@",
        .aspect_ratio = 0.6,
        .size = 93,
    },
    (ASCII_FONT){
        .symbols = " .:-=+*#%@",
        .aspect_ratio = 0.6,
        .size = 10,
    },
};

#ifdef ASCII_ART_IMPLEMENTATION

#define MAX_BRIGHTNESS 255
#define QUALITY_DECREASE 99

#define ARRAY_LEN(xs) sizeof(xs)/sizeof(xs[0])

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} RGBA;

static char get_ascii_symbol(ASCII_FONT font, int brightness) {
    if (brightness <= 0)
        return font.symbols[0];
    if (brightness >= 255)
        return font.symbols[font.size - 1];
    
    size_t index = brightness * (font.size - 1) / 255;
    char symbol = font.symbols[index];
    return symbol;
}

static RGBA get_pixel(int index, unsigned char *data, int channels) {
    switch (channels) {
        case 1: {
            return (RGBA){
                .r = data[index],
                .g = data[index],
                .b = data[index],
                .a = 255,
            };
        }

        case 2: {
            return (RGBA){
                .r = data[index],
                .g = data[index],
                .b = data[index],
                .a = data[index + 1],
            };
        }

        case 3: {
            return (RGBA){
                .r = data[index],
                .g = data[index + 1],
                .b = data[index + 2],
                .a = 255,
            };
        }

        case 4: {
            return (RGBA){
                .r = data[index],
                .g = data[index + 1],
                .b = data[index + 2],
                .a = data[index + 3],
            };
        }
            
        default: return (RGBA){0};
    }
}

static int rgba_to_grayscale(RGBA rgba) {
    float brightness = (float)rgba.r * 0.299f + (float)rgba.g * 0.587f + (float)rgba.b * 0.114f;
    float weight = (float)rgba.a / MAX_BRIGHTNESS;
    int res = (int)(brightness * weight); return res;
}

void generate_ascii_art(String_Builder *buffer, int art_quality_level, ASCII_FONT art_font, unsigned char *img_pixels, int img_width, int img_height, int img_channels) {
    if (art_quality_level < 1)
        art_quality_level = 1;
    
    const float quality = (float)QUALITY_DECREASE / (float)art_quality_level;
    const int pixels_per_symbol = (int)(1 + quality);
    
    int y_step = (int)(pixels_per_symbol / art_font.aspect_ratio);
    
    for (int y = 0; y < img_height; y += y_step) {
        for (int x = 0; x < img_width; x += pixels_per_symbol) {
            int sum = 0, count = 0;
            for (int i = y; i < y + pixels_per_symbol; ++i) {
                for (int j = x; j < x + pixels_per_symbol; ++j) {
                    if (i >= img_height ||  j >= img_width)
                        continue;

                    int index = (i * img_width + j) * img_channels;
                    sum += rgba_to_grayscale(get_pixel(index, img_pixels, img_channels));
                    ++count;
                }
            }

            int avg = 0;
            
            if (count != 0){
                avg = (int)((float)sum / (float)count);
            }
            
            sb_appendc(buffer, get_ascii_symbol(art_font, avg));
        }
        sb_appendc(buffer, '\n');
    }
}

#endif // ASCII_ART_IMPLEMENTATION

#endif //ASCII_ART_H
