#ifndef SHARED_MEM_H_
#define SHARED_MEM_H_

typedef struct
{
    float iref;
} shared_mem_m2c_t;

typedef struct
{
    float i_measured;
} shared_mem_c2m_t;

void shared_mem_init( void );

extern shared_mem_m2c_t shm_m2c_read;
extern shared_mem_c2m_t shm_c2m_write;

#endif /*SHARED_MEM_H_*/
