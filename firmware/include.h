/*-------------------------------------------------------------------------------------------*-
	Include file
	Author: Bon Van Ho
	Date: 22/07/2017
-*-------------------------------------------------------------------------------------------*/

#ifndef INCLUDE_H_
#define INCLUDE_H_

//------------------------------------------------
//	declare device to use ROM library
//------------------------------------------------
#define TARGET_IS_BLIZZARD_RB1
//------------------------------------------------
//	system library
//------------------------------------------------
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "utils/ustdlib.h"
#include "utils/uartstdio.h"
#include "utils/cmdline.h"

//------------------------------------------------
//	driver API library
//------------------------------------------------
#include "driverlib/gpio.h"
#include "driverlib/can.h"
#include "driverlib/eeprom.h"
#include "driverlib/i2c.h"
#include "driverlib/lcd.h"
#include "driverlib/mpu.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/timer.h"
#include "driverlib/adc.h"
#include "driverlib/interrupt.h"
#include "driverlib/qei.h"
#include "driverlib/fpu.h"
#include "driverlib/uart.h"
#include "driverlib/rom.h"

//-------------------------------------------------
//	hardware library
//-------------------------------------------------
#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"

//-------------------------------------------------
//	user declarition library
//-------------------------------------------------
#include "qei.h"
#include "uart.h"
#include "PWM.h"
#include "gpio.h"
#include "PID.h"
#include "MPU6050.h"

#endif /* INCLUDE_H_ */
