/**
 *   Desenvolvido por Jakson Bonaldo
 *   @02/08/2012
 */

#include "includes/iir_filter.h"

float w1[] = { 0, 0 }; 
float coefs1[2][3] = {                                                                 
                       { 0.053802079102160,   0.107604158204319,   0.053802079102160 }, //a, coefs parte x
                       { 1.000000000000000,  -1.245218912390424,   0.460427228799062 }  //b, coefs parte y
                     };

float a[] = { 0.053802079102160,   0.107604158204319,   0.053802079102160 }; //a, coefs parte x
float b[] = { 1.000000000000000,  -1.245218912390424,   0.460427228799062 };  //b, coefs parte y
float w[] = {0, 0};
iir_biquad_t iir_in;

int iir_filters_init( void )
{
    iir_in.coefs = (float *) coefs1;
    iir_in.w     = (float *) w1;
    iir_in.nsect = 1;
    
    return 0;
}
 
float iir(float in, iir_biquad_t *filt)
{
    //float *w, *coef_a, *coef_b;
    float temp;
    float y;
    int i;
    
    //w = filt->w;
    //coef_a = &(filt->coefs[0]);
    //coef_b = &(filt->coefs[3]);
    //coef_a = coefs1[0];
    //coef_b = coefs1[1];
    
  //  for(i = 0; i < filt->nsect; i++) 
   // {

        temp = in - b[1]*w[0] - b[2]*w[1];
        y = a[0]*temp + a[1]*w[0] + a[2]*w[1];

        w[1] = w[0];
        w[0] = temp;
        
    //    coef_a += BIQUAD_N_COEFS;
    //    coef_b += BIQUAD_N_COEFS;
    //}
    
    return y;
}
