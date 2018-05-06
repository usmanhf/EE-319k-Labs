// Timer0.c
// Runs on LM4F120/TM4C123
// Use TIMER0 in 32-bit periodic mode to request interrupts at a periodic rate
// Daniel Valvano
// Last Modified: 3/6/2015 
// You can use this timer only if you learn how it works

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2015
  Program 7.5, example 7.6

 Copyright 2015 by Jonathan W. Valvano, valvano@mail.utexas.edu
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
#include <stdint.h>
#include "Timer0.h"
#include "../inc/tm4c123gh6pm.h"
#include "Images.h"
#include "ST7735.h"
#include "SlidePot.h"
#include "shoot.h"

extern uint32_t liveMap[160][4];
extern "C" void TIMER0A_Handler(void);
shootG shot; //global

void (*PeriodicTask0)(void);   // user function

// ***************** Timer0_Init ****************
// Activate TIMER0 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
// Outputs: none
void Timer0_Init(void(*task)(void), uint32_t period){
  SYSCTL_RCGCTIMER_R |= 0x01;   // 0) activate TIMER0
  PeriodicTask0 = task;          // user function
  TIMER0_CTL_R = 0x00000000;    // 1) disable TIMER0A during setup
  TIMER0_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER0_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER0_TAILR_R = period-1;    // 4) reload value
  TIMER0_TAPR_R = 0;            // 5) bus clock resolution
  TIMER0_ICR_R = 0x00000001;    // 6) clear TIMER0A timeout flag
  TIMER0_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x80000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 35, interrupt number 19
  NVIC_EN0_R = 1<<19;           // 9) enable IRQ 19 in NVIC
  TIMER0_CTL_R = 0x00000001;    // 10) enable TIMER0A
}
void TIMER0A_Handler(void){
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER0A timeout
  (*PeriodicTask0)();                // execute user task
}

//********************************************************************************************************************
//martian class
//********************************************************************************************************************
extern void setMapPixel(uint8_t x, uint8_t y);

void martian::init(uint8_t xCor, uint8_t yCor, uint8_t dir, int8_t lives){
	this->wasShooting = false;
	this->life = true;
	this->stateCount = 0;
	this->lives = lives;
	this->x = xCor;
	this->y = yCor;
	this->direction = dir % 4;
	if(this->direction == 0){
		this->state = martianStandUp;
	} else if(this->direction == 1){
		this->state = martianStandDown;
	} else if(this->direction == 2){
		this->state = martianStandLeft;
	} else{
		this->state = martianStandRight;
	}
}

void martian::moveNorth(void){
	if(this->y > 16){
		this->direction = 0;
		this->stateCount++;
		this->y--;
		uint8_t i;
		for(i = 0; i < 14; i++){
			setMapPixel((this->x + i), (this->y - 13));
		}
	}
}
void martian::moveSouth(void){
	if(this->y < 158){
		this->direction = 1;
		this->stateCount++;
		this->y++;
		uint8_t i;
		for(i = 0; i < 14; i++){
			setMapPixel((this->x + i), this->y);
		}
	}
}
void martian::moveEast(void){
	if(this->x > 1){
		this->direction = 2;
		this->stateCount++;
		this->x--;
		uint8_t i;
		for(i = 0; i < 14; i++){
			setMapPixel(this->x, (this->y - i));
		}
	}
}
void martian::moveWest(void){
	if(this->x < 113){
		this->direction = 3;
		this->stateCount++;
		this->x++;
		uint8_t i;
		for(i = 0; i < 14; i++){
			setMapPixel((this->x + 13), (this->y - i));
		}
	}
}



//holds adc input
uint32_t data[2];

void martian::move(void){

	if(((data[0] > 3200) && (data[0] < 3400)) && ((data[1] > 2700) && (data[1] < 2900))){		//move up
		this->state = martianStandUp;
		print();
		moveNorth();
	print();
		moveNorth();
	} else if(((data[0] > 3050) && (data[0] < 3350)) && (data[1] > 3050 && data[1] < 3350)){		//move down

	} else if((((data[0] > 3900) && (data[0] < 4300)) && ((data[1] > 3900) && (data[1] < 4300))) || (((data[0] > 2800) && (data[0] < 3000)) && ((data[1] > 2800) && (data[1] < 3000)))){		//move down
		this->state = martianStandRight;
		print();
		moveWest();
	print();
		moveWest();
	} else if( ((data[1] > 3900) && (data[1] < 4200))){		//move left
		this->state = martianStandLeft;
		print();
		moveEast();
		print();
		moveEast();
	} else{
		this->state = martianStandDown;
		print();
		moveSouth();
		print();
		moveSouth();
	}
}


void martian::print(void){
	ST7735_DrawBitmap(this->x, this->y, this->state, 14, 14);
	if(this->stateCount >= 3){
	if(this->state == martianStandRight){
		this->state = martianRunRight;
	} else if(this->state == martianRunRight){
		this->state = martianStandRight;
	} else if(this->state == martianStandLeft){
		this->state = martianRunLeft;
	} else if(this->state == martianRunLeft){
		this->state = martianStandLeft;
	} else if(this->state == martianStandUp){
		this->state = martianRunUp;
	} else if(this->state == martianRunUp){
		this->state = martianStandUp;
	} else if(this->state == martianStandDown){
		this->state = martianRunDown;
	} else if(this->state == martianRunDown){
		this->state = martianStandDown;
	}
	this->stateCount = 0;
}
	
}

//intro theatrics
void martian::enterPlayer(){
	uint8_t i;
	uint32_t j;
	init(0, 16, 3, this->lives);
	print();
	for(i = 0; i < 58; i++){
		moveWest();
		print();
		for(j = 0; j < 500000; j++);
	}
	this->state = martianStandDown;
	print();
	for(i = 0; i < 75; i++){
		moveSouth();
		print();
		for(j = 0; j < 500000; j++);
	}
	this->state = martianStandRight;
	this->direction = 3;
	//for(j = 0; j < 500000; j++);
	print();
}

uint8_t martian::xCord(void){
	return this->x;
}	
uint8_t martian::yCord(void){
	return this->y;
}

void martian::killed(void){
	life = false;
	lives--;
}
bool martian::stillGoing(void){
	if(lives > 0){
		life = true;
		return true;
	} else{
		return false;
	}
}



void martian::shoot(){
	isShooting = true;
   shot.FIRE(this->x, this->y, this->direction);
	wasShooting = true;

}

void martian::doneShooting(void){
	shot.eraseLaser();
	wasShooting = false;
}
//********************************************************************************************************************
//********************************************************************************************************************

