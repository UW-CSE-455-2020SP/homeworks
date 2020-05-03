![panorama of field](../../figs/field_panorama.jpg)

# CSE 455 Homework 3 #

Welcome, it's time for homework 3! Similar to last homework, perform

```
git pull
make clean
make
```

and then `./main test hw3` and/or `python tryhw3.py`. There are two files in this folder: `panorama_helpers.c` containing helper functions (already written for you) and `panorama_image.c` containing the TODOs. Ideally you only need to make changes in `panorama_image.c`. **Remember to write comments in your code!**

This homework covers finding interest points in an image, describing those interest points, matching them to the corresponding points in another image, computing the transform from one image to the other, and stitching them together into a panorama. The high-level algorithm is already done for you! You can find it near the bottom of `src/hw3/panorama_helpers.c`, it looks approximately like:

    image panorama_image(image a, image b, float sigma, float thresh, int nms, float inlier_thresh, int iters, int cutoff)
    {
        // Calculate corners and descriptors
        descriptor *ad = harris_corner_detector(a, sigma, thresh, nms, &an);
        descriptor *bd = harris_corner_detector(b, sigma, thresh, nms, &bn);

        // Find matches
        match *m = match_descriptors(ad, an, bd, bn, &mn);

        // Run RANSAC to find the homography
        matrix H = RANSAC(m, mn, inlier_thresh, iters, cutoff);

        // Stitch the images together with the homography
        image combine = combine_images(a, b, H);
        return combine;
    }

So we'll find the corner points in an image using a Harris corner detector. Then we'll match together the descriptors of those corners. We'll use RANSAC to estimate a homography from one image to the other. Finally, we'll stitch together the images using this homography. **Note: We don't have tests to check the correctness of most of the functions individually, we will check them manually.**

## 1. Harris corner detection ##

We'll be implementing Harris corner detection as discussed in class. The basic algorithm is:

    Calculate image derivatives Ix and Iy.
    Calculate measures Ix^2, Iy^2, and Ix * Iy.
    Calculate structure matrix S as weighted sum of nearby measures.
    Calculate Harris "cornerness" as estimate of 2nd eigenvalue: det(S) - α trace(S)^2, α = .06
    Run non-max suppression on response map
    Compute corners from the response map using a threshold and compute descriptors for the corners

## 1.1 Compute the structure matrix ##

#### TO DO ####
Fill in `image structure_matrix(image im, float sigma)`. This will perform the first 3 steps of the algorithm: calculating derivatives, the corresponding measures, and the weighted sum of nearby derivative information. Use Sobel filter and associated functions from HW2 to calculate the derivatives. The measures are element-wise multiplications. The weighted sum can be easily computed with a Gaussian blur as discussed in homework slides. Use the parameter `sigma` to create the Gaussian kernel and convolve the result with it.

### 1.1.1 EXTRA CREDIT: Make a fast smoother ###

If you want a fast corner detector you can decompose the Gaussian blur from one large 2D convolution to 2 1D convolutions. Instead of using an N x N filter you can convolve with a 1 x N filter followed by the same filter flipped to be N x 1. **Note: Use the correct constant in front of the exponential, even if it doesn't matter since you normalize the filter.** This is because we are not just testing correctness of the code but also understanding of the concepts.

#### TO DO ####
Fill in `image make_1d_gaussian(float sigma)`. You may optionally change the 2-D Gaussian part in the `structure_matrix()` to 1-D to use this decomposed Gaussian smoothing.

## 1.2 Computer cornerness from structure matrix ##

#### TO DO ####
Fill in `image cornerness_response(image S)` to calculate cornerness response map `R` from the structure matrix `S` using the formula mentioned in the code.

## 1.3 Non-maximum suppression ##

We only want local maximum responses to our corner detector so that the matching is easier. For every pixel in `R`, check every neighbor within `w` pixels, i.e. check the `2w+1` window centered at each pixel. If any of the neighboring responses are stronger (neighboring pixel value > central pixel value), suppress that pixel's response (set the central pixel value to a very low negative number mentioned in code).

#### TO DO ####
Fill in `image nms_image(image R, int w)`.

## 1.4 Compute the descriptors ##

Code for calculating the descriptor for a (corner) pixel is provided in the `make_descriptor()` function in `panorama_helpers.c`. It consists of the pixel values in a 5x5 neighborhood, i.e. a descriptor is an array of size 5x5x#channels. The center pixel value is subtracted, which gives us some small amount of invariance to lighting conditions. The `descriptor` structure is defined in `image.h`.

#### TO DO ####
Fill in the missing sections of `descriptor *harris_corner_detector(image im, float sigma, float thresh, int nms, int *n)`. The function should return an array of descriptors for corners in the image `im`. You need to call the `make_descriptor()` function for every corner pixel (every pixel in the NMS-ed response map whose value exceeds the given `thresh`old). The integer `*n` is set to be the number of corners found in the image.

After you complete this function you should be able to calculate corners and their descriptors for an image! Try running the python code in `tryhw3.py` that uses `find_and_mark_corners()` defined in `panorama_helpers.c`:

    im = load_image("data/Rainier1.png")
    find_and_mark_corners(im, 2, 50, 3)
    save_image(im, "corners")

This will detect corners using a Gaussian window of sigma = 2, a "cornerness" threshold of 100, and an nms distance of 3 (or window of 7x7). It should give you something like this:

![rainier corners](../../figs/corners.jpg)

Corners are marked with the magenta colored "+" symbols. They seem pretty sensible! Lots of corners near where snow meets rock (edges). You can try playing with the different values to see how they affect our corner detector.

