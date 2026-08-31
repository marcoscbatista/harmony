#pragma once

#include <gtk/gtk.h>

#include "hsla.h"

#define GNOME_TYPE_COLOR_WHEEL (harmony_wheel_get_type())

G_DECLARE_FINAL_TYPE(
    HarmonyWheel,
    harmony_wheel,
    GNOME,
    COLOR_WHEEL,
    GtkWidget
)

HarmonyWheel *harmony_wheel_new(void);
void harmony_wheel_set_color(HarmonyWheel *self, HSLA color);
