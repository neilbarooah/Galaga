// Name: Neil Barooah

#include "ship.h"
#include "enemy_ship1.h"
#include "enemy_ship2.h"
#include "enemy_ship3.h"
#include "end_screen.h"
#include "welcome_screen.h"
#include "mylib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define REG_DISPCNT *(u16*) 0x4000000
#define MODE_3 3
#define BG2_EN (1 << 10)

enum {
	START_GAME, PLAY_GAME, LOST_LIFE, END_GAME, WON_GAME
};

Ship p1;
Insect insects[10];
int num_insects;

Bullet bullets[10];
int num_bullets;

int buttonA;
int num_lives;
int current_state;
int current_stage;
int score;

// prototypes
void welcomeScreen();
void playGame();
void getResponse();
void newShip();
void newInsects();
void newBullets();
void lostLife();
void lostGame();
void wonGame();
void getInsects();
void nextstage();
void resetShip();
void resetInsects();

int main(void) {
	REG_DISPCNT = MODE_3 | BG2_EN;
	srand(100);

	num_insects = 0;
	num_bullets = 0;

	buttonA = 0;
	num_lives = INITIAL_LIVES;
	current_stage = 1;

	current_state = START_GAME;
	while(1) {
		while (KEY_PRESSED(BUTTON_SELECT)) {
			current_state = START_GAME;
		} switch (current_state) {
			case START_GAME:
				welcomeScreen();
				clearScreen();
				break;
			case PLAY_GAME:
				playGame();
				break;
			case LOST_LIFE:
				lostLife();
				break;
			case END_GAME:
				lostGame();
				break;
			case WON_GAME:
				wonGame();
				break;
		}
	}
	return 0;
}

void welcomeScreen() {
	drawImage3(0, 0, 240, 160, welcome_screen);
	
	while (!KEY_PRESSED(BUTTON_START));
	current_state = PLAY_GAME;
	
	num_lives = INITIAL_LIVES;
	current_stage = 1;
	num_insects = 0;
	num_bullets = 0;
	score = 0;

	p1.xPos = INITIAL_Xpos;
	p1.yPos = INITIAL_Ypos;
	p1.prevX = p1.xPos;
	p1.prevY = p1.yPos;
	p1.width = SHIP_WIDTH;
	p1.height = SHIP_HEIGHT;

	getInsects();
}

void playGame() {
	getResponse();
	newShip();
	newInsects();
        newBullets();
	
	char string[20];
	sprintf(string, "STAGE %d", current_stage);
        drawText(5, 5, string, 0x07ff);
	sprintf(string, "SCORE:%d", score);
	drawText(5, 100, string, 0x07ff);
	if (num_lives == 1) {
		sprintf(string, "%d LIFE", num_lives);
	} else {
		sprintf(string, "%d LIVES", num_lives);
	}
        drawText(5, 200, string, 0x07ff);
}

void getResponse() {
	p1.prevX = p1.xPos;
	p1.prevY = p1.yPos;
	
	if (KEY_PRESSED(BUTTON_UP)) {
		p1.yPos--;
	}
	if (KEY_PRESSED(BUTTON_DOWN)) {
		p1.yPos++;
	}
	if (KEY_PRESSED(BUTTON_LEFT)) {
		p1.xPos--;
	}
	if (KEY_PRESSED(BUTTON_RIGHT)) {
		p1.xPos++;
	}
	if (buttonA == 0 && KEY_PRESSED(BUTTON_A) && num_bullets < 1) {
		Bullet newBullet;
		newBullet.xPos = p1.xPos + p1.width / 2;
		newBullet.yPos = p1.yPos - p1.height / 2;
		newBullet.width = 2;
		newBullet.height = 4;
		newBullet.prevX = newBullet.xPos;
		newBullet.prevY = newBullet.yPos;
		bullets[num_bullets++] = newBullet;
		buttonA = 1;
	} else if (buttonA == 1 && !KEY_PRESSED(BUTTON_A))
		buttonA = 0;
}

void newShip() {
	if (p1.yPos < 15) {
		p1.yPos = 15;
	} 
	if (p1.yPos + p1.width >= 155) {
		p1.yPos = 155 - p1.width;
	}
	if (p1.xPos < 0) {
		p1.xPos = 0;
	}	
	if (p1.xPos + p1.height > 240) {
		p1.xPos = 240 - p1.height;
	}
	waitForVBlank();
	//removeShip(p1.prevY, p1.prevX, p1.width, p1.height, 0x00000);
	drawImage3(p1.yPos, p1.xPos, p1.width, p1.height, ship);
}

