/*-----------------------------------------------------*-
		Uart module Tiva TM4C123G
		Author: Bon Van Ho
		Reference: PIF club
		Date: 27/07/2017
-*-----------------------------------------------------*/

#ifndef UART_H_
#define UART_H_

//----------------------------------------------------
//  User define
//----------------------------------------------------
#define _UART0_BASE

//----------------------------------------------------

#ifdef _UART0_BASE
#define UART_BASE  UART0_BASE
#endif

#ifdef _UART2_BASE
#define UART_BASE UART2_BASE
#endif

#ifdef _UART3_BASE
#define UART_BASE UART3_BASE
#endif

#ifdef _UART4_BASE
#define UART_BASE UART4_BASE
#endif

#ifdef _UART5_BASE
#define UART_BASE UART5_BASE
#endif

#ifdef _UART6_BASE
#define UART_BASE UART6_BASE
#endif

#ifdef _UART7_BASE
#define UART_BASE UART7_BASE
#endif

//-------------------------------------------------------
// Define parity variables
//-------------------------------------------------------
#define NONE  0
#define EVEN  1
#define ODD   2
#define ONE   3
#define ZERO  4

void uart_init(unsigned long baudrate, unsigned char data_bits, unsigned char parity, unsigned char stop_bit);

void uart_put_str(char *str);

void uart_put_num(int number);

void CheckNetworkFrame(uint8_t reset);

#endif /* UART_H_ */
