#include "image_adjustment.h"
#include <algorithm>

using namespace std;

void adjust_brightness(unsigned char* data, int width, int height, float brightness) {
    float b = brightness * 255;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = (y * width + x) * 3;
            
            for (int i = 0; i < 3; i++) {
                int val = data[index + i] + b;
                val = max(0, min(val, 255));
                data[index + i] = val;
            }
        }
    }
}


void adjust_contrast(unsigned char* data, int width, int height, float contrast) {
    float c = contrast * 255.0f;
    float f = (259.0f * (c + 255.0f)) / (255.0f * (259.0f - c));
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = (y * width + x) * 3;
            for (int i = 0; i < 3; i++) {
                int val = f * (data[index + i] - 128.0f) + 128.0f;
                val = max(0, min(val, 255));
                data[index + i] = val;
            }
        }
    }
}

void rgb2hsv(float r, float g, float b, float& h, float& s, float& v) {
    float min_val = min(min(r, g), b);
    float max_val = max(max(r, g), b);

    v = max_val;

    float delta = max_val - min_val;

    if (delta == 0) {
        h = 0;
        s = 0;
    } else {
        s = delta / max_val;

        if (r == max_val) {
            h = (g - b) / delta;
        } else if (g == max_val) {
            h = 2 + (b - r) / delta;
        } else {
            h = 4 + (r - g) / delta;
        }

        h *= 60.0;

        if (h < 0) {
            h += 360.0;
        }
    }
}

void hsv2rgb(float h, float s, float v, float& r, float& g, float& b) {
    if (s == 0) {
        r = v;
        g = v;
        b = v;
    } else {
        h /= 60.0;
        int i = (int)h;
        float f = h - i;

        float p = v * (1.0 - s);
        float q = v * (1.0 - s * f);
        float t = v * (1.0 - s * (1.0 - f));

        switch (i) {
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

void adjust_hue(unsigned char* data, int width, int height, float hue) {
    float a = hue;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = (y * width + x) * 3;
            float r = data[index + 0];
            float g = data[index + 1];
            float b = data[index + 2];

            float h, s, v;
            rgb2hsv(r, g, b, h, s, v);

            h += a;

            if (h < 0.0f) h += 360.0f;
            if (h >= 360.0f) h -= 360.0f;

            hsv2rgb(h, s, v, r, g, b);

            data[index + 0] = static_cast<unsigned char>(r);
            data[index + 1] = static_cast<unsigned char>(g);
            data[index + 2] = static_cast<unsigned char>(b);

        }
    }
}
void adjust_saturation(unsigned char* data, int width, int height, float saturation) {
    saturation = min(max(saturation, -1.0f), 1.0f);
    float ratio = saturation + 1.0f;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = (y * width + x) * 3;
            float r = data[index],g = data[index + 1],b = data[index + 2];
            float h, s, v;
            rgb2hsv(r, g, b, h, s, v);

            s *= ratio;
            s = min(max(s, 0.0f), 1.0f);

            hsv2rgb(h, s, v, r, g, b);

            data[index] = static_cast<unsigned char>(r);
            data[index + 1] = static_cast<unsigned char>(g);
            data[index + 2] = static_cast<unsigned char>(b);
        }
    }
}



