/* harmony-window.c
 *
 * Copyright 2026 Marcos Coelho
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "config.h"

#include "harmony-window.h"
#include "hsla.h"

struct _HarmonyWindow
{
  AdwApplicationWindow parent_instance;

  /* Template widgets */
  GtkLabel *primary_title;
  GtkLabel *color_label;
  GtkColorButton *picker_button;
};

G_DEFINE_FINAL_TYPE (HarmonyWindow, harmony_window, ADW_TYPE_APPLICATION_WINDOW)

static void
harmony_window_class_init (HarmonyWindowClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  gtk_widget_class_set_template_from_resource (widget_class, "/com/marcoscbatista/harmony/harmony-window.ui");
  gtk_widget_class_bind_template_child (widget_class, HarmonyWindow, primary_title);
  gtk_widget_class_bind_template_child (widget_class, HarmonyWindow, color_label);
  gtk_widget_class_bind_template_child (widget_class, HarmonyWindow, picker_button);
}

static void
on_color_changed (GtkColorDialogButton *button,
                  GParamSpec *pspec,
                  gpointer user_data)
{
  const GdkRGBA *rgba = gtk_color_dialog_button_get_rgba (button);
  HSLA hsla = gdk_rgba_to_hsla (*rgba);
  ColorPallete complementary = hsla_get_complementary_color (&hsla);
  g_print ("HSLA  : %.2f, %.2f, %.2f, %.2f\n",
           hsla.h,
           hsla.s,
           hsla.l,
           hsla.a);

  g_print ("HSLA  COMPLEMENTARY: %.2f, %.2f, %.2f, %.2f\n",
           complementary.colors[1].h,
           complementary.colors[1].s,
           complementary.colors[1].l,
           complementary.colors[1].a);
  // gtk_label_set_label (GtkLabel *self, const char *str)
}

static void
harmony_window_init (HarmonyWindow *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));
  g_signal_connect (self->picker_button, "notify::rgba", G_CALLBACK (on_color_changed), NULL);
}