void newInsects() {

	for (int j = 0; j < num_insects; j++) {
		if (j % 4 == 0)
			waitForVBlank();
		if (insects[j].shoot == 0) {
			if (rand() % 500 < 2) {
				insects[j].shoot = 6; // speed of attacking
                        }
		} 
		// insect in attacking mode
                if (insects[j].shoot != 0) {
			insects[j].yPos += insects[j].shoot;
		}
		// manipulate movement of insect
                if (insects[j].yPos + insects[j].width >= 155) {
			insects[j].shoot *= -1;
			insects[j].yPos = 155 - insects[j].width;
		}
		if (insects[j].yPos <= insects[j].origY) {
			insects[j].shoot = 0;
			insects[j].yPos = insects[j].origY;
		}

		// remove previous position of insect
		removeShip(insects[j].prevY, insects[j].prevX, insects[j].width, insects[j].height, 0x00000);
		
		// Draw the insects
		if (insects[j].type == 1) {
			drawImage3(insects[j].yPos, insects[j].xPos, insects[j].width, insects[j].height, enemy_ship1);
		} else if (insects[j].type == 2) {
			drawImage3(insects[j].yPos, insects[j].xPos, insects[j].width, insects[j].height, enemy_ship2);
                } else if (insects[j].type == 3) {
			drawImage3(insects[j].yPos, insects[j].xPos, insects[j].width, insects[j].height, enemy_ship3);
                }
		
		insects[j].prevX = insects[j].xPos;
		insects[j].prevY = insects[j].yPos;

		// adjust insects so they remain within the screen view
		if (insects[j].xPos - insects[j].origX > 35 || insects[j].xPos <= 0)
			insects[j].direction *= -1;
		else if (insects[j].xPos - insects[j].origX < -35 || insects[j].xPos >= 240)
			insects[j].direction *= -1;

		insects[j].xPos += insects[j].direction;

		
		if (insects[j].yPos < 0) {
			insects[j].yPos = 0;
		}
		if (insects[j].yPos + insects[j].width >= 155) {
			insects[j].yPos = 155 - insects[j].width;
		}
		if (insects[j].xPos < 0) {
			insects[j].xPos = 0;
		}
		if (insects[j].xPos + insects[j].height > 240) {
			insects[j].xPos = 240 - insects[j].height;
		}
		
		// detects a clash with the spaceship
		if ((p1.xPos <= insects[j].xPos + insects[j].height &&
				p1.xPos >= insects[j].xPos &&
				p1.yPos <= insects[j].yPos + insects[j].width &&
				p1.yPos >= insects[j].yPos) ||
				(insects[j].xPos <= p1.xPos + p1.height &&
				insects[j].xPos >= p1.xPos &&
				insects[j].yPos <= p1.yPos + p1.width &&
				insects[j].yPos >= p1.yPos)) {

			removeShip(insects[j].yPos - 1, insects[j].xPos - 1, insects[j].width + 2, insects[j].height + 2, 0x00000);
			removeShip(p1.yPos, p1.xPos, p1.width, p1.height, 0x00000);
			// deletes the dead insect from the array
			for (int i = j; i < num_insects - 1; i++) {
				insects[i] = insects[i + 1];
			}

                        num_insects--; // decrements dead insect

			num_lives--; // decrement number of lives
			
			// determine state of game
			if (num_lives > 0) {
				current_state = LOST_LIFE;
			} else {
				current_state = END_GAME;
			}
                        j--;
		}
		// state of game = won
		if (num_insects == 0 && num_lives > 0) {
			clearScreen();
			current_state = WON_GAME;
		}
	}
}