## 2. Matching descriptors ##

To get a panorama we have to match up the detected corners in one image with their appropriate counterparts in the other image.

## 2.1 Find the best matches from a to b ##

First we'll look through descriptors for `image a` (descriptor array `*a`) and find their best match with descriptors from `image b` (descriptor array `*b`). For comparing descriptors we'll use L1 distance. Squared error (L2 distance) can be problematic with outliers, we don't want a few rogue pixels to throw off our matching function. L1 distance (sum absolute difference) is better behaved with some outliers.

#### TO DO ####
Fill in the first `TODO` in `match *match_descriptors(descriptor *a, int an, descriptor *b, int bn, int *mn)`. 


## 2.2 Eliminate multiple matches to the same descriptor in b  ##

Each descriptor in `a` will only appear in one match. But several of them may match with the same descriptor in `b`. This can be problematic. Namely, if a bunch of matches go to the same point there is an easy homography to estimate that just shrinks the whole image down to one point to project from `a` to `b`. But we know that's wrong. So let's just get rid of these duplicate matches and make our matches be one-to-one.

To do this, we sort the matches based on distance so shortest distance is first. Next, you should loop through the matches in order and keep track of which elements in `b` we've seen. If we see an element more than once, throw it out! Check the pseudocode in the starter code for better understanding.

#### TO DO ####
Fill in the second `TODO` in `match *match_descriptors(descriptor *a, int an, descriptor *b, int bn, int *mn)`. Once this is done we can show the matches we discover between the images:

    a = load_image("data/Rainier1.png")
    b = load_image("data/Rainier2.png")
    m = find_and_mark_matches(a, b, 2, 50, 3)
    save_image(m, "matches")

Which gives you:

![matches](../../figs/matches.jpg)


## 3. Run RANSAC to compute the homography ##

Now that we have some matches we need to predict the projection between these two sets of points! However, this can be hard because we have a lot of noisy matches. Many of them are correct but we also have some outliers hiding in the data.

## 3.1 Projecting points with a homography ##

#### TO DO ####
Implement `point project_point(matrix H, point p)` to project a point `p` using matrix `H`. The matrix multiplication portion is already implemented, simply compute the homogenous coordinates as written in the code.

## 3.2 Calculate model inliers ##

Compute how many matches are inliers. We also bring inliers to the front of the array of matches; this helps us later on with the fitting functions.

#### TO DO ####
Fill in `int model_inliers(matrix H, match *m, int n, float thresh)` to loop over each matching point in one image, project it using the homography `H`, and check if the projected point is within some `thresh`old distance of the actual matching point in the other image. Use L2 distance between two points. A detailed pseudocode is written in the starter code.

## 3.3 Implement RANSAC ##

Implement the RANSAC algorithm as discussed in class and the homework slides. A detailed pseudocode is written in the starter code.

#### TO DO ####
Fill in `matrix RANSAC(match *m, int n, float thresh, int k, int cutoff)`.

## 4. Combine the images with a homography ##

Now we have to stitch the images together with our homography!

Some of this is already filled in. The first step is to figure out the bounds to the image. To do this we'll project the corners of `b` back onto the coordinate system for `a` using `Hinv` (matrix inverse of homography `H`. Then we can figure out our "new" coordinates and where to put `image a` on the canvas. Paste `a` in the appropriate spot in the canvas.

Next we need to loop over pixels that might map to `image b`, perform the mapping to see if they do, and if so fill in the pixels with the appropriate color from `b`. Our mapping will likely land between pixel values in `b` so use bilinear interpolation to compute the pixel value (use your implementation from HW1).

#### TO DO ####
Fill in `image combine_images(image a, image b, matrix H)`. With all this working you should be able to create some basic panoramas:

    im1 = load_image("data/Rainier1.png")
    im2 = load_image("data/Rainier2.png")
    pan = panorama_image(im1, im2, thresh=50, draw=1)
    save_image(pan, "panorama")

![panorama](../../figs/easy_panorama.jpg)

## 5. Extra Credit ##

Try out some of the other panorama creation in `rainier_panorama()` in `tryhw3.py`. If you stitch together multiple images you should set `draw=0` to prevent drawing corners (inliers) on the stitched image.

#### TO DO ####
Include the final panorama in your report.

## 6. Super Extra Credit ##

Mapping all the images back to the same coordinates is bad for large field-of-view panoramas, as discussed in class. Use cylindrical projection to project an image to cylindrical coordinates and then unroll it. The pseudocode is given below:

    Compute center of projection (xc,yc) = center of image
    Create a new image of same height and #channels as input image but of width = image width - 2 * x_offset
        here x_offset = atan((0.0 - xc)/f)*f + xc + 1 is an integer
    For each pixel of input image:
        compute x', y' using the equations in the lecture slides; offset x-coordinate by x_offset
        find pixel value at x',y' of input image using bilinear interpolation
        assign it to the corresponding pixel in the new image
    Return the new image

#### TO DO ####
Implement `image cylindrical_project(image im, float f)`. Then stitch together some very big panoramas using `field_panorama()` in `tryhw3.py` and include the panorama in your report. Use your own images if you want!

## Turn it in ##

Turn in your `panorama_image.c` and `report.pdf` on canvas under Homework 3. Your report should contain:
- Page 1: `inliers.jpg`, followed by `panorama.jpg`; created by running `tryhw3.py`.
- Page 2 (optional): `rainier_panorama_5.jpg` if you attempt extra credit.
- Page 3 (optional): `field_panorama_5.jpg` if you attempt super extra credit.
