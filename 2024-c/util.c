/** 
 * @file util.c
 * @author Pedro Paccola
 * 
 * This file contains a few utilities created for the Advent of Code 2024 event.
 */

#include <stdio.h>
#include <stdlib.h>
#include "util.h"

/**
 * Array Functions
*/

/**
 * Allocates an array of int64_t integers.
 *
 * @param[in] capacity The capacity of the array.
 * @return Returns NULL if memory allocation fails, 
 * otherwise, return a pointer to an Int64Array.
*/
Int64Array* int64array_new(uint32_t capacity)
{
	Int64Array *array = (Int64Array*)malloc(sizeof(Int64Array));
	if (array == NULL)
	{
		return NULL;
	}
	array->data = (int64_t*)malloc(sizeof(int64_t) * capacity);
	if (array->data == NULL)
	{
		free(array);
		return NULL;
	}
	array->capacity = capacity;
	array->size = 0;
	return array;
}

/**
 * Frees an array of int64_t integers.
 *
 * @param[out] array The Int64Array to be freed.
*/
void int64array_free(Int64Array *array)
{
	free(array->data);
	free(array);
}

/**
 * Prints the elements of an array of int64_t integers.
 *
 * @param[in] array The Int64Array to be printed.
*/
void int64array_print(Int64Array *array)
{
	printf("Size: %d, Capacity: %d, ", array->size, array->capacity);
	printf("Array: [");
	for (uint32_t i = 0; i < array->size; i++)
	{
		printf("%ld", array->data[i]);
		if (i < array->size - 1)
		{
			printf(", ");
		}
	}
	printf("]\n");
}

/**
 * Puts an item into an int64_t array sorted in ascending order.
 * Returns if array is full or NULL.
 *
 * @param[out] array The Int64Array.
 * @param[in]  item  The int64_t integer to be appended.
*/
void int64array_put_sorted(Int64Array *array, int64_t item)
{
	if (array == NULL || array->size >= array->capacity)
	{
		return;
	}

	uint32_t i;	

	for (i = array->size; i > 0 && array->data[i-1] > item; i--)
	{
		array->data[i] = array->data[i-1];
	}
	
	array->data[i] = item;

	array->size++;
}

/*
 * String Functions
*/

/**
 * Checks if char 'c' is a digit.
 *
 * @param[in] c Character to be checked.
*/
uint8_t is_digit(char c)
{
	return c >= '0' && c <= '9';
}