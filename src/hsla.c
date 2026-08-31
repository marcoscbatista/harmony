#include "hsla.h"
#include <math.h>
#include <stdio.h>

static HSLA
shift_hue (HSLA color, float degrees)
{
  color.h += degrees;
  if (color.h >= 360)
    {
      color.h -= 360;
    };
  return color;
}

HSLA
gdk_rgba_to_hsla (GdkRGBA rgba)
{
  float r = (float)rgba.red;
  float g = (float)rgba.green;
  float b = (float)rgba.blue;
  float a = (float)rgba.alpha;

  float max = fmaxf (r, fmaxf (g, b));
  float min = fminf (r, fminf (g, b));
  float h = 0.0f, s = 0.0f, l = (max + min) / 2.0f;

  if (max != min)
    {
      float d = max - min;
      s = l > 0.5f ? d / (2.0f - max - min) : d / (max + min);

      if (max == r)
        {
          h = (g - b) / d + (g < b ? 6.0f : 0.0f);
        }
      else if (max == g)
        {
          h = (b - r) / d + 2.0f;
        }
      else
        {
          h = (r - g) / d + 4.0f;
        }
      h /= 6.0f;
    }
  HSLA hsla = { h * 360.0f, s * 100.0f, l * 100.0f, a };
  return hsla;
}

ColorPallete
hsla_get_complementary_color (HSLA *base_color)
{
  return (ColorPallete) {
    .count = 2,
    .colors = {
        *base_color,
        shift_hue (*base_color, 180.0) }
  };
}

ColorPallete
hsla_get_triade_colors (HSLA *base_color)
{
  {
    return (ColorPallete) {
      .count = 3,
      .colors = {
          *base_color,
          shift_hue (*base_color, 120.0f),
          shift_hue (*base_color, 240.0f)

      }
    };
  }
}

ColorPallete
hsla_get_square_colors (HSLA *base_color)
{
  {
    return (ColorPallete) {
      .count = 4,
      .colors = {
          *base_color,
          shift_hue (*base_color, 90.0f),
          shift_hue (*base_color, 180.0f),
          shift_hue (*base_color, 270.0f),

      }
    };
  }
}

