#include <graphics.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <conio.h>
using namespace std;

const int screenWidth = 800;
const int screenHeight = 800;
const int platformCount = 10;

int bestScore = 0;

// ------------------ Platform Class ------------------
class Platform {
public:
    int x, y;
    int color;
    int dx;

    Platform() {
        reset();
        dx = (rand() % 2 == 0) ? 1 : -1;
    }

    void draw() {
        setcolor(BLACK);
        rectangle(x, y, x + 60, y + 10);
        setfillstyle(SOLID_FILL, color);
        bar(x, y, x + 60, y + 10);
    }

    void move(int score) {
        int speed = 1 + min(score / 100, 3);
        x += dx * speed;

        if (x <= 0 || x + 60 >= screenWidth) {
            dx = -dx;
        }
    }

    void reset(int score = 0) {
        x = rand() % (screenWidth - 60);
        int baseGap = 40;
        int maxGap = 160;
        int gap = baseGap + min(score, 120);
        y = -gap;
        int colorChoices[] = { GREEN, YELLOW, MAGENTA, RED };
        color = colorChoices[rand() % 4];
        dx = (rand() % 2 == 0) ? 1 : -1;
    }
};

// ------------------ Player Class ------------------
class Player {
public:
    int x, y;
    int dy;
    int score;
    int prevY;

    int getJumpStrength() {
        int maxJump = -35;
        int scaledJump = -20 - (score / 20);
        return max(maxJump, scaledJump);
    }

    Player() {
        x = screenWidth / 2;
        y = screenHeight - 100;
        dy = 0;
        score = 0;
        prevY = y;
    }

    void draw() {
        setcolor(BLUE);
        setfillstyle(SOLID_FILL, BLUE);
        fillellipse(x, y, 15, 15);
    }

    void update(Platform platforms[]) {
        dy += 1;
        prevY = y;
        y += dy;

        int playerLeft = x - 15;
        int playerRight = x + 15;
        int playerBottom = y + 15;
        int prevBottom = prevY + 15;

        for (int i = 0; i < platformCount; i++) {
            int platLeft = platforms[i].x;
            int platRight = platforms[i].x + 60;
            int platTop = platforms[i].y;
            int platBottom = platforms[i].y + 10;

            bool horizontallyAligned = playerRight > platLeft && playerLeft < platRight;
            bool fallingOntoplatform = prevBottom <= platTop && playerBottom >= platTop;
            bool isFalling = dy > 0;

            if (horizontallyAligned && fallingOntoplatform && isFalling) {
                dy = getJumpStrength();
            }
        }

        if (y < screenHeight / 2) {
            score += abs(dy);
            for (int i = 0; i < platformCount; i++) {
                platforms[i].y -= dy;
                if (platforms[i].y > screenHeight) {
                    platforms[i].reset(score);
                    platforms[i].y = 0;
                }
            }
            y = screenHeight / 2;
        }
    }

    void moveLeft() {
        x -= 10;
        if (x < 0) x = 0;
    }

    void moveRight() {
        x += 10;
        if (x > screenWidth) x = screenWidth;
    }

    void drawScore() {
        setcolor(WHITE);
        settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
        char scoreText[30];
        sprintf(scoreText, "Score: %d", score);
        outtextxy(10, 10, scoreText);

        char bestScoreText[30];
        sprintf(bestScoreText, "Best: %d", bestScore);
        outtextxy(10, 30, bestScoreText);
    }
};

// ------------------ Best Score I/O ------------------
void loadBestScore() {
    ifstream file("best_score.txt");
    if (file.is_open()) {
        file >> bestScore;
        file.close();
    }
}

void saveBestScore(int score) {
    if (score > bestScore) {
        ofstream file("best_score.txt");
        if (file.is_open()) {
            file << score;
            file.close();
        }
    }
}

