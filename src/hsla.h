#ifndef HSLA_H
#define HSLA_H

#include <gtk/gtk.h>

#define COLOR_PALLETE_MAX 8



typedef struct {
  float h;
  float s;
  float l;
  float a;
} HSLA;

typedef struct {
  HSLA colors[COLOR_PALLETE_MAX];
  guint count;
} ColorPallete;
HSLA gdk_rgba_to_hsla (GdkRGBA rgba);

ColorPallete hsla_get_complementary_color (HSLA *base_color);

#endif
