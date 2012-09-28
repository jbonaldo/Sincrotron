
#include "includes/shared_mem_arm.h"

shared_mem_m2c_t shm_m2c_write;
shared_mem_c2m_t shm_c2m_read;

int m3_r_array[2048]; // this array is mapped to S0

int m3_r_w_array[2048]; // this array is mapped to S2

#pragma DATA_SECTION(shm_c2m_read,"SHARERAMS0");
#pragma DATA_SECTION(shm_m2c_write,"SHARERAMS2");

void shared_mem_init( void )
{
    shm_m2c_write.iref = 0.1;
}

void shm_setPiRef( float ref )
{
    shm_m2c_write.iref = ref;
}

float shm_getIout( void )
{
    return shm_c2m_read.i_measured;
}
