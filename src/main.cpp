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
int birdVariant = 0;

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
    case 0: // Car
      display.fillRect(x, groundLevel - 8, 16, 8, SSD1306_WHITE); // Body
      display.drawLine(x + 3, groundLevel - 8, x + 6, groundLevel - 12, SSD1306_WHITE); // Windshield
      display.drawLine(x + 6, groundLevel - 12, x + 10, groundLevel - 12, SSD1306_WHITE); // Roof
      display.drawLine(x + 10, groundLevel - 12, x + 13, groundLevel - 8, SSD1306_WHITE); // Back slope
      display.fillCircle(x + 4, groundLevel, 2, SSD1306_WHITE); // Left wheel
      display.fillCircle(x + 12, groundLevel, 2, SSD1306_WHITE); // Right wheel
      break;

    case 1: // Pine Tree
      display.fillRect(x + 3, groundLevel - 6, 2, 6, SSD1306_WHITE); 
      display.fillTriangle(x - 4, groundLevel - 6, x + 4, groundLevel - 14, x + 12, groundLevel - 6, SSD1306_WHITE);
      display.fillTriangle(x - 3, groundLevel - 10, x + 4, groundLevel - 18, x + 11, groundLevel - 10, SSD1306_WHITE);
      break;

    case 2: // Bird Variants
  switch (birdVariant) {
    case 0: // Small Bird
      display.fillCircle(x + 5, groundLevel - 20, 2, SSD1306_WHITE); // Body
      display.drawPixel(x + 3, groundLevel - 20, SSD1306_WHITE);     // Head
      display.drawPixel(x + 2, groundLevel - 20, SSD1306_WHITE);     // Beak
      display.drawLine(x + 5, groundLevel - 20, x + 7, groundLevel - 22, SSD1306_WHITE); // Wing top
      display.drawLine(x + 5, groundLevel - 20, x + 7, groundLevel - 18, SSD1306_WHITE); // Wing bottom
      display.drawPixel(x + 7, groundLevel - 20, SSD1306_WHITE);     // Tail
      break;


    case 1: // Wide-Wing Bird (soaring)
      display.drawLine(x + 3, groundLevel - 20, x + 7, groundLevel - 20, SSD1306_WHITE); // Wingspan
      display.drawPixel(x + 5, groundLevel - 21, SSD1306_WHITE); // Head/body
      display.drawPixel(x + 6, groundLevel - 21, SSD1306_WHITE); // Back
      break;

    case 2: // Perched-Looking Bird (longer body)
      display.fillRect(x + 4, groundLevel - 21, 3, 3, SSD1306_WHITE); // Body
      display.drawPixel(x + 3, groundLevel - 21, SSD1306_WHITE); // Head
      display.drawPixel(x + 2, groundLevel - 21, SSD1306_WHITE); // Beak
      display.drawPixel(x + 5, groundLevel - 24, SSD1306_WHITE); // Wing tip
      display.drawPixel(x + 7, groundLevel - 20, SSD1306_WHITE); // Tail feather
      break;
  }
  break;

    case 3: // Walking dog
      display.fillRect(x + 2, groundLevel - 8, 10, 5, SSD1306_WHITE);   // Body
      display.drawRect(x - 1, groundLevel - 10, 4, 4, SSD1306_WHITE);   // Head
      display.drawPixel(x, groundLevel - 11, SSD1306_WHITE);           // Ear
      display.drawLine(x + 10, groundLevel - 7, x + 12, groundLevel - 6, SSD1306_WHITE); // Tail
      display.drawLine(x + 3, groundLevel - 3, x + 3, groundLevel, SSD1306_WHITE); // Front leg
      display.drawLine(x + 5, groundLevel - 3, x + 5, groundLevel, SSD1306_WHITE); // Mid leg
      display.drawLine(x + 7, groundLevel - 3, x + 7, groundLevel, SSD1306_WHITE); // Mid leg
      display.drawLine(x + 9, groundLevel - 3, x + 9, groundLevel, SSD1306_WHITE); // Back leg
      break;

      case 4: // Cactus with head
      display.drawLine(x + 3, groundLevel - 14, x + 3, groundLevel, SSD1306_WHITE); // Main trunk
      display.drawLine(x + 3, groundLevel - 10, x, groundLevel - 10, SSD1306_WHITE); // Left arm
      display.drawLine(x, groundLevel - 10, x, groundLevel - 6, SSD1306_WHITE);     // Left arm vertical
      display.drawLine(x + 3, groundLevel - 12, x + 6, groundLevel - 12, SSD1306_WHITE); // Right arm
      display.drawLine(x + 6, groundLevel - 12, x + 6, groundLevel - 8, SSD1306_WHITE);  // Right arm vertical

      // 🌵 Head (a circle-like cactus top)
      display.drawPixel(x + 2, groundLevel - 15, SSD1306_WHITE);
      display.drawPixel(x + 3, groundLevel - 16, SSD1306_WHITE);
      display.drawPixel(x + 4, groundLevel - 15, SSD1306_WHITE);
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
  display.setTextSize(1.5);
  display.setCursor(0, 15);
  display.println("made by: ");
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
      gameSpeed = hardMode ? 10 : 50; //gameSpeed = hardMode ? 30 : 50; // Adjust game speed "for hard mode"
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
    obstacleType = random(0, 5); // Randomly select obstacle type
    int birdVariant = random(0, 3); // 0 = small, 1 = wide-wing, 2 = perched-looking
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

    if (hardMode && gameSpeed > 5) gameSpeed -= 5; //if (hardMode && gameSpeed > 25) gameSpeed--; // Adjust game speed for hard mode
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
