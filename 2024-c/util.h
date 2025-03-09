#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

/* 
 * Array of int64_t elements
 * @param *data int64_t
 * @param size uint32_t
 * @param capacity uint32_t
 */
typedef struct {
	int64_t *data;
	uint32_t size;
	uint32_t capacity;
} Int64Array;

Int64Array* int64array_new(uint32_t capacity);
void int64array_print(Int64Array *array);
void int64array_free(Int64Array *array);
void int64array_put_sorted(Int64Array *array, int64_t item);

uint8_t is_digit(char c);

#endif