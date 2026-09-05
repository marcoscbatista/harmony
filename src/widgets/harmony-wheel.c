#include "harmony-wheel.h"
#include <gtk/gtk.h>
#include <math.h>

enum
{
  PROP_PALETTE = 1,
  N_PROPERTIES
};

static GParamSpec *obj_properties[N_PROPERTIES] = {
  NULL,
};

struct _HarmonyWheel
{
  GtkWidget parent_instance;

  ColorPallete palette;
};

G_DEFINE_FINAL_TYPE (
    HarmonyWheel,
    harmony_wheel,
    GTK_TYPE_WIDGET)

static void
harmony_wheel_set_property (GObject *object,
                                guint prop_id,
                                const GValue *value,
                                GParamSpec *pspec)
{
  HarmonyWheel *self = HARMONY_WHEEL (object);

  switch (prop_id)
    {
    case PROP_PALETTE:
      {
        ColorPallete *palette_received =
            g_value_get_pointer (value);

        if (palette_received != NULL)
          {
            self->palette = *palette_received;
          }

        break;
      }

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (
          object,
          prop_id,
          pspec);

      break;
    }
}

static double
wrap01 (double value)
{
  value = fmod (value, 1.0);

  if (value < 0)
    return value + 1.0;

  return value;
}

static double
find_color (double t,
            double p,
            double q)
{
  if (t < 0.0)
    t += 1.0;

  if (t > 1.0)
    t -= 1.0;

  if (t < 1.0 / 6.0)
    {
      return p + (q - p) * 6.0 * t;
    }

  if (t < 1.0 / 2.0)
    {
      return q;
    }

  if (t <= 2.0 / 3.0)
    {
      return p + (q - p) * (2.0 / 3.0 - t) * 6.0;
    }

  return p;
}

static GdkRGBA
hue_to_gdk_rgba (HSLA hsla_color)
{
  double new_h = hsla_color.h / 360.0;
  double new_s = hsla_color.s / 100.0;
  double new_l = hsla_color.l / 100.0;

  double q =
      new_l < 0.5
          ? new_l * (1.0 + new_s)
          : new_l + new_s - (new_l * new_s);

  double p = 2.0 * new_l - q;

  double r = wrap01 (new_h + 1.0 / 3.0);
  double g = wrap01 (new_h);
  double b = wrap01 (new_h - 1.0 / 3.0);

  r = find_color (r, p, q);
  g = find_color (g, p, q);
  b = find_color (b, p, q);

  return (GdkRGBA) {
    r,
    g,
    b,
    hsla_color.a
  };
}

static void
harmony_wheel_snapshot (GtkWidget *widget,
                            GtkSnapshot *snapshot)
{
  HarmonyWheel *self =
      HARMONY_WHEEL (widget);

  int width =
      gtk_widget_get_width (widget);

  int height =
      gtk_widget_get_height (widget);

  graphene_rect_t bounds =
      GRAPHENE_RECT_INIT (
          0,
          0,
          width,
          height);

  cairo_t *cr =
      gtk_snapshot_append_cairo (
          snapshot,
          &bounds);

  double center_x =
      width / 2.0;

  double center_y =
      height / 2.0;

  double radius =
      MIN (width, height) / 2.0;

  ColorPallete palette =
      self->palette;

  if (palette.count == 0)
    {
      cairo_destroy (cr);
      return;
    }

  double angle =
      2.0 * G_PI / palette.count;

  for (guint i = 0;
       i < palette.count;
       i++)
    {
      double start_angle =
          i * angle;

      double end_angle =
          (i + 1) * angle;

      GdkRGBA rgba_color =
          hue_to_gdk_rgba (
              palette.colors[i]);

      cairo_move_to (
          cr,
          center_x,
          center_y);

      cairo_arc (
          cr,
          center_x,
          center_y,
          radius,
          start_angle,
          end_angle);

      cairo_line_to (
          cr,
          center_x,
          center_y);

      cairo_close_path (cr);

      cairo_set_source_rgba (
          cr,
          rgba_color.red,
          rgba_color.green,
          rgba_color.blue,
          rgba_color.alpha);

      cairo_fill (cr);
    }

  cairo_destroy (cr);
}

static void
harmony_wheel_class_init (
    HarmonyWheelClass *klass)
{
  GObjectClass *gobject_class =
      G_OBJECT_CLASS (klass);

  GtkWidgetClass *widget_class =
      GTK_WIDGET_CLASS (klass);

  gobject_class->set_property =
      harmony_wheel_set_property;

  widget_class->snapshot =
      harmony_wheel_snapshot;

  obj_properties[PROP_PALETTE] =
      g_param_spec_pointer (
          "palette",
          "Color Palette",
          "The color palette displayed by the wheel",
          G_PARAM_WRITABLE |
              G_PARAM_CONSTRUCT);

  g_object_class_install_properties (
      gobject_class,
      N_PROPERTIES,
      obj_properties);
}

static void
harmony_wheel_init (
    HarmonyWheel *self)
{
}

void
harmony_wheel_set_palette (
    HarmonyWheel *self,
    ColorPallete palette)
{
  g_return_if_fail (
      HARMONY_IS_WHEEL (self));

  self->palette = palette;

  gtk_widget_queue_draw (
      GTK_WIDGET (self));
}

ColorPallete
harmony_wheel_get_palette (
    HarmonyWheel *self)
{
  g_return_val_if_fail (
      HARMONY_IS_WHEEL (self),
      (ColorPallete) { 0 });

  return self->palette;
}

HarmonyWheel *
harmony_wheel_new (void)
{
  return g_object_new (
      HARMONY_TYPE_WHEEL,
      NULL);
}
