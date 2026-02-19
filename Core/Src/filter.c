#include "filter.h"

void filter_init(filter_str *b)
{
	b->pointer = 0;
	b->size = 50;

	for (int i=0; i < b->size; i++)
	{
		b->data[i] = 0;
	}
}

void filter_add_data(filter_str *b, uint32_t new_data)
{
	b->data[b->pointer] = new_data;
	b->pointer++;
	b->pointer = b->pointer % b->size;
}

uint32_t average(filter_str *b)
{
	uint32_t sum = 0;

	for (int i = 0; i < b->size; i++)
	{
	  sum +=*(b->data + i);
	}

	return (sum/b->size);
}
