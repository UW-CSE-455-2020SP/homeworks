#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"


/***********************************************************************
  We've been talking a lot about resizing and interpolation in class,
  now's your time to do it!
  In order to make grading easier, please only edit the files we mention to submit.
  You will submit the resize_image.c file on Canvas.
************************************************************************/


/******************************** Resizing *****************************
  To resize we'll need some interpolation methods and a function to create
  a new image and fill it in with our interpolation methods.
************************************************************************/

float nn_interpolate(image im, float x, float y, int c)
{
    // TODO
    /***********************************************************************
      This function performs nearest-neighbor interpolation on image "im"
      given a floating column value "x", row value "y" and integer channel "c",
      it interpolates and returns the interpolated value.
      Remember to use the closest "int", not just type-cast because in C that
      will truncate towards zero.
    ************************************************************************/
    return 0;
}

image nn_resize(image im, int w, int h)
{
    // TODO Fill in (also fix the return line)
    /***********************************************************************
      This function uses nearest-neighbor interpolation on image "im" to
      construct a new image of size "w x h". Hint:
      - Create a new image that is "w x h" and the same number of channels as "im"
      - Loop over the pixels and map back to the old coordinates.
      - Use nearest-neighbor interpolate to fill in the image.
    ************************************************************************/
    return make_image(1,1,1);
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    // TODO
    /***********************************************************************
      This function performs bilinear interpolation on image "im" given
      a floating column value "x", row value "y" and integer channel "c".
      It interpolates and returns the interpolated value.
    ************************************************************************/
    return 0;
}

image bilinear_resize(image im, int w, int h)
{
    // TODO
    /***********************************************************************
      This function uses bilinear interpolation on image "im" to construct
      a new image of size "w x h". Hint:
      - Create a new image that is "w x h" and the same number of channels as "im".
      - Loop over the pixels and map back to the old coordinates.
      - Use bilinear interpolate to fill in the image.
    ************************************************************************/
    return make_image(1,1,1);
}

/***************************** Box filter *******************************
  We want to create a box filter. We will only use square box filters.
  One way to do this is make an image,fill it in with all 1s, and then
  normalize it.That's what we'll do because the normalization function may
  be useful in the future!
************************************************************************/
void l1_normalize(image im)
{
    // TODO
    /***********************************************************************
      This function divides each value in an image "im" by the sum of all the
      values in the image and modifies the image in place.
    ************************************************************************/
}

image make_box_filter(int w)
{
    // TODO
    /***********************************************************************
      This function makes a square filter of size "w x w". Hint:
      Change the "make_image" arguments so that it is a square image of
      width = height = w and number of channels = 1, with all entries equal
      to 1. Then use "l1_normalize" to normalize your filter.
    ************************************************************************/

    return make_image(1,1,1);
}
