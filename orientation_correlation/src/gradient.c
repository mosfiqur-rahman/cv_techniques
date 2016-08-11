/*
 ============================================================================
 Name          : gradient.c
 Author        : mosfiqur-rahman
 Version      : 1.0
 Copyright   : © 2016 MOSFIQUR RAHMAN ALL RIGHTS RESERVED
 Description : This module contains functions to compute the gradient of an  image.
 ============================================================================
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "image.h"

/**
 * @param img Pointer to the image
 * @param width Number of columns in the image
 * @param height Number of rows in the image
 * 
 * returns Pointer to the partial in the \a x direction or
 * NULL if the malloc fails.
 */
double *di_dx( double *img, int width, int height )
{
   double *der, *p, *q ;
   int i, j ;

   der = (double *)malloc( width * height * sizeof( double )) ;
   if( !der ) {
      return( NULL ) ;
   }
   p = der ;
   for( i = 0; i < height; ++i ) {
      *(p++) = 0.0 ;
      q = img + i * width ;
      for( j = 1; j < width - 1; ++j ) {
         *(p++) = *q - *(q+2) ;
         ++q ;
      }
      *(p++) = 0.0 ;
   }
   return( der ) ;
}

/**
 * @param img Pointer to the image
 * @param width Number of columns in the image
 * @param height Number of rows in the image
 * 
 * \returns Pointer to the partial in the \a y direction or
 * NULL if the malloc fails.
 */
double *di_dy( double *img, int width, int height )
{
   double *der, *p, *q ;
   int i, j ;

   der = (double *)malloc( width * height * sizeof( double )) ;
   if( !der ) {
      return( NULL ) ;
   }
   p = der ;
   for( j = 0; j < width; ++j ) {
      *(p++) = 0.0 ;
   }
   for( i = 1; i < height - 1; ++i ) {
      q = img + ( i - 1 ) * width ;
      for( j = 0; j < width; ++j ) {
         *(p++) = *q - *( q + 2 * width ) ;
         ++q ;
      }
   }
   for( j = 0; j < width; ++j ) {
      *(p++) = 0.0 ;
   }
   return( der ) ;
}

/**
 * @param img Pointer to the image
 * @param width Number of columns in the image
 * @param height Number of rows in the image
 * 
 * @returns Pointer to the magnitude of the gradient
 * or NULL if anything fails.
 */
double *grad_mag( double *img, int width, int height )
{
   double *px, *py, *mag ;
   double *p, *q, *r ;
   int i ;

   px = di_dx( img, width, height ) ;
   py = di_dy( img, width, height ) ;
   mag = (double *)malloc( width * height * sizeof( double )) ;
   if( !px || !py || !mag ) {
      if( px ) free( px ) ;
      if( py ) free( py ) ;
      if( mag ) free( mag ) ;
      return( NULL ) ;
   }
   p = mag ;
   q = px ;
   r = py ;
   for( i = 0; i < width * height; ++i ) {
      *(p++) = sqrt(( *r * *r ) + ( *q * *q )) ;
      ++r ;
      ++q ;
   }
   free( px ) ;
   free( py ) ;
   return( mag ) ;
}

/**
 * @param img Pointer to the image
 * @param width Number of columns in the image
 * @param height Number of rows in the image
 * 
 */
void grad_dir( double *img, int width, int height, double **px, double **py )
{
   double *q, *r ;
   double t ;
   int i ;

   q = *px = di_dx( img, width, height ) ;
   r = *py = di_dy( img, width, height ) ;
   if( !(*px) || !(*py)) {
      if( *px ) free( *px ) ;
      if( *py ) free( *py ) ;
      *px = *py = NULL ;
      return ;
   }
   for( i = 0; i < width * height; ++i, ++q, ++r ) {
      t = sqrt(( *r * *r ) + ( *q * *q )) ;
      if( t != 0.0 ) {
         *r /= t ;
         *q /= t ;
      }
   }
}
