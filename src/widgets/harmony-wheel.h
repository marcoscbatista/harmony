#pragma once

#include <gtk/gtk.h>

#include "hsla.h"

#define HARMONY_TYPE_WHEEL (harmony_wheel_get_type ())

G_DECLARE_FINAL_TYPE (
    HarmonyWheel,
    harmony_wheel,
    HARMONY,
    WHEEL,
    GtkWidget)

HarmonyWheel *harmony_wheel_new (void);

void
harmony_wheel_set_palette (
    HarmonyWheel *self,
    ColorPallete palette);

ColorPallete
harmony_wheel_get_palette (
    HarmonyWheel *self);
