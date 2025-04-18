#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define BUTTON_PIN 2

int playerX = 5;
int playerY = 50;
bool isJumping = false;
int jumpCounter = 0;
int groundLevel = 56;
int obstacleX = SCREEN_WIDTH;
int score = 0;
bool gameStarted = false;

void drawPlayer(int x, int y, bool jumping) {
  // Head
  display.fillCircle(x + 2, y - 6, 2, SSD1306_WHITE);
  // Body
  display.drawLine(x + 2, y - 4, x + 2, y, SSD1306_WHITE);

  if (jumping) {
    // Jumping legs (spread)
    display.drawLine(x + 2, y, x, y + 3, SSD1306_WHITE);
    display.drawLine(x + 2, y, x + 4, y + 3, SSD1306_WHITE);
  } else {
    // Standing legs
    display.drawLine(x + 2, y, x + 2, y + 4, SSD1306_WHITE);
  }

  // Arms
  display.drawLine(x + 2, y - 3, x, y - 2, SSD1306_WHITE);
  display.drawLine(x + 2, y - 3, x + 4, y - 2, SSD1306_WHITE);
}

void drawObstacle(int x) {
  display.fillRect(x, groundLevel - 8, 6, 8, SSD1306_WHITE);
}

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
  if (obstacleX < -6) {
    obstacleX = SCREEN_WIDTH;
    score++;
  }

  display.clearDisplay();

  // Score
  display.setCursor(0, 0);
  display.print("Score: ");
  display.print(score);

  // Ground
  display.drawLine(0, groundLevel + 4, SCREEN_WIDTH, groundLevel + 4, SSD1306_WHITE);

  // Draw player and obstacle
  drawPlayer(playerX, playerY, isJumping);
  drawObstacle(obstacleX);

  // Collision
  if (obstacleX <= playerX + 4 && obstacleX + 6 >= playerX) {
    if (playerY + 6 >= groundLevel - 4) {
      display.setCursor(30, 20);
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
