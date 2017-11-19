/*-----------------------------------------------------------------*-
    DC motor control TM4C123G
    Author: Bon Van Ho
    Discription:

    Reference: PIF
    Date: 1/8/2017
-*-----------------------------------------------------------------*/

//------------------------------------------------------------------
//  Include file
//------------------------------------------------------------------
#include "include.h"
//------------------------------------------------------------------
//  Declare variables
//------------------------------------------------------------------
extern PIDType PIDPosition_motor_0, PIDPosition_motor_1, PIDVelocity_motor_0, PIDVelocity_motor_1;
int32_t set_point = 0, pos_motor_left = 0, pos_motor_right = 0, NumSpdSet=0;
int32_t speed_motor_left = 0, speed_motor_right = 0, speed_check = 0, t = 0;
int32_t left_speed_set = 0, right_speed_set = 0;
bool state_start = false, start = false; left_flag = false, right_flag = false; process_start = false;
bool process_done=true;
bool state_begin = true;
bool uartisr_start = false;
unsigned char m,n=0;
char str[10] = " ", str_left[6] = " ", str_right[6] = " ";
char piece1[10], piece2[10];
char *pch1, *pch2, x=0;
int a, b;
bool status_odd_send = true;
//------------------------------------------------------------------
//  QEI RPM
//------------------------------------------------------------------
#define LOAD    320000
#define PPR     8400
#define EDGES   4
#define CLOCKS  16000000

//------------------------------------------------------------------
//  MPU6050
//------------------------------------------------------------------

extern int16_t accaxisX, accaxisY, accaxisZ, gyroaxisX, gyroaxisY, gyroaxisZ;

//------------------------------------------------------------------
//  Declare function
//------------------------------------------------------------------
static void ex_isr();
static void timer5_isr();
static void timer3_isr();
static void uart0_isr();

//------------------------------------------------------------------
//  Routine function
//------------------------------------------------------------------
void io_init()
{
    // Enable clock for PortF
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    // Configure pins that connect leds
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, LED_RED | LED_BLUE | LED_GREEN);
    // Config Pins that connect buttons is input, 2mA, pull up resistor
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0x4C4F434B;//Unlocks the GPIO
    //Commit (GPIOCR) register for write access PF0
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) = SW1|SW2;
    // Configure pin that connect button is input, 2mA, pull up resistor
    ROM_GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, SW1|SW2);
    ROM_GPIOPadConfigSet(GPIO_PORTF_BASE, SW1|SW2, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    // Turn off all leds
    ROM_GPIOPinWrite(GPIO_PORTF_BASE, LED_RED | LED_BLUE | LED_GREEN, 0);
    // GPIO external intterupt, falling edge
    ROM_GPIOIntTypeSet(GPIO_PORTF_BASE, SW1|SW2, GPIO_FALLING_EDGE);
    GPIOIntRegister(GPIO_PORTF_BASE, &ex_isr);
    GPIOIntEnable(GPIO_PORTF_BASE, SW1|SW2);
    ROM_IntEnable(INT_GPIOF);
    ROM_IntMasterEnable();
}

void uart_config()
{
    uart_init(115200, 8, NONE, 1);
    // Config UART interrupt
    ROM_UARTIntEnable(UART_BASE, UART_INT_RX | UART_INT_RT);// Only enable RX and TX interrupt
    UARTIntRegister(UART_BASE, &uart0_isr);
    ROM_UARTEnable(UART_BASE);
}

void pid_parm_init()
{
    // Speed
    // PID for motor 0
    PIDVelocity_motor_0.SetPoint = 0;
    PIDVelocity_motor_0.Kp       = 0.02;    //0.02
    PIDVelocity_motor_0.Ki       = 0.0;     //0.0
    PIDVelocity_motor_0.Kd       = 0.0015;   //0.015
    PIDVelocity_motor_0.Enable   = 0;

    // PID for motor 1
    PIDVelocity_motor_1.SetPoint = 0;
    PIDVelocity_motor_1.Kp       = 0.02;    //0.02
    PIDVelocity_motor_1.Ki       = 0.0;     //0
    PIDVelocity_motor_1.Kd       = 0.0015;   //0.015
    PIDVelocity_motor_1.Enable   = 0;

    // Position
    PIDPosition_motor_0.SetPoint = 0;
    PIDPosition_motor_0.Kp       = 0.02;
    PIDPosition_motor_0.Ki       = 0;
    PIDPosition_motor_0.Kd       = 0.025;
    PIDPosition_motor_0.Enable   = 1;

    PIDPosition_motor_1.SetPoint = 0;
    PIDPosition_motor_1.Kp       = 0.035;
    PIDPosition_motor_1.Ki       = 0;
    PIDPosition_motor_1.Kd       = 0.025;
    PIDPosition_motor_1.Enable   = 1;
}

