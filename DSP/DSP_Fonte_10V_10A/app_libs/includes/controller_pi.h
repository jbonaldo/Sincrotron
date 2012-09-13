

typedef struct {
    float ref;
    float up;
    float ui;
    float u;
    float kp;
    float ki;
    float max;
} pi_controller_t;

typedef struct {
    float value;
    float value_limited;
    float delta_max;
} d_dt_t; 

extern pi_controller_t pi_io;

void pi_controller_init(float sample_freq);
float pi_controller(float in, pi_controller_t *pi);
void pi_set_ref( float ref );
float rate_limiter( float in, d_dt_t *d_dt);
