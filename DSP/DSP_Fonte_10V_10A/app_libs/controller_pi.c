/**
 * Controlador PI
 * 
 * PI(S) = Kp + Ki/s;
 * 
 * ki_dig = Ki * Ts;
 * 
 */
 
#include <math.h> 
#include "DSP28x_Project.h"
#include "F28M35x_EPwm_defines.h"  
#include "includes/controller_pi.h"
#include "../config.h"

#pragma CODE_SECTION(pi_controller, "ramfuncs");
#pragma CODE_SECTION(rate_limiter, "ramfuncs");

#define saturation(obj, limite)         \
    if(obj > limite)  obj =  limite;    \
    if(obj < -limite) obj = -limite;    

#define L_MAGNET            0.025
#define VDC_MAX             15.0
#define DI_DT_MAX           (VDC_MAX / L_MAGNET)
#define DELTA_I_MAX(FREQ)   (DI_DT_MAX / (float)FREQ)

#define DELTA_OUT_MAX   0.1     //pu de corrente

d_dt_t i_ref;
d_dt_t out; 
pi_controller_t pi_io;
Uint16 DebugErro_di_dt = 0;

float pi_controller(float in, pi_controller_t *pi)
{
    float erro;
    float dynamic_lim;
    float temp;
    
    erro = rate_limiter(pi->ref, &i_ref) - in;
    //erro = pi->ref - in;
    
    temp = erro * pi->kp;
    saturation(temp, pi->max);
    pi->up = temp;
    
    if(temp < 0.0)
        temp = -temp;    
    dynamic_lim = (pi->max - temp);

    temp = pi->ui + erro * pi->ki;
    saturation(temp, dynamic_lim);
    pi->ui = temp;
    
    temp = pi->ui + pi->up;
    
    //pi->u = rate_limiter(temp, &out);
    pi->u = temp;
    return pi->u;
}

void pi_controller_init(float sample_freq)
{
//    pi_io.kp = 2;
//    pi_io.ki = 1000 / (float) sample_freq;
    pi_io.kp = 2.5;
    pi_io.ki = 4808 / (float) sample_freq;
    pi_io.max = 0.95;
    pi_io.ui = 0;
    pi_io.up = 0;
    pi_io.ref = 0.0;
    
    i_ref.value = 0;
    i_ref.value_limited = 0;
    i_ref.delta_max = DELTA_I_MAX(sample_freq);
    out.value = 0;
    out.value_limited = 0;
    out.delta_max   = DELTA_OUT_MAX;
    
}

void pi_set_ref( float ref )
{
    pi_io.ref = ref;   
}


float rate_limiter( float in, d_dt_t *d_dt)
{
    float di;
    float di_abs;

    if(in != d_dt->value_limited)    
    {
        d_dt->value = in;
    
        di = d_dt->value - d_dt->value_limited;
        
        if(di > 0.0) di_abs = di;
        else         di_abs = -di; 
           
        if( di_abs > d_dt->delta_max)
        {
            if(di > 0)
                d_dt->value_limited +=  d_dt->delta_max;
            else
                d_dt->value_limited -=  d_dt->delta_max;
        }
        else
             d_dt->value_limited =  d_dt->value;
    }         
    return d_dt->value_limited;
}