void newBullets() {

// counter for each bullet fired
for (int j = 0; j < num_bullets; j++) {

		removeShip(bullets[j].prevY, bullets[j].prevX, bullets[j].width, bullets[j].height, 0x00000);

		removeShip(bullets[j].yPos, bullets[j].xPos, bullets[j].width, bullets[j].height, 0xfbe0);
                //
		bullets[j].prevX = bullets[j].xPos;
		bullets[j].prevY = bullets[j].yPos;
		bullets[j].yPos -= 3;
		if (bullets[j].yPos < 0) {
			removeShip(bullets[j].prevY, bullets[j].prevX, bullets[j].width, bullets[j].height, 0x00000);
			for (int k = j; j < num_bullets - 1; k++) {
				bullets[k] = bullets[k + 1];
			}
                        num_bullets--;
			j--;
		}
		// bullet hits insect
		for (int i = 0; i < num_insects; i++) {
			if ((bullets[j].xPos <= insects[i].xPos + insects[i].height && bullets[j].xPos >= insects[i].xPos && bullets[j].yPos <= insects[i].yPos + insects[i].width && bullets[j].yPos >= insects[i].yPos) ||
(insects[i].xPos <= bullets[j].xPos + bullets[j].height && insects[i].xPos >= bullets[j].xPos && insects[i].yPos <= bullets[j].yPos + bullets[i].width && insects[i].yPos >= bullets[j].yPos)) {

				removeShip(insects[i].prevY, insects[i].prevX, insects[i].width, insects[i].height, 0x00000);
                                
				removeShip(bullets[j].prevY, bullets[j].prevX, bullets[j].width, bullets[j].height, 0x00000);
				removeShip(5, 100, 40, 20, 0x00000);
				score += 100;
				for (int a = j; a < num_bullets - 1; a++)
					bullets[a] = bullets[a + 1];
				for (int a = i; a < num_insects - 1; a++)
					insects[a] = insects[a + 1];
				num_insects--;
				removeShip(150, 0, 240, 100, 0x00000);
				num_bullets--;
				j--;
				//strike = 1;
			}
		}
		if (num_insects == 0) {
			clearScreen();
			num_bullets = 0;
			num_insects = 0;
			current_state = WON_GAME;
		}
	}
}

// losing a life, not game
void lostLife() {
	char string[25];
	if (num_lives > 1) {
		sprintf(string, "The aliens killed you!");
		drawText(100, 60, string, 0x07ff);
	} else {
		sprintf(string, "LAST CHANCE!");
		drawText(100, 90, string, 0x07ff);
	}
	if (KEY_PRESSED(BUTTON_START)) {
		num_bullets = 0;
		current_state = PLAY_GAME;
		clearScreen();
		resetShip();
		resetInsects();
	}
}

// losing the game
void lostGame() {
	// display end screen
	drawImage3(0, 0, 240, 160, end_screen);

	// pressing start will start a new game
	if (KEY_PRESSED(BUTTON_START)) {
		num_lives = INITIAL_LIVES;		
		current_stage = 1;
		num_bullets = 0;
		num_insects = 0;
		current_state = START_GAME;
		
		clearScreen();
		nextstage();
		getInsects();
	}
}

// winning a stage
void wonGame() {
	drawText(100, 100, "You Win!", 0x01f);
	if (current_stage + 1 <= 4) {
		drawText(120, 60, "Press Start For Next Stage", 0x01f);
	} else {
		drawText(120, 60, "Press Start To Play Again", 0x01f);
	}
	if (KEY_PRESSED(BUTTON_START)) {
		current_stage++;
		if (current_stage <= 4)
			current_state = PLAY_GAME;
		else {
			current_state = START_GAME;
			current_stage = 1;
		}
		clearScreen();
		nextstage();
		getInsects();
	}
}

