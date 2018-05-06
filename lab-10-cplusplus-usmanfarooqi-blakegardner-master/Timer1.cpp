// Timer1.c
// Runs on LM4F120/TM4C123
// Use TIMER1 in 32-bit periodic mode to request interrupts at a periodic rate
// Daniel Valvano
// Last Modified: 3/6/2015 
// You can use this timer only if you learn how it works

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
#include <stdint.h>
#include "Timer1.h"
#include "../inc/tm4c123gh6pm.h"
#include "Images.h"
#include "ST7735.h"
#include "Random.h"
#include "shoot.h"
#include "Sound.h"

extern uint32_t liveMap[160][4];
extern shootG shot;

void (*PeriodicTask1)(void);   // user function
//extern "C" void TIMER1A_Handler(void);

// ***************** TIMER1_Init ****************
// Activate TIMER1 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
// Outputs: none
void Timer1_Init(void(*task)(void), uint32_t period){
  SYSCTL_RCGCTIMER_R |= 0x02;   // 0) activate TIMER1
  PeriodicTask1 = task;          // user function
  TIMER1_CTL_R = 0x00000000;    // 1) disable TIMER1A during setup
  TIMER1_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER1_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER1_TAILR_R = period-1;    // 4) reload value
  TIMER1_TAPR_R = 0;            // 5) bus clock resolution
  TIMER1_ICR_R = 0x00000001;    // 6) clear TIMER1A timeout flag
  TIMER1_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0xFFFF00FF)|0x00008000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 37, interrupt number 21
  NVIC_EN0_R = 1<<21;           // 9) enable IRQ 21 in NVIC
  TIMER1_CTL_R = 0x00000001;    // 10) enable TIMER1A
}
extern "C" void TIMER1A_Handler(void);
extern void moveEnemies(void);

bool redEnemy::lifeStat(void){return this->life;}

void redEnemy::init(uint8_t xCor, uint8_t yCor, uint8_t dir, bool life){
	this->life = life;
	this->shotCount = 0;
	this->beingShot = false;
	this->stateCount = 0;
	this->x = xCor;
	this->y = yCor;
  this->direction = dir % 4;
	if(this->direction != 2){
		this->state = redEnemyStand;
	} else{
		this->state = redEnemyStandLeft;
	}
}



bool redEnemy::checkNorth(void){
	uint32_t indexLeft = 0x00000001 << (this->x % 32);
	uint32_t indexMiddle = 0x00000001 << ((this->x + 7) % 32);
	uint32_t indexRight = 0x00000001 << ((this->x + 13) % 32);
	uint32_t checkLeft = liveMap[(this->y - 14)][(this->x / 32)] & indexLeft;
	uint32_t checkMiddle = liveMap[(this->y - 14)][((this->x + 7) / 32)] & indexMiddle;
	uint32_t checkRight = liveMap[(this->y - 14)][((this->x + 13) / 32)] & indexRight;
	if((checkLeft) && (checkRight) && (checkMiddle)){
		return true;
	} else{
		return false;
	}
}

bool redEnemy::checkNorthTunnel(uint8_t py){
	uint8_t i;
	uint32_t index;
	uint32_t check;
	for(i = 0; i < 20; i++){
		index = 0x00000001 << ((this->x + 7) % 32);
		check = liveMap[(this->y - 14 - i)][((this->x + 7) / 32)] & index;
		if((this->y - 14 - i) == py){return true;}
		else if(check){continue;}
		else{return false;}
	}
	return true;
}

bool redEnemy::checkSouth(void){
	uint32_t indexLeft = 0x00000001 << (this->x % 32);
	uint32_t indexMiddle = 0x00000001 << ((this->x + 7) % 32);
	uint32_t indexRight = 0x00000001 << ((this->x + 13) % 32);
	uint32_t checkLeft = liveMap[(this->y + 1)][(this->x / 32)] & indexLeft;
	uint32_t checkRight = liveMap[(this->y + 1)][((this->x + 13) / 32)] & indexRight;
	uint32_t checkMiddle = liveMap[(this->y + 1)][((this->x + 7) / 32)] & indexMiddle;
	if((checkLeft) && (checkRight) && (checkMiddle)){
		return true;
	} else{
		return false;
	}
}

bool redEnemy::checkSouthTunnel(uint8_t py){
	uint8_t i;
	uint32_t index;
	uint32_t check;
	for(i = 0; i < 20; i++){
		index = 0x00000001 << ((this->x + 7) % 32);
		check = liveMap[(this->y + 1 + i)][((this->x + 7) / 32)] & index;
		if((this->y + i) == py){return true;} 
		else if(check){continue;}
		else{return false;}
	}
	return true;
}

