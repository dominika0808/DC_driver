#include "motor.h"
#include "timer.h"
#include "stm32f3xx.h"

#define ENCODER_REVOLUTION				48
#define MOTOR_GEAR						75
#define SECEND_IN_MINUTE				60
#define CALCULATIONS_PER_SECEND 		10

void motor_str_init(serwo_str *m, char identifier)
{
	m->id = identifier;
	m->resolution = ENCODER_REVOLUTION * MOTOR_GEAR;
	m->pulse_count = 0;
	m->current = 0;
	m->speed = 0;
	m->set_speed = 0;
	m->actual_PWM = 0;
}

void motor_calculate_speed(serwo_str *m)
{
	switch(m->id)
	{
	case 'R':
		m->pulse_count = Tim3GetValue();
		break;
	case 'L':
		m->pulse_count = Tim4GetValue();
		break;
	default:
		break;
	}

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

	m->actual_PWM += output;

	switch(m->id)
	{
	case 'R':
		if(m->actual_PWM > 0)
		{
			right_motor_forward(m->actual_PWM);
		}
		else if(m->actual_PWM < 0)
		{
			right_motor_backward(-(m->actual_PWM));
		}
		else
		{
			right_motor_stop();
		}
		break;

	case 'L':
		if(m->actual_PWM > 0)
		{
			left_motor_forward(m->actual_PWM);
		}
		else if(m->actual_PWM < 0)
		{
			left_motor_backward(-(m->actual_PWM));
		}
		else
		{
			left_motor_stop();
		}
		break;
	default:
		break;
	}
}

void right_motor_forward(uint16_t duty)
{
	if(duty >= 98) duty = 98;

	Tim1HighRightTranssistor(0);
	Tim1LowRightTranssistor(0);
	Tim1HighLeftTranssistor(duty);
	Tim1LowLeftTranssistor(98);
}

void right_motor_backward(uint16_t duty)
{
	if(duty >= 98) duty = 98;

	Tim1HighRightTranssistor(duty);
	Tim1LowRightTranssistor(98);
	Tim1HighLeftTranssistor(0);
	Tim1LowLeftTranssistor(0);
}

void right_motor_stop(void)
{
	Tim1HighRightTranssistor(0);
	Tim1LowRightTranssistor(0);
	Tim1HighLeftTranssistor(0);
	Tim1LowLeftTranssistor(0);
}

void left_motor_forward(uint16_t duty)
{
	if(duty >= 98) duty = 98;

	Tim2HighRightTranssistor(duty);
	Tim2LowRightTranssistor(98);
	Tim2HighLeftTranssistor(0);
	Tim2LowLeftTranssistor(0);
}

void left_motor_backward(uint16_t duty)
{
	if(duty >= 98) duty = 98;

	Tim2HighRightTranssistor(0);
	Tim2LowRightTranssistor(0);
	Tim2HighLeftTranssistor(duty);
	Tim2LowLeftTranssistor(98);
}

void left_motor_stop(void)
{
	Tim2HighRightTranssistor(0);
	Tim2LowRightTranssistor(0);
	Tim2HighLeftTranssistor(0);
	Tim2LowLeftTranssistor(0);
}
