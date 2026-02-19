#ifndef __MOTOR_H__
#define __MOTOR_H__

#include <stdlib.h>
#include "main.h"
#include "tim.h"
#include "pid.h"
#include "filter.h"

typedef struct
{
	TIM_HandleTypeDef *motor_timer;
	TIM_HandleTypeDef *encoder_timer;

	uint16_t resolution;
	int32_t pulse_count;
	float current;
	int32_t speed;
	int32_t set_speed;
	int32_t direction;
	int32_t actual_PWM;

	pid_str pid_controller;
	filter_str filter;
}serwo_str;

void motor_str_init(serwo_str *m, TIM_HandleTypeDef *tim1, TIM_HandleTypeDef *tim2, int32_t dir);
void motor_calculate_speed(serwo_str *m);
void motor_calculate_current(serwo_str *m);
void motor_set_speed(serwo_str *m, int16_t speed);

void motor_run_pid(serwo_str *m);

void motor_right(TIM_HandleTypeDef *tim, uint8_t duty);
void motor_left(TIM_HandleTypeDef *tim, uint8_t duty);
void motor_stop(TIM_HandleTypeDef *tim);

#endif /*__MOTOR_H__*/
