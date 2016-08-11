/*
 ============================================================================
 Name          : orientation_correlation.c
 Author        : mosfiqur-rahman
 Version      : 1.0
 Copyright   : © 2016 MOSFIQUR RAHMAN ALL RIGHTS RESERVED
 Description : Implementing  a correlation of two images using the techniques of  orientation correlation
 ============================================================================
 */

#include <stdlib.h>
#include <fftw3.h>
#include "image.h"

static int initialized = 0 ;

struct dimage *orient_corr( struct dimage *img1, struct dimage *img2 )
{
   fftw_plan p1, p2, p3 ;
   fftw_complex *f, *g ;
   struct dimage *ret ;
   double *px1, *py1, *px2, *py2 ;
   double re, im ;
   int i ;

   if( img1->width != img2->width || img1->height != img2->height )
      return( 0 ) ;
   grad_dir( img1->img, img1->width, img1->height, &px1, &py1 ) ;
   grad_dir( img2->img, img2->width, img2->height, &px2, &py2 ) ;
   f = fftw_malloc( img1->width * img1->height * sizeof( fftw_complex )) ;
   g = fftw_malloc( img2->width * img2->height * sizeof( fftw_complex )) ;
   if( !px1 || !py1 || !px2 || !py2 || !f || !g ) {
      ret = NULL ;
      goto odone ;
   }
   ret = (struct dimage *)malloc( sizeof( struct dimage )) ;
   if( !ret ) {
      goto odone ;
   }
   ret->img = malloc( img1->width * img1->height * sizeof( double )) ;
   if( !ret->img ) {
      free( ret ) ;
      ret = NULL ;
      goto odone ;
   }
   ret->width = img1->width ;
   ret->height = img1->height ;
   if( !initialized ) {
      fftw_import_system_wisdom() ;
      initialized = 1 ;
   }
   p1 = fftw_plan_dft_2d( img1->width, img1->height, f, f,
      FFTW_FORWARD, FFTW_ESTIMATE ) ;
   p2 = fftw_plan_dft_2d( img2->width, img2->height, g, g,
      FFTW_FORWARD, FFTW_ESTIMATE ) ;
   p3 = fftw_plan_dft_2d( img1->width, img1->height, f, f,
      FFTW_BACKWARD, FFTW_ESTIMATE ) ;
  for( i = 0; i < img1->width * img1->height; ++i ) {
      f[i][0] = px1[i] ;
      f[i][1] = py1[i] ;
      g[i][0] = px2[i] ;
      g[i][1] = py2[i] ;
   }
   fftw_execute( p1 ) ;
   fftw_execute( p2 ) ;
   for( i = 0; i < img1->width * img1->height; ++i ) {
      re = ( f[i][0] * g[i][0] ) - ( f[i][1] * -g[i][1] ) ;
      im = ( f[i][0] * -g[i][1] ) + ( f[i][1] * g[i][0] ) ;
      f[i][0] = re ;
      f[i][1] = im ;
   }
   fftw_execute( p3 ) ;
   for( i = 0; i < img1->width * img1->height; ++i ) {
      ret->img[i] = f[i][0] ;
   }
   fftw_destroy_plan( p1 ) ;
   fftw_destroy_plan( p2 ) ;
   fftw_destroy_plan( p3 ) ;
odone:
   if( px1 ) free( px1 ) ;
   if( px2 ) free( px2 ) ;
   if( py1 ) free( py1 ) ;
   if( py2 ) free( py2 ) ;
   if( f ) fftw_free( f ) ;
   if( g ) fftw_free( g ) ;
   return( ret ) ;
}
