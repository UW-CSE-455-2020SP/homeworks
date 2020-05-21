import sys, os
from ctypes import *
import math
import random

lib = CDLL(os.path.join(os.path.dirname(__file__), "visionlib.so"), RTLD_GLOBAL)

def c_array(ctype, values):
    arr = (ctype*len(values))()
    arr[:] = values
    return arr

class IMAGE(Structure):
    _fields_ = [("w", c_int),
                ("h", c_int),
                ("c", c_int),
                ("data", POINTER(c_float))]
    def __add__(self, other):
        return add_image(self, other)
    def __sub__(self, other):
        return sub_image(self, other)

class POINT(Structure):
    _fields_ = [("x", c_float),
                ("y", c_float)]

class DESCRIPTOR(Structure):
    _fields_ = [("p", POINT),
                ("n", c_int),
                ("data", POINTER(c_float))]

class MATRIX(Structure):
    _fields_ = [("rows", c_int),
                ("cols", c_int),
                ("data", POINTER(POINTER(c_double))),
                ("shallow", c_int)]

class DATA(Structure):
    _fields_ = [("X", MATRIX),
                ("y", MATRIX)]

class LAYER(Structure):
    _fields_ = [("in", MATRIX),
                ("dw", MATRIX),
                ("w", MATRIX),
                ("v", MATRIX),
                ("out", MATRIX),
                ("activation", c_int)]

class MODEL(Structure):
    _fields_ = [("layers", POINTER(LAYER)),
                ("n", c_int)]


(LINEAR, LOGISTIC, RELU, LRELU, SOFTMAX) = range(5)

make_image = lib.make_image
make_image.argtypes = [c_int, c_int, c_int]
make_image.restype = IMAGE

free_image = lib.free_image
free_image.argtypes = [IMAGE]

get_pixel = lib.get_pixel
get_pixel.argtypes = [IMAGE, c_int, c_int, c_int]
get_pixel.restype = c_float

set_pixel = lib.set_pixel
set_pixel.argtypes = [IMAGE, c_int, c_int, c_int, c_float]
set_pixel.restype = None

rgb_to_grayscale = lib.rgb_to_grayscale
rgb_to_grayscale.argtypes = [IMAGE]
rgb_to_grayscale.restype = IMAGE

copy_image = lib.copy_image
copy_image.argtypes = [IMAGE]
copy_image.restype = IMAGE

rgb_to_hsv = lib.rgb_to_hsv
rgb_to_hsv.argtypes = [IMAGE]
rgb_to_hsv.restype = None

clamp_image = lib.clamp_image
clamp_image.argtypes = [IMAGE]
clamp_image.restype = None

hsv_to_rgb = lib.hsv_to_rgb
hsv_to_rgb.argtypes = [IMAGE]
hsv_to_rgb.restype = None

shift_image = lib.shift_image
shift_image.argtypes = [IMAGE, c_int, c_float]
shift_image.restype = None

load_image_lib = lib.load_image
load_image_lib.argtypes = [c_char_p]
load_image_lib.restype = IMAGE

def load_image(f):
    return load_image_lib(f.encode('ascii'))

save_png_lib = lib.save_png
save_png_lib.argtypes = [IMAGE, c_char_p]
save_png_lib.restype = None

def save_png(im, f):
    return save_png_lib(im, f.encode('ascii'))

save_image_lib = lib.save_image
save_image_lib.argtypes = [IMAGE, c_char_p]
save_image_lib.restype = None

def save_image(im, f):
    return save_image_lib(im, f.encode('ascii'))

same_image = lib.same_image
same_image.argtypes = [IMAGE, IMAGE]
same_image.restype = c_int

##### HOMEWORK 1

nn_resize = lib.nn_resize
nn_resize.argtypes = [IMAGE, c_int, c_int]
nn_resize.restype = IMAGE

bilinear_resize = lib.bilinear_resize
bilinear_resize.argtypes = [IMAGE, c_int, c_int]
bilinear_resize.restype = IMAGE

make_box_filter = lib.make_box_filter
make_box_filter.argtypes = [c_int]
make_box_filter.restype = IMAGE

##### HOMEWORK 2

make_highpass_filter = lib.make_highpass_filter
make_highpass_filter.argtypes = []
make_highpass_filter.restype = IMAGE

make_emboss_filter = lib.make_emboss_filter
make_emboss_filter.argtypes = []
make_emboss_filter.restype = IMAGE

make_sharpen_filter = lib.make_sharpen_filter
make_sharpen_filter.argtypes = []
make_sharpen_filter.restype = IMAGE

