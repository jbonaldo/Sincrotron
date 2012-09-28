/**
 *   Desenvolvido por Jakson Bonaldo
 *   @02/08/2012
 */

#include "includes/iir_filter.h"

#pragma CODE_SECTION(iir, "ramfuncs");

#define N_SECT  1
float w[N_SECT][2]; 
float coefs[1][2][3] = {
                         {                                                                 
                            { 0.2928932309,   2.0*0.2928932309,   0.2928932309 }, //a, coefs parte x
                            { 1, -1.300707235e-16,    0.171572879 }  //b, coefs parte y
                         }
                       };  

iir_biquad_t iir_in;

int iir_filters_init( void )
{
    int i;
    
    iir_in.coefs = (float *) coefs;
    iir_in.w     = (float *) w;
    iir_in.nsect = N_SECT;
    
    for(i=0; i<(iir_in.nsect * 2); i++)
        iir_in.w[i] = 0;
    
    return 0;
}
 
float iir_cascade(float in, iir_biquad_t *filt)
{
    float *w, *coef_a, *coef_b;
    float temp, temp_w;
    float y;
    int i;
    
    w = filt->w;
    coef_a = &(filt->coefs[0]);
    coef_b = &(filt->coefs[3]);
    
    temp = in;
    for(i = 0; i < filt->nsect; i++) 
    {

        //temp = in - b[1]*w[0] - b[2]*w[1];
        //y = a[0]*temp + a[1]*w[0] + a[2]*w[1];
        //w[1] = w[0];
        //w[0] = temp;

        temp_w = temp - coef_b[1]*w[0] - coef_b[2]*w[1];
        temp = coef_a[0]*temp_w + coef_a[1]*w[0] + coef_a[2]*w[1];

        w[1] = w[0];
        w[0] = temp_w;
        
        coef_a += BIQUAD_N_COEFS;
        coef_b += BIQUAD_N_COEFS;
        w      += 2;
    }
    
    return temp;
}

float iir_parallel(float in, iir_biquad_t *filt)
{
    float *w, *coef_a, *coef_b;
    float temp_w;
    float y;
    int i;
    
    w = filt->w;
    coef_a = &(filt->coefs[0]);
    coef_b = &(filt->coefs[3]);
    
    y = 0;
    
    for(i = 0; i < filt->nsect; i++) 
    {

        //temp = in - b[1]*w[0] - b[2]*w[1];
        //y = a[0]*temp + a[1]*w[0] + a[2]*w[1];
        //w[1] = w[0];
        //w[0] = temp;

        temp_w = in - coef_b[1]*w[0] - coef_b[2]*w[1];
        y += coef_a[0]*temp_w + coef_a[1]*w[0] + coef_a[2]*w[1];

        w[1] = w[0];
        w[0] = temp_w;
        
        coef_a += BIQUAD_N_COEFS;
        coef_b += BIQUAD_N_COEFS;
        w      += 2;
    }
    
    return y;
}
