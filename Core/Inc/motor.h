#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "pid.h"
#include "filter.h"

typedef struct
{
	char id;
	uint16_t resolution;
	int16_t pulse_count;
	float current;
	int32_t speed;
	int32_t set_speed;
	int32_t direction;
	int32_t actual_PWM;

	pid_str pid_controller;
	filter_str filter;
}serwo_str;

void motor_str_init(serwo_str *m, char identifier);
void motor_calculate_speed(serwo_str *m);
void motor_calculate_current(serwo_str *m);
void motor_set_speed(serwo_str *m, int16_t speed);

void motor_run_pid(serwo_str *m);

void right_motor_forward(uint16_t duty);
void right_motor_backward(uint16_t duty);
void right_motor_stop(void);

void left_motor_forward(uint16_t duty);
void left_motor_backward(uint16_t duty);
void left_motor_stop(void);

#endif /*__MOTOR_H__*/
