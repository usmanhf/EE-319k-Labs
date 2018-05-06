// main.cpp
// Runs on LM4F120/TM4C123
// Jonathan Valvano and Daniel Valvano
// This is a starter project for the EE319K Lab 10 in C++

// Last Modified: 4/19/2018 
// http://www.spaceinvaders.de/
// sounds at http://www.classicgaming.cc/classics/spaceinvaders/sounds.php
// http://www.classicgaming.cc/classics/spaceinvaders/playguide.php
/* This example accompanies the books
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2017

   "Embedded Systems: Introduction to Arm Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2017

 Copyright 2018 by Jonathan W. Valvano, valvano@mail.utexas.edu
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
// ******* Possible Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PD2/AIN5
// Slide pot pin 3 connected to +3.3V 
// fire button connected to PE0
// special weapon fire button connected to PE1
// 8*R resistor DAC bit 0 on PB0 (least significant bit)
// 4*R resistor DAC bit 1 on PB1
// 2*R resistor DAC bit 2 on PB2
// 1*R resistor DAC bit 3 on PB3 (most significant bit)
// LED on PB4
// LED on PB5

// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) unconnected
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (SSI0Fss)
// CARD_CS (pin 5) unconnected
// Data/Command (pin 4) connected to PA6 (GPIO), high for data, low for command
// RESET (pin 3) connected to PA7 (GPIO)
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "PLL.h"
#include "ST7735.h"
#include "Random.h"
#include "PLL.h"
#include "SlidePot.h"
//#include "Images.h"
#include "UART.h"
#include "Timer0.h"
#include "Timer1.h"
#include "Sound.h"
#include "shoot.h"
#include "mapCreate.h"
#include <vector>

//SlidePot my(1500,0);

extern "C" void DisableInterrupts(void);
extern "C" void EnableInterrupts(void);
extern "C" void SysTick_Handler(void);

//map of pixels(0 = dirt, 1 = no dirt)

uint32_t liveMap[160][4];    //row major   


//**********************************************************************************************************************
// main loop
//**********************************************************************************************************************
martian player;
redEnemy enemyOne;
redEnemy enemyTwo;
redEnemy enemyThree;
redEnemy enemyFour;
uint8_t enemyFlag = 1;
uint8_t points = 0;
bool movePlayer = false;
extern uint32_t data[2];

void pause(void){
  while(GPIO_PORTE_DATA_R&0x02){}
  while((GPIO_PORTE_DATA_R&0x02) == 0){
  }
  EnableInterrupts();
}

void playerKilled(uint8_t level){
  uint32_t k;
  DisableInterrupts();
  player.killed();
  if(player.stillGoing()){
  gameOver();
  for(k = 0; k < 2000000; k++){}
  for(k = 0; k < 2000000; k++){}
  for(k = 0; k < 100000; k++){}
  restartLevel(level);
  enemyOne.init(34, 120, 2, enemyOne.lifeStat());
  enemyTwo.init(100, 40, 1, enemyTwo.lifeStat());
  enemyThree.init(15, 50, 1, enemyThree.lifeStat());
  enemyFour.init(88, 125, 1, enemyFour.lifeStat());
  enemyOne.print();
  enemyTwo.print();
  enemyThree.print();  
  enemyFour.print();
  player.enterPlayer();
  EnableInterrupts();
}
  }


void PortF_Init(void){
  //*** students write this ******
  int i;
  SYSCTL_RCGCGPIO_R |= 0x20;
  for(i = 0; i < 10; i++);
  GPIO_PORTF_LOCK_R = 0x4C4F434B;
  GPIO_PORTF_CR_R = 0x000000FF;
  GPIO_PORTF_PCTL_R |= 0x000000FF;
  GPIO_PORTF_AFSEL_R &= ~0x14;
  GPIO_PORTF_AMSEL_R &= ~0x14;
  GPIO_PORTF_PCTL_R &= ~0x00FFFFFF;
  GPIO_PORTF_DIR_R |= 0x04;
  GPIO_PORTF_PUR_R |= 0x14;    //pull up resistor on switch PF1-PF4(negative logic)
  GPIO_PORTF_DEN_R |= 0xFF;
}

void Button_Init(){
    GPIO_PORTE_DIR_R &= 0xF9;
    GPIO_PORTE_DEN_R |= 0x06;
}

void moveEnemies(void){
  enemyFlag = 1;
}

void playerMove(void){
  if(!player.isShooting){
    ADC_In(data);
    movePlayer = true;
  }
}

void engine(uint8_t level){
  while(player.stillGoing()){
    if((GPIO_PORTE_DATA_R&0x02)==0x02){
      GPIO_PORTF_DATA_R ^= 0x0E;  
      player.shoot();
    } else{
      player.isShooting = false;
    }
    if(movePlayer){
      player.move();
      movePlayer = false;
      player.print();
    }else{
      if(player.wasShooting){
        player.doneShooting();
      }
      movePlayer = false;
    }
    if(player.isShooting){
      enemyOne.checkShot();
      enemyTwo.checkShot();
      enemyThree.checkShot();
      enemyFour.checkShot();
      }
    if(enemyFlag == 1){
      enemyOne.move(player.xCord(), player.yCord());
      enemyTwo.move(player.xCord(), player.yCord());
      enemyThree.move(player.xCord(), player.yCord());
      enemyFour.move(player.xCord(), player.yCord());
      enemyOne.print();
      enemyTwo.print();
      enemyThree.print();
      enemyFour.print();
      enemyFlag = 0;
			GPIO_PORTF_DATA_R ^= 0x0E;
    }
    
    if(enemyOne.collision(player.xCord(), player.yCord()) || enemyTwo.collision(player.xCord(), player.yCord()) || enemyThree.collision(player.xCord(), player.yCord()) ||enemyFour.collision(player.xCord(), player.yCord())){
        Sound_End();
				playerKilled(level);
		
        if(!player.stillGoing()){break;}
    }
    if((GPIO_PORTE_DATA_R&0x04)==0x04){
				DisableInterrupts();
        pause();
    }
    if(!enemyOne.lifeStat() && !enemyTwo.lifeStat() && !enemyThree.lifeStat() && !enemyFour.lifeStat()){
      DisableInterrupts();
      return;
    }
		if(level == 1 && points == 4){return;}
		if(level == 2 && points == 8){return;}
		if(level == 3 && points == 12){return;}
    
  }
}

//**************************************************************************
//levels
//**************************************************************************

void levelOne(void){
  fillLiveMap();
  printLevelOne();
  player.init(0, 16, 3, 3);
  enemyOne.init(34, 120, 2, true);
  enemyTwo.init(100, 40, 1, true);
  enemyThree.init(15, 50, 1, true);
  enemyFour.init(88, 125, 1, true);
  enemyOne.print();
  enemyTwo.print();
  enemyThree.print();  
  enemyFour.print();
  player.enterPlayer();
	Sound_Next();
  EnableInterrupts();
  engine(1);
}

void levelTwo(void){
  Sound_Killed();
  fillLiveMap();
  printLevelTwo();
  player.init(0, 16, 3, player.lives);
  enemyOne.init(34, 120, 2, true);
  enemyTwo.init(100, 40, 1, true);
  enemyThree.init(15, 50, 1, true);
  enemyFour.init(88, 125, 1, true);
  enemyOne.print();
  enemyTwo.print();
  enemyThree.print();  
  enemyFour.print();
	Timer1_Init(&moveEnemies,7999999);
  player.enterPlayer();
	Sound_Next();
  EnableInterrupts();
  engine(2);
}

void levelThree(void){
  Sound_Killed();
  fillLiveMap();
  printLevelThree();
  player.init(0, 16, 3, player.lives);
  enemyOne.init(34, 120, 2, true);
  enemyTwo.init(100, 40, 1, true);
  enemyThree.init(15, 50, 1, true);
  enemyFour.init(91, 125, 1, true);
  enemyOne.print();
  enemyTwo.print();
  enemyThree.print();  
  enemyFour.print();
	Timer1_Init(&moveEnemies,6999999); 
  player.enterPlayer();
	Sound_Next();
  EnableInterrupts();
  engine(3);
}

int main(void){
	bool again = true;
  PLL_Init(Bus80MHz);       // Bus clock is 80 MHz 
  Random_Init(1);
  PortF_Init();
  GPIO_PORTF_DATA_R &= ~0x0E;
  DisableInterrupts();
  Output_Init();
  ADC_Init();
  Button_Init();
  Sound_Init();
  Timer0_Init(&playerMove,15000000); 
  Timer1_Init(&moveEnemies,8999999); 
	while(again){
		intro();
		Sound_Killed();
		levelOne();
		if(player.stillGoing()){
			levelTwo();
		}
		if(player.stillGoing()){
			levelThree();
		}
		outro();
		while(1){
			if(GPIO_PORTE_DATA_R&0x06){
				again = true;
				break;
			}
    }
	}
}