void config_pid_timer(uint32_t ms_update)
{
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER5);
    // Timer 5 32 bit, periodic mode
    ROM_TimerConfigure(TIMER5_BASE, TIMER_CFG_PERIODIC);
    // Set count value
    ROM_TimerLoadSet(TIMER5_BASE, TIMER_A, ((ROM_SysCtlClockGet()/1000)*ms_update));
    // Timer interrupt
    TimerIntRegister(TIMER5_BASE, TIMER_A, &timer5_isr);
    ROM_IntEnable(INT_TIMER5A);
    ROM_TimerIntEnable(TIMER5_BASE, TIMER_TIMA_TIMEOUT);
    ROM_TimerIntClear(TIMER5_BASE, TIMER_TIMA_TIMEOUT);
    ROM_TimerEnable(TIMER5_BASE, TIMER_A);
}

void config_imu_timer(uint32_t ms_update)
{
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);
    // Timer 5 32 bit, periodic mode
    ROM_TimerConfigure(TIMER3_BASE, TIMER_CFG_PERIODIC);
    // Set count value
    ROM_TimerLoadSet(TIMER3_BASE, TIMER_A, ((ROM_SysCtlClockGet()/1000)*ms_update));
    // Timer interrupt
    TimerIntRegister(TIMER3_BASE, TIMER_A, &timer3_isr);
    ROM_IntEnable(INT_TIMER3A);
    ROM_TimerIntEnable(TIMER3_BASE, TIMER_TIMA_TIMEOUT);
    ROM_TimerIntClear(TIMER3_BASE, TIMER_TIMA_TIMEOUT);
    ROM_TimerEnable(TIMER3_BASE, TIMER_A);
}

//split for string: 'split_sign'num1'split_sign'num2'split_sign'
void split_str(char *str, char split_sign, char *str_out1, char *str_out2)
{
    unsigned char i=1,z,y=0, q=0;
    const unsigned int NUMLEN = (int)strlen(str_out1);
    char str_temp[10];
    for(z=0;z<2;z++)
    {
        while(*(str+i)!=split_sign)
        {
            str_temp[y++] = *(str+i++);
        }
        // place garbage values to ' ' -> avoid error process --> fix bug
        for(q=y;q<10;q++)
        {
            str_temp[q] = ' ';
        }
        y=0; i++;
        if(z==0)
            strcpy(str_out1, str_temp);
        else
            strcpy(str_out2, str_temp);
    }
}


int strtoint(char *snum)
{
    int idx, strIdx = 0, accum = 0;
    const unsigned int NUMLEN = (int)strlen(snum);

    /* Check if negative number and flag it. */
   // if(snum[0] == 0x2d)
   //     numIsNeg = 1;

    for(idx = NUMLEN - 1; idx >= 0; idx--)
    {
        // Only process numbers from 0 through 9.
        if(snum[idx] >= 0x30 && snum[idx] <= 0x39)
            accum += (snum[idx] - 0x30) * pow(10, strIdx++);
    }
   /* for(idx = 0;idx < NUMLEN; idx++){
        accum +=(snum[idx]-0x30)*pow(10,NUMLEN-idx-1);
    }
*/
    /* Check flag to see if originally passed -ve number and convert result if so. */
    if (accum > 2000)
    {
    return (2000-accum);}
    else return accum;
}

