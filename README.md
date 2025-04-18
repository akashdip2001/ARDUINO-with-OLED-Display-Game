# ARDUINO-with-OLED-Display 📟

<img src="public/ARDUINO-with-OLED-Display 01.jpg">

<details>
  <summary style="opacity: 0.85;"><b>⚙️📌 Upload in GitHub without Login</b></summary><br>

I want to **push just this one project folder** to a **different GitHub account/repo (temporarily)** using my **personal access token**, without affecting your existing projects or GitHub account setup in VS Code.

---

## ✅ Steps to Upload This Folder to That Repo (Temporarily)

### 📂 1. Open terminal in the project folder

```bash
cd path/to/ARDUINO-with-OLED-Display
```

### 🌱 2. Initialize Git (if not already)

```bash
git init
```

### 📝 3. Add and commit all files

```bash
git add .
git commit -m "Initial commit for OLED project"
```

### 🔗 4. Add the remote with token (for one-time push)

![Screenshot (328)](https://github.com/user-attachments/assets/b66abca0-c6e4-4285-aa8b-4fc74923ccdc)

Replace `<YOUR_TOKEN_HERE>` with your actual **Personal Access Token**:

```bash
git remote add origin https://<YOUR_TOKEN_HERE>@github.com/Arkadip2007/ARDUINO-with-OLED-Display.git
```

**Important**: Don’t put `<>` around the token when actually typing.

✅ Example:
```bash
git remote add origin https://ghp_abcd123456789yourtoken@github.com/Arkadip2007/ARDUINO-with-OLED-Display.git
```

> This way it won’t ask for username/password and uses your token directly.

### 🛠️ 5. Set the branch name (if needed)

```bash
git branch -M main
```

### 🚀 6. Push to GitHub

```bash
git push -u origin main
```

---

## ✅ Optional: Clean up after push (secure)

After you're done, you can **remove the remote with token** to avoid accidentally leaking it:

```bash
git remote set-url origin https://github.com/Arkadip2007/ARDUINO-with-OLED-Display.git
```

Or just delete the `.git` folder if this was a one-time thing:

```bash
rm -rf .git
```

</details>

---

## only check the Display working or not ?

### Wiring Connections:

```go
OLED VCC -> Arduino 5V

OLED GND -> Arduino GND

OLED SDA -> Arduino A4

OLED SCL -> Arduino A5

Jump Button -> Arduino D2 (other leg to GND), using INPUT_PULLUP
```

![ARDUINO-with-OLED-Display](https://github.com/user-attachments/assets/9df7aa80-0795-4f0e-8859-f09b91ff5278)

https://github.com/user-attachments/assets/2608ca3f-f541-46a6-96dd-fb340bdfc8a1

---

```cpp
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println("Hello");
  display.setCursor(0, 35);
  display.println("Arkadip :)");
  display.display();
}

void loop() {
  // Nothing here for now
}
```

### .ini (libraryes)
```ini
[env:uno]
platform = atmelavr
board = uno
framework = arduino
monitor_speed = 9600

lib_deps =
  adafruit/Adafruit SSD1306@^2.5.7
  adafruit/Adafruit GFX Library@^1.11.9
```

---
