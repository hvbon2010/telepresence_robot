/*-------------------------------------------------------------------------------------------*-
	PWM for H-Bridge TM4C123G
	Author: Bon Van Ho
	Reference: PIF
	Date: 22/07/2017
-*-------------------------------------------------------------------------------------------*/

#ifndef PWM_H_
#define PWM_H_

#define FREQ_PWM	20000

void config_pwm();

void set_pwm_channel_right(uint32_t freq, int32_t duty_cycle);

void set_pwm_channel_left(uint32_t freq, int32_t duty_cycle);

#endif /* PWM_H_ */
