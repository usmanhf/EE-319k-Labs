
#include "mapCreate.h"
#include <stdint.h>
#include <vector>
#include "ST7735.h"
#include "Images.h"
#include "Sound.h"

using namespace std;

extern uint32_t liveMap[160][4];


void setMapPixel(uint8_t x, uint8_t y){
  uint32_t index = 0x00000001 << (x % 32);
  liveMap[y][(x / 32)] |= index;
}

void fillLiveMap(){                //fills map with dirt except for the surface
  int i;
  int j;
  for(i = 0; i <= 2; i++){
    for(j = 0; j < 4; j++){
      liveMap[i][j] &= 0;
    }    
  }
  for(i = 3; i <= 16; i++){
    for(j = 0; j < 4; j++){
      liveMap[i][j] |= 0xFFFFFFFF;
    }    
  }
  for(i = 17; i < 160; i++){
    for(j = 0; j < 4; j++){
      liveMap[i][j] &= 0;
    }    
  }
  for(i = 0; i < 75; i++){
  }
}

void printTunnelRight(uint16_t x, uint16_t y){
  int i;
  ST7735_DrawPixel(x, y, 0x0000);
  setMapPixel(x,y);
  ST7735_DrawPixel((x + 1), (y - 1), 0x0000);
  setMapPixel((x + 1),(y - 1));
  ST7735_DrawPixel((x + 1), (y - 12), 0x0000);
  setMapPixel((x + 1),(y - 12));
  ST7735_DrawPixel(x, (y - 13), 0x0000);
  setMapPixel(x,(y - 13));
  for(i = 0; i < 10; i++){
    ST7735_DrawPixel((x + 2), (y - 2 - i), 0x0000);
    setMapPixel((x + 2),(y - 2 - i));
  }
}

void printTunnelLeft(uint16_t x, uint16_t y){
  int i;
  ST7735_DrawPixel(x, y, 0x0000);
  setMapPixel(x,y);
  ST7735_DrawPixel((x - 1), (y - 1), 0x0000);
  setMapPixel((x - 1),(y - 1));
  ST7735_DrawPixel((x - 1), (y - 12), 0x0000);
  setMapPixel((x - 1),(y - 12));
  ST7735_DrawPixel(x, (y - 13), 0x0000);
  setMapPixel(x,(y - 13));
  for(i = 0; i < 10; i++){
    ST7735_DrawPixel((x - 2), (y - 2 - i), 0x0000);
    setMapPixel((x - 2),(y - 2 - i));
  }
}


void printTunnelDown(uint16_t x, uint16_t y){
  int i;
  ST7735_DrawPixel(x, (y - 2), 0x0000);
  setMapPixel(x,(y - 2));
  ST7735_DrawPixel((x + 1), (y - 1), 0x0000);
  setMapPixel((x + 1),(y - 1));
  ST7735_DrawPixel((x + 12), (y - 1), 0x0000);
  setMapPixel((x + 12),(y - 1));
  ST7735_DrawPixel((x + 13), (y - 2), 0x0000);
  setMapPixel((x + 13),(y - 2));
  for(i = 2; i < 12; i++){
    ST7735_DrawPixel((x + i), y, 0x0000);
    setMapPixel((x + i),y);
  }
}

void printTunnelUp(uint16_t x, uint16_t y){
  int i;
  ST7735_DrawPixel(x, (y + 2), 0x0000);
  setMapPixel(x,(y + 2));
  ST7735_DrawPixel((x + 1), (y + 1), 0x0000);
  setMapPixel((x + 1),(y + 1));
  ST7735_DrawPixel((x + 12), (y + 1), 0x0000);
  setMapPixel((x + 12),(y + 1));
  ST7735_DrawPixel((x + 13), (y + 2), 0x0000);
  setMapPixel((x + 13),(y + 2));
  for(i = 2; i < 12; i++){
    ST7735_DrawPixel((x + i), y, 0x0000);
    setMapPixel((x + i), y);
  }
  
}


void printLevelOne(void){
  int i;
	ST7735_DrawBitmap(0, 159, Background, 128,160);
  ST7735_DrawBitmap(112, 16, flower, 14,14);
  //player start tunnel
  for(i = 0; i < 20; i++){
    printTunnelRight(63 + i, 91);
  }
  for(i = 0; i < 20; i++){
    printTunnelLeft(66 - i, 91);
  }
  //enemy1 start tunnel
  for(i = 0; i < 20; i++){
    printTunnelUp(15, 50 - i);
  }
  for(i = 0; i < 20; i++){
    printTunnelDown(15, 50 + i);
  }
  //enemy2 start tunnel
  for(i = 0; i < 17; i++){
    printTunnelLeft(103 - i, 40);
  }
  for(i = 0; i < 17; i++){
    printTunnelRight(100 + i, 40);
  }
  //enemy3 start tunnel
  for(i = 0; i < 23; i++){
    printTunnelLeft(36 - i, 120);
  }
  for(i = 0; i < 23; i++){
    printTunnelRight(33 + i, 120);
  }
  //enemy4 start tunnel
  for(i = 0; i < 21; i++){
    printTunnelUp(88, 115 - i);
  }
  for(i = 0; i < 21; i++){
    printTunnelDown(88, 115 + i);
  }
}