bool redEnemy::checkEast(void){
	uint32_t indexTop = 0x00000001 << ((this->x - 1) % 32);
	uint32_t checkTop = liveMap[(this->y - 13)][((this->x - 1) / 32)] & indexTop;
	uint32_t checkMiddle = liveMap[(this->y - 7)][((this->x - 1) / 32)] & indexTop;
	uint32_t checkBottom = liveMap[this->y][((this->x - 1) / 32)] & indexTop;
	if((checkTop) && (checkBottom) && (checkMiddle)){
		return true;
	} else{
		return false;
	}
}

bool redEnemy::checkEastTunnel(uint8_t px){
	uint8_t i;
	uint32_t index;
	uint32_t check;
	for(i = 0; i < 20; i++){
		index = 0x00000001 << ((this->x - 1 - i) % 32);
		check = liveMap[(this->y - 7)][((this->x - 1 - i) / 32)] & index;
		if((this->x - i) == px){return true;}
		else if(check){continue;}
		else{return false;}
	}
	return true;
}

bool redEnemy::checkWest(void){
	uint32_t indexTop = 0x00000001 << ((this->x + 14) % 32);
	uint32_t checkTop = liveMap[(this->y - 13)][((this->x + 14) / 32)] & indexTop;
	uint32_t checkMiddle = liveMap[(this->y - 7)][((this->x + 14) / 32)] & indexTop;
	uint32_t checkBottom = liveMap[this->y][((this->x + 14) / 32)] & indexTop;
	if((checkTop) && (checkBottom) && (checkMiddle)){
		return true;
	} else{
		return false;
	}
}

bool redEnemy::checkWestTunnel(uint8_t px){
	uint8_t i;
	uint32_t index;
	uint32_t check;
	for(i = 0; i < 20; i++){
		index = 0x00000001 << ((this->x + 14 + i) % 32);
		check = liveMap[(this->y - 7)][((this->x + 14 + i) / 32)] & index;
		if((this->x + 14 + i) == px){return true;}
		else if(check){continue;}
		else{return false;}
	}
	return true;
}

bool redEnemy::Y_dist(uint8_t p1){
	if(this->y > p1){return true;}
	else{return false;}
}
bool redEnemy::X_dist(uint8_t p1){
	if(this->x > p1){return true;}
	else{return false;}
}

void redEnemy::move(uint8_t px, uint8_t py){
	if(this->life){
	if(!this->beingShot){
	this->shotCount = 0;
	this->stateCount++;
	uint8_t xDist;
	uint8_t yDist;
	if(px >= this->x){xDist = px - this->x;}
	else{xDist = this->x - px;}
	if(py >= this->y){yDist = py - this->y;}
	else{yDist = this->y - py;}
	
	if(xDist < 20 || yDist < 20){
		if(yDist >= xDist){ //we should try to turn up or down first
		if(Y_dist(py)){
			if(checkNorthTunnel(py)){
				if(!checkNorth()){
					if(this->direction == 2){if(checkEast()){this->x--; this->state = redEnemyStandLeft; this->stateCount++;print();}else{this->direction = 3;}}
					else if(this->direction == 3){if(checkWest()){this->x++; this->state = redEnemyStand; this->stateCount++;print();}else{this->direction = 2;}}
					else{ normalCheck(); return;}
				}
				this->y--;
				this->direction = 0; this->state = redEnemyStand;
			} else{
				normalCheck();
			}
		} else{
			if(checkSouthTunnel(py)){
				if(!checkSouth()){
					if(this->direction == 2){if(checkEast()){this->x--; this->state = redEnemyStandLeft; this->stateCount++;print();}else{this->direction = 3;}}
					else if(this->direction == 3){if(checkWest()){this->x++; this->state = redEnemyStand; this->stateCount++;print();}else{this->direction = 2;}}
					else{ normalCheck(); return;}
				}
				this->y++;
				this->direction = 1; this->state = redEnemyStand;
			} else{
				normalCheck();
			}
		}
	} else{							 //we should try to turn left or right first
		if(X_dist(px)){
			if(checkEastTunnel(px)){
				if(!checkEast()){
					if(this->direction == 0){if(checkNorth()){this->y--; this->state = redEnemyStand; this->stateCount++;print();} else{this->direction = 1;}}
						else if(this->direction == 1){if(checkSouth()){this->y++; this->state = redEnemyStand; this->stateCount++;print();}else{this->direction = 0;}}
					else{ normalCheck(); return;}
				}
				this->x--;
				this->direction = 2; this->state = redEnemyStandLeft;
			} else{
				normalCheck();
			}
		} else{
			if(checkWestTunnel(px)){
					if(!checkWest()){
						if(this->direction == 0){if(checkNorth()){this->y--; this->state = redEnemyStand; this->stateCount++;print();} else{this->direction = 1;}}
						else if(this->direction == 1){if(checkSouth()){this->y++; this->state = redEnemyStand; this->stateCount++;print();}else{this->direction = 0;}}
						else{ normalCheck(); return;}
					}
					this->x++;
					this->direction = 3; this->state = redEnemyStand;
				} else{
				normalCheck();
				}	
		}
	}
	} else{
		normalCheck();
	}
}
}
}

