#include "image_adjustment.h"
#include <algorithm>

using namespace std;

void adjust_brightness(unsigned char* data, int width, int height, float brightness) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int index = (i * width + j) * 3;
            
            for (int k = 0; k < 3; k++) {
                int val = data[index + k] + brightness * 255;
                val = max(0, min(val, 255));
                data[index + k] = val;
            }
        }
    }
}


void adjust_contrast(unsigned char* data, int width, int height, float contrast) {
    auto c = contrast * 255.0f;
    float f = (259.0f * (c + 255.0f)) / (255.0f * (259.0f - c));
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int i = (y * width + x) * 3;
            float r = data[i];
            float g = data[i+1];
            float b = data[i+2];

            r = f * (r - 128.0f) + 128.0f;
            g = f * (g - 128.0f) + 128.0f;
            b = f * (b - 128.0f) + 128.0f;

            if (r < 0.0f) r = 0.0f;
            if (r > 255.0f) r = 255.0f;
            if (g < 0.0f) g = 0.0f;
            if (g > 255.0f) g = 255.0f;
            if (b < 0.0f) b = 0.0f;
            if (b > 255.0f) b = 255.0f;

            data[i] = (unsigned char)r;
            data[i+1] = (unsigned char)g;
            data[i+2] = (unsigned char)b;
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
    for (int i = 0; i < width * height; i++) {
        float r = data[3*i+0] / 255.0f;
        float g = data[3*i+1] / 255.0f;
        float b = data[3*i+2] / 255.0f;

        float h, s, v;
        rgb2hsv(r, g, b, h, s, v);

        h += hue;
        if (h < 0.0f) h += 360.0f;
        if (h >= 360.0f) h -= 360.0f;

        hsv2rgb(h, s, v, r, g, b);

        data[3*i+0] = static_cast<unsigned char>(r * 255.0f);
        data[3*i+1] = static_cast<unsigned char>(g * 255.0f);
        data[3*i+2] = static_cast<unsigned char>(b * 255.0f);
    }
}

void adjust_saturation(unsigned char* data, int width, int height, float saturation) {
    saturation = min(max(saturation, -1.0f), 1.0f);
    float ratio = (saturation < 0.0f) ? (saturation + 1.0f) : saturation * 2.0f;
    float h, s, v;
    float r, g, b;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int offset = i * width * 3 + j * 3;
            r = data[offset];
            g = data[offset + 1];
            b = data[offset + 2];
            rgb2hsv(r, g, b, h, s, v);
            s *= ratio;
            s = min(max(s, 0.0f), 1.0f);
            hsv2rgb(h, s, v, r, g, b);
            data[offset] = static_cast<unsigned char>(r);
            data[offset + 1] = static_cast<unsigned char>(g);
            data[offset + 2] = static_cast<unsigned char>(b);
        }
    }
}



