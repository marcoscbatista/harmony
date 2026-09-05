#pragma once

#include <gtk/gtk.h>

#include "hsla.h"

#define HARMONY_TYPE_COLOR_WHEEL (harmony_color_wheel_get_type ())

G_DECLARE_FINAL_TYPE (
    HarmonyColorWheel,
    harmony_color_wheel,
    HARMONY,
    COLOR_WHEEL,
    GtkWidget)

HarmonyColorWheel *harmony_color_wheel_new (void);

void
harmony_color_wheel_set_palette (
    HarmonyColorWheel *self,
    ColorPallete palette);

ColorPallete
harmony_color_wheel_get_palette (
    HarmonyColorWheel *self);
