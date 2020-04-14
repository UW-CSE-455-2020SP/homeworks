# CSE 455 Homework 1 #

Welcome, it's time for assignment 1! This one may be a little harder than the last one so remember to start early! In order to make grading easier, please only edit the files we mention to submit. You will submit the `src/hw1/resize_image.c` file on Canvas.

First, you need to run `git pull` from inside your `homeworks` folder to get the latest changes from GitHub. Then run

```
make clean
make
```
and then run `./main test hw1`. For python tests use `python tryhw1.py`.

## 1. Image resizing ##

We've been talking a lot about resizing and interpolation in class, now's your time to do it! To resize we'll need some interpolation methods and a function to create a new image and fill it in with our interpolation methods.

### 1.1 Nearest Neighbor Interpolation ###

#### TO DO ####
Fill in `float nn_interpolate(image im, float x, float y, int c);` in `src/hw1/resize_image.c`. It should perform nearest neighbor interpolation. Remember to use the closest `int`, not just type-cast because in C that will truncate towards zero.

### 1.2 Nearest Neighbor Resizing ###

#### TO DO ####
Fill in `image nn_resize(image im, int w, int h)`. It should:
- Create a new image that is `w x h` and the same number of channels as `im`
- Loop over the pixels and map back to the old coordinates
- Use nearest-neighbor interpolate to fill in the image

Now you should be able to run the following python command in `tryhw1.py`:

    from uwimg import *
    im = load_image("data/dogsmall.jpg")
    a = nn_resize(im, im.w*4, im.h*4)
    save_image(a, "dog4x-nn")

Your image should look something like:

![blocky dog](../../figs/dog4x-nn.png)

### 1.3 Bilinear Interpolation ###

#### TO DO ####
Fill in the function `float bilinear_interpolate(image im, float x, float y, int c)` for bilinear interpolation.

### 1.4 Bilinear Resizing ###

#### TO DO ####
Fill in `image bilinear_resize(image im, int w, int h)` to perform resizing using bilinear interpolation. Try it out again in python:

    im = load_image("data/dogsmall.jpg")
    a = bilinear_resize(im, im.w*4, im.h*4)
    save_image(a, "dog4x-bl")

![smooth dog](../../figs/dog4x-bl.png)

These functions will work fine for small changes in size, but when we try to make our image smaller, say a thumbnail, we get very noisy results:

    im = load_image("data/dog.jpg")
    a = nn_resize(im, im.w//7, im.h//7)
    save_image(a, "dog7th-bl")

![jagged dog thumbnail](../../figs/dog7th-nn.png)

As we discussed, we need to filter before we do this extreme resize operation!

## 2. Image filtering ##

We'll start out by filtering the image with a box filter. We'll create the box filter in this homework and will use it together with convolution function in the next homework!

### 2.1 Create your box filter ###

We want to create a box filter, which as discussed in class looks like this:

![box filter](../../figs/boxfilter.png)

One way to do this is make an image, fill it in with all 1s, and then normalize it. That's what we'll do because the normalization function may be useful in the future!

#### TO DO ####
Fill in `void l1_normalize(image im)`. This should divide each value in an image by the sum of all the values in the image.

#### TO DO ####
Fill in `image make_box_filter(int w)`. We will only use square box filters, so just make your filter `w x w`. In other words, change the `make_image` arguments so that it is a square image of width = height = w and number of channels = 1, with all entries equal to 1. Then use `l1_normalize` to normalize your filter.


## Turn it in ##

Turn in your `resize_image.c` (including `l1_normalize` and `make_box_filter`) on canvas under Homework 1. Save `l1_normalize` and `make_box_filter` for your next assignment. You will need them. We'll grade them with Homework 2.

