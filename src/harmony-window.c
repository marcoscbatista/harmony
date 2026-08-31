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

#include <widgets/harmony-wheel.h>

#include "harmony-window.h"
#include "hsla.h"

struct _HarmonyWindow
{
  AdwApplicationWindow parent_instance;

  /* Template widgets */
  GtkLabel *primary_title;
  GtkLabel *color_label;
  GtkColorButton *picker_button;
  GtkBox *main_box;
};

G_DEFINE_FINAL_TYPE (
    HarmonyWindow,
    harmony_window,
    ADW_TYPE_APPLICATION_WINDOW)

static void
harmony_window_class_init (HarmonyWindowClass *klass)
{
  GtkWidgetClass *widget_class =
      GTK_WIDGET_CLASS (klass);

  gtk_widget_class_set_template_from_resource (
      widget_class,
      "/com/marcoscbatista/harmony/harmony-window.ui");

  gtk_widget_class_bind_template_child (
      widget_class,
      HarmonyWindow,
      primary_title);

  gtk_widget_class_bind_template_child (
      widget_class,
      HarmonyWindow,
      color_label);

  gtk_widget_class_bind_template_child (
      widget_class,
      HarmonyWindow,
      picker_button);

  gtk_widget_class_bind_template_child (
      widget_class,
      HarmonyWindow,
      main_box);
}

static void
on_color_changed (GtkColorDialogButton *button,
                  GParamSpec *pspec,
                  gpointer user_data)
{
  const GdkRGBA *rgba =
      gtk_color_dialog_button_get_rgba (button);

  HarmonyWheel *color_wheel =
      HARMONY_WHEEL (user_data);

  HSLA hsla =
      gdk_rgba_to_hsla (*rgba);

  ColorPallete complementary =
      hsla_get_complementary_color (&hsla);

  g_print (
      "HSLA: %.2f, %.2f, %.2f, %.2f\n",
      hsla.h,
      hsla.s,
      hsla.l,
      hsla.a);

  for (guint i = 0;
       i < complementary.count;
       i++)
    {
      g_print (
          "COLOR %u: %.2f, %.2f, %.2f, %.2f\n",
          i,
          complementary.colors[i].h,
          complementary.colors[i].s,
          complementary.colors[i].l,
          complementary.colors[i].a);
    }

  harmony_wheel_set_palette (
      color_wheel,
      complementary);
}

static void
harmony_window_init (HarmonyWindow *self)
{
  gtk_widget_init_template (
      GTK_WIDGET (self));

  HarmonyWheel *wheel =
      harmony_wheel_new ();

  gtk_widget_set_size_request (
      GTK_WIDGET (wheel),
      200,
      200);

  gtk_box_append (
      GTK_BOX (self->main_box),
      GTK_WIDGET (wheel));

  g_signal_connect (
      self->picker_button,
      "notify::rgba",
      G_CALLBACK (on_color_changed),
      wheel);
}

