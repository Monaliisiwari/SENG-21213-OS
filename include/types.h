#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <stddef.h>

#ifndef __bool_true_false_are_defined
#define __bool_true_false_are_defined 1
#ifndef __cplusplus
#define bool _Bool
#define true 1
#define false 0
#else
typedef uint8_t bool;
#endif
#endif

#endif
