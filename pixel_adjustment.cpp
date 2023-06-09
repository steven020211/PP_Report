#include "image_adjustment.h"
#include <algorithm>

using namespace std;

float normalize(float value, float min_value, float max_value)
{
    return min(max_value, max(min_value, value));
}

float normalize_rgb(float value)
{
    return normalize(value, 0.0f, 255.0f);
}

void adjust_pixel_brightness(float &r, float &g, float &b, float bias)
{
    r = normalize_rgb(r + bias);
    g = normalize_rgb(g + bias);
    b = normalize_rgb(b + bias);
}

void adjust_pixel_contrast(float &r, float &g, float &b, float factor)
{
    r = normalize_rgb(factor * (r - 128.0f) + 128.0f);
    g = normalize_rgb(factor * (g - 128.0f) + 128.0f);
    b = normalize_rgb(factor * (b - 128.0f) + 128.0f);
}

void adjust_pixel_saturation(float &s, float ratio)
{
    s = normalize(s * ratio, 0.0f, 1.0f);
}

void adjust_pixel_hue(float &h, float angle)
{
    h += angle;
    if (h < 0.0f)
        h += 360.0f;
    if (h >= 360.0f)
        h -= 360.0f;
}

void adjust_pixel(uint8_t *pixel, image_attributes &attributes)
{
    float r = pixel[0];
    float g = pixel[1];
    float b = pixel[2];

    // brightness
    if (attributes.adjust_brightness())
        adjust_pixel_brightness(r, g, b, attributes.b);
    // contrast
    if (attributes.adjust_contrast())
        adjust_pixel_contrast(r, g, b, attributes.f);

    if (attributes.adjust_saturation() || attributes.adjust_hue())
    {
        // convert rgb to hsv
        float h, s, v;
        rgb2hsv(r, g, b, h, s, v);

        // saturation
        if (attributes.adjust_saturation())
            adjust_pixel_saturation(s, attributes.r);

        // hue
        if (attributes.adjust_hue())
            adjust_pixel_hue(h, attributes.a);
        // convert hsv to rgb
        hsv2rgb(h, s, v, r, g, b);
    }

    pixel[0] = r;
    pixel[1] = g;
    pixel[2] = b;
}

void rgb2hsv(float r, float g, float b, float &h, float &s, float &v)
{
    float min_val = min(min(r, g), b);
    float max_val = max(max(r, g), b);

    v = max_val;

    float delta = max_val - min_val;

    if (delta == 0)
    {
        h = 0;
        s = 0;
    }
    else
    {
        s = delta / max_val;

        if (r == max_val)
        {
            h = (g - b) / delta;
        }
        else if (g == max_val)
        {
            h = 2 + (b - r) / delta;
        }
        else
        {
            h = 4 + (r - g) / delta;
        }

        h *= 60.0;

        if (h < 0)
        {
            h += 360.0;
        }
    }
}

void hsv2rgb(float h, float s, float v, float &r, float &g, float &b)
{
    if (s == 0)
    {
        r = v;
        g = v;
        b = v;
    }
    else
    {
        h /= 60.0;
        int i = (int)h;
        float f = h - i;

        float p = v * (1.0 - s);
        float q = v * (1.0 - s * f);
        float t = v * (1.0 - s * (1.0 - f));

        switch (i)
        {
        case 0:
            r = v;
            g = t;
            b = p;
            break;
        case 1:
            r = q;
            g = v;
            b = p;
            break;
        case 2:
            r = p;
            g = v;
            b = t;
            break;
        case 3:
            r = p;
            g = q;
            b = v;
            break;
        case 4:
            r = t;
            g = p;
            b = v;
            break;
        default:
            r = v;
            g = p;
            b = q;
            break;
        }
    }
}
