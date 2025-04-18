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
bool welcomeDone = false;
bool hardMode = false;
bool inCountdown = false;

int gameSpeed = 50;

// ========== PLAYER ============
void drawPlayer(int x, int y, bool jumping) {
  display.fillCircle(x + 2, y - 6, 2, SSD1306_WHITE); // Head
  display.drawLine(x + 2, y - 4, x + 2, y, SSD1306_WHITE); // Body
  display.drawLine(x + 2, y - 3, x, y - 2, SSD1306_WHITE); // Arms
  display.drawLine(x + 2, y - 3, x + 4, y - 2, SSD1306_WHITE);
  if (jumping) {
    display.drawLine(x + 2, y, x, y + 3, SSD1306_WHITE); // Jumping legs
    display.drawLine(x + 2, y, x + 4, y + 3, SSD1306_WHITE);
  } else {
    display.drawLine(x + 2, y, x + 1, y + 4, SSD1306_WHITE); // Standing legs
    display.drawLine(x + 2, y, x + 3, y + 4, SSD1306_WHITE);
  }
}

// ========== OBSTACLES ============
void drawObstacle(int x, int type) {
  switch (type) {
    case 0: // box
      display.fillRect(x, groundLevel - 8, 6, 8, SSD1306_WHITE);
      break;
    case 1: // tree
      display.fillRect(x + 2, groundLevel - 10, 2, 10, SSD1306_WHITE); // trunk
      display.fillTriangle(x - 2, groundLevel - 10, x + 3, groundLevel - 16, x + 8, groundLevel - 10, SSD1306_WHITE);
      break;
    case 2: // bird
      display.fillCircle(x + 2, groundLevel - 20, 2, SSD1306_WHITE); // body
      display.drawPixel(x + 1, groundLevel - 22, SSD1306_WHITE); // wing top
      display.drawPixel(x + 3, groundLevel - 22, SSD1306_WHITE);
    break;
  }
}

// ========== SCREENS ============
void showWelcomeScreen() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 15);
  display.println("WELCOME TO");
  display.setCursor(0, 35);
  display.println("THE GAME");
  display.display();
  delay(2000);

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(25, 15);
  display.println("made by:");
  display.setCursor(0, 35);
  display.println("ARKADIP MAHAPATRA");
  display.display();
  delay(2000);
  welcomeDone = true;
}

void showDifficultyScreen(bool selectedHard, int countdown) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(5, 15);
  display.print("Select Difficulty:");
  display.setCursor(22, 35);
  display.print(selectedHard ? "  Easy   >Hard<" : ">Easy<   Hard");

  display.setCursor(20, 50);
  display.print("Starting in: ");
  display.print(countdown);
  display.display();
}

// ========== SETUP ============
void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
}

// ========== MAIN LOOP ============
void loop() {
  static bool waitingToStart = true;
  static bool difficultySelected = false;
  static unsigned long countdownStartTime = 0;
  static int countdown = 3;
  static unsigned long lastCountdownUpdate = 0;

  // Show welcome screen once
  if (!welcomeDone) {
    showWelcomeScreen();
    welcomeDone = true;
    return;
  }

  // Start screen: wait for first press
  if (!gameStarted && waitingToStart) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 30);
    display.print("Press button to start");
    display.display();

    if (digitalRead(BUTTON_PIN) == LOW) {
      delay(200); // debounce
      waitingToStart = false;
      countdownStartTime = millis();
      lastCountdownUpdate = millis();
      countdown = 3;
      inCountdown = true;
    }
    return;
  }

  // Difficulty selection with countdown
  if (!gameStarted && inCountdown) {
    // Handle button press to toggle difficulty
    if (digitalRead(BUTTON_PIN) == LOW) {
      delay(200); // debounce
      hardMode = !hardMode;
      countdown = 3;
      countdownStartTime = millis();
      lastCountdownUpdate = millis();
    }

    // Update countdown every second
    if (millis() - lastCountdownUpdate >= 1000) {
      countdown--;
      lastCountdownUpdate = millis();
    }

    showDifficultyScreen(hardMode, countdown);

    if (countdown <= 0) {
      inCountdown = false;
      gameStarted = true;
      gameSpeed = hardMode ? 20 : 50; //gameSpeed = hardMode ? 30 : 50; // Adjust game speed "for hard mode"
    }
    return;
  }

  // ================= GAME LOOP =================
  if (digitalRead(BUTTON_PIN) == LOW && !isJumping) {
    isJumping = true;
    jumpCounter = 0;
    delay(100);
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
  if (obstacleX < -10) {
    obstacleX = SCREEN_WIDTH;
    obstacleType = random(0, 3);
    score++;

    if (score % 10 == 0) {
      display.clearDisplay();
      display.setCursor(0, 20);
      display.setTextSize(1);
      if (score % 20 == 0)
        display.print("YOU REALLY HAVE QUICK REFLEXES!");
      else
        display.print("Great job!");
      display.display();
      delay(1500);
    }

    if (hardMode && gameSpeed > 10) gameSpeed -= 2; //if (hardMode && gameSpeed > 25) gameSpeed--; // Adjust game speed for hard mode
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Score: ");
  display.print(score);

  display.drawLine(0, groundLevel + 4, SCREEN_WIDTH, groundLevel + 4, SSD1306_WHITE);
  drawPlayer(playerX, playerY, isJumping);
  drawObstacle(obstacleX, obstacleType);

// Collision detection
if (obstacleX <= playerX + 4 && obstacleX + 6 >= playerX) {
  if (obstacleType == 2) {
    // Bird: check if player jumped into it
    if (playerY <= groundLevel - 16) {
      display.setCursor(30, 20);
      display.print("Game Over!");
      display.display();
      delay(2000);

      // Reset game state
      score = 0;
      obstacleX = SCREEN_WIDTH;
      gameStarted = false;
      waitingToStart = true;
      hardMode = false;
      inCountdown = false;
      return;
    }
  } else {
    // Ground obstacle collision
    if (playerY + 6 >= groundLevel - 6) {
      display.setCursor(30, 20);
      display.print("Game Over!");
      display.display();
      delay(2000);

      // Reset game state
      score = 0;
      obstacleX = SCREEN_WIDTH;
      gameStarted = false;
      waitingToStart = true;
      hardMode = false;
      inCountdown = false;
      return;
    }
  }
}


  display.display();
  delay(gameSpeed);
}
