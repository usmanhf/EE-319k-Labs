// put implementations for functions, explain how it works
// put your names here, date
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "DAC.h"			

// **************DAC_Init*********************
// Initialize 4-bit DAC, called once 
// Input: none
// Output: none
void DAC_Init(void){
	volatile uint32_t delay;
    SYSCTL_RCGCGPIO_R |= 0x00000002;  // 1) activate clock for Port B
    delay = SYSCTL_RCGCGPIO_R;        // allow time for clock to start
    GPIO_PORTB_AMSEL_R &= ~0x00;      // Disables Analog
    GPIO_PORTB_PCTL_R = 0x00000000;   // 4) PCTL GPIO on P3-0
    GPIO_PORTB_DIR_R = 0x0F;          // 5) PB3-0 out
    GPIO_PORTB_DEN_R = 0x0F;          // 7) enable digital I/O on PB3-0
}

// **************DAC_Out*********************
// output to DAC
// Input: 4-bit data, 0 to 15 
// Input=n is converted to n*3.3V/15
// Output: none
void DAC_Out(uint8_t data){
	 GPIO_PORTB_DATA_R = data;
}

