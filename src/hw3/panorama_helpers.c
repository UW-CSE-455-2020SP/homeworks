#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <time.h>
#include "image.h"
#include "matrix.h"

/*********************************************
**********************************************
FILE CONTAINING HELPER FUNCTIONS FOR CREATING PANORAMAS
**********************************************
*********************************************/


point make_point(float x, float y)
/*********************************************
Creates a 2-D point using the structure "point" defined in image.h
*********************************************/
{
    point p;
    // Set the x,y elements of the point "p" to the inputs "x" and "y"
    p.x = x; p.y = y;
    return p;
}


descriptor make_descriptor(image im, int i)
/*********************************************
Creates a feature descriptor for an image pixel.
Here "im" is the image and "i" is the index for the pixel.
*********************************************/
{
    // Create an empty descriptor; the "descriptor" structure is defined in "image.h"
    descriptor d;
    // Descriptor contains an element "p" which is a "point" structure with x,y coordinates
    // Compute x,y coordinates of the descriptor from the input index 
    d.p.x = i % im.w;
    d.p.y = i / im.w;
    // Descriptor contains an element "n" which is the descriptor array size; assign it
    // We will take a 5x5 neighborhood of a pixel
    int w = 5;
    d.n = w*w*im.c;
    // Descriptor contains an element "data" which is the descriptor array; initialize it
    d.data = calloc(w*w*im.c, sizeof(float));
    
    int count = 0;
    // Loop over the channels
    for(int c = 0; c < im.c; ++c){
    	// Get pixel value as center value
    	float centerval = get_pixel(im, d.p.x, d.p.y, c);
    	// Loop over all neighboring pixels
        for(int dx = -w/2; dx < (w+1)/2; ++dx){
            for(int dy = -w/2; dy < (w+1)/2; ++dy){
            	// Get neighboring pixel value
                float val = get_pixel(im, d.p.x + dx, d.p.y + dy, c);
                // Subtract the neighboring pixel value from the central value to compensate for exposure/lighting changes.
                d.data[count++] = centerval - val;
            }
        }
    }
    return d;
}


void free_descriptors(descriptor *d, int n)
/**************************************************
Frees the memory occupied by an array of descriptors "d" of size "n"
**************************************************/
{
    for(int i = 0; i < n; ++i){
        free(d[i].data);
    }
    free(d);
}

void mark_corners(image im, descriptor *d, int n)
/**************************************************
Mark each corner in the image "im" with a magenta colored "+" symbol.
d: array of descriptors of corners
n: number of corners
**************************************************/
{
    // Loop over each corner pixel
    for(int count = 0; count < n; ++count){
        // Get x, y coordinates of the corner pixel
    	int x = d[count].p.x;
    	int y = d[count].p.y;
    	// color the neighboring pixels
	    for(int i = -9; i <= 9; ++i){
	    	// Set R,G,B values of the horizontal neighbors to Magenta
	        set_pixel(im, x+i, y, 0, 1);
	        set_pixel(im, x+i, y, 1, 0);
	        set_pixel(im, x+i, y, 2, 1);
	        // Set R,G,B values of the vertical neighbors to Magenta
	        set_pixel(im, x, y+i, 0, 1);	        
	        set_pixel(im, x, y+i, 1, 0);	        
	        set_pixel(im, x, y+i, 2, 1);
	    }
    }
}


void find_and_mark_corners(image im, float sigma, float thresh, int nms)
/**************************************************
Find corners in image "im" using Harris corner detector and mark them.
float sigma: std. dev for harris corner detection
float thresh: threshold for cornerness in harris corner detection
int nms: distance to look for local-maxes in response map in harris corner detection
**************************************************/
{
    int n = 0;
    descriptor *d = harris_corner_detector(im, sigma, thresh, nms, &n);
    mark_corners(im, d, n);
}


image mark_matches(image a, image b, match *matches, int n, int inliers)
/*********************************************
Draws lines between matching pixels in two images "a" and "b".
match *matches: array of matches between "a" and "b"
The "match" structure is defined in "image.h"
int n: number of matches
int inliers: number of inliers at the beginning of "matches"
*********************************************/
{
    // image with both "a" and "b" side-by-side
    image both = make_image(a.w + b.w, a.h > b.h ? a.h : b.h, a.c > b.c ? a.c : b.c);
    int i,j,k;
    // Set 1st part of "both" to "a"
    for(k = 0; k < a.c; ++k){
        for(j = 0; j < a.h; ++j){
            for(i = 0; i < a.w; ++i){
                set_pixel(both, i, j, k, get_pixel(a, i, j, k));
            }
        }
    }
    // Set 2nd part of "both" to "b"
    for(k = 0; k < b.c; ++k){
        for(j = 0; j < b.h; ++j){
            for(i = 0; i < b.w; ++i){
                set_pixel(both, i+a.w, j, k, get_pixel(b, i, j, k));
            }
        }
    }
    // For each match..
    for(i = 0; i < n; ++i){
    	// Get x,y coordinates of match
        int px = matches[i].p.x, py = matches[i].p.y;
        int qx = matches[i].q.x, qy = matches[i].q.y; 

        for(j = px; j < qx + a.w; ++j){
            int r = (float)(j-px)/(qx + a.w - px) * (qy - py) + py;

            // Set the R,G,B values of the inlier matches to green and remaining matches to red
            set_pixel(both, j, r, 0, i<inliers?0:1);
            set_pixel(both, j, r, 1, i<inliers?1:0);
            set_pixel(both, j, r, 2, 0);
        }
    }
    return both;
}


