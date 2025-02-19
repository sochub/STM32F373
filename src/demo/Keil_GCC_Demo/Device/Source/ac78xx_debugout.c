/* Copyright Statement:
 *
 * This software/firmware and related documentation ("AutoChips Software") are
 * protected under relevant copyright laws. The information contained herein is
 * confidential and proprietary to AutoChips Inc. and/or its licensors. Without
 * the prior written permission of AutoChips inc. and/or its licensors, any
 * reproduction, modification, use or disclosure of AutoChips Software, and
 * information contained herein, in whole or in part, shall be strictly
 * prohibited.
 *
 * AutoChips Inc. (C) 2016. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("AUTOCHIPS SOFTWARE")
 * RECEIVED FROM AUTOCHIPS AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER
 * ON AN "AS-IS" BASIS ONLY. AUTOCHIPS EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
 * NONINFRINGEMENT. NEITHER DOES AUTOCHIPS PROVIDE ANY WARRANTY WHATSOEVER WITH
 * RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 * INCORPORATED IN, OR SUPPLIED WITH THE AUTOCHIPS SOFTWARE, AND RECEIVER AGREES
 * TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
 * RECEIVER EXPRESSLY ACKNOWLEDGES THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO
 * OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES CONTAINED IN AUTOCHIPS
 * SOFTWARE. AUTOCHIPS SHALL ALSO NOT BE RESPONSIBLE FOR ANY AUTOCHIPS SOFTWARE
 * RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND AUTOCHIPS'S
 * ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE AUTOCHIPS SOFTWARE
 * RELEASED HEREUNDER WILL BE, AT AUTOCHIPS'S OPTION, TO REVISE OR REPLACE THE
 * AUTOCHIPS SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE
 * CHARGE PAID BY RECEIVER TO AUTOCHIPS FOR SUCH AUTOCHIPS SOFTWARE AT ISSUE.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "ac78xx.h"
#include "ac78xx_gpio.h"
#include "core_cm3.h"

#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))  
#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))  
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))  
#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))  
#define TRCENA          0x01000000  
  

#define MAX_DEBUG_BUFF_SIZE 100

///< debug uart receive buf
uint8_t g_debugBuff[MAX_DEBUG_BUFF_SIZE] = {0};

///< debug init flag
static uint8_t s_debugInit = 0;

#ifdef ITM_DEBUG_OUT
/**
* @param[in]  ch: the char to put
* @return return the char of be put
*
* @brief  put a char to ITM
*/
int debug_putchar(int ch)
{
    if (s_debugInit)
    {
        ITM_SendChar(ch);
    }
    return ch;
}
volatile int32_t ITM_RxBuffer;
/**
* @return -1: not get char,  other: the char get from UART
*
* @brief  get a char
*/
int debug_getchar(void)
{
    int ch = ITM_ReceiveChar();

    return ch;
}
#else
#include "ac78xx_uart.h"

#define DEBUG_UART                UART2
#define DEBUG_UART_IRQ            UART2_IRQn
#define DEBUG_UART_CLK            CLK_UART2
#define DEBUG_UART_SRST           SRST_UART2

int debug_putchar(int ch)
{                   
    /* Write character to Serial Port  */
    if (s_debugInit)
    {
        UART_SendData(DEBUG_UART, ch);
        if (DEMCR & TRCENA)   
        {  
            while (ITM_Port32(0) == 0);  
            ITM_Port8(0) = ch;  
        }  
    }
}

/**
* @return -1: not get char,  other: the char get from UART
*
* @brief  get a char
*/
int debug_getchar(void)
{
    int ch = -1;
    if (s_debugInit)
    {
        if (UART_RxIsDataReady(DEBUG_UART))
        {
            ch = UART_ReceiveData(DEBUG_UART);
        }
        
    }
    return ch;
}

#endif

/**
* @return none
*
* @brief  set the debug out is invalid
*/
void DeinitDebug(void)
{
    s_debugInit = 0;
}

/**
* @return none
*
* @brief  init debug out , and set the debug out is valid
*/
void InitDebug(void)
{
#ifdef ITM_DEBUG_OUT
#else

#if DEBUG_CMD_INTERRUPT
    NVIC_SetPriority(DEBUG_UART_IRQ, 15);
    NVIC_ClearPendingIRQ(DEBUG_UART_IRQ);
    NVIC_EnableIRQ(DEBUG_UART_IRQ);
#endif

    GPIO_SetFunc(49, 1);
    GPIO_SetFunc(50, 1);
    CKGEN_Enable(DEBUG_UART_CLK, 1);
    CKGEN_SoftReset(DEBUG_UART_SRST, 1);

    UART_SetDivisor(DEBUG_UART, APB_BUS_FREQ / 16.0 / 115200); //apbbus: 48M

    UART_SetDataBits(DEBUG_UART, 8);
    UART_SetStopBit(DEBUG_UART, 1);
    UART_EnableTX(DEBUG_UART, 1);
    UART_EnableRX(DEBUG_UART, 1);
    UART_Set2ByteFIFO(DEBUG_UART, 1);

    UART_SetIntEn(DEBUG_UART, 9);
#endif

    s_debugInit = 1;
}

/**
* @return none
*
* @brief  uart interrupt handler if use interrupt mode for debug out
*/
#if DEBUG_CMD_INTERRUPT
void UART2_IRQHandler(void)
{
    int  cmdChar  = 0;

    uint32_t irqStatus = DEBUG_UART->Ux_LSR0;

    cmdChar = getchar();
    if (-1 != cmdChar)
    {
        HandleDebugCmd(cmdChar);
    }

    DEBUG_UART->Ux_LSR0 = irqStatus;
}
#endif

/**
* @param[in]  stackInfo:  stack infomation
* @return none
*
* @brief  print the stack infomation when occur HardFault
*/
void HardFaultDebug(const unsigned int * stackInfo)
{
    unsigned int stackR0;
    unsigned int stackR1;
    unsigned int stackR2;
    unsigned int stackR3;
    unsigned int stackR12;
    unsigned int stackLR;
    unsigned int stackPC;
    unsigned int stackPSR;

    stackR0 = ((unsigned long) stackInfo[0]);
    stackR1 = ((unsigned long) stackInfo[1]);
    stackR2 = ((unsigned long) stackInfo[2]);
    stackR3 = ((unsigned long) stackInfo[3]);
    stackR12 = ((unsigned long) stackInfo[4]);
    stackLR = ((unsigned long) stackInfo[5]);
    stackPC = ((unsigned long) stackInfo[6]);
    stackPSR = ((unsigned long) stackInfo[7]);

    printf("R0=%x\n", stackR0);
    printf("R1=%x\n", stackR1);
    printf("R2=%x\n", stackR2);
    printf("R3=%x\n", stackR3);
    printf("R12=%x\n", stackR12);
    printf("LR[R14]=%x subroutine call return address\n", stackLR);
    printf("PC[R15]=%x program counter\n", stackPC);
    printf("PSR=%x\n", stackPSR);

    printf("BFAR=%x\n", (*((volatile unsigned int *)(0xE000ED38))));
    printf("CFSR=%x\n", (*((volatile unsigned int *)(0xE000ED28))));
    printf("HFSR=%x\n", (*((volatile unsigned int *)(0xE000ED2C))));
    printf("DFSR=%x\n", (*((volatile unsigned int *)(0xE000ED30))));
    printf("AFSR=%x\n", (*((volatile unsigned int *)(0xE000ED3C))));

    printf("SCB_SHCSR=%x\n", (unsigned int)SCB->SHCSR);

    while(1);
}




