#ifndef IIR_FILTER_H_
#define IIR_FILTER_H_


#define BIQUAD_N_COEFS  6

typedef struct {
    float *w;
    float *coefs;
    float nsect;
} iir_biquad_t;

int iir_filters_init( void );
float iir(float in, iir_biquad_t *filt);

extern iir_biquad_t iir_in;

#endif /*IIR_FILTER_H_*/