image find_and_mark_matches(image a, image b, float sigma, float thresh, int nms)
/*********************************************
Find corners, match them, and mark them between two images "a" and "b".
float sigma: gaussian filter size for harris corner detector. Typical: 2
float thresh: threshold for corner detection from response map. Typical: 1-5
int nms: neighborhood size to perform nms on. Typical: 3
*********************************************/
{
    int an = 0, bn = 0, mn = 0;
    // Find corners in images "a" and "b"
    descriptor *ad = harris_corner_detector(a, sigma, thresh, nms, &an);
    descriptor *bd = harris_corner_detector(b, sigma, thresh, nms, &bn);

    // Find matches
    match *m = match_descriptors(ad, an, bd, bn, &mn);

    // Mark corners and matches
    mark_corners(a, ad, an);
    mark_corners(b, bd, bn);
    image lines = mark_matches(a, b, m, mn, 0);

    free_descriptors(ad, an);
    free_descriptors(bd, bn);
    free(m);
    return lines;
}


int match_compare(const void *a, const void *b)
/*********************************************
Comparator for matches. The "match" structure is defined in "image.h".
const void *a, *b: pointers to the matches to compare
returns: result of comparison, 0 if same, 1 if a > b, -1 if a < b
*********************************************/
{
    match *ra = (match *)a;
    match *rb = (match *)b;
    if (ra->distance < rb->distance) return -1;
    else if (ra->distance > rb->distance) return  1;
    else return 0;
}


matrix compute_homography(match *matches, int mn, int n)
/**************************************************
Computes homography between two images using "n" random matches from an array "matches" of "mn" matches.
**************************************************/
{
    matrix M = make_matrix(n*2, 8);
    matrix b = make_matrix(n*2, 1);
	int index;

    for(int i = 0; i < n; ++i){
        // Select a random match and get the x,y coordinates
        if (n==4) index = rand()%mn;
        else index = i;
        double px = matches[index].p.x, py = matches[index].p.y;
        double qx = matches[index].q.x, qy = matches[index].q.y;

        // Fill in the matrices M and b.
        M.data[i*2][0] = px;
        M.data[i*2][1] = py;
        M.data[i*2][2] = 1;
        M.data[i*2][6] = -px*qx;
        M.data[i*2][7] = -py*qx;
        
        M.data[i*2+1][3] = px;
        M.data[i*2+1][4] = py;
        M.data[i*2+1][5] = 1;
        M.data[i*2+1][6] = -px*qy;
        M.data[i*2+1][7] = -py*qy;
        
        b.data[i*2][0] = qx;
        b.data[i*2+1][0] = qy;
    }

    // Find the solution to the equation Ma = b
    matrix a = solve_system(M, b);
    free_matrix(M); free_matrix(b); 

    // If a solution can't be found, return empty matrix;
    matrix none = {0};
    if(!a.data) return none;

    matrix H = make_matrix(3, 3);
    // Fill in the homography H based on the result in a.
    H.data[0][0] = a.data[0][0]; H.data[0][1] = a.data[1][0]; H.data[0][2] = a.data[2][0];
    H.data[1][0] = a.data[3][0]; H.data[1][1] = a.data[4][0]; H.data[1][2] = a.data[5][0];
    H.data[2][0] = a.data[6][0]; H.data[2][1] = a.data[7][0]; H.data[2][2] = 1;

    free_matrix(a);
    return H;
}


image panorama_image(image a, image b, float sigma, float thresh, int nms, float inlier_thresh, int iters, int cutoff, int draw)
/**************************************************
Create a panorama between two images "a" and "b".
float sigma: gaussian filter size for harris corner detector. Typical: 2
float thresh: threshold for corner detection in harris corner detector. Typical: 1-5
int nms: neighborhood size to perform nms on. Typical: 3
float inlier_thresh: threshold for RANSAC inliers. Typical: 2-5
int iters: number of RANSAC iterations. Typical: 1,000-50,000
int cutoff: RANSAC inlier cutoff. Typical: 10-100
**************************************************/
{
    srand(10);
    int an = 0, bn = 0, mn = 0;
    
    // Calculate descriptors of corners
    descriptor *ad = harris_corner_detector(a, sigma, thresh, nms, &an);
    descriptor *bd = harris_corner_detector(b, sigma, thresh, nms, &bn);

    // Find matches
    match *m = match_descriptors(ad, an, bd, bn, &mn);

    // Run RANSAC to find the homography
    matrix H = RANSAC(m, mn, inlier_thresh, iters, cutoff);

    if(draw){
        // Mark corners and matches between images and save the image
        mark_corners(a, ad, an);
        mark_corners(b, bd, bn);
        int inliers = model_inliers(H, m, mn, thresh);
        image inlier_matches = mark_matches(a, b, m, mn, inliers);
        save_image(inlier_matches, "inliers");
    }

    free_descriptors(ad, an);
    free_descriptors(bd, bn);
    free(m);

    // Stitch the images together with the homography
    image combined = combine_images(a, b, H);
    return combined;
}
