#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include "matrix.h"
#include "image.h"
#include "test.h"
#include "args.h"


float avg_diff(image a, image b)
{
    float diff = 0;
    int i;
    for(i = 0; i < a.w*a.h*a.c; ++i){
        diff += b.data[i] - a.data[i];
    }
    return diff/(a.w*a.h*a.c);
}

image center_crop(image im)
{
    image c = make_image(im.w/2, im.h/2, im.c);
    int i, j, k;
    for(k = 0; k < im.c; ++k){
        for(j = 0; j < im.h/2; ++j){
            for(i = 0; i < im.w/2; ++i){
                set_pixel(c, i, j, k, get_pixel(im, i+im.w/4, j+im.h/4, k));
            }
        }
    }
    return c;
}

void feature_normalize2(image im)
{
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

int tests_total = 0;
int tests_fail = 0;

int within_eps(float a, float b, float eps){
    return a-eps<b && b<a+eps;
}

int same_point(point p, point q, float eps)
{
    return within_eps(p.x, q.x, eps) && within_eps(p.y, q.y, eps);
}

int same_matrix(matrix m, matrix n)
{
    if(m.rows != n.rows || m.cols != n.cols) return 0;
    int i,j;
    for(i = 0; i < m.rows; ++i){
        for(j = 0; j < m.cols; ++j){
            if(!within_eps(m.data[i][j], n.data[i][j], EPS)) return 0;
        }
    }
    return 1;
}

int same_image(image a, image b, float eps)
{
    int i;
    if(a.w != b.w || a.h != b.h || a.c != b.c) {
        //printf("Expected %d x %d x %d image, got %d x %d x %d\n", b.w, b.h, b.c, a.w, a.h, a.c);
        return 0;
    }
    for(i = 0; i < a.w*a.h*a.c; ++i){
        float thresh = (fabs(b.data[i]) + fabs(a.data[i])) * eps / 2;
        if (thresh > eps) eps = thresh;
        if(!within_eps(a.data[i], b.data[i], eps)) 
        {
            printf("The value should be %f, but it is %f! \n", b.data[i], a.data[i]);
            return 0;
        }
    }
    return 1;
}

void test_get_pixel(){
    image im = load_image("data/dots.png");
    // Test within image
    TEST(within_eps(0, get_pixel(im, 0,0,0), EPS));
    TEST(within_eps(1, get_pixel(im, 1,0,1), EPS));
    TEST(within_eps(0, get_pixel(im, 2,0,1), EPS));

    // Test padding
    TEST(within_eps(1, get_pixel(im, 0,3,1), EPS));
    TEST(within_eps(1, get_pixel(im, 7,8,0), EPS));
    TEST(within_eps(0, get_pixel(im, 7,8,1), EPS));
    TEST(within_eps(1, get_pixel(im, 7,8,2), EPS));
    free_image(im);
}

void test_set_pixel(){
    image gt = load_image("data/dots.png");
    image d = make_image(4,2,3);
    set_pixel(d, 0,0,0,0); set_pixel(d, 0,0,1,0); set_pixel(d, 0,0,2,0); 
    set_pixel(d, 1,0,0,1); set_pixel(d, 1,0,1,1); set_pixel(d, 1,0,2,1); 
    set_pixel(d, 2,0,0,1); set_pixel(d, 2,0,1,0); set_pixel(d, 2,0,2,0); 
    set_pixel(d, 3,0,0,1); set_pixel(d, 3,0,1,1); set_pixel(d, 3,0,2,0); 

    set_pixel(d, 0,1,0,0); set_pixel(d, 0,1,1,1); set_pixel(d, 0,1,2,0); 
    set_pixel(d, 1,1,0,0); set_pixel(d, 1,1,1,1); set_pixel(d, 1,1,2,1); 
    set_pixel(d, 2,1,0,0); set_pixel(d, 2,1,1,0); set_pixel(d, 2,1,2,1); 
    set_pixel(d, 3,1,0,1); set_pixel(d, 3,1,1,0); set_pixel(d, 3,1,2,1); 

    // Test images are same
    TEST(same_image(d, gt, EPS));
    free_image(gt);
    free_image(d);
}

void test_grayscale()
{
    image im = load_image("data/colorbar.png");
    image gray = rgb_to_grayscale(im);
    image gt = load_image("figs/gray.png");
    TEST(same_image(gray, gt, EPS));
    free_image(im);
    free_image(gray);
    free_image(gt);
}

void test_copy()
{
    image gt = load_image("data/dog.jpg");
    image c = copy_image(gt);
    TEST(same_image(c, gt, EPS));
    free_image(gt);
    free_image(c);
}

void test_clamp()
{
    image im = load_image("data/dog.jpg");
    image c = copy_image(im);
    set_pixel(im, 10, 5, 0, -1);
    set_pixel(im, 15, 15, 1, 1.001);
    set_pixel(im, 130, 105, 2, -0.01);
    set_pixel(im, im.w-1, im.h-1, im.c-1, -.01);

    set_pixel(c, 10, 5, 0, 0);
    set_pixel(c, 15, 15, 1, 1);
    set_pixel(c, 130, 105, 2, 0);
    set_pixel(c, im.w-1, im.h-1, im.c-1, 0);
    clamp_image(im);
    TEST(same_image(c, im, EPS));
    free_image(im);
    free_image(c);
}

void test_shift()
{
    image im = load_image("data/dog.jpg");
    image c = copy_image(im);
    shift_image(c, 1, .1);
    TEST(within_eps(c.data[0], im.data[0], EPS));
    TEST(within_eps(c.data[im.w*im.h + 13], im.data[im.w*im.h+13] + .1, EPS));
    TEST(within_eps(c.data[2*im.w*im.h + 72], im.data[2*im.w*im.h+72], EPS));
    TEST(within_eps(c.data[im.w*im.h + 47], im.data[im.w*im.h+47] + .1, EPS));
    free_image(im);
    free_image(c);
}

void test_rgb_to_hsv()
{
    image im = load_image("data/dog.jpg");
    rgb_to_hsv(im);
    image hsv = load_image("figs/dog.hsv.png");
    TEST(same_image(im, hsv, EPS));
    free_image(im);
    free_image(hsv);
}

void test_hsv_to_rgb()
{
    image im = load_image("data/dog.jpg");
    image c = copy_image(im);
    rgb_to_hsv(im);
    hsv_to_rgb(im);
    TEST(same_image(im, c, EPS));
    free_image(im);
    free_image(c);
}

void test_hw0()
{
    test_get_pixel();
    test_set_pixel();
    test_copy();
    test_shift();
    test_clamp();
    test_grayscale();
    test_rgb_to_hsv();
    test_hsv_to_rgb();
    printf("%d tests, %d passed, %d failed\n", tests_total, tests_total-tests_fail, tests_fail);
}