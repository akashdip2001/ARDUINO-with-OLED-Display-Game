# ARDUINO-with-OLED-Display

![ARDUINO-with-OLED-Display](https://github.com/user-attachments/assets/9df7aa80-0795-4f0e-8859-f09b91ff5278)

<details>
  <summary style="opacity: 0.85;"><b>Upload in GitHub without Login</b></summary><br>

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

https://github.com/user-attachments/assets/2608ca3f-f541-46a6-96dd-fb340bdfc8a1
