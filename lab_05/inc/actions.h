#ifndef ACTIONS_H
#define ACTIONS_H

#include <stdio.h>
#include <sys/time.h>
#include <math.h>

#define MEASURES 1000

#define ENSURE(x) {error_codes_e rc; if ((rc = x) != ERROR_SUCCESS) return rc; }

typedef enum
{
    EXIT,
    SET_SIMULATION_PARAMS,
    RUN_SIMULATION_ARR,
    RUN_SIMULATION_LIST,
    RUN_CALCULATION_ARR,
    RUN_CALCULATION_LIST,
    TOGGLE_VERBOSE_MODE,
    TOGGLE_PRINT_MEMORY_INFO,
    EFFICIENCY_TEST,
    ACTION_COUNT
} action_e;

#include "common.h"
#include "errors.h"

error_codes_e handle_set_params(simulation_params_t *params);
error_codes_e handle_efficiency_test(void);

#endif /* ACTIONS_H */