// place insects based on stage/level
void getInsects() {
	if (current_stage == 1) {
		for (int i = 0; i < ((int) (sizeof(stage1) / sizeof(int))); i += 4) {
			Insect newInsect;

			// obtain position of insects
			newInsect.xPos = stage1[i + 0];
			newInsect.yPos = stage1[i + 1];
			newInsect.prevX = newInsect.xPos;
			newInsect.prevY = newInsect.yPos;
			newInsect.origX = newInsect.xPos;
			newInsect.origY = newInsect.yPos;
			newInsect.shoot = 0;
			newInsect.direction = stage1[i + 2];
			newInsect.type = stage1[i + 3];

			// height/width of insect may differ
			if (newInsect.type == 1) {
				newInsect.height = ENEMY_SHIP1_HEIGHT;
				newInsect.width = ENEMY_SHIP1_WIDTH;
			} else if (newInsect.type == 2) {
				newInsect.height = ENEMY_SHIP2_HEIGHT;
				newInsect.width = ENEMY_SHIP2_WIDTH;
			} else if (newInsect.type == 3) {
                                newInsect.height = ENEMY_SHIP3_HEIGHT;
                                newInsect.width = ENEMY_SHIP3_WIDTH;
                        }

			// incrememnt number of insects
			insects[num_insects++] = newInsect;
		}
	} else if (current_stage == 2) {
		for (int i = 0; i < ((int) (sizeof(stage2) / sizeof(int))); i += 4) {
			Insect newInsect;

			newInsect.xPos = stage2[i + 0];
			newInsect.yPos = stage2[i + 1];
			newInsect.prevX = newInsect.xPos;
			newInsect.prevY = newInsect.yPos;
			newInsect.origX = newInsect.xPos;
			newInsect.origY = newInsect.yPos;
			newInsect.shoot = 0;
			newInsect.direction = stage2[i + 2];
			newInsect.type = stage2[i + 3];
			

			if (newInsect.type == 1) {
				newInsect.height = ENEMY_SHIP1_HEIGHT;
				newInsect.width = ENEMY_SHIP1_WIDTH;
			} else if (newInsect.type == 2) {
				newInsect.height = ENEMY_SHIP2_HEIGHT;
				newInsect.width = ENEMY_SHIP2_WIDTH;
			} else if (newInsect.type == 3) {
                                newInsect.height = ENEMY_SHIP3_HEIGHT;
                                newInsect.width = ENEMY_SHIP3_WIDTH;
                        }

			insects[num_insects++] = newInsect;
		}
	} else if (current_stage == 3) {
		for (int i = 0; i < ((int) (sizeof(stage3) / sizeof(int))); i += 4) {
			Insect newInsect;

			newInsect.xPos = stage3[i + 0];
			newInsect.yPos = stage3[i + 1];
			newInsect.prevX = newInsect.xPos;
			newInsect.prevY = newInsect.yPos;
			newInsect.origX = newInsect.xPos;
			newInsect.origY = newInsect.yPos;
			newInsect.shoot = 0;
			newInsect.direction = stage3[i + 2];
			newInsect.type = stage3[i + 3];
			
			if (newInsect.type == 1) {
				newInsect.height = ENEMY_SHIP1_HEIGHT;
				newInsect.width = ENEMY_SHIP1_WIDTH;
			} else if (newInsect.type == 2) {
				newInsect.height = ENEMY_SHIP2_HEIGHT;
				newInsect.width = ENEMY_SHIP2_WIDTH;
			} else if (newInsect.type == 3) {
                                newInsect.height = ENEMY_SHIP3_HEIGHT;
                                newInsect.width = ENEMY_SHIP3_WIDTH;
                        }

			insects[num_insects++] = newInsect;
		}
	} else if (current_stage == 4) {
		for (int i = 0; i < ((int) (sizeof(stage4) / sizeof(int))); i += 4) {
			Insect newInsect;

			newInsect.xPos = stage4[i + 0];
			newInsect.yPos = stage4[i + 1];
			newInsect.prevX = newInsect.xPos;
			newInsect.prevY = newInsect.yPos;
			newInsect.origX = newInsect.xPos;
			newInsect.origY = newInsect.yPos;
			newInsect.shoot = 0;
			newInsect.direction = stage4[i + 2];
			newInsect.type = stage4[i + 3];

			if (newInsect.type == 1) {
				newInsect.height = ENEMY_SHIP1_HEIGHT;
				newInsect.width = ENEMY_SHIP1_WIDTH;
			} else if (newInsect.type == 2) {
				newInsect.height = ENEMY_SHIP2_HEIGHT;
				newInsect.width = ENEMY_SHIP2_WIDTH;
			} else if (newInsect.type == 3) {
                                newInsect.height = ENEMY_SHIP3_HEIGHT;
                                newInsect.width = ENEMY_SHIP3_WIDTH;
                        }
			
			insects[num_insects++] = newInsect;
		}
	}

}

// advance to next stage; reset counters for bullets and insects
void nextstage() {
	resetShip();
	num_insects = 0;
	num_bullets = 0;
}

// initialize insects
void resetInsects() {
	for (int i = 0; i < num_insects; i++) {
		insects[i].yPos = insects[i].origY;
		insects[i].shoot = 0;
	}
}

// initialize ship
void resetShip() {
	p1.xPos = INITIAL_Xpos;
	p1.yPos = INITIAL_Ypos;
}
