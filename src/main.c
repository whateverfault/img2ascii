#include <stdio.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "stb_image.h"

#define ASCII_ART_IMPLEMENTATION
#include "ascii_art.h"

#define DEFAULT_QUALITY 100
#define DEFAULT_ASCII_SET 0

void usage(char** argv) {
    fprintf(stderr, "Usage: %s <image_path> [--quality <1..100>] [--ascii-set <1..8>]", argv[0]);
    getchar();
}

int parse_int_arg(int *arg, char *name, int argc, char** argv) {
    for (size_t i = 0; i < argc; ++i) {
        if (strcmp(argv[i], name) != 0)
            continue;

        if (i >= argc) {
            usage(argv);
            return -1;
        }

        char *val = argv[++i];
        char *end;
        *arg = strtol(val, &end, 10);

        if (end == val) {
            usage(argv);
            return -1;
        }
        
        return 1;
    }

    return 0;
}

void print_ascii_art(const String_Builder *sb) {
    for (size_t i = 0; i < sb->count; ++i) {
        printf("%c", sb->items[i]);
    }

    printf("\n");
}

int main(int argc, char** argv) {
    if (argc < 2) {
        usage(argv);
        return 1;
    }

    int quality_level = DEFAULT_QUALITY;
    int ascii_set = DEFAULT_ASCII_SET;

    if (parse_int_arg(&quality_level, "--quality", argc, argv) < 0) {
        return 1;
    }
    
    if (parse_int_arg(&ascii_set, "--ascii-set", argc, argv) < 0) {
        return 1;
    }

    --ascii_set;
    
    if (quality_level < 1) quality_level = 1;
    if (quality_level > 100) quality_level = 100;

    if (ascii_set < 0) ascii_set = 0;
    if (ascii_set >= 3) ascii_set = 2;
    
    int width, height, channels;
    char *filename = argv[1];
    
    unsigned char *pixels = stbi_load(filename, &width, &height, &channels, STBI_default);
    if (pixels == NULL) {
        fprintf(stderr, stbi_failure_reason());
	getchar();
        return 1;
    }

    String_Builder sb = {0};
    
    generate_ascii_art(&sb, quality_level, default_ascii_fonts[ascii_set], pixels, width, height, channels);
    print_ascii_art(&sb);
    getchar();
    
    da_free(sb);
    stbi_image_free(pixels);
    return 0;
}