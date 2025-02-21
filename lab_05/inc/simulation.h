#ifndef SIMULATION_H
#define SIMULATION_H

#include "common.h"

#define CLOCK_RESOLUTION 0.001

error_codes_e run_simulation(void *queue_ptr, error_codes_e (*push)(void *, task_t), error_codes_e (*pop)(void *, task_t *), bool (*is_empty)(void *), size_t (*get_queue_len)(void *), simulation_params_t *params);

#endif /* SIMULATION_H */
