# CSE 455 Homework 2 #

We will continue with image filtering in this homework. First run the following:

```
git pull
make clean
make
```

Then run `./main test hw2`. For python tests use `python tryhw2.py`.

### 2.1 Write a convolution function ###

#### TO DO ####
Fill in `image convolve_image(image im, image filter, int preserve)`. The parameter `preserve` takes a value of either 0 or 1. For this function we have a few scenarios. With normal convolutions we do a weighted sum over an area of the image. With multiple channels in the input image there are a few possible cases we want to handle:

- If `filter` and `im` have the same number of channels then it's just a normal convolution. We sum over spatial and channel dimensions and produce a 1 channel image. UNLESS:
- If `preserve` is set to 1 we should produce an image with the same number of channels as the input. This is useful if, for example, we want to run a box filter over an RGB image and get out an RGB image. This means each channel in the image will be filtered by the corresponding channel in the filter. UNLESS:
- If the `filter` only has one channel but `im` has multiple channels we want to apply the filter to each of those channels. Then we either sum between channels or not depending on if `preserve` is set.

Also, `filter` should have the same number of channels as `im` or have 1 channel. This is checked with an `assert`.

We are calling this a convolution but you don't need to flip the filter or anything (we're actually doing a cross-correlation). Just apply it to the image as we discussed in class:

![covolution](../../figs/convolution.png)

Once you are done, test out your convolution by filtering our image (we need to use preserve because we want to produce an image that is still RGB).

```
from uwimg import *
im = load_image("data/dog.jpg")
f = make_box_filter(7)
blur = convolve_image(im, f, 1)
save_image(blur, "dog-box7")
```

We'll get some output that looks like this:

![covolution](../../figs/dog-box7.png)

Now we can use this to perform our thumbnail operation:

![covolution](../../figs/dogthumb.png)

Look at how much better our new resized thumbnail is!

Resize                     |  Blur and Resize
:-------------------------:|:-------------------------:
![](../../figs/dog7th-nn.png)    | ![](../../figs/dogthumb.png)

### 2.2 Make some more filters and try them out! ###

#### TO DO ####
Fill in the functions `image make_highpass_filter()`, `image make_sharpen_filter()`, and `image make_emboss_filter()` to return the example kernels we covered in class. You can try them out on some images! 

Highpass                   |  Sharpen                  | Emboss
:-------------------------:|:-------------------------:|:--------------------|
![](../../figs/highpass.png)     | ![](../../figs/sharpen.png)     | ![](../../figs/emboss.png)

#### TO DO ####
Now, answer Questions 2.2.1 and 2.2.2 in the source file (put your answer just right there).


### 2.3 Implement a Gaussian kernel ###

#### TO DO ####
Fill in `image make_gaussian_filter(float sigma)` which will take a standard deviation value and return a filter that smooths using a gaussian with that sigma. How big should the filter be? 99% of the probability mass for a gaussian is within +/- 3 standard deviations, so make the kernel be 6 times the size of sigma. But also we want an odd number, so make it be the next highest odd integer from 6 x sigma. We need to fill in our kernel with some values. Use the probability density function for a 2D gaussian:

![2d gaussian](../../figs/2dgauss.png)

Technically this isn't perfect, what we would really want to do is integrate over the area covered by each cell in the filter. But that's much more complicated and this is a decent estimate. Remember though, this is a blurring filter so we want all the weights to sum to 1. Now you should be able to try out your new blurring function:

```
im = load_image("data/dog.jpg")
f = make_gaussian_filter(2)
blur = convolve_image(im, f, 1)
save_image(blur, "dog-gauss2")
```

It should have much less noise than the box filter:

![blurred dog](../../figs/dog-gauss2.png)

## 3. Hybrid images ##

Gaussian filters are cool because they are a true low-pass filter for the image. This means when we run them on an image we only get the low-frequency changes in an image like color. Conversely, we can subtract this low-frequency information from the original image to get the high frequency information!

