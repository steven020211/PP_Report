#pragma once
#include <stdint.h>
#include "image_attributes.h"


void adjust_brightness(uint8_t* data, int width, int height, float brightness);

void adjust_contrast(uint8_t* data, int width, int height, float contrast);

void adjust_hue(uint8_t* data, int width, int height, float hue);

void adjust_saturation(uint8_t* data, int width, int height, float saturation);

void adjust_attributes(uint8_t* data, int width, int height, image_attributes& attributes);

//pixel adjustment
float normalize(float value, float min_value, float max_value);

float normalize_rgb(float value);

void adjust_pixel(uint8_t* pixel, image_attributes& attributes);

void rgb2hsv(float r, float g, float b, float& h, float& s, float& v);

void hsv2rgb(float h, float s, float v, float& r, float& g, float& b);