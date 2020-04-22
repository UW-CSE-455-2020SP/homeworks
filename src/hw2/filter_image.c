#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853


// void l1_normalize(image im)
// {
//     // TODO: cut and paste this function from your previous homework
//     /***********************************************************************
//       This function divides each value in an image "im" by the sum of all the
//       values in the image and modifies the image in place.
//     ************************************************************************/
// }

// image make_box_filter(int w)
// {
//     // TODO: cut and paste this function from your previous homework
//     /***********************************************************************
//       This function makes a square filter of size "w x w". Hint:
//       Change the "make_image" arguments so that it is a square image of width = height = w and number of channels = 1, with all entries equal to 1. Then use "l1_normalize" to normalize your filter.
//     ************************************************************************/

//     return make_image(1,1,1);
// }



image convolve_image(image im, image filter, int preserve)
{
    // TODO
    /***********************************************************************
    im: an image with shape "h x w x c"
    filter: a convolution filter with shape "k1 x k2 x k3". 
    Preserve: an integer, which is usually either 0 or 1.

    - If `filter` and `im` have the same number of channels then it's just a normal 
    convolution. We sum over spatial and channel dimensions and produce a 1 channel image. 
    UNLESS:
        If `preserve` is set to 1 we should produce an image with the same number of 
        channels as the input. This is useful if, for example, we want to run a box 
        filter over an RGB image and get out an RGB image. This means each channel in 
        the image will be filtered by the corresponding channel in the filter.
    If the `filter` only has one channel but `im` has multiple channels we want to
    apply the filter to each of those channels. Then we either sum between channels 
    or not depending on if `preserve` is set.

    Also, `filter` better have either the same number of channels as `im` or have one channel. 
    I check this with an `assert`.
    ************************************************************************/


    return make_image(1,1,1);
}

image make_highpass_filter()
{
    // TODO
    /***********************************************************************
    Create a 3x3 highpass filter and return it
    ************************************************************************/
    return make_image(1,1,1);
}

image make_sharpen_filter()
{
    // TODO
    /***********************************************************************
    Create a 3x3 sharpen filter and return it
    ************************************************************************/
    return make_image(1,1,1);
}

image make_emboss_filter()
{
    // TODO
    /***********************************************************************
    Create a 3x3 emboss filter and return it
    ************************************************************************/
    return make_image(1,1,1);
}

// Question 2.2.1: Which of these filters should we use `preserve = 1` when we run our convolution and which ones should we not? Why?
// Answer: TODO

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: TODO

image make_gaussian_filter(float sigma)
{
    // TODO
    /***********************************************************************
    sigma: a float number for the Gaussian.

    Create a Gaussian filter with the given sigma. Note that the kernel size 
    is the next highest odd integer from 6x sigma.

    Return the Gaussian filter.
    ************************************************************************/
    return make_image(1,1,1);
}

image add_image(image a, image b)
{
    // TODO
    /***********************************************************************
    The input images a and image b have the same height, width, and channels.
    Sum the given two images and return the result. 
    The result image should also have the same height, width, and channels as the inputs.
    ************************************************************************/
    return make_image(1,1,1);
}

image sub_image(image a, image b)
{
    // TODO
    /***********************************************************************
    The input image a and image b have the same height, width, and channels.
    Subtract the given two images (a - b) and return the result.
    The result image should also have the same height, width, and channels as the inputs.
    ************************************************************************/
    return make_image(1,1,1);
}

image make_gx_filter()
{
    // TODO
    /***********************************************************************
    Create a 3x3 Sobel Gx filter and return it
    ************************************************************************/
    return make_image(1,1,1);
}

image make_gy_filter()
{
    // TODO
    /***********************************************************************
    Create a 3x3 Sobel Gy filter and return it
    ************************************************************************/
    return make_image(1,1,1);
}

image *sobel_image(image im)
{
    // TODO
    /***********************************************************************
    im: the input image is either a "h x w x 3" RGB image or "h x w x 1" grayscale 
    image.
    
    Apply Sobel filter to the given image, get the magnitude and gradient, 
    and return the result. 

    Hint: the "calloc" function can allocate the memory for your output. You can
    assess the first image (magnitute) by calling rst[0] and the second image 
    by calling rst[1]
    ************************************************************************/
    image *rst = calloc(2, sizeof(image));


    rst[0] = make_image(im.w, im.h, 1);
    rst[1] = make_image(im.w, im.h, 1);

    return rst;
}


void normalize_image(image im)
{
    /***********************************************************************
    Calculate minimum and maximum pixel values. Normalize the image by
    subtracting the minimum and dividing by the max-min difference.
    This is a helper function to visualize the sobel magnitude image better.
    No TODO here :)
    ***********************************************************************/
    int i;
    float min = im.data[0];
    float max = im.data[0];
    for(i = 0; i < im.w*im.h*im.c; ++i){
        if(im.data[i] > max) max = im.data[i];
        if(im.data[i] < min) min = im.data[i];
    }
    for(i = 0; i < im.w*im.h*im.c; ++i){
        im.data[i] = (im.data[i] - min)/(max-min);
    }
}


// EXTRA CREDITS BELOW
int compare_float(const void * a, const void * b)
{
    // This function is provided for your convenience
    float fa = *(const float*) a;
    float fb = *(const float*) b;
    return (fa > fb) - (fa < fb);
}

image apply_median_filter(image im, int kernel_size)
{
    image out = make_image(im.w, im.h, im.c);

    // TODO (EXTRA CREDIT)
    /***********************************************************************
    im is the input image.
    kernel_size is a positive odd number.

    We assume a median filter is a square, with the same height and width.
    The kernel size is always a positive odd number. We use "clamp" padding
    for borders and corners. The output image should have the same width, 
    height, and channels as the input image. You should apply median filter
    to each channel of the input image `im`.

    Hint: use the qsort() function to sort an array. Make use of compare_float() as needed.
    ************************************************************************/

    return out;
}
