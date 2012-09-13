
#include "includes/shared_mem.h"

shared_mem_m2c_t shm_m2c_read;
shared_mem_c2m_t shm_c2m_write;


//long int c28_r_array[2048];     // mapped to S2 of shared RAM owned by M3
//long int c28_r_w_array[2048];   // mapped to S0 of shared RAM owned by c28

#pragma DATA_SECTION(shm_m2c_read,"SHARERAMS2");
#pragma DATA_SECTION(shm_c2m_write,"SHARERAMS0");

void shared_mem_init( void )
{
    shm_c2m_write.i_measured = 2.222;    
}