void printLevelTwo(void){
  int i;
	ST7735_DrawBitmap(0, 159, Background, 128,160);
  ST7735_DrawBitmap(112, 16, flower, 14,14);
  ST7735_DrawBitmap(98, 16, flower, 14,14);
  //player start tunnel
  for(i = 0; i < 20; i++){
    printTunnelRight(63 + i, 91);
  }
  for(i = 0; i < 20; i++){
    printTunnelLeft(66 - i, 91);
  }
  //enemy1 start tunnel
  for(i = 0; i < 40; i++){
    printTunnelUp(15, 50 - i);
  }
  for(i = 0; i < 20; i++){
    printTunnelDown(15, 50 + i);
  }
  //enemy2 start tunnel
  for(i = 0; i < 45; i++){
    printTunnelLeft(103 - i, 40);
  }
  for(i = 0; i < 17; i++){
    printTunnelRight(100 + i, 40);
  }
  //enemy3 start tunnel
  for(i = 0; i < 23; i++){
    printTunnelLeft(36 - i, 120);
  }
  for(i = 0; i < 23; i++){
    printTunnelRight(33 + i, 120);
  }
  //enemy4 start tunnel
  for(i = 0; i < 21; i++){
    printTunnelUp(88, 115 - i);
  }
  for(i = 0; i < 21; i++){
    printTunnelDown(88, 115 + i);
  }
}

void printLevelThree(void){
	ST7735_DrawBitmap(0, 159, Background, 128,160);
  ST7735_DrawBitmap(112, 16, flower, 14,14);
  ST7735_DrawBitmap(98, 16, flower, 14,14);
  ST7735_DrawBitmap(84, 16, flower, 14,14);
  int i;
  //player start tunnel
  for(i = 0; i < 40; i++){
    printTunnelRight(63 + i, 91);
  }
  for(i = 0; i < 40; i++){
    printTunnelLeft(66 - i, 91);
  }
  //enemy1 start tunnel
  for(i = 0; i < 12; i++){
    printTunnelLeft(15 - i, 50);
  }
  for(i = 0; i < 12; i++){
    printTunnelRight(15 + i, 50);
  }
  //enemy2 start tunnel
  for(i = 0; i < 45; i++){
    printTunnelLeft(103 - i, 40);
  }
  for(i = 0; i < 17; i++){
    printTunnelRight(100 + i, 40);
  }
  //enemy3 start tunnel
  for(i = 0; i < 15; i++){
    printTunnelLeft(36 - i, 120);
  }
  for(i = 0; i < 40; i++){
    printTunnelRight(33 + i, 120);
  }
  for(i = 0; i < 72; i++){
    printTunnelUp(33, 120 - i);
  }
  
  //enemy4 start tunnel
  for(i = 0; i < 76; i++){
    printTunnelUp(91, 115 - i);
  }
  for(i = 0; i < 19; i++){
    printTunnelDown(91, 115 + i);
  }
}
//**********************************************************************************************************************
//intro screen and menu
//**********************************************************************************************************************

void intro(void){
  ST7735_DrawBitmap(0, 159, startScreen, 128,160);
}

//**********************************************************************************************************************
//outro;
//**********************************************************************************************************************

void LCD_OutDec(uint32_t input){
  uint32_t n = input;
  if(n<10){
    ST7735_OutChar((n+0x30) & 0x000000FF);
    return;
  }
  else{
    LCD_OutDec(n/10);
    n= n%10;
    ST7735_OutChar((n+0x30) & 0x000000FF);
  }
}

extern uint8_t points;

/*void outro(void){
  ST7735_DrawBitmap(0, 159, GameOver, 128,160);
  Sound_Killed();
  ST7735_DrawBitmap(0, 159, highScores, 128,160);
  Sound_Killed();
  ST7735_SetCursor(64, 91);
  LCD_OutDec(points * 100);
}*/

vector <uint8_t> pointsVector; //add this to the global variables


void outro(void){
    int x =64;
    int y =91;
  ST7735_DrawBitmap(0, 159, GameOver, 128,160);
  Sound_Killed();
  ST7735_DrawBitmap(0, 159, highScores, 128,160);
  Sound_Killed();
	ST7735_SetCursor(x, y);
	char string[13]= "Your Score: ";
  ST7735_OutString(string);
  LCD_OutDec(points * 100);
    if(pointsVector.size()>0){
        for(int i=0; i<pointsVector.size() && y<140;i++){
            y+=10;
            LCD_OutDec(pointsVector[i] * 100);
        }
    }
    pointsVector.push_back(points);
}

// resets map

void restartLevel(uint8_t level){
  uint8_t i;
  uint8_t j;
  uint32_t check;
  uint32_t index;
  ST7735_DrawBitmap(0, 159, Background, 128,160);
  for(i = 159; i >= 0; i--){
    for(j = 0; j < 128; j++){
      index = 0x00000001 << (j % 32);
      check = liveMap[i][(j / 32)] & index;
      if(check){
        ST7735_DrawPixel(j, i, 0x0000);
      }
    }
    if(i== 0){break;}
  }
  ST7735_DrawBitmap(112, 16, flower, 14,14);
  if(level == 2 || level == 3){
    ST7735_DrawBitmap(98, 16, flower, 14,14);
  }
  if(level == 3){
    ST7735_DrawBitmap(84, 16, flower, 14,14);
  }
}

void gameOver(void){
	ST7735_DrawBitmap(0, 159, GameOver, 128,160);
}