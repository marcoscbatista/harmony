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

  GtkColorDialogButton *picker_button;
  AdwComboRow *harmony_combo_row;
  GtkBox *wheel_container;
  HarmonyWheel *wheel;
  AdwToastOverlay *toast_overlay;
};

G_DEFINE_FINAL_TYPE (
    HarmonyWindow,
    harmony_window,
    ADW_TYPE_APPLICATION_WINDOW)

static void
update_color_wheel_palette (HarmonyWindow *self)
{
  const GdkRGBA *rgba = gtk_color_dialog_button_get_rgba (self->picker_button);
  guint selected_harmony = adw_combo_row_get_selected (self->harmony_combo_row);

  if (rgba == NULL)
    return;

  HSLA hsla = gdk_rgba_to_hsla (*rgba);
  ColorPallete palette;

  switch (selected_harmony)
    {
    case 0:
      palette = hsla_get_complementary_color (&hsla);
      break;
    case 1:
      palette = hsla_get_triade_colors (&hsla);
      break;
    case 2:
      palette = hsla_get_square_colors (&hsla);
      break;
    default:
      g_assert_not_reached ();
    }

  harmony_wheel_set_palette (self->wheel, palette);
}

static void
on_color_changed (GtkColorDialogButton *button,
                  GParamSpec           *pspec,
                  gpointer              user_data)
{
  HarmonyWindow *self = HARMONY_WINDOW (user_data);
  update_color_wheel_palette (self);
}

static void
on_harmony_changed (AdwComboRow *combo_row,
                    GParamSpec  *pspec,
                    gpointer     user_data)
{
  HarmonyWindow *self = HARMONY_WINDOW (user_data);
  update_color_wheel_palette (self);
}

static void
harmony_window_class_init (HarmonyWindowClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  gtk_widget_class_set_template_from_resource (
      widget_class,
      "/com/marcoscbatista/harmony/harmony-window.ui");

  gtk_widget_class_bind_template_child (
      widget_class,
      HarmonyWindow,
      picker_button);

  gtk_widget_class_bind_template_child (
      widget_class,
      HarmonyWindow,
      harmony_combo_row);

  gtk_widget_class_bind_template_child (
      widget_class,
      HarmonyWindow,
      wheel_container);

  gtk_widget_class_bind_template_child (
      widget_class,
      HarmonyWindow,
      toast_overlay);
}

static void
harmony_window_init (HarmonyWindow *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));

  self->wheel = harmony_wheel_new ();
  gtk_widget_set_size_request (GTK_WIDGET (self->wheel), 260, 260);
  gtk_box_append (self->wheel_container, GTK_WIDGET (self->wheel));

  g_signal_connect (
      self->picker_button,
      "notify::rgba",
      G_CALLBACK (on_color_changed),
      self);

  g_signal_connect (
      self->harmony_combo_row,
      "notify::selected",
      G_CALLBACK (on_harmony_changed),
      self);

  update_color_wheel_palette (self);
}

