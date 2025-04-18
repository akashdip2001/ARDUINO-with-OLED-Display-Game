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
int obstacleType = 0;
int score = 0;
bool gameStarted = false;
int gameSpeed = 50;

void drawPlayer(int x, int y, bool jumping) {
  display.fillCircle(x + 2, y - 6, 2, SSD1306_WHITE);              // Head
  display.drawLine(x + 2, y - 4, x + 2, y, SSD1306_WHITE);         // Body

  if (jumping) {
    display.drawLine(x + 2, y, x, y + 3, SSD1306_WHITE);           // Legs
    display.drawLine(x + 2, y, x + 4, y + 3, SSD1306_WHITE);
  } else {
    display.drawLine(x + 2, y, x + 2, y + 4, SSD1306_WHITE);       // Standing leg
  }

  display.drawLine(x + 2, y - 3, x, y - 2, SSD1306_WHITE);         // Arms
  display.drawLine(x + 2, y - 3, x + 4, y - 2, SSD1306_WHITE);
}

void drawObstacle(int x, int type) {
  if (type == 0) {
    display.fillRect(x, groundLevel - 8, 6, 8, SSD1306_WHITE);           // Box
  } else if (type == 1) {
    display.fillRect(x + 2, groundLevel - 6, 2, 6, SSD1306_WHITE);       // Tree trunk
    display.fillCircle(x + 3, groundLevel - 8, 3, SSD1306_WHITE);        // Leaves
  } else if (type == 2) {
    display.drawPixel(x, groundLevel - 16, SSD1306_WHITE);              // Bird (V shape)
    display.drawPixel(x + 1, groundLevel - 17, SSD1306_WHITE);
    display.drawPixel(x + 2, groundLevel - 16, SSD1306_WHITE);
  }
}

void showWelcomeScreen() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 10);
  String title = "WELCOME TO";
  for (int i = 0; i < title.length(); i++) {
    display.print(title[i]);
    display.display();
    delay(80);
  }

  display.setCursor(0, 32);
  String title2 = "THE GAME";
  for (int i = 0; i < title2.length(); i++) {
    display.print(title2[i]);
    display.display();
    delay(80);
  }

  display.setTextSize(1);
  display.setCursor(0, 52);
  String sub = "Made by ARKADIP MAHAPATRA";
  for (int i = 0; i < sub.length(); i++) {
    display.print(sub[i]);
    display.display();
    delay(60);
  }

  delay(1500);
  display.clearDisplay();
  display.display();
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(SSD1306_WHITE);
  showWelcomeScreen();
}

void loop() {
  if (!gameStarted && digitalRead(BUTTON_PIN) == LOW) {
    gameStarted = true;
    delay(200);
  }

  if (!gameStarted) {
    display.clearDisplay();
    display.setCursor(0, 30);
    display.setTextSize(1);
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
    obstacleType = random(0, 3);
    score++;

    if (score % 10 == 0 && score != 0) {
      display.clearDisplay();
      display.setCursor(0, 20);
      if (score % 20 == 0) {
        display.print("GREAT JOB!");
      } else {
        display.print("QUICK RESPONSE!");
      }
      display.display();
      delay(1000);
    }

    if (gameSpeed > 20) gameSpeed -= 2;
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.print("Score: ");
  display.print(score);

  display.drawLine(0, groundLevel + 4, SCREEN_WIDTH, groundLevel + 4, SSD1306_WHITE);

  drawPlayer(playerX, playerY, isJumping);
  drawObstacle(obstacleX, obstacleType);

  // Collision detection
  if (obstacleX <= playerX + 4 && obstacleX + 6 >= playerX) {
    if (playerY + 6 >= groundLevel - 4) {
      display.setCursor(30, 20);
      display.print("Game Over!");
      display.display();
      delay(2000);
      score = 0;
      obstacleX = SCREEN_WIDTH;
      gameStarted = false;
      gameSpeed = 50;
      return;
    }
  }

  display.display();
  delay(gameSpeed);
}
