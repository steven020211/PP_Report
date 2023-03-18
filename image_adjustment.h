#pragma once
#include <cmath>

#define PI acos(-1)

void adjust_brightness(unsigned char* data, int width, int height, float brightness);

void adjust_contrast(unsigned char* data, int width, int height, float contrast);

void adjust_hue(unsigned char* data, int width, int height, float hue);

void adjust_saturation(unsigned char* data, int width, int height, float saturation);