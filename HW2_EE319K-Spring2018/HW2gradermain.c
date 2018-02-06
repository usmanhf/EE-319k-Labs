// HW2gradermain.c
// Runs on LM4F120/TM4C123
// Used to grade HW2
// Jonathan Valvano Valvano
// January 17, 2018

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2017

 Copyright 2017 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

// U0Rx (VCP receive) connected to PA0
// U0Tx (VCP transmit) connected to PA1
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"

#include "PLL.h"
#include "UART.h"
void InitPB5(void); 
void SetPB5(void); 
void Inc16(void); 
void Dec8(void); 
void Set3Clear29(void);
extern int16_t icount;
extern uint8_t bcount;
extern uint8_t keepOut;
extern uint32_t N;
//debug code
int main(void){uint32_t score=0; uint32_t Ntest,Ncorrect,i,ok;

  PLL_Init(Bus50MHz);       // 50  MHz
  UART_Init();              // initialize UART

  UART_OutString("\n\r HW2 grader-Spring 2018 ");
  
  UART_OutString("\n\r InitPB5 test ");
  InitPB5();
  if(SYSCTL_RCGCGPIO_R&0x02){
    UART_OutString("Clock on, ");
    if(SYSCTL_RCGCGPIO_R&0x2C){
      UART_OutString(" **ERROR**, turned on other clocks ");
    }else{
      score += 10;
    }
    if(GPIO_PORTB_DIR_R&0x20){
      UART_OutString(" PB5 output, ");
      if(GPIO_PORTB_DIR_R&~0x20){
        UART_OutString(" **ERROR**, turned on DIR bits ");
      }else{
        score += 5;
      }
    }
    if(GPIO_PORTB_DEN_R&0x20){
      UART_OutString(" PB5 enabled, ");
      if(GPIO_PORTB_DEN_R&~0x20){
        UART_OutString(" **ERROR**, turned on DEN bits ");
      }else{
        score += 5;
      }
    }
  }else{
    UART_OutString(" **ERROR**, Port B Clock is off ");
  }
  if(score == 20){
    UART_OutString("\n\r SetPB5 test ");
    GPIO_PORTB_DIR_R |= 0xDF; // activate 7,6,4,3,2,1,0
    GPIO_PORTB_DEN_R |= 0xDF; // activate 7,6,4,3,2,1,0
    GPIO_PORTB_DATA_R = 0xC5; 
    SetPB5();
    if((GPIO_PORTB_DATA_R&0x20) != 0x20){
      UART_OutString("PB5 not high");
    }else{
      UART_OutString("PB5 is high, "); score += 10;
      if(GPIO_PORTB_DATA_R != 0xE5){
        UART_OutString("not friendly");
      }else{
        UART_OutString("friendly"); score += 10;
      }
    }  
  }else{
    UART_OutString("\n\r SetPB5 test skipped because InitPB5 not perfect ");
  }   
  
  UART_OutString("\n\r Inc16 test ");
  icount = -5; bcount = 42; keepOut = 57; N = 1000;
  Inc16();
  if(icount != -4){
    UART_OutString("incorrect");
  }else{
    UART_OutString("correct"); score += 10;
  }  
  if((bcount == 42)&&(keepOut == 57)&&(N == 1000)){
    score += 10;
  }else{
    UART_OutString(" your software writes outside of icount variable");
  }

  UART_OutString("\n\r Dec8 test ");
  icount = -6; bcount = 99; keepOut = 31; N = 21234;
  Dec8();
  if(bcount != 98){
    UART_OutString("incorrect");
  }else{
    UART_OutString("correct"); score += 10;
  }  
  if((icount == -6)&&(keepOut == 31)&&(N == 21234)){
    score += 10;
  }else{
    UART_OutString(" your software writes outside of bcount variable");
  }
  
  Ntest = 0xC34FF4E7; ok=1;
  UART_OutString("\n\r Set3Clear29 test ");
  for(i=0;i<5;i++){
    icount = -4+(int16_t)i; bcount = 71+2*i; keepOut = 33+i; 
    N = Ntest;
    Ncorrect = (N&(~(1<<29)))|(1<<3); // set bit 29 clear bit 3
    Set3Clear29();
    if(N != Ncorrect){
     UART_OutString("\n\rincorrect. Init=");UART_OutUHex(Ntest);
     UART_OutString("Your=");UART_OutUHex(N);
     UART_OutString("Ans =");UART_OutUHex(Ncorrect);
     ok = 0;
    }else{
      score += 2;
    }  
    if((icount == -4+(int16_t)i)&&(bcount == 71+2*i)&&(keepOut == 33+i)){
      score += 2;
    }else{
      UART_OutString(" your software writes outside of N variable\n\r");
     ok = 0;
    }
    Ntest = Ntest*1664525+1013904223;
  }
  if(ok){
    UART_OutString(" Set3Clear29 is perfect");
  }
  else{
    UART_OutString(" Set3Clear29 has bugs");
  }
  UART_OutString("\n\r HW2 total score = ");UART_OutUDec(score);

  while(1){};

}
