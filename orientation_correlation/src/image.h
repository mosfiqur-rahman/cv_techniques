/*
 ============================================================================
 Name          : image.h
 Author        : mosfiqur-rahman
 Version      : 1.0
 Copyright   : © 2016 MOSFIQUR RAHMAN ALL RIGHTS RESERVED
 ============================================================================
 */

#ifndef __IMAGE_H__
#define __IMAGE_H__

struct dimage {
   double *img ;
   int width, height ;
} ;

struct point {
   int x, y ;
} ;

double *di_dx( double *img, int width, int height ) ;
double *di_dy( double *img, int width, int height ) ;
double *grad_mag( double *img, int width, int height ) ;
void grad_dir( double *img, int width, int height, double **px, double **py ) ;
struct dimage *orient_corr( struct dimage *img1, struct dimage *img2 ) ;

#endif
