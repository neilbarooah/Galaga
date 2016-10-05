

typedef unsigned short u16;

typedef unsigned int u32;

typedef struct DMA{
		const volatile void *src;
		const volatile void *dst;
		u32 cnt;
		}DMAREC; 

typedef struct Ship {
	int xPos;
	int yPos;
	int prevX;
	int prevY;
	int width;
	int height;
} Ship;

typedef struct Insect {
	int xPos;
	int yPos;
	int prevX;
	int prevY;
	int origX;
	int origY;
	int direction;
	int shoot;
	int type;
	int width;
	int height;
} Insect;

typedef struct Bullet {
	int xPos;
	int yPos;
	int prevX;
	int prevY;
	int width;
	int height;
} Bullet;

extern const int stage1[24];
extern const int stage2[32];
extern const int stage3[36];
extern const int stage4[40];
		
extern u16* videoBuffer;

void setPixel(int x, int y, u16 color);
void removeShip(int x, int y, int width, int height, u16 color);
void drawImage3(int x, int y, int width, int height, const u16* image);
void clearScreen();
void drawChar(int row, int col, char ch, unsigned short color);
void drawText(int row, int col, char *str, unsigned short color);
void delay(int n);


#define OFFSET(r, c, rowlen) ((r)*(rowlen) + (c))
#define INITIAL_LIVES	3
#define INITIAL_Xpos	120
#define INITIAL_Ypos	130

#define BUTTONS 			*(volatile unsigned int*) 0x4000130
#define BUTTON_A			(1 << 0)	// Z
#define BUTTON_B			(1 << 1)	// X
#define BUTTON_SELECT		        (1 << 2)	// Backspace
#define BUTTON_START		        (1 << 3)	// Enter
#define BUTTON_RIGHT		        (1 << 4)	// Right Arrow
#define BUTTON_LEFT			(1 << 5)	// Left Arrow
#define BUTTON_UP			(1 << 6)	// Up Arrow
#define BUTTON_DOWN			(1 << 7)	// Down Arrow
#define BUTTON_R			(1 << 8)	// S
#define BUTTON_L			(1 << 9)	// A
#define KEY_PRESSED(key)	        (~(BUTTONS) & key)

#define SCANLINECOUNTER 	*(volatile unsigned short*) 0x4000006

#define DMA ((volatile DMAREC *)0x040000B0)

/*channel 0*/
#define REG_DMA0SAD         *(u32*)0x40000B0  // source address
#define REG_DMA0DAD         *(u32*)0x40000B4  // destination address
#define REG_DMA0CNT         *(u32*)0x40000B8  // control register

/* DMA channel 1 register definitions */

#define REG_DMA1SAD         *(u32*)0x40000BC  // source address
#define REG_DMA1DAD         *(u32*)0x40000C0  // destination address
#define REG_DMA1CNT         *(u32*)0x40000C4  // control register

/* DMA channel 2 register definitions */

#define REG_DMA2SAD         *(u32*)0x40000C8  // source address
#define REG_DMA2DAD         *(u32*)0x40000CC  // destination address
#define REG_DMA2CNT         *(u32*)0x40000D0  // control register

/* DMA channel 3 register definitions */

#define REG_DMA3SAD         *(u32*)0x40000D4  // source address
#define REG_DMA3DAD         *(u32*)0x40000D8  // destination address
#define REG_DMA3CNT         *(u32*)0x40000DC  // control register

#define DMA_CHANNEL_0 0
#define DMA_CHANNEL_1 1
#define DMA_CHANNEL_2 2
#define DMA_CHANNEL_3 3

#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)
#define DMA_DESTINATION_RESET (3 << 21)

#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

#define DMA_REPEAT (1 << 25)

#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

#define DMA_NOW (0 << 28)
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

#define DMA_IRQ (1 << 30)
#define DMA_ON (1 << 31)
void waitForVBlank();
