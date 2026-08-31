#include "harmony-wheel.h"
#include <gtk/gtk.h>
#include <math.h>

enum
{
  PROP_COLOR = 1,
  N_PROPERTIES
};

static GParamSpec *obj_properties[N_PROPERTIES] = {
  NULL,
};

struct _HarmonyWheel
{
  GtkWidget parent_instance;
  HSLA color;
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
    case PROP_COLOR:
      HSLA *color_received = g_value_get_pointer (value);
      if (color_received != NULL)
        {
          self->color = *color_received;
        };
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static double
wrap01 (double value)
{
  value = fmod (value, 1.0);

  if (value < 0)
    {
      return value += 1.0;
    };
  return value;
}
static double
find_color (double t, double p, double q)
{
  if (t < 0.0)
    t += 1.0;
  if (t > 1.0)
    t -= 1.0;
  if (t < 1.0 / 6.0)
    {
      return p + (q - p) * 6.0 * t;
    }
  if (1.0 / 6.0 <= t && t < 1.0 / 2.0)
    {
      return q;
    }
  if (1.0 / 2.0 <= t && t <= 2.0 / 3.0)
    {
      return p + (q - p) * (2.0 / 3.0 - t) * 6.0;
    }
  if (t >= 2.0 / 3.0)
    {
      return p;
    }
  return -1;
}

static GdkRGBA
hue_to_gdk_rgba (HSLA hsla_color)
{
  GdkRGBA color;
  double new_h = hsla_color.h / 360.0;
  double new_s = hsla_color.s / 100.0;
  double new_l = hsla_color.l / 100.0;
  double q = new_l < 0.5 ? new_l * (1.0 + new_s) : new_l + new_s - (new_l * new_s);
  double p = 2.0 * new_l - q;
  /* double new_h = hsla_color.h / 360; */

  double r = wrap01 (new_h + 1.0 / 3.0);
  double g = wrap01 (new_h);
  double b = wrap01 (new_h - 1.0 / 3.0);
  double a = hsla_color.a;

  r = find_color (r, p, q);
  g = find_color (g, p, q);
  b = find_color (b, p, q);
  color = (GdkRGBA) { r, g, b, a };
  return color;
}

static void
harmony_wheel_snapshot (GtkWidget *widget,
                            GtkSnapshot *snapshot)
{
  g_print ("entrou");
  HarmonyWheel *self = HARMONY_WHEEL (widget);
  GdkRGBA rgba_color = hue_to_gdk_rgba (self->color);
  int width = gtk_widget_get_width (widget);
  int height = gtk_widget_get_height (widget);

  graphene_rect_t bounds =
      GRAPHENE_RECT_INIT (0, 0, width, height);

  cairo_t *cr =
      gtk_snapshot_append_cairo (snapshot, &bounds);

  cairo_arc (
      cr,
      width / 2.0,
      height / 2.0,
      MIN (width, height) / 2.0,
      0,
      G_PI / 2);

  cairo_line_to (
      cr,
      width / 2.0,
      height / 2.0);

  cairo_close_path (cr);

  g_print ("%f, %f, %f", rgba_color.red, rgba_color.green, rgba_color.blue);

  cairo_set_source_rgb (
      cr,
      rgba_color.red,
      rgba_color.green,
      rgba_color.blue);

  cairo_fill (cr);

  cairo_destroy (cr);
}

static void
harmony_wheel_class_init (HarmonyWheelClass *klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);
  gobject_class->set_property = harmony_wheel_set_property;

  widget_class->snapshot = harmony_wheel_snapshot;

  obj_properties[PROP_COLOR] = g_param_spec_pointer ("hsla-color",
                                                     "HSLA Color",
                                                     "The HSLA color send by the window",
                                                     G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY);

  g_object_class_install_properties (gobject_class, N_PROPERTIES, obj_properties);
}

void
harmony_wheel_set_color (HarmonyWheel *self, HSLA color)
{
  g_return_if_fail (GNOME_IS_COLOR_WHEEL (self));

  self->color = color;
  gtk_widget_queue_draw (GTK_WIDGET (self));
}
static void
harmony_wheel_init (HarmonyWheel *self)
{
}

HarmonyWheel *
harmony_wheel_new (void)
{
  return g_object_new (
      GNOME_TYPE_COLOR_WHEEL,
      NULL);
}

