#pragma once

#define DEF_B 0.0f
#define DEF_F 1.0f
#define DEF_R 1.0f
#define DEF_A 0.0f

class  image_attributes
{
public:

    float brightness;
    float contrast;
    float saturation;
    float hue;

    //brightness parameter (bias)
    float b;
    //contrast parameter (factor)
    float f;
    //saturation parameter (ratio)
    float r;
    //hue (angle)
    float a;

    image_attributes() :
        brightness{0.0f},
        contrast{0.0f},
        saturation{0.0f},
        hue{0.0f},
        b{DEF_B},
        f{DEF_F},
        r{DEF_R},
        a{DEF_A}
    {};

    void set_brightness(float value) { b = (brightness = value) * 255; };
    void set_contrast(float value) 
    {
        contrast = value; 
        float c = contrast * 255.0f;
        f = (259.0f * (c + 255.0f)) / (255.0f * (259.0f - c));
    };
    void set_saturation(float value) { r = (saturation = value) + 1.0f; };
    void set_hue(float value) { a = hue = value; };

    bool adjust_brightness() { return b != DEF_B; };
    bool adjust_contrast() { return f != DEF_F; };
    bool adjust_saturation() { return r != DEF_R; };
    bool adjust_hue() { return a != DEF_A; };
};