add_image = lib.add_image
add_image.argtypes = [IMAGE, IMAGE]
add_image.restype = IMAGE

sub_image = lib.sub_image
sub_image.argtypes = [IMAGE, IMAGE]
sub_image.restype = IMAGE

make_gy_filter = lib.make_gy_filter
make_gy_filter.argtypes = []
make_gy_filter.restype = IMAGE

make_gx_filter = lib.make_gx_filter
make_gx_filter.argtypes = []
make_gx_filter.restype = IMAGE

sobel_image = lib.sobel_image
sobel_image.argtypes = [IMAGE]
sobel_image.restype = POINTER(IMAGE)

normalize_image = lib.l1_normalize
normalize_image.argtypes = [IMAGE]
normalize_image.restype = None

make_gaussian_filter = lib.make_gaussian_filter
make_gaussian_filter.argtypes = [c_float]
make_gaussian_filter.restype = IMAGE

convolve_image = lib.convolve_image
convolve_image.argtypes = [IMAGE, IMAGE, c_int]
convolve_image.restype = IMAGE

apply_median_filter = lib.apply_median_filter
apply_median_filter.argtypes = [IMAGE, c_int]
apply_median_filter.restype = IMAGE

##### HOMEWORK 3

structure_matrix = lib.structure_matrix
structure_matrix.argtypes = [IMAGE, c_float]
structure_matrix.restype = IMAGE

harris_corner_detector = lib.harris_corner_detector
harris_corner_detector.argtypes = [IMAGE, c_float, c_float, c_int, POINTER(c_int)]
harris_corner_detector.restype = POINTER(DESCRIPTOR)

mark_corners = lib.mark_corners
mark_corners.argtypes = [IMAGE, POINTER(DESCRIPTOR), c_int]
mark_corners.restype = None

find_and_mark_corners = lib.find_and_mark_corners
find_and_mark_corners.argtypes = [IMAGE, c_float, c_float, c_int]
find_and_mark_corners.restype = None

find_and_mark_matches = lib.find_and_mark_matches
find_and_mark_matches.argtypes = [IMAGE, IMAGE, c_float, c_float, c_int]
find_and_mark_matches.restype = IMAGE

cylindrical_project = lib.cylindrical_project
cylindrical_project.argtypes = [IMAGE, c_float]
cylindrical_project.restype = IMAGE

panorama_image_lib = lib.panorama_image
panorama_image_lib.argtypes = [IMAGE, IMAGE, c_float, c_float, c_int, c_float, c_int, c_int, c_int]
panorama_image_lib.restype = IMAGE

def panorama_image(a, b, sigma=2, thresh=5, nms=3, inlier_thresh=2, iters=10000, cutoff=30, draw=0):
    return panorama_image_lib(a, b, sigma, thresh, nms, inlier_thresh, iters, cutoff, draw)

##### HOMEWORK 4

draw_flow = lib.draw_flow
draw_flow.argtypes = [IMAGE, IMAGE, c_float]
draw_flow.restype = None

box_filter_image = lib.box_filter_image
box_filter_image.argtypes = [IMAGE, c_int]
box_filter_image.restype = IMAGE

optical_flow_images = lib.optical_flow_images
optical_flow_images.argtypes = [IMAGE, IMAGE, c_int, c_int]
optical_flow_images.restype = IMAGE

optical_flow_webcam = lib.optical_flow_webcam
optical_flow_webcam.argtypes = [c_int, c_int, c_int]
optical_flow_webcam.restype = None

##### HOMEWORK 5

train_model = lib.train_model
train_model.argtypes = [MODEL, DATA, c_int, c_int, c_double, c_double, c_double]
train_model.restype = None

accuracy_model = lib.accuracy_model
accuracy_model.argtypes = [MODEL, DATA]
accuracy_model.restype = c_double

forward_model = lib.forward_model
forward_model.argtypes = [MODEL, MATRIX]
forward_model.restype = MATRIX

load_classification_data = lib.load_classification_data
load_classification_data.argtypes = [c_char_p, c_char_p, c_int]
load_classification_data.restype = DATA

make_layer = lib.make_layer
make_layer.argtypes = [c_int, c_int, c_int]
make_layer.restype = LAYER

def make_model(layers):
    m = MODEL()
    m.n = len(layers)
    m.layers = (LAYER*m.n) (*layers)
    return m

if __name__ == "__main__":
    im = load_image("data/dog.jpg")
    save_image(im, "hey")