void main()
{
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    io_init();
    qei_init(20);
    initI2C();
    initMPU6050();
    config_pwm();
    pid_parm_init();
    PIDPositionSet_motor_1(50000);
    PIDPositionSet_motor_0(50000);
    config_pid_timer(20);
    config_imu_timer(30);
    uart_config();
    ROM_IntMasterEnable();
    while(1)
    {
        if(uartisr_start == false)
        {
            if(status_odd_send == true)
            {
                ROM_UARTCharPut(UART0_BASE, 'e');
                ROM_UARTCharPut(UART0_BASE, '\t');
                uart_put_num(pos_motor_left);
                ROM_UARTCharPut(UART0_BASE, '\t');
                uart_put_num(pos_motor_right);
                ROM_UARTCharPut(UART0_BASE, '\n');
                // send speed
                ROM_UARTCharPut(UART0_BASE, 's');
                ROM_UARTCharPut(UART0_BASE, '\t');
                uart_put_num(PIDVelocity_motor_0.SetPoint);
                ROM_UARTCharPut(UART0_BASE, '\t');
                uart_put_num(PIDVelocity_motor_1.SetPoint);
                ROM_UARTCharPut(UART0_BASE, '\n');
                status_odd_send = false;
            }
            else
            {
                getMPU6050Data();
                // send raw imu data
                ROM_UARTCharPut(UART0_BASE, 'i');
                ROM_UARTCharPut(UART0_BASE, '\t');
                uart_put_num(gyroaxisX);
                ROM_UARTCharPut(UART0_BASE, '\t');
                uart_put_num(gyroaxisY);
                ROM_UARTCharPut(UART0_BASE, '\t');
                uart_put_num(gyroaxisZ);
                ROM_UARTCharPut(UART0_BASE, '\t');
                uart_put_num(accaxisX);
                ROM_UARTCharPut(UART0_BASE, '\t');
                ROM_UARTCharPut(UART0_BASE, '\n');
                status_odd_send = true;
            }
        }
        if(start==false)
        {
            split_str(str,' ',piece1, piece2);
            PIDVelocity_motor_0.SetPoint  = -1*(int32_t)strtoint(piece1);
            if(PIDVelocity_motor_0.SetPoint > 800)
                PIDVelocity_motor_0.SetPoint = 800;
            else if(PIDVelocity_motor_0.SetPoint < -800)
                PIDVelocity_motor_0.SetPoint = -800;
            PIDVelocity_motor_1.SetPoint  = -1*(int32_t)strtoint(piece2);
            if(PIDVelocity_motor_1.SetPoint > 800)
                PIDVelocity_motor_1.SetPoint = 800;
            else if(PIDVelocity_motor_1.SetPoint < -800)
                PIDVelocity_motor_1.SetPoint = -800;
        }
    }
}

static void ex_isr()
{
    state_start = true;

    if(!GPIOPinRead(GPIO_PORTF_BASE, SW1))
    {
        if(PIDVelocity_motor_0.Enable || PIDVelocity_motor_1.Enable)
        {
            PIDVelocity_motor_0.Enable = 0;
            PIDVelocity_motor_1.Enable = 0;
        }
        else
        {
            PIDVelocity_motor_0.Enable = 1;
            PIDVelocity_motor_1.Enable = 1;
        }
    }
    else if(!GPIOPinRead(GPIO_PORTF_BASE, SW2))
    {
        PIDVelocity_motor_0.Enable = 0;
        PIDVelocity_motor_1.Enable = 0;
        //pos = qei_getPosLeft();
        set_pwm_channel_right(FREQ_PWM, 0);
        set_pwm_channel_left(FREQ_PWM, 0);

        PIDPosition_motor_1.iPart = 0;
        PIDPosition_motor_1.PIDErrorTemp1 = 0;

        PIDPosition_motor_0.iPart = 0;
        PIDPosition_motor_0.PIDErrorTemp1 = 0;
    }
    // clear external interrupt flag
    GPIOIntClear(GPIO_PORTF_BASE, SW1|SW2);
}

