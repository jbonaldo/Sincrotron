#ifndef SHARED_MEM_ARM_H_
#define SHARED_MEM_ARM_H_

typedef struct
{
    float iref;
} shared_mem_m2c_t;

typedef struct
{
    float i_measured;
} shared_mem_c2m_t;

void shared_mem_init( void );
void shm_setPiRef( float ref );
float shm_getIout( void );

#endif /*SHARED_MEM_ARM_H_*/
