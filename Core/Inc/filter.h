#ifndef INC_FILTER_H_
#define INC_FILTER_H_

#include<stdint.h>

typedef struct
{
	uint32_t data[50];
    uint32_t pointer;
    uint32_t size;
} filter_str;

void filter_init(filter_str *b);
void filter_add_data(filter_str *b, uint32_t new_data);
uint32_t average(filter_str *b);

#endif /* INC_FILTER_H_ */