Using this frequency separation we can do some pretty neat stuff. For example, check out [this tutorial on retouching skin](https://petapixel.com/2015/07/08/primer-using-frequency-separation-in-photoshop-for-skin-retouching/) in Photoshop (but only if you want to).

We can also make [really trippy images](http://cvcl.mit.edu/hybrid/OlivaTorralb_Hybrid_Siggraph06.pdf) that look different depending on if you are close or far away from them. That's what we'll be doing. They are hybrid images that take low frequency information from one image and high frequency info from another. Here's an example: 

Small                     |  Medium | Large
:-------------------------:|:-------:|:------------------:
![](../../figs/marilyn-einstein-small.png)   | ![](../../figs/marilyn-einstein-medium.png) | ![](../../figs/marilyn-einstein.png)

Check out `figs/marilyn-einstein.png` and view it from far away and up close. Your job is to produce a similar image. But instead of famous dead people we'll be using famous fictional people from the Harry Potter franchise! Like this:

Small                     | Large
:-------------------------:|:------------------:
![](../../figs/ronbledore-small.jpg)   | ![](../../figs/ronbledore.jpg) 

For this task you'll have to extract the high frequency and low frequency from some images. You already know how to get low frequency, using your gaussian filter. To get high frequency you just subtract the low frequency data from the original image.

#### TO DO ####
Fill in `image add_image(image a, image b)` to add two images a and b and `image sub_image(image a, image b)` to subtract image b from image a, so that we can perform our transformations of `+` and `-` like this:

```
im = load_image("data/dog.jpg")
f = make_gaussian_filter(2)
lfreq = convolve_image(im, f, 1)
hfreq = im - lfreq
reconstruct = lfreq + hfreq
save_image(lfreq, "low-frequency")
save_image(hfreq, "high-frequency")
save_image(reconstruct, "reconstruct")
```

The functions should probably include some checks that the images are the same size and such. Now we should be able to get these results:

Low frequency           |  High frequency | Reconstruction
:-------------------------:|:-------:|:------------------:
![](../../figs/low-frequency.png)   | ![](../../figs/high-frequency.png) | ![](../../figs/reconstruct.png)

Note, the high-frequency image overflows when we save it to disk. Is this a problem for us? Why or why not? Think!

Use these functions to recreate your own Ronbledore image. You will need to tune your standard deviations for the gaussians you use. You will probably need different values for each image to get it to look good.

## 4. Sobel filters ##

The [Sobel filter](https://www.researchgate.net/publication/239398674_An_Isotropic_3x3_Image_Gradient_Operator) is cool because we can estimate the gradients and direction of those gradients in an image. They should be straightforward now that you all are such pros at image filtering.

### 4.1 Make the filters ###

#### TO DO ####
Fill in the functions `image make_gx_filter()` and `image make_gy_filter()` to make our sobel filters. They are for estimating the gradient in the x and y direction:

Gx                 |  Gy 
:-----------------:|:------------------:
![](../../figs/gx.png)   |  ![](../../figs/gy.png)


### 4.2 Feature normalization ###

To visualize our sobel operator we'll want another normalization strategy, [feature normalization](https://en.wikipedia.org/wiki/Feature_scaling). This strategy is simple, we just want to scale the image so all values lie between [0-1]. In particular we will be [rescaling](https://en.wikipedia.org/wiki/Feature_scaling#Rescaling) the image by subtracting the minimum from all values and dividing by the range (i.e. max - min) of the data. If the range is zero you should just set the whole image to 0 (don't divide by 0). 

#### TO DO ####
Fill in the function `void feature_normalize(image im)`.

### 4.3 Calculate gradient magnitude and direction ###

#### TO DO ####
Fill in the function `image *sobel_image(image im)`. It should return two images, the gradient magnitude and direction. The strategy can be found [here](https://en.wikipedia.org/wiki/Sobel_operator#Formulation). We can visualize our magnitude using our normalization function:

```
im = load_image("data/dog.jpg")
res = sobel_image(im)
mag = res[0]
feature_normalize(mag)
save_image(mag, "magnitude")
```

which results in:

![](../../figs/magnitude.png)

### 4.4 EXTRA CREDIT: Make a colorized representation ###

Now using your sobel filter you can make a cool, stylized one. 
#### TO DO (extra credit) ####
Write a function `image colorize_sobel(image im)`. Use the magnitude to specify the saturation and value of an image and the angle to specify the hue. Using some smoothing, the result should look like this:

![](../../figs/lcolorized.png)

### 4.5 SUPER EXTRA CREDIT: Let's blur but slightly differently ###

Now let's try blurring by not just assigning weights to surrounding pixels based on their spatial location in relation to the center pixel but also by how far away they are in terms of color from the center pixel. The idea of the [bilateral filter](https://cs.jhu.edu/~misha/ReadingSeminar/Papers/Tomasi98.pdf) is to blur everything in the image but the color edges. 

Once again we will be forming a filter, except now it will be different per pixel. The weights for a pixel's filter can be described as such:

![W](../../figs/bilateral_W.png)

where the individual weights are

![W](../../figs/bilateral_wij.png)

and the normalization factor is 

![W](../../figs/bilateral_norm.png)

for a kernel of size (2k+1).

#### TO DO (super extra credit) ####
Write a function `image bilateral_filter(image im)`. Your image should have a similar effect to the image below, so we suggest testing out a few spatial and color sigma parameters before submitting your final image (you can find the before image in `/data/bilateral_raw.png`. Note that it is 40x40 pixels and is being upsampled in this README). To submit the final image apply your filter to `cliffs.jpg` (Credit: Sylvian Paris) and name it `bilateral.jpg`. Good luck!

Before                 |  After 
:-----------------:|:------------------:
![](../../figs/bilateral_pre.png)   |  ![](../../figs/bilateral_post.png)

![cat](../../figs/bilateral_cat.png)

![scene](../../figs/bilateral_filter.jpg)

## 5. Turn it in ##

Turn in your `filter_image.c`, `ronbledore.jpg`, `sobel.jpg` and `bilateral.jpg` (if any) on Canvas under Homework 2.