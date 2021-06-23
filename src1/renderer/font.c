#include "font.h"

#include "freetype-gl.h"
#include <string.h>

#include "renderer.h"

struct Font {
    texture_font_t* texture_font;
};

char* preload_range(int start, int end) {
    // TODO: FIX THIS
    char* codepoints = (char*)malloc(end - start + 2);

    for (int i = 0; i <= end - start; i++) {
        codepoints[i] = (char)(i + start);
    }

    codepoints[end - start + 1] = '\0';
    return codepoints;
}

// texture-font.c char* to ui32

texture_glyph_t* tf_find_glyph(texture_font_t* self, char_t* codepoint) {
    size_t i;
    texture_glyph_t *glyph;
    uint32_t ucodepoint = *codepoint;

    for( i = 0; i < self->glyphs->size; ++i )
    {
        glyph = *(texture_glyph_t **) vector_get( self->glyphs, i );
        // If codepoint is -1, we don't care about outline type or thickness
        if( (glyph->codepoint == ucodepoint) &&
            ((ucodepoint == -1) ||
             ((glyph->rendermode == self->rendermode) &&
              (glyph->outline_thickness == self->outline_thickness)) ))
        {
            return glyph;
        }
    }

    return NULL;
}

texture_glyph_t* tf_load_glyph(texture_font_t* self, char_t* codepoint) {
}

texture_glyph_t* get_glyph(struct Font* font, char_t* c) {
    texture_glyph_t *glyph;

    if ((glyph = tf_find_glyph(font->texture_font, c)))
        return glyph;

    // TODO: Load glyph
    // if (tf_load_glyph(font->texture_font, c))
    //     return tf_find_glyph(font->texture_font, c);

    return NULL;
}

// defs

struct Font* font_load_from_file(const char* path, unsigned int size) {
    texture_atlas_t* atlas = texture_atlas_new(1024, 1024, 1);
    texture_font_t* texture_font = texture_font_new_from_file(atlas, size, path);
    struct Font* font = malloc(sizeof(struct Font));

    if (!texture_font) {
        ERROR_M("Unable to load font:", path);
    }

    // TODO: CJK & other preloaded characters support
    char* codepoints = preload_range(32, 126);
    texture_font_load_glyphs(texture_font, codepoints);
    texture_font_load_glyphs(texture_font, "\u25A1"); // White Square (□)

    texture_font->atlas->id = renderer_create_texture();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, texture_font->atlas->width, texture_font->atlas->height, 0, GL_RED, GL_UNSIGNED_BYTE, texture_font->atlas->data);

    font->texture_font = texture_font;

    return font;
}

void font_render_atlas(struct Font* font, float pos_x, float pos_y, float scale) {
    DRAW_TEXTURE(font->texture_font->atlas->id, pos_x, pos_y, font->texture_font->atlas->width * scale, font->texture_font->atlas->height * scale);
}

int font_line_height(struct Font* font) {
    return font->texture_font->height;
}

int font_render_char(struct Font* font, char_t* c, float pos_x, float pos_y, float scale) {
    texture_glyph_t* glyph = get_glyph(font, c);
    
    if (!glyph) {
        return;
    }

    float x0 = glyph->offset_x;
    float y0 = font->texture_font->size - glyph->offset_y;

    x0 *= scale;
    y0 *= scale;

    DRAW_TEXTURE_SOURCE(font->texture_font->atlas->id, pos_x + x0, pos_y + y0, glyph->width * scale, glyph->height * scale, glyph->s0, glyph->t0, glyph->s1, glyph->t1);

    return glyph->advance_x * scale;
}
