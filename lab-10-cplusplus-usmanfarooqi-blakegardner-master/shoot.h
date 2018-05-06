#include <stdint.h>
class shootG{
private:
    uint8_t xPos;
    uint8_t yPos;
		uint8_t direction;
    const unsigned short* state;
    bool isShooting; //laser in action
    void laserFire();
public:
	void FIRE(uint8_t x, uint8_t y, uint8_t d);
		uint8_t xCord(void){return this->xPos;};
		uint8_t yCord(void){return this->yPos;};
		uint8_t dir(void){return this->direction;};
    bool isReadyFire(void);
    void shootInit();
		void eraseLaser(void);
		void printLaserNorth(void);
		void printLaserSouth(void);
		void printLaserEast(void);
		void printLaserWest(void);
		void eraseLaserNorth(void);
		void eraseLaserSouth(void);
		void eraseLaserEast(void);
		void eraseLaserWest(void);
};