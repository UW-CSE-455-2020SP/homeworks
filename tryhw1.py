from uwimg import *

###############################################################################
# Testing your resizing function with nearest neighbor interpolation 
# to 4 times of the original image
###############################################################################
im = load_image("data/dogsmall.jpg")
a = nn_resize(im, im.w*4, im.h*4)
save_image(a, "dog4x-nn")

###############################################################################
# Testing your resizing function with bilinear interpolation 
# to 4 times of the original image
###############################################################################
im = load_image("data/dogsmall.jpg")
a = bilinear_resize(im, im.w*4, im.h*4)
save_image(a, "dog4x-bl")

###############################################################################
# Testing your resizing function with nearest neighbor interpolation 
# to 1/7th of the original image
###############################################################################
im = load_image("data/dog.jpg")
a = nn_resize(im, im.w//7, im.h//7)
save_image(a, "dog7th-bl")