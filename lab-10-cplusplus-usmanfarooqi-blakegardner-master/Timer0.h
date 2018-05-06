// Timer0.h
// Runs on LM4F120/TM4C123
// Use Timer0 in 32-bit periodic mode to request interrupts at a periodic rate
// Daniel Valvano
// March 20, 2014

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2013
  Program 7.5, example 7.6

 Copyright 2013 by Jonathan W. Valvano, valvano@mail.utexas.edu
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

#ifndef __TIMER0INTS_H__ // do not include more than once
#define __TIMER0INTS_H__
#include <stdint.h>


// ***************** Timer0_Init ****************
// Activate Timer0 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
// Outputs: none
void Timer0_Init(void(*task)(void), uint32_t period);

class martian{
private:
	uint8_t x;
	uint8_t y;
	uint8_t direction;
	const unsigned short* state;
	uint16_t stateCount;
	
	bool life; //0 is dead and 1 is alive
	bool form; // 0 is normal and 1 is fly
	void moveNorth(void);
	void moveSouth(void);
	void moveEast(void);
	void moveWest(void);
public:
	int8_t lives;
	bool wasShooting;
	uint8_t shootingTime;
	bool isShooting;
	uint8_t xCord(void);
	uint8_t yCord(void);
	void init(uint8_t xCor, uint8_t yCor, uint8_t dir, int8_t lives);
	void move(void);
	void print(void);
	void fly(void);
	void enterPlayer(void);
	void killed(void);
	bool stillGoing(void);
	void shoot(void);
	void doneShooting(void);
};

#endif // __TIMER2INTS_H__