static void timer5_isr()
{
    // clear timer interrupt flag
    ROM_TimerIntClear(TIMER5_BASE, TIMER_TIMA_TIMEOUT);
    //PIDVelocity_motor_0.SetPoint = (int32_t)left_speed_set*11.2;
    //PIDVelocity_motor_1.SetPoint = (int32_t)right_speed_set*11.2;
    // get velocity
    // read 2 time to avoid read encoder error
    speed_motor_left = ROM_QEIVelocityGet(QEI0_BASE) * ROM_QEIDirectionGet(QEI0_BASE);
    speed_motor_right = ROM_QEIVelocityGet(QEI1_BASE) * ROM_QEIDirectionGet(QEI1_BASE);

    //Get Position
    pos_motor_left = ((int32_t)ROM_QEIPositionGet(QEI0_BASE)) / 2;
    pos_motor_right = ((int32_t)ROM_QEIPositionGet(QEI1_BASE)) / 2;
    // led effect
    if(speed_motor_left > 500 && speed_motor_left <= 1800)
    {
        ROM_GPIOPinWrite(GPIO_PORTF_BASE, LED_RED | LED_BLUE | LED_GREEN, LED_BLUE);
    }
    else if(speed_motor_left > 1800)
    {
        ROM_GPIOPinWrite(GPIO_PORTF_BASE, LED_RED | LED_BLUE | LED_GREEN, LED_RED);
    }
    else
        ROM_GPIOPinWrite(GPIO_PORTF_BASE, LED_RED | LED_BLUE | LED_GREEN, LED_GREEN);

    if(PIDVelocity_motor_0.Enable || PIDVelocity_motor_1.Enable)
    {
        PIDVerCalc_motor_0(speed_motor_left, 90);
        set_pwm_channel_left(FREQ_PWM, (long)PIDVelocity_motor_0.PIDResult);

        PIDVerCalc_motor_1(speed_motor_right, 90);
        set_pwm_channel_right(FREQ_PWM, (long)PIDVelocity_motor_1.PIDResult);

/*      if (PIDPosition_motor_1.Enable)
        {
            if(pos<=PIDPosition_motor_1.SetPoint+300 && pos>= PIDPosition_motor_1.SetPoint-300)
            {
                set_pwm_channel_right(FREQ_PWM, 0);
                set_pwm_channel_left(FREQ_PWM, 0);
                PIDPosition_motor_1.Enable = 0;
                PIDPosition_motor_0.Enable = 0;
                PIDVelocity_motor_0.Enable = 0;
                PIDVelocity_motor_1.Enable = 0;
            }
        }*/
    }
}

static void timer3_isr()
{
    // clear timer interrupt flag
    ROM_TimerIntClear(TIMER3_BASE, TIMER_TIMA_TIMEOUT);
    if(uartisr_start == true)
    {
        if(status_odd_send == true)
        {
            ROM_UARTCharPut(UART0_BASE, 'e');
            ROM_UARTCharPut(UART0_BASE, '\t');
            uart_put_num(pos_motor_left);
            ROM_UARTCharPut(UART0_BASE, '\t');
            uart_put_num(pos_motor_right);
            ROM_UARTCharPut(UART0_BASE, '\n');
            // send speed
            ROM_UARTCharPut(UART0_BASE, 's');
            ROM_UARTCharPut(UART0_BASE, '\t');
            uart_put_num(PIDVelocity_motor_0.SetPoint);
            ROM_UARTCharPut(UART0_BASE, '\t');
            uart_put_num(PIDVelocity_motor_1.SetPoint);
            ROM_UARTCharPut(UART0_BASE, '\n');
            status_odd_send = false;
        }
        else
        {
            getMPU6050Data();
            // send raw imu data
            ROM_UARTCharPut(UART0_BASE, 'i');
            ROM_UARTCharPut(UART0_BASE, '\t');
            uart_put_num(gyroaxisX);
            ROM_UARTCharPut(UART0_BASE, '\t');
            uart_put_num(gyroaxisY);
            ROM_UARTCharPut(UART0_BASE, '\t');
            uart_put_num(gyroaxisZ);
            ROM_UARTCharPut(UART0_BASE, '\t');
            uart_put_num(accaxisX);
            ROM_UARTCharPut(UART0_BASE, '\t');
            ROM_UARTCharPut(UART0_BASE, '\n');
            status_odd_send = true;
        }
    }
}
void uart0_isr()
{
    uint32_t ui32Status;
    uartisr_start = true;
    if(ROM_UARTCharGet(UART0_BASE) == 's')
    {
        start = true;
        m=0;
    }
    while(ROM_UARTCharsAvail(UART0_BASE)) //loop while there are chars
    {
        if(start == true)
            str[m++] = ROM_UARTCharGet(UART0_BASE);
        else
            break;
    }
    if(start==true)
    {
        start=false;
    }
    ui32Status = ROM_UARTIntStatus(UART0_BASE, true); //get interrupt status
    ROM_UARTIntClear(UART0_BASE, ui32Status); //clear the asserted interrupts
}

