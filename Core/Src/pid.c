#include "pid.h"

void pid_init(pid_str *pid, float kp_i, float ki_i, float kd_i, int16_t anti_windup_i)
{
	pid -> previous_error = 0;
	pid -> total_error = 0;
	pid -> Kp = kp_i;
	pid -> Ki = ki_i;
	pid -> Kd = kd_i;
	pid -> anti_windup = anti_windup_i;
}

void pid_reset(pid_str *pid)
{
	pid -> total_error = 0;
	pid -> previous_error = 0;
}

int32_t pid_calculate(pid_str *pid, int32_t setpoint, int32_t process_variable)
{
	int32_t error;
	float p, i, d;

	error = setpoint - process_variable;
	pid->total_error += error;

	p = (float)(pid->Kp * error);
	i = (float)(pid->Ki * pid->total_error);
	d = (float)(pid->Kd * (error - pid->previous_error));

	if(i >= pid->anti_windup) i = pid->anti_windup;
	else if(i <= -pid->anti_windup) i = -pid->anti_windup;

	pid->previous_error = error;

	return (int32_t)(p + i + d);
}
