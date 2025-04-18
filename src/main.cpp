#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define SPRITE_COUNT 7
#define CELL_SIZE    8
#define COLS         16
#define ROWS          2
#define HERO_X        1

// 8x8 sprites stored in PROGMEM
static const uint8_t PROGMEM spriteBitmaps[SPRITE_COUNT][8] = {
{ // Run frame 1
B01100, B01100, B00000, B01110, B11100, B01100, B11010, B10011
},
{ // Run frame 2
B01100, B01100, B00000, B01100, B01100, B01100, B01100, B01110
},
{ // Jump start
B01100, B01100, B00000, B11110, B01101, B11111, B10000, B00000
},
{ // Jump peak
B11110, B01101, B11111, B10000, B00000, B00000, B00000, B00000
},
{ // Ground full
B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111
},
{ // Ground right
B00011, B00011, B00011, B00011, B00011, B00011, B00011, B00011
},
{ // Ground left
B11000, B11000, B11000, B11000, B11000, B11000, B11000, B11000
}
};

// Terrain buffers
char terrainUpper[COLS];
char terrainLower[COLS];
bool buttonPushed = false;

// Draw a single 8x8 sprite at grid position (col, row)
void drawSprite(int col, int row, uint8_t index) {
display.drawBitmap(col * CELL_SIZE,
row * CELL_SIZE,
spriteBitmaps[index],
CELL_SIZE, CELL_SIZE,
SSD1306_WHITE);
}

// Initialize terrain to empty
void initializeGraphics() {
for (int i = 0; i < COLS; i++) {
terrainUpper[i] = 0;
terrainLower[i] = 0;
}
}

// Scroll terrain and insert newType at right end
void advanceTerrain(char* terrain, uint8_t newType) {
for (int i = 0; i < COLS; i++) {
char curr = terrain[i];
char next = (i == COLS - 1) ? newType : terrain[i + 1];
if (curr == 0)
terrain[i] = (next == 1) ? 5 : 0;
else if (curr == 1)
terrain[i] = (next == 0) ? 6 : 1;
else if (curr == 5)
terrain[i] = 4;
else if (curr == 6)
terrain[i] = 0;
}
}

// Render scene: terrain, hero, and score
void drawScene(byte heroPos, unsigned int score) {
display.clearDisplay();

// Draw terrain rows
for (int i = 0; i < COLS; i++) {
// Upper row
drawSprite(i, 0, terrainUpper[i] ? 4 : 4);
// Lower row
drawSprite(i, 1, terrainLower[i] ? 1 : 4);
}

// Draw hero
int row = (heroPos >= 1 && heroPos <= 2) ? 1 : 0;
uint8_t spriteIndex;
if (heroPos <= 2) spriteIndex = heroPos - 1;
else if (heroPos >= 3 && heroPos <= 8)
spriteIndex = 2 + (((heroPos <= 4 || heroPos >= 7) ? 1 : 0));
else
spriteIndex = 0;

drawSprite(HERO_X, row, spriteIndex);

// Draw score
char buf[6];
itoa(score, buf, 10);
int16_t w = strlen(buf) * 6;
display.setCursor(SCREEN_WIDTH - w, 0);
display.setTextSize(1);
display.setTextColor(SSD1306_WHITE);
display.print(buf);

display.display();
}

// Interrupt for button press
void buttonPush() {
buttonPushed = true;
}

void setup() {
// Button on D2
pinMode(2, INPUT_PULLUP);
attachInterrupt(digitalPinToInterrupt(2), buttonPush, FALLING);

// Initialize display
display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
initializeGraphics();
}

void loop() {
static byte heroPos = 1;
static byte newType = 0;
static byte newDur = 1;
static bool playing = false;
static unsigned int dist = 0;

if (!playing) {
drawScene(heroPos, dist >> 3);
delay(250);
if (buttonPushed) {
initializeGraphics();
dist = 0;
playing = true;
buttonPushed = false;
}
return;
}

// Scroll terrain and spawn new obstacles randomly
advanceTerrain(terrainLower, newType ? 1 : 0);
advanceTerrain(terrainUpper, newType ? 1 : 0);
if (--newDur == 0) {
newType = (random(3) == 0);
newDur = random(10) + 1;
}

// Handle jump
if (buttonPushed && heroPos < 3) {
heroPos = 3;
buttonPushed = false;
}

// TODO: Update heroPos, detect collisions
dist++;
drawScene(heroPos, dist >> 3);
delay(100);
}

