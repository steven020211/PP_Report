#pragma once

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
        b{0.0f},
        f{1.0f},
        r{1.0f},
        a{0.0f}
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
};
