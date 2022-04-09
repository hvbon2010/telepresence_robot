/*-------------------------------------------------*-
		Uart module Tiva TM4C123G
		Author: Bon Van Ho
		Reference: PIF club
		Date: 27/07/2017
-*-------------------------------------------------*/

//--------------------------------------------------
//	Include file
//--------------------------------------------------
#include "include.h"
#include "uart.h"
//--------------------------------------------------
//	Private function prototype
//--------------------------------------------------

//--------------------------------------------------
//	Declare variable
//--------------------------------------------------

void uart_init(unsigned long baudrate, unsigned char data_bits, unsigned char parity, unsigned char stop_bit)
{
    volatile unsigned long uart_select;
    switch(UART_BASE)
    {
        case UART0_BASE:
            ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
            ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
            ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
            ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
            ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
            break;
        case UART1_BASE:
            break;
        case UART2_BASE:
            ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);
            ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
            ROM_GPIOPinConfigure(GPIO_PD6_U2RX);
            ROM_GPIOPinConfigure(GPIO_PD7_U2TX);
            ROM_GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);
            break;
        case UART3_BASE:
            ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);
            ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
            ROM_GPIOPinConfigure(GPIO_PC6_U3RX);
            ROM_GPIOPinConfigure(GPIO_PC7_U3TX);
            ROM_GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);
            break;
        case UART4_BASE:
            ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART4);
            ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
            ROM_GPIOPinConfigure(GPIO_PC4_U4RX);
            ROM_GPIOPinConfigure(GPIO_PC5_U4TX);
            ROM_GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5);
            break;
        case UART5_BASE:
            ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);
            ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
            ROM_GPIOPinConfigure(GPIO_PE4_U5RX);
            ROM_GPIOPinConfigure(GPIO_PE5_U5TX);
            ROM_GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);
            break;
        case UART6_BASE:
            ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART6);
            ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
            ROM_GPIOPinConfigure(GPIO_PD4_U6RX);
            ROM_GPIOPinConfigure(GPIO_PD5_U6TX);
            ROM_GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_4 | GPIO_PIN_5);
            break;
        case UART7_BASE:
            ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART7);
            ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
            ROM_GPIOPinConfigure(GPIO_PE0_U7RX);
            ROM_GPIOPinConfigure(GPIO_PE1_U7TX);
            ROM_GPIOPinTypeUART(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    }
    switch(data_bits)
    {
        case 5:
            uart_select = UART_CONFIG_WLEN_5;
            break;
        case 6:
            uart_select = UART_CONFIG_WLEN_6;
            break;
        case 7:
            uart_select = UART_CONFIG_WLEN_7;
            break;
        case 8:
            uart_select = UART_CONFIG_WLEN_8;
            break;
    }
    switch(parity)
    {
    case NONE:
        uart_select |= UART_CONFIG_PAR_NONE;
        break;
    case EVEN:
        uart_select |= UART_CONFIG_PAR_EVEN;
        break;
    case ODD:
        uart_select |= UART_CONFIG_PAR_ODD;
        break;
    case ZERO:
        uart_select |= UART_CONFIG_PAR_ZERO;
        break;
    case ONE:
        uart_select |= UART_CONFIG_PAR_ONE;
    }
    switch(stop_bit)
    {
    case 1:
        uart_select |= UART_CONFIG_STOP_ONE;
        break;
    case 2:
        uart_select |= UART_CONFIG_STOP_TWO;
    }
    ROM_UARTConfigSetExpClk(UART_BASE, ROM_SysCtlClockGet(), baudrate, uart_select);
}

void uart_put_str(char *str)
{
	while(*str)
	{
		ROM_UARTCharPut(UART_BASE, *str);
		str++;
	}
	ROM_SysCtlDelay(ROM_SysCtlClockGet()/1000);
}

void uart_put_num(int number)
{
    int i;
    char number_array[10];
    if(number == 0)
    {
        ROM_UARTCharPutNonBlocking(UART_BASE, 48);
        ROM_SysCtlDelay(ROM_SysCtlClockGet()/1000);
    }
    else
    {
        if (number < 0)
        {
            ROM_UARTCharPutNonBlocking(UART_BASE, '-');
            ROM_SysCtlDelay(ROM_SysCtlClockGet()/1000);
            number *= -1;
        }
        const unsigned char n = log10(number) + 1;
        for(i=0;i<n;++i,number/=10)
        {
            number_array[i] = number%10;
        }
        for(i=n-1;i>=0;i--)
        {
            ROM_UARTCharPutNonBlocking(UART_BASE, number_array[i]+48);
        }
    }
}

void CheckNetworkFrame(uint8_t reset)
{
	static uint8_t ByteCount = 0;	//Used for split Network frame
	ByteCount++;
	if (reset)
	{
		ByteCount = 8;
	}
	if (ByteCount == 8)
	{
		ROM_UARTCharPutNonBlocking(UART_BASE, '\r');
		ROM_SysCtlDelay(ROM_SysCtlClockGet() / 300);	//Delay 10ms
		ByteCount = 0;
	}
}
