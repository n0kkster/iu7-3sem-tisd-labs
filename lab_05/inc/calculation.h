#ifndef CALCULATION_H
#define CALCULATION_H

#include <math.h>
#include "common.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))

error_codes_e run_calculation(void *queue_ptr, error_codes_e (*push)(void *, task_t), error_codes_e (*pop)(void *, task_t *), bool (*is_empty)(void *), size_t (*get_queue_len)(void *), simulation_params_t *params);

#endif /* CALCULATION_H */
