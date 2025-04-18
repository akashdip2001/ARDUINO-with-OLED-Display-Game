#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define BUTTON_PIN 2

int playerX = 0;
int playerY = 50; // moved down
bool isJumping = false;
int jumpCounter = 0;
int groundLevel = 56; // moved down
int obstacleX = SCREEN_WIDTH;
int score = 0;
bool gameStarted = false;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.display();
}

void loop() {
  if (!gameStarted && digitalRead(BUTTON_PIN) == LOW) {
    gameStarted = true;
    delay(200);
  }

  if (!gameStarted) {
    display.clearDisplay();
    display.setCursor(0, 30);
    display.print("Press button to start");
    display.display();
    return;
  }

  if (digitalRead(BUTTON_PIN) == LOW && !isJumping) {
    isJumping = true;
    jumpCounter = 0;
  }

  if (isJumping) {
    if (jumpCounter < 10) {
      playerY -= 2;
    } else if (jumpCounter < 20) {
      playerY += 2;
    } else {
      isJumping = false;
      playerY = groundLevel - 6;
    }
    jumpCounter++;
  }

  obstacleX -= 2;
  if (obstacleX < 0) {
    obstacleX = SCREEN_WIDTH;
    score++;
  }

  display.clearDisplay();

  // Score (stay at top)
  display.setCursor(0, 0);
  display.print("Score: ");
  display.print(score);

  // Ground (moved down)
  display.setCursor(0, groundLevel);
  display.print("--------------------");

  // Player
  display.setCursor(playerX, playerY);
  display.print("I");

  // Obstacle
  display.setCursor(obstacleX, groundLevel - 6);
  display.print("#");

  // Collision detection
  if (obstacleX <= playerX + 5 && obstacleX >= playerX) {
    if (playerY >= groundLevel - 6) {
      display.setCursor(0, 20);
      display.print("Game Over!");
      display.display();
      delay(2000);
      score = 0;
      obstacleX = SCREEN_WIDTH;
      gameStarted = false;
      return;
    }
  }

  display.display();
  delay(50);
}