void redEnemy::normalCheck(void){
switch(this->direction){
		case 0 :
			if(checkNorth()){
				if(this->y > 16){this->y--;}
				else{this->direction = (Random() % 2) + 2;}
			}
			else if(checkEast()){this->direction = 2; this->state = redEnemyStandLeft;}
			else if(checkWest()){this->direction = 3; /*this->state = redEnemyStand;*/}
			else{this->direction = 1;}
			break;
		case 1 :
			if(checkSouth()){
				if(this->y < 159){this->y++;}
				else{this->direction = (Random() % 2) + 2;}
			}
			else if(checkEast()){this->direction = 2; this->state = redEnemyStandLeft;}
			else if(checkWest()){this->direction = 3; /*this->state = redEnemyStand;*/}
			else{this->direction = 0;}
			break;
		case 2 :
			if(checkEast()){
				if(this->x > 0){this->x--;}
				else{this->direction = Random() % 2;}
			}
			else if(checkNorth()){this->direction = 0; this->state = redEnemyStand;}
			else if(checkSouth()){this->direction = 1; this->state = redEnemyStand;}
			else{this->direction = 3; this->state = redEnemyStand;}
			break;
		case 3 :
			if(checkWest()){
				if(this->x < 127){this->x++;}
				else{this->direction = Random() % 2;}
			}
			else if(checkNorth()){this->direction = 0;}
			else if(checkSouth()){this->direction = 1;}
			else{this->direction = 2; this->state = redEnemyStandLeft;}
			break;
	}
}

void redEnemy::print(void){
	if(this->life){
	ST7735_DrawBitmap(this->x, this->y, this->state, 14, 14);
	if(this->stateCount >= 3){
		if(this->state == redEnemyStand){
			this->state = redEnemyRun;
		} else if(this->state == redEnemyRun){
			this->state = redEnemyStand;
		} else if(this->state == redEnemyStandLeft){
			this->state = redEnemyRunLeft;
		} else{
			this->state = redEnemyStandLeft;
		}
		this->stateCount = 0;
	}
}
}

bool redEnemy::collision(uint8_t px, uint8_t py){
	if(this->life){
	if((((this->x + 4) >= px) &&((this->y - 10) >= (py - 13))) && (((this->x + 4) <= (px + 13)) && ((this->y - 10) <= py))){
		return true;
	}
	else if((((this->x + 3) >= px) && ((this->y - 3) <= py)) && (((this->x + 3) <= (px + 13)) && ((this->y - 3) >= (py - 13)))){
		return true;
	}
	else if((((this->x + 10) <= (px + 13)) && ((this->y - 3) <= py)) && (((this->x + 10) >= px) && ((this->y - 3) >= (py - 13)))){
		return true;
	}
	else if((((this->x + 10) >= px) && ((this->y - 10) <= py)) && (((this->x + 10) <= (px + 13)) && ((this->y - 10) >= (py - 13)))){
		return true;
	}
	return false;
} return false;
}

void redEnemy::checkShot(void){
	uint8_t xs = shot.xCord();
	uint8_t ys = shot.yCord();
	uint8_t dir = shot.dir();
	if((dir == 0) && checkSouthTunnel(ys + 14)){
		if((this->y >= (ys - 10)) && (this->y < ys) && (this->x >= (xs - 13)) && (this->x < xs)){
			this->beingShot = true;
			this->shotCount++;
		}
	} else if((dir == 1) && checkNorthTunnel(ys - 14)){
		if(((this->y - 13) < (ys + 10)) && ((this->y - 13) > ys) && (this->x >= (xs - 13)) && (this->x < xs)){
			this->beingShot = true;
			this->shotCount++;
		}
	} else if((dir == 2) && checkWestTunnel(xs + 1)){
		if(((this->y - 13) < (ys)) && (this->y > ys) && ((this->x + 13) >= (xs - 10)) && ((this->x + 13) < xs)){
			this->beingShot = true;
			this->shotCount++;
		}
	} else if(checkEastTunnel(xs - 14)){
		if(((this->y - 13) < (ys)) && (this->y > ys) && (this->x <= (xs + 10)) && (this->x > xs)){
			this->beingShot = true;
			this->shotCount++;
		}
	}
	checkDead();
	
}

extern uint8_t points;

bool redEnemy::checkDead(void){
	if(this->lifeStat()){
	if(this->shotCount >= 1){
		this->life = false;
		ST7735_DrawBitmap(this->x, this->y, disappear, 14, 14);
		points++;
		Sound_Shoot();
		return true;
	}
}
	return false;
	
}

//*********************************************************************************************************************
//*********************************************************************************************************************

extern "C" void TIMER1A_Handler(void);
void TIMER1A_Handler(void){
  TIMER1_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER1A timeout
  (*PeriodicTask1)();                // execute user task
}

