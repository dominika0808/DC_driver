#include "motor.h"

#define HIGH_SIDE_RIGHT_TRANSSISTOR 	TIM_CHANNEL_1
#define LOW_SIDE_RIGHT_TRANSSISTOR 		TIM_CHANNEL_2
#define HIGH_SIDE_LEFT_TRANSSISTOR 		TIM_CHANNEL_3
#define LOW_SIDE_LEFT_TRANSSISTOR 		TIM_CHANNEL_4

#define ENCODER_REVOLUTION				48
#define MOTOR_GEAR						75
#define SECEND_IN_MINUTE				60
#define CALCULATIONS_PER_SECEND 		10

void motor_str_init(serwo_str *m, TIM_HandleTypeDef *tim1, TIM_HandleTypeDef *tim2, int32_t dir)
{
	m->motor_timer = tim1;
	m->encoder_timer = tim2;

	m->resolution = ENCODER_REVOLUTION * MOTOR_GEAR;
	m->pulse_count = 0;
	m->current = 0;
	m->speed = 0;
	m->set_speed = 0;
	m->direction = dir;
	m->actual_PWM = 0;
}

void motor_calculate_speed(serwo_str *m)
{
	m->pulse_count = (int16_t)__HAL_TIM_GET_COUNTER(m->encoder_timer);
	__HAL_TIM_SET_COUNTER(m->encoder_timer, 0);

	m->speed = (m->pulse_count * CALCULATIONS_PER_SECEND * SECEND_IN_MINUTE) / m->resolution;
}

void motor_calculate_current(serwo_str *m)
{
	m->current = (float)average(&(m->filter)) * 3.3f / 4096.0f / 50.0f / 0.01f;
}

void motor_set_speed(serwo_str *m, int16_t speed)
{
	if(speed < 10 && speed > -10) m->set_speed = 0;
	else if(speed <= 120 && speed >= -120) m->set_speed = speed;
}

void motor_run_pid(serwo_str *m)
{
	int32_t output = pid_calculate(&(m->pid_controller), m->set_speed, m->speed);

	if((m->actual_PWM * m->direction) >= 0)
	{
		m->actual_PWM += output;
		motor_right(m->motor_timer, abs(m->actual_PWM));
	}
	else
	{
		m->actual_PWM += output;
		motor_left(m->motor_timer, abs(m->actual_PWM));
	}
}

void motor_right(TIM_HandleTypeDef *tim, uint8_t duty)
{
	if(duty >= 98) duty = 90;
	__HAL_TIM_SET_COMPARE(tim, HIGH_SIDE_RIGHT_TRANSSISTOR, 0);
	__HAL_TIM_SET_COMPARE(tim, LOW_SIDE_LEFT_TRANSSISTOR, 98);
	__HAL_TIM_SET_COMPARE(tim, LOW_SIDE_RIGHT_TRANSSISTOR, (0));
	__HAL_TIM_SET_COMPARE(tim, HIGH_SIDE_LEFT_TRANSSISTOR, duty);

}

void motor_left(TIM_HandleTypeDef *tim, uint8_t duty)
{
	if(duty >= 98) duty = 90;
	__HAL_TIM_SET_COMPARE(tim, LOW_SIDE_RIGHT_TRANSSISTOR, 98);
	__HAL_TIM_SET_COMPARE(tim, HIGH_SIDE_LEFT_TRANSSISTOR, 0);
	__HAL_TIM_SET_COMPARE(tim, LOW_SIDE_LEFT_TRANSSISTOR, (0));
	__HAL_TIM_SET_COMPARE(tim, HIGH_SIDE_RIGHT_TRANSSISTOR, duty);
}

void motor_stop(TIM_HandleTypeDef *tim)
{
	__HAL_TIM_SET_COMPARE(tim, LOW_SIDE_RIGHT_TRANSSISTOR, 0);
	__HAL_TIM_SET_COMPARE(tim, LOW_SIDE_LEFT_TRANSSISTOR, 0);
	__HAL_TIM_SET_COMPARE(tim, HIGH_SIDE_RIGHT_TRANSSISTOR, 0);
	__HAL_TIM_SET_COMPARE(tim, HIGH_SIDE_LEFT_TRANSSISTOR, 0);
}