// ------------------ Game Over Screen ------------------
char showGameOver(int score) {
    saveBestScore(score);

    cleardevice();
    setbkcolor(BLACK);
    setcolor(RED);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 4);
    outtextxy(screenWidth / 2 - 120, screenHeight / 2 - 60, "GAME OVER");

    setcolor(YELLOW);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
    char scoreText[50];
    sprintf(scoreText, "Your Score: %d", score);
    outtextxy(screenWidth / 2 - 100, screenHeight / 2, scoreText);

    char bestText[50];
    sprintf(bestText, "Best Score: %d", bestScore);
    outtextxy(screenWidth / 2 - 100, screenHeight / 2 + 30, bestText);

    outtextxy(screenWidth / 2 - 160, screenHeight / 2 + 60, "Press R to Restart, M for Menu, ESC to Exit");

    while (true) {
        if (GetAsyncKeyState('R')) return 'r';
        if (GetAsyncKeyState('M')) return 'm';
        if (GetAsyncKeyState(VK_ESCAPE)) return 'e';
        delay(50);
    }
}

// ------------------ Menu ------------------
int showMenu() {
    int selection = 0;
    bool selecting = true;

    while (selecting) {
        cleardevice();
        setbkcolor(BLACK);
        setcolor(WHITE);

        settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 4);
        setcolor(CYAN); 
        outtextxy(screenWidth / 2 - 150, 150, "DOODLE JUMP BY YEFF");

        settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);

        if (selection == 0) setcolor(YELLOW);
        else setcolor(WHITE);
        outtextxy(screenWidth / 2 - 100, 300, ">> Start a Game");

        if (selection == 1) setcolor(YELLOW);
        else setcolor(WHITE);
        outtextxy(screenWidth / 2 - 100, 350, ">> Exit");

        if (GetAsyncKeyState(VK_DOWN)) {
            selection = (selection + 1) % 2;
            delay(200);
        } else if (GetAsyncKeyState(VK_UP)) {
            selection = (selection - 1 + 2) % 2;
            delay(200);
        } else if (GetAsyncKeyState(VK_RETURN)) {
            selecting = false;
        }

        delay(50);
    }

    return selection;
}

// ------------------ Main ------------------
int main() {
    srand(time(0));
    initwindow(screenWidth, screenHeight, "Doodle Jump");

    loadBestScore();

    while (true) {
        int choice = showMenu();
        if (choice == 1) break;

        char restart;
        do {
            Player player;
            Platform platforms[platformCount];

            platforms[0].x = screenWidth / 2 - 30;
            platforms[0].y = screenHeight - 80;
            platforms[0].color = GREEN;

            for (int i = 1; i < platformCount; i++) {
                platforms[i].reset(player.score);
                platforms[i].y = rand() % screenHeight;
            }

            bool gameOver = false;
            bool paused = false;

            while (!gameOver) {
                setbkcolor(LIGHTBLUE);
                cleardevice();

                // --- Pause/Resume ---
                if (GetAsyncKeyState('P')) paused = true;
                if (paused) {
                    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 4);
                    setcolor(WHITE);
                    outtextxy(screenWidth / 2 - 80, screenHeight / 2 - 20, "PAUSED");
                    outtextxy(screenWidth / 2 - 140, screenHeight / 2 + 20, "Press R to Resume");

                    while (paused) {
                        if (GetAsyncKeyState('R')) paused = false;
                        delay(100);
                    }
                    continue;
                }

                if (GetAsyncKeyState(VK_LEFT)) player.moveLeft();
                if (GetAsyncKeyState(VK_RIGHT)) player.moveRight();

                player.update(platforms);

                for (int i = 0; i < platformCount; i++) {
                    platforms[i].move(player.score);
                }

                for (int i = 0; i < platformCount; i++)
                    platforms[i].draw();
                player.draw();
                player.drawScore();

                if (player.y > screenHeight) {
                    gameOver = true;
                    break;
                }

                delay(20);
            }

            restart = showGameOver(player.score);

        } while (restart == 'r');
    }

    closegraph();
    return 0;
}