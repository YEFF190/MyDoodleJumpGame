# 🐸 Doodle Jump — C++ (WinBGIm)

A desktop clone of the classic Doodle Jump game built in C++ using the WinBGIm graphics library. Jump across moving platforms, rack up your high score, and see how high you can go before you fall!

---

## 🎮 Gameplay

- Control a bouncy character that automatically jumps whenever it lands on a platform
- Platforms move horizontally and become more spaced out as your score increases
- Fall below the screen and it's game over
- Your best score is saved locally between sessions

---

## ✨ Features

- **Moving platforms** — platforms slide left and right, speeding up as you climb
- **Increasing difficulty** — platform gaps widen and jump strength scales with score
- **Persistent high score** — best score saved to `best_score.txt` and displayed each run
- **Pause/Resume** — press `P` to pause, `R` to resume
- **Main menu** — navigate with arrow keys, press Enter to select
- **Game over screen** — restart, return to menu, or exit after each run

---

## 🕹️ Controls

| Key | Action |
|-----|--------|
| ← / → Arrow Keys | Move left / right |
| `P` | Pause game |
| `R` | Resume (when paused) |
| `R` | Restart (on game over screen) |
| `M` | Return to main menu (on game over screen) |
| `ESC` | Exit (on game over screen) |
| ↑ / ↓ Arrow Keys | Navigate menu |
| `Enter` | Confirm menu selection |

---

## 🛠️ Requirements

- **Windows** (WinBGIm is Windows-only)
- **MinGW / g++** compiler
- **WinBGIm** graphics library ([download here](http://winbgim.codecutter.org/))

---

## 🚀 Getting Started

### 1. Install WinBGIm

Download WinBGIm and place the following files in your MinGW installation:

- `graphics.h` → `MinGW/include/`
- `winbgim.h` → `MinGW/include/`
- `libbgi.a` → `MinGW/lib/`

### 2. Compile

```bash
g++ jump.cpp -o jump -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32
```

### 3. Run

```bash
./jump.exe
```

> The game window will open automatically (800×800 px).

---

## 📁 Project Structure

```
.
├── jump.cpp          # Main game source file
├── best_score.txt    # Auto-generated; stores your best score
└── README.md
```

---

## 🔧 How It Works

The game is structured around two main classes:

**`Platform`** — Handles spawning, drawing, and horizontal movement of each platform. Platforms reset above the screen when they scroll off the bottom, with increasing vertical gaps tied to the player's score.

**`Player`** — Manages physics (gravity + upward bounce), horizontal movement, collision detection with platforms, and score tracking. As the player climbs above the screen's midpoint, the world scrolls downward instead of the player moving up.

---

## 📌 Known Limitations

- Windows only (WinBGIm does not support Linux/macOS)
- No sound effects
- Single-player only

---

## 👤 Author

Made by **YEFF190**
