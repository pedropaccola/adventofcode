#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "util.h"

#define BASE 10
#define BUF 1024

int main(int argc, char *argv[])
{
	int64_t distance = 0, similarity = 0;
	int64_t value = 0;
	uint8_t digit_flag = 0;
	int ch;

	Int64Array *list_l = int64array_new(BUF);
	Int64Array *list_r = int64array_new(BUF);

	while ((ch = getchar ()) != EOF)
	{
		switch (ch)
		{
			case '\n':
				if (digit_flag != 0)
				{
					int64array_put_sorted(list_r, value);
					value = 0;
					digit_flag = 0;
				}
				break;
			case ' ':
				if (digit_flag != 0)
				{
					int64array_put_sorted(list_l, value);
					value = 0;
					digit_flag = 0;
				}
				break;
			default:
				value = BASE * value + (int64_t)(ch - '0');
				digit_flag = 1;
		}

	}


	// Distance
	for (uint32_t i = 0; i < list_l->size && i < list_r->size; i++)
	{
		int64_t diff = list_l->data[i] - list_r->data[i];
		distance += (diff >= 0) ? diff : -diff;
	}

	// Similarity
	for (uint32_t i = 0; i < list_l->size; i++)
	{
		uint64_t appearances = 0;
		for (uint32_t j = 0; j < list_r->size; j++)
		{
			if (list_l->data[i] == list_r->data[j])
			{
				appearances++;
			}
		}
		similarity += list_l->data[i] * appearances;
	}

	printf("Total distance: %ld\n", distance);
	printf("Total similarities: %ld\n", similarity);
	int64array_free(list_l);
	int64array_free(list_r);
	return 0;
}