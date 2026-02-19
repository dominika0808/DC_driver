#ifndef INC_PID_H_
#define INC_PID_H_

#include<stdint.h>

typedef struct
{
	int32_t previous_error;
	int32_t total_error;
	float Kp;
	float Ki;
	float Kd;
	int16_t anti_windup;
}pid_str;

void pid_init(pid_str *pid, float kp_i, float ki_i, float kd_i, int16_t anti_windup_i);
void pid_reset(pid_str *pid);
int32_t pid_calculate(pid_str *pid, int32_t setpoint, int32_t process_variable);

#endif /* INC_PID_H_ */
