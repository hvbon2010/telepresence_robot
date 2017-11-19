/*-------------------------------------------------------------------------------------------*-
	PWM for H-Bridge TM4C123G
	Author: Bon Van Ho
	Reference: PIF
	Date: 22/07/2017
-*-------------------------------------------------------------------------------------------*/

//---------------------------------------------------------------------------------------------
//	Include file
//---------------------------------------------------------------------------------------------
#include "include.h"

void config_pwm()
{
	// PWM Clock = System Clock / 1
	ROM_SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
	// Enable module 0 PWM
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
	// Enable port B
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	// Config PB6, PB7
	// PB6 & PB7 use for motor 1
	ROM_GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6 | GPIO_PIN_7);
	ROM_GPIOPinConfigure(GPIO_PB7_M0PWM1);
	ROM_GPIOPinConfigure(GPIO_PB6_M0PWM0);
	// Set mode of operation for PWM generaton 0
	ROM_PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	// Set frequency for PWM
	ROM_PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, ROM_SysCtlClockGet() / FREQ_PWM);
	// Set pulse width PWM output(duty_cycle = 50% then motor not turn)
	ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, (ROM_SysCtlClockGet() / FREQ_PWM) * 0.5);
	ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, (ROM_SysCtlClockGet() / FREQ_PWM) * 0.5);
	// Enable PWM output
	ROM_PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT, true);
	ROM_PWMOutputState(PWM0_BASE, PWM_OUT_1_BIT, true);
	// Set the inversion mode for PWM output
	ROM_PWMOutputInvert(PWM0_BASE, PWM_OUT_1_BIT, true);// upper haft H-Bridge
	ROM_PWMOutputInvert(PWM0_BASE, PWM_OUT_0_BIT, false);// lower haft H-Bridge
	// Enable PWM generator 0
	ROM_PWMGenEnable(PWM0_BASE, PWM_GEN_0);

	// Enable port E
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	// PE4, PE5 to use PWM function
	// PE4 & PE5 use for motor 2
	ROM_GPIOPinTypePWM(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);
	ROM_GPIOPinConfigure(GPIO_PE4_M0PWM4);
	ROM_GPIOPinConfigure(GPIO_PE5_M0PWM5);
	// Set mode of operation for PWM generation 2
	ROM_PWMGenConfigure(PWM0_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	// Set frequency for PWM
	ROM_PWMGenPeriodSet(PWM0_BASE, PWM_GEN_2, ROM_SysCtlClockGet() / FREQ_PWM);
	// Set pulse width PWM output(duty_cycle = 50% then motor not turn)
	ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_4, (ROM_SysCtlClockGet() / FREQ_PWM) * 0.5);
	ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_5, (ROM_SysCtlClockGet() / FREQ_PWM) * 0.5);
	// Enable PWM output
	ROM_PWMOutputState(PWM0_BASE, PWM_OUT_4_BIT, true);
	ROM_PWMOutputState(PWM0_BASE, PWM_OUT_5_BIT, true);
	// Set the inversion mode for PWM output
	ROM_PWMOutputInvert(PWM0_BASE, PWM_OUT_4_BIT, true);// upper haft H-Bridge
	ROM_PWMOutputInvert(PWM0_BASE, PWM_OUT_5_BIT, false);// lower haft H-Bridge
	// Enable PWM generator 2
	ROM_PWMGenEnable(PWM0_BASE, PWM_GEN_2);
}

//--------------------------------------------------------------------------------------
//	set_pwm_channel_x function
//	freq: frequency of PWM(rely to H bridge using)
//	duty_cycle: adjust -90 -> 90 (not max speed)
//	+ duty_cycle < 0 <=> Ton < Toff: motor turn in the opposite direction
//	+ duty_cylce > 0 <=> Ton > Toff: motor turn in the forward direction
//--------------------------------------------------------------------------------------
void set_pwm_channel_right(uint32_t freq, int32_t duty_cycle)
{
	uint32_t period;
	period = ROM_SysCtlClockGet() / freq;
	if(duty_cycle > 90)	duty_cycle = 90;
	else if(duty_cycle < -90) duty_cycle = -90;
	ROM_PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, period - 1);
	ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, (period*(100 + duty_cycle) / 200) - 1);
	ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, (period*(100 + duty_cycle) / 200) - 1);
}

void set_pwm_channel_left(uint32_t freq, int32_t duty_cycle)
{
	uint32_t period;
	period = ROM_SysCtlClockGet() / freq;
	if(duty_cycle > 90)	duty_cycle = 90;
	else if(duty_cycle < -90) duty_cycle = -90;
	ROM_PWMGenPeriodSet(PWM0_BASE, PWM_GEN_2, period - 1);
	ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_4, (period*(100 + duty_cycle) / 200) - 1);
	ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_5, (period*(100 + duty_cycle) / 200) - 1);
}
