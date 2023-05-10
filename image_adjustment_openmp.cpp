#include "image_adjustment.h"

using namespace std;

void adjust_attributes(uint8_t *data, int width, int height, image_attributes &attributes)
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            uint8_t *pixel = data + (y * width + x) * 3;
            adjust_pixel(pixel, attributes);
        }
    }
}