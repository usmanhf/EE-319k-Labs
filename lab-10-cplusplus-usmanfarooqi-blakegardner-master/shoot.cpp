#include "shoot.h"
#include "Images.h"
#include "ST7735.h"

extern uint32_t liveMap[160][4];


void shootG::shootInit(){
    this->isShooting=false;
		this->state = Laser;
}
void shootG::FIRE(uint8_t x, uint8_t y, uint8_t direction){
		this->direction = direction;
    if(direction == 0){ //north
			this->xPos = x + 9;
			this->yPos = y - 14;
			printLaserNorth();
    }
    if(direction == 1){ //south
				this->xPos = x + 9;
				this->yPos = y + 1;
        printLaserSouth();
    }
    if(direction == 2){ //east
				this->xPos = x - 1;
			this->yPos = y - 4;
        printLaserEast();
    }
    if(direction == 3){ //west
				this->xPos = x + 14;
			this->yPos = y - 4;
        printLaserWest();
    }
}

bool shootG::isReadyFire(){
    return !(this->isShooting);
}

void shootG::eraseLaser(void){
	if(this->direction == 0){ //north
				eraseLaserNorth();
    }
    if(this->direction == 1){ //south
        eraseLaserSouth();
    }
    if(this->direction == 2){ //east
        eraseLaserEast();
    }
    if(this->direction == 3){ //west
        eraseLaserWest();
    }
}

void shootG::eraseLaserNorth(void){
	uint8_t i;
	uint32_t index;
	uint32_t check;
	for(i = 0; i < 10;i++){
		index = 0x00000001 << ((this->xPos) % 32);
		check = liveMap[(this->yPos - i)][((this->xPos) / 32)] & index;
		if(check){
			ST7735_DrawPixel((this->xPos), (this->yPos - i), 0x0000);
		}else{
			break;
		}
	}
}

void shootG::eraseLaserSouth(void){
		uint8_t i;
	uint32_t index;
	uint32_t check;
	for(i = 0; i < 10;i++){
		index = 0x00000001 << ((this->xPos) % 32);
		check = liveMap[(this->yPos + i)][((this->xPos) / 32)] & index;
		if(check){
			ST7735_DrawPixel((this->xPos), (this->yPos + i), 0x0000);
		}else{
			break;
		}
	}
}

void shootG::eraseLaserEast(void){
		uint8_t i;
	uint32_t index;
	uint32_t check;
	for(i = 0; i < 10;i++){
		index = 0x00000001 << ((this->xPos - i) % 32);
		check = liveMap[(this->yPos)][((this->xPos - i) / 32)] & index;
		if(check){
			ST7735_DrawPixel((this->xPos - i), (this->yPos), 0x0000);
		}else{
			break;
		}
	}
}

void shootG::eraseLaserWest(void){
	uint8_t i;
	uint32_t index;
	uint32_t check;
	for(i = 0; i < 10;i++){
		index = 0x00000001 << ((this->xPos + i) % 32);
		check = liveMap[(this->yPos)][((this->xPos + i) / 32)] & index;
		if(check){
			ST7735_DrawPixel((this->xPos + i), (this->yPos), 0x0000);
		}else{
			break;
		}
	}
}


void shootG::printLaserNorth(void){
	uint8_t i;
	uint32_t index;
	uint32_t check;
	for(i = 0; i < 10;i++){
		index = 0x00000001 << ((this->xPos) % 32);
		check = liveMap[(this->yPos - i)][((this->xPos) / 32)] & index;
		if(check){
			ST7735_DrawPixel((this->xPos), (this->yPos - i), 0x003F);
		}else{
			break;
		}
	}
}

void shootG::printLaserSouth(void){
	uint8_t i;
	uint32_t index;
	uint32_t check;
	for(i = 0; i < 10;i++){
		index = 0x00000001 << ((this->xPos) % 32);
		check = liveMap[(this->yPos + i)][((this->xPos) / 32)] & index;
		if(check){
			ST7735_DrawPixel((this->xPos), (this->yPos + i), 0x003F);
		}else{
			break;
		}
	}
}

void shootG::printLaserEast(void){
	uint8_t i;
	uint32_t index;
	uint32_t check;
	for(i = 0; i < 10;i++){
		index = 0x00000001 << ((this->xPos - i) % 32);
		check = liveMap[(this->yPos)][((this->xPos - i) / 32)] & index;
		if(check){
			ST7735_DrawPixel((this->xPos - i), (this->yPos), 0x003F);
		}else{
			break;
		}
	}
}

void shootG::printLaserWest(void){
	uint8_t i;
	uint32_t index;
	uint32_t check;
	for(i = 0; i < 10;i++){
		index = 0x00000001 << ((this->xPos + i) % 32);
		check = liveMap[(this->yPos)][((this->xPos + i) / 32)] & index;
		if(check){
			ST7735_DrawPixel((this->xPos + i), (this->yPos), 0x003F);
		}else{
			break;
		}
	}
}