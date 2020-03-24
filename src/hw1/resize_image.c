#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"

float nn_interpolate(image im, float x, float y, int c)
{
    // TODO Fill in
    return 0;
}

image nn_resize(image im, int w, int h)
{
    // TODO Fill in (also fix that first line)
    return make_image(1,1,1);
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    // TODO
    return 0;
}

image bilinear_resize(image im, int w, int h)
{
    // TODO
    return make_image(1,1,1);
}

void l1_normalize(image im)
{
    // TODO
}

image make_box_filter(int w)
{
    // TODO
    return make_image(1,1,1);
}
