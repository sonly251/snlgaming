#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>

// OLED setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_SDA 21
#define OLED_SCL 22
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Buttons
#define BTN_UP 27
#define BTN_DOWN 26
#define BTN_SELECT 25
#define BTN_BACK 14
#define BTN_LEFT 32
#define BTN_RIGHT 33

// EEPROM addresses
#define EE_SNAKE 0
#define EE_PONG 1
#define EE_CAR 2
#define EE_DINO 3
#define EE_SHOOTER 4
#define EE_THEME 5
#define EE_FLAPPY 6
#define EE_ARKANOID 7
#define EE_TANKS 8
#define EE_SIGNATURE 9
#define EE_RANDOM_MIN 10
#define EE_RANDOM_MAX 11
#define EE_PET_HUNGER 12
#define EE_PET_MOOD 13
#define EE_PET_SCORE 14

// Для белой темы (чёрный логотип на белом фоне)
const unsigned char logoWhite[] PROGMEM = {0xff, 0xff, 0xff, 0x39, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xf9, 0x9f, 
 0x39, 0x33, 0xe7, 0x3e, 0x1d, 0x98, 0xcf, 0xff, 0xff, 0xf0, 0xff, 0xf1, 0x8e, 0x1b, 0x13, 0xe6, 
 0x30, 0x38, 0x99, 0xcf, 0xff, 0xff, 0xf0, 0xff, 0xf1, 0x8e, 0x19, 0x03, 0xe6, 0x11, 0x98, 0x49, 
 0xcf, 0xff, 0xff, 0xf0, 0xff, 0xf1, 0x8c, 0x19, 0x01, 0xe2, 0x10, 0x08, 0x09, 0xcf, 0xff, 0xff, 
 0xf0, 0xff, 0xf1, 0x20, 0x09, 0x21, 0xe2, 0x90, 0x39, 0x08, 0xcf, 0xff, 0xff, 0xf0, 0xff, 0xf0, 
 0x60, 0x09, 0x31, 0xe0, 0x91, 0xc9, 0x88, 0xcf, 0xff, 0xff, 0xf0, 0xff, 0xf0, 0x64, 0xc9, 0x31, 
 0xe0, 0x92, 0x01, 0x88, 0xcf, 0xff, 0xff, 0xf0, 0xff, 0xfa, 0x64, 0xc9, 0x39, 0xe1, 0x80, 0x0d, 
 0xcc, 0x0f, 0xff, 0xff, 0xf0, 0xff, 0xfa, 0x60, 0xc9, 0x39, 0xe5, 0xc0, 0x3d, 0xce, 0x1f, 0xff, 
 0xff, 0xf0, 0xff, 0xfb, 0xf5, 0xef, 0xbf, 0xff, 0xd9, 0xfd, 0xef, 0xff, 0xff, 0xff, 0xf0, 0xff, 
 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0};

// Для чёрной темы (белый логотип на чёрном фоне)
const unsigned char logoBlack[] PROGMEM = { 0xff, 0xff, 0xff, 0x39, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xf9, 0x9f, 
 0x39, 0x33, 0xe7, 0x3e, 0x1d, 0x98, 0xcf, 0xff, 0xff, 0xf0, 0xff, 0xf1, 0x8e, 0x1b, 0x13, 0xe6, 
 0x30, 0x38, 0x99, 0xcf, 0xff, 0xff, 0xf0, 0xff, 0xf1, 0x8e, 0x19, 0x03, 0xe6, 0x11, 0x98, 0x49, 
 0xcf, 0xff, 0xff, 0xf0, 0xff, 0xf1, 0x8c, 0x19, 0x01, 0xe2, 0x10, 0x08, 0x09, 0xcf, 0xff, 0xff, 
 0xf0, 0xff, 0xf1, 0x20, 0x09, 0x21, 0xe2, 0x90, 0x39, 0x08, 0xcf, 0xff, 0xff, 0xf0, 0xff, 0xf0, 
 0x60, 0x09, 0x31, 0xe0, 0x91, 0xc9, 0x88, 0xcf, 0xff, 0xff, 0xf0, 0xff, 0xf0, 0x64, 0xc9, 0x31, 
 0xe0, 0x92, 0x01, 0x88, 0xcf, 0xff, 0xff, 0xf0, 0xff, 0xfa, 0x64, 0xc9, 0x39, 0xe1, 0x80, 0x0d, 
 0xcc, 0x0f, 0xff, 0xff, 0xf0, 0xff, 0xfa, 0x60, 0xc9, 0x39, 0xe5, 0xc0, 0x3d, 0xce, 0x1f, 0xff, 
 0xff, 0xf0, 0xff, 0xfb, 0xf5, 0xef, 0xbf, 0xff, 0xd9, 0xfd, 0xef, 0xff, 0xff, 0xff, 0xf0, 0xff, 
 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0};

// Game states
enum MainState { STARTUP, MAIN_MENU, GAMES_MENU, SETTINGS_MENU, APPS_MENU,
                 SNAKE, PONG, CAR, DINO, SHOOTER, STOPWATCH, ABOUT, SHOW_RECORDS, THEME_SETTINGS,
                 FLAPPY, ARKANOID, TANKS, RANDOM_GEN, CODE_VIEWER, PET_MENU, PET_FEED, FLASHLIGHT, CALCULATOR };
MainState currentState = STARTUP;
int menuSelection = 0;
int submenuSelection = 0;
int gamesPage = 0;
int recordsPage = 0;

// Плавная анимация перехода
float menuSmoothY = 0;      // Текущая плавная позиция Y
int targetMenuIndex = 0;    // Целевой индекс пункта
int currentMenuIndex = 0;   // Текущий индекс для отрисовки
unsigned long lastAnimUpdate = 0;
float animSpeed = 0.5;      // Скорость анимации (0.1-0.5)
// Для MAIN_MENU плавная анимация
float mainMenuSmoothY = 0;
int mainMenuTargetIndex = 0;
int mainMenuCurrentIndex = 0;

// Для CALCULATOR анимация нажатия
int calcButtonPressAnim = -1;  // Индекс нажатой кнопки
unsigned long calcButtonPressTime = 0;
int calcLastCursorX = -1, calcLastCursorY = -1;

// Random generator variables
int randomMin = 1;
int randomMax = 100;
int randomResult = 0;
int randomEditMode = 0;

// Code viewer
int codeLine = 0;
bool codeScrolling = false;
unsigned long lastCodeScroll = 0;

// Pet variables
int petHunger = 50; // 0-100, 0-голодный, 100-сытый
int petMood = 50;
int petScore = 0;
int petEmotion = 0; // 0-нормальное, 1-счастливый, 2-грустный
unsigned long petLastUpdate = 0;
unsigned long petGameTime = 0;
int petHeartsCollected = 0;

// Feed game variables
int feedHeartX = -100, feedHeartY = -100;
int feedBasketX = 60;
int feedScore = 0;
bool feedGameActive = false;
unsigned long feedLastSpawn = 0;
unsigned long feedLastMove = 0;

// Snake variables
#define MAX_SNAKE 100
int snakeX[MAX_SNAKE], snakeY[MAX_SNAKE];
int snakeLength;
int foodX, foodY;
int dirX, dirY;
unsigned long lastMoveTime = 0;
int snakeScore = 0;
bool gameActive = false;
int snakeSpeed = 120;

// Pong variables
int paddleY;
int ballX, ballY;
int ballDX, ballDY;
int pongScore;
unsigned long pongLastUpdate = 0;
int ballSpeed = 30;

// Car game variables
int playerLane = 1;
int obstacleLane[3] = {-1, -1, -1};
int obstacleY[3] = {-20, -40, -60};
int carScore = 0;
bool carGameActive = false;
unsigned long lastCarMove = 0;
int carSpeed = 80;

// Dino game variables
int dinoY = 44;
int dinoVelocity = 0;
bool isJumping = false;
int obstacleX = 128;
int dinoScore = 0;
unsigned long lastDinoUpdate = 0;
int dinoSpeed = 50;
bool dinoCrouch = false;

// Flappy Bird variables
int flappyBirdY = 32;
int flappyBirdVelocity = 0;
int flappyPipeX = 128;
int flappyPipeGap = 40;
int flappyPipeTopHeight = 20;
int flappyScore = 0;
bool flappyActive = false;
unsigned long lastFlappyUpdate = 0;
int flappySpeed = 60;

// Arkanoid variables
int arkanoidPaddleX = 54;
int arkanoidBallX = 64, arkanoidBallY = 50;
int arkanoidBallDX = 1, arkanoidBallDY = -1;
int arkanoidBricks[5][12];
int arkanoidScore = 0;
bool arkanoidActive = false;
unsigned long lastArkanoidUpdate = 0;
int arkanoidSpeed = 35;
int arkanoidLives = 3;

// Tanks variables
int tankX = 60, tankY = 50;
int tankDirX = 0, tankDirY = -1;
int tankHealth = 10;
int enemyTanks[10][4];
int enemyHealth[10];
int tankBulletX = -10, tankBulletY = -10;
int tankBulletDirX = 0, tankBulletDirY = 0;
bool tankBulletActive = false;
int enemyBullets[10][3];
unsigned long lastTankMove = 0;
unsigned long lastTankShot = 0;
int tankScore = 0;
bool tankGameActive = false;
int tankSpeed = 200;
int currentWave = 1;
int enemiesInWave = 3;
int enemiesRemaining = 0;
bool waveTransition = false;

// Shooter variables
int sPlayerX = 60;
int sPlayerHealth = 100;
#define S_MAX_BULLETS 10
int sBulletX[S_MAX_BULLETS], sBulletY[S_MAX_BULLETS];
bool sBulletActive[S_MAX_BULLETS];
unsigned long sLastShot = 0;
int sShootDelay = 250;
#define S_MAX_ENEMIES 8
int sEnemyX[S_MAX_ENEMIES], sEnemyY[S_MAX_ENEMIES];
int sEnemyHealth[S_MAX_ENEMIES];
bool sEnemyActive[S_MAX_ENEMIES];
int sEnemyCount = 0;
unsigned long sLastEnemySpawn = 0;
int sEnemySpeed = 100;
bool sGameActive = false;
int sScore = 0;
int sKillCount = 0;
unsigned long sLastEnemyMove = 0;
int sEnemySpawnDelay = 2000;

// Calculator variables
String calcExpression = "";
String calcResult = "";
int calcCursorX = 0, calcCursorY = 0;
char calcButtons[5][4] = {
  {'7', '8', '9', '+'},
  {'4', '5', '6', '-'},
  {'1', '2', '3', '*'},
  {'0', 'C', '=', '/'},
  {' ', ' ', ' ', ' '}
};
bool calcResultDisplay = false;
bool calcButtonPressed = false;

// Stopwatch variables
unsigned long stopwatchStartTime = 0;
unsigned long stopwatchElapsed = 0;
bool stopwatchRunning = false;

// Button debounce
unsigned long lastButtonPress = 0;
#define DEBOUNCE_DELAY 200

// Theme
bool whiteTheme = false;

// High scores
int highSnake = 0, highPong = 0, highCar = 0, highDino = 0, highShooter = 0;
int highFlappy = 0, highArkanoid = 0, highTanks = 0;

// Boot animation
unsigned long bootStartTime = 0;
bool eepromInitialized = false;

// ========== Theme Drawing Functions ==========
inline void setDrawColor() {
  if (whiteTheme) display.setTextColor(BLACK);
  else display.setTextColor(WHITE);
}

inline void setInverseColor() {
  if (whiteTheme) display.setTextColor(WHITE);
  else display.setTextColor(BLACK);
}

inline void drawRect(int x, int y, int w, int h, bool fill) {
  if (fill) {
    if (whiteTheme) display.fillRect(x, y, w, h, BLACK);
    else display.fillRect(x, y, w, h, WHITE);
  } else {
    if (whiteTheme) display.drawRect(x, y, w, h, BLACK);
    else display.drawRect(x, y, w, h, WHITE);
  }
}

inline void drawCircle(int x, int y, int r, bool fill) {
  if (fill) {
    if (whiteTheme) display.fillCircle(x, y, r, BLACK);
    else display.fillCircle(x, y, r, WHITE);
  } else {
    if (whiteTheme) display.drawCircle(x, y, r, BLACK);
    else display.drawCircle(x, y, r, WHITE);
  }
}

inline void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, bool fill) {
  if (fill) {
    if (whiteTheme) display.fillTriangle(x1, y1, x2, y2, x3, y3, BLACK);
    else display.fillTriangle(x1, y1, x2, y2, x3, y3, WHITE);
  } else {
    if (whiteTheme) display.drawTriangle(x1, y1, x2, y2, x3, y3, BLACK);
    else display.drawTriangle(x1, y1, x2, y2, x3, y3, WHITE);
  }
}

inline void drawLine(int x1, int y1, int x2, int y2) {
  if (whiteTheme) display.drawLine(x1, y1, x2, y2, BLACK);
  else display.drawLine(x1, y1, x2, y2, WHITE);
}

// ========== EEPROM Functions ==========
void initEEPROM() {
  EEPROM.begin(64);
  int signature = EEPROM.read(EE_SIGNATURE);
  if (signature != 0xAA) {
    EEPROM.write(EE_SNAKE, 0);
    EEPROM.write(EE_PONG, 0);
    EEPROM.write(EE_CAR, 0);
    EEPROM.write(EE_DINO, 0);
    EEPROM.write(EE_SHOOTER, 0);
    EEPROM.write(EE_FLAPPY, 0);
    EEPROM.write(EE_ARKANOID, 0);
    EEPROM.write(EE_TANKS, 0);
    EEPROM.write(EE_THEME, 0);
    EEPROM.write(EE_RANDOM_MIN, 1);
    EEPROM.write(EE_RANDOM_MAX, 100);
    EEPROM.write(EE_PET_SCORE, 0);
    EEPROM.write(EE_PET_HUNGER, 50);
    EEPROM.write(EE_PET_MOOD, 50);
    EEPROM.write(EE_SIGNATURE, 0xAA);
    EEPROM.commit();
  }
  eepromInitialized = true;
}

void loadHighScores() {
  initEEPROM();
  
  highSnake = EEPROM.read(EE_SNAKE);
  highPong = EEPROM.read(EE_PONG);
  highCar = EEPROM.read(EE_CAR);
  highDino = EEPROM.read(EE_DINO);
  highShooter = EEPROM.read(EE_SHOOTER);
  highFlappy = EEPROM.read(EE_FLAPPY);
  highArkanoid = EEPROM.read(EE_ARKANOID);
  highTanks = EEPROM.read(EE_TANKS);
  whiteTheme = EEPROM.read(EE_THEME);
  randomMin = EEPROM.read(EE_RANDOM_MIN);
  randomMax = EEPROM.read(EE_RANDOM_MAX);
  petHunger = EEPROM.read(EE_PET_HUNGER);
  petMood = EEPROM.read(EE_PET_MOOD);
  petScore = EEPROM.read(EE_PET_SCORE);

  if (highSnake > 255) highSnake = 0;
  if (highPong > 255) highPong = 0;
  if (highCar > 255) highCar = 0;
  if (highDino > 255) highDino = 0;
  if (highShooter > 255) highShooter = 0;
  if (highFlappy > 255) highFlappy = 0;
  if (highArkanoid > 255) highArkanoid = 0;
  if (highTanks > 255) highTanks = 0;
  if (whiteTheme > 1) whiteTheme = false;
  if (randomMin < 1) randomMin = 1;
  if (randomMax < randomMin) randomMax = randomMin + 1;
  if (randomMax > 999) randomMax = 999;
  if (petHunger < 0) petHunger = 50;
  if (petHunger > 100) petHunger = 50;
  if (petMood < 0) petMood = 50;
  if (petMood > 100) petMood = 50;
}

void saveHighScore(int gameAddr, int score, int &highScore) {
  if (score > highScore && score <= 255) {
    highScore = score;
    EEPROM.write(gameAddr, highScore);
    EEPROM.commit();
    delay(10);
  }
}

void saveTheme() {
  EEPROM.write(EE_THEME, whiteTheme ? 1 : 0);
  EEPROM.commit();
}

void saveRandomRange() {
  EEPROM.write(EE_RANDOM_MIN, randomMin);
  EEPROM.write(EE_RANDOM_MAX, randomMax);
  EEPROM.commit();
}

void savePetData() {
  EEPROM.write(EE_PET_HUNGER, petHunger);
  EEPROM.write(EE_PET_MOOD, petMood);
  EEPROM.write(EE_PET_SCORE, petScore);
  EEPROM.commit();
}

// ========== ПЛАВНАЯ АНИМАЦИЯ ПРЯМОУГОЛЬНИКА ==========
void updateSmoothCursor(int targetIndex, int startY, int stepY) {
  targetMenuIndex = targetIndex;
  float targetY = startY + targetIndex * stepY;
  
  unsigned long now = millis();
  if (now - lastAnimUpdate > 16) { // ~60 FPS
    lastAnimUpdate = now;
    // Плавное движение (easing)
    menuSmoothY = menuSmoothY + (targetY - menuSmoothY) * animSpeed;
    
    // Допуск для остановки анимации
    if (abs(menuSmoothY - targetY) < 0.5) {
      menuSmoothY = targetY;
      currentMenuIndex = targetMenuIndex;
    }
  }
}

void drawSmoothCursor(int x, int w, int h, int startY, int stepY) {
  int cursorY = (int)menuSmoothY;
  // Рисуем прямоугольник в плавной позиции
  drawRect(x, cursorY - 2, w, h, true);
}

// ========== PET FUNCTIONS ==========
void updatePet() {
  if (millis() - petLastUpdate > 60000) { // Каждую минуту
    petLastUpdate = millis();
    petHunger -= 2;
    if (petHunger < 0) petHunger = 0;
    
    if (petHunger < 20) {
      petMood -= 5;
      petEmotion = 2; // Грустный
    } else if (petHunger > 80) {
      petMood += 2;
      petEmotion = 1; // Счастливый
    } else {
      petEmotion = 0; // Нормальный
    }
    if (petMood > 100) petMood = 100;
    if (petMood < 0) petMood = 0;
    savePetData();
  }
}

void drawPet() {
  if (petEmotion == 1) { // Счастливый
    display.setCursor(2, 5);
    display.print("(>-<)");
  } else if (petEmotion == 2) { // Грустный 
    display.setCursor(2, 5);
    display.print("(o_o)");

  } else { // Нормальный 
    display.setCursor(2, 5);
    display.print("(>-<)");
  }
}

// ========== PET MENU ==========
void runPetMenu() {
  if (millis() - lastButtonPress > DEBOUNCE_DELAY) {
    if (digitalRead(BTN_SELECT) == LOW) {
      changeState(PET_FEED);
      lastButtonPress = millis();
    }
    if (digitalRead(BTN_BACK) == LOW) {
      changeState(MAIN_MENU);
      lastButtonPress = millis();
    }
  }
  
  display.clearDisplay();
  if (whiteTheme) display.fillScreen(WHITE);
  setDrawColor();
  display.setTextSize(2);
  
  drawRect(0, 0, 128, 10, true);
  setInverseColor();
  display.setTextSize(1);
  display.setCursor(2, 2);
  display.print("minigotchi");
  setDrawColor();
  
  display.setTextSize(3);
  display.setCursor(2, 14);
  
  if (petEmotion == 1) {
    display.print("(>-<)");
    display.setTextSize(1);
    display.setCursor(72, 37);
    display.print("..HAPPY!");
  } else if (petEmotion == 2) {
    display.print("(._.)");
    display.setTextSize(1);
    display.setCursor(72, 37 );
    display.print("..FEED ME!");
  } else {
    display.print("(>-<)");
    display.setTextSize(1);
    display.setCursor(72, 37);
    display.print(" ..HELLO!");
  }
  
  display.setTextSize(1);
  display.setCursor(2, 40);
  display.print("HUNGER:");
  display.print(petHunger);
  display.setCursor(2, 50);
  display.print("SCORE:");
  display.print(petScore);

  
  display.display();
}

// ========== FEED GAME ==========
int heartX = -100;
int heartY = -100;
int basketX = 60;
int catchScore = 0;
bool heartActive = false;
unsigned long lastHeartMove = 0;
unsigned long lastHeartSpawn = 0;

void initFeedGame() {
  basketX = 60;
  catchScore = 0;
  heartActive = false;
  heartX = -100;
  heartY = -100;
  lastHeartSpawn = millis();
  lastHeartMove = millis();
}

void drawFeedGame() {
  setDrawColor();
  
  // Рисуем корзину
  drawRect(basketX-8, 56, 16, 8, true);
  drawRect(basketX-6, 54, 12, 4, true);
  
  // Рисуем сердечко (только если оно не в зоне счёта)
  if (heartActive && heartY > 0 && heartY < 64) {
    // Не рисуем сердечко если оно в зоне счёта (Y < 15)
    if (heartY > 15) {
      drawCircle(heartX-3, heartY-2, 3, true);
      drawCircle(heartX+3, heartY-2, 3, true);
      drawTriangle(heartX-4, heartY-1, heartX+4, heartY-1, heartX, heartY+4, true);
    }
  }
}

void updateFeedGame() {
  // Управление корзиной
  if (digitalRead(BTN_LEFT) == LOW) {
    basketX -= 10;
    if (basketX < 20) basketX = 20;
    lastButtonPress = millis();
  }
  if (digitalRead(BTN_RIGHT) == LOW) {
    basketX += 10;
    if (basketX > 108) basketX = 108;
    lastButtonPress = millis();
  }
  
  // Спавн сердечка (начиная с Y=15, чтобы не появлялось в зоне счёта)
  if (!heartActive && millis() - lastHeartSpawn > 1500) {
    heartActive = true;
    heartX = random(25, 103);
    heartY = 15;  // Начинаем с Y=15, ниже счёта
    lastHeartSpawn = millis();
  }
  
  // Движение сердечка
  if (heartActive && millis() - lastHeartMove > 50) {
    lastHeartMove = millis();
    heartY += 3;
    
    // Проверка поймали ли сердечко
    if (heartY > 50 && heartY < 60 && abs(heartX - basketX) < 15) {
      heartActive = false;
      petScore++;
      petHunger += 5;
      if (petHunger > 100) petHunger = 100;
      if (petScore > 999) petScore = 999;
      savePetData();
      heartX = -100;
      heartY = -100;
    }
    
    // Проверка упало ли сердечко
    if (heartY > 64) {
      heartActive = false;
      heartX = -100;
      heartY = -100;
    }
  }
}

void runFeedGame() {
  if (!feedGameActive) {
    initFeedGame();
    feedGameActive = true;
    return;
  }
  
  updateFeedGame();
  
  display.clearDisplay();
  if (whiteTheme) display.fillScreen(WHITE);
  setDrawColor();
  
  // Верхняя панель (перерисовывается после всего, чтобы быть поверх)
  drawRect(0, 0, 128, 12, true);
  setInverseColor();
  display.setTextSize(1);
  display.setCursor(2, 3);
  display.print("SCORE: ");
  display.print(petScore);
  setDrawColor();
  
  drawFeedGame();
  
  display.display();
  
  if (digitalRead(BTN_BACK) == LOW) {
    feedGameActive = false;
    savePetData();
    changeState(PET_MENU);
    lastButtonPress = millis();
  }
}

// ========== Main Menu Pet ==========
void runMainMenu() {
  int startY = 35;
  int stepY = 9;
  int itemsCount = 3;
  
  if (millis() - lastButtonPress > DEBOUNCE_DELAY) {
    if (digitalRead(BTN_UP) == LOW) { 
      menuSelection = (menuSelection - 1 + 3) % 3; 
      mainMenuTargetIndex = menuSelection;
      lastButtonPress = millis(); 
    }
    if (digitalRead(BTN_DOWN) == LOW) { 
      menuSelection = (menuSelection + 1) % 3; 
      mainMenuTargetIndex = menuSelection;
      lastButtonPress = millis(); 
    }
    if (digitalRead(BTN_SELECT) == LOW) {
      if (menuSelection == 0) { gamesPage = 0; submenuSelection = 0; changeState(GAMES_MENU); }
      else if (menuSelection == 1) changeState(SETTINGS_MENU);
      else if (menuSelection == 2) changeState(APPS_MENU);
      lastButtonPress = millis();
    }
    if (digitalRead(BTN_RIGHT) == LOW) {
      changeState(PET_MENU);
      lastButtonPress = millis();
    }
  }
  
  // Плавное движение курсора
  float targetY = startY + mainMenuTargetIndex * stepY;
  unsigned long now = millis();
  if (now - lastAnimUpdate > 16) {
    lastAnimUpdate = now;
    mainMenuSmoothY = mainMenuSmoothY + (targetY - mainMenuSmoothY) * animSpeed;
    if (abs(mainMenuSmoothY - targetY) < 0.5) {
      mainMenuSmoothY = targetY;
    }
  }
  
  display.clearDisplay();
  if (whiteTheme) display.fillScreen(WHITE);
  
  drawRect(0, 0, 128, 10, true);
  setInverseColor();
  display.setTextSize(1);
  display.setCursor(2, 2);
  display.print("SNLGAMING");
  setDrawColor();
  
// Рисуем логотип в зависимости от темы
if (whiteTheme) {
  // Белая тема: рисуем чёрный логотип (из массива logoWhite)
  display.drawBitmap(0, 15, logoWhite, 100, 12, BLACK);
} else {
  // Чёрная тема: рисуем белый логотип (из массива logoBlack)
  display.drawBitmap(0, 15, logoBlack, 100, 12, WHITE);
}
  
  const char* items[] = {"GAMES", "SETTINGS", "APPS"};
  
  // Рисуем плавный прямоугольник
  int cursorY = (int)mainMenuSmoothY;
  drawRect(5, cursorY - 1, 70, 9, true);
  
  // Рисуем текст с инверсией внутри прямоугольника
  for (int i = 0; i < itemsCount; i++) {
    int y = startY + i * stepY;
    int cursorTop = cursorY - 1;
    int cursorBottom = cursorY + 8;
    
    if (y >= cursorTop && y <= cursorBottom) {
      setInverseColor();
    } else {
      setDrawColor();
    }
    
    display.setCursor(10, y);
    display.print(items[i]);
  }
  
  setDrawColor();
  
  // Питомец справа снизу
  display.setTextSize(1);
  if (petEmotion == 1) {
    display.setCursor(95, 55);
    display.print("(>-<)");
  } else if (petEmotion == 2) {
    display.setCursor(95, 55);
    display.print("(._.)");
  } else {
    display.setCursor(95, 55);
    display.print("(>-<)");
  }
  
  display.display();
}

// ========== GAME OVER FUNCTION ==========
void gameOver(String game, int score, int eeAddr, int &highScore) {
  gameActive = false;
  carGameActive = false;
  flappyActive = false;
  arkanoidActive = false;
  tankGameActive = false;
  sGameActive = false;
  
  // Добавляем очки питомцу за игру
  petScore += 10;
  petHunger += 5;
  if (petHunger > 100) petHunger = 100;
  if (petScore > 999) petScore = 999;
  savePetData();

  // Добавляем очки питомцу за время игры
  petScore += 10;
  petHunger += 5;
  if (petHunger > 100) petHunger = 100;
  savePetData();
  
  if (score > highScore && score <= 255) {
    highScore = score;
    EEPROM.write(eeAddr, highScore);
    EEPROM.commit();
    delay(10);
  }
  
  display.clearDisplay();
  if (whiteTheme) display.fillScreen(WHITE);
  display.setTextSize(2);
  display.setCursor(10, 10);
  setDrawColor();
  display.println("GAME OVER");
  display.setTextSize(1);
  display.setCursor(20, 35);
  display.print("Game: "); display.print(game);
  display.setCursor(20, 50);
  display.print("Score: "); display.print(score);
  display.display();
  delay(3000);
  changeState(GAMES_MENU);
}

// ========== Setup ==========
void setup() {
  Serial.begin(115200);
  
  Wire.begin(OLED_SDA, OLED_SCL);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for(;;);
  }

  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BTN_SELECT, INPUT_PULLUP);
  pinMode(BTN_BACK, INPUT_PULLUP);

  randomSeed(analogRead(0));
  loadHighScores();
  
  bootStartTime = millis();
  petLastUpdate = millis();

  // Инициализация плавного курсора
  menuSmoothY = 14; 
  targetMenuIndex = 0;
  currentMenuIndex = 0;
  lastAnimUpdate = millis();
  // Инициализация анимации MAIN_MENU
  mainMenuSmoothY = 35;
  mainMenuTargetIndex = 0;
  mainMenuCurrentIndex = 0;
  
  // Инициализация анимации CALCULATOR
  calcButtonPressAnim = -1;
  calcLastCursorX = 0;
  calcLastCursorY = 0;

}

// ========== Boot Animation ==========
void runBootAnimation() {
  unsigned long elapsed = millis() - bootStartTime;
  int progress = constrain((elapsed * 100) / 1000, 0, 100);
  
  display.clearDisplay();
  if (whiteTheme) display.fillScreen(WHITE);
  
  display.setTextSize(2);
  display.setCursor(10, 17);
  setDrawColor();
  display.print("SNL");
  display.setCursor(10, 33);
  display.print("GAMING");
  
  drawRect(14, 56, 100, 4, false);
  drawRect(15, 57, progress, 2, true);
  
  display.display();
  
  if (elapsed >= 1000) {
    changeState(MAIN_MENU);
  }
}

// ========== Loop ==========
void loop() {
  if (currentState == STARTUP) {
    runBootAnimation();
    return;
  }
  
  updatePet();
  
  if (digitalRead(BTN_BACK) == LOW && millis() - lastButtonPress > DEBOUNCE_DELAY) {
    if (currentState != MAIN_MENU && currentState != STARTUP && currentState != PET_MENU && currentState != PET_FEED) {
      if (currentState == SNAKE || currentState == PONG || currentState == CAR || 
          currentState == DINO || currentState == SHOOTER || currentState == FLAPPY || 
          currentState == ARKANOID || currentState == TANKS ) {
        changeState(GAMES_MENU);
      }
      else if (currentState == THEME_SETTINGS || currentState == SHOW_RECORDS || currentState == CODE_VIEWER) {
        changeState(SETTINGS_MENU);
      }
      else if (currentState == STOPWATCH || currentState == FLASHLIGHT || 
               currentState == CALCULATOR || currentState == RANDOM_GEN) {
        changeState(APPS_MENU);
      }
      else if (currentState == ABOUT) {
        changeState(SETTINGS_MENU);
      }
      else {
        changeState(MAIN_MENU);
      }
      lastButtonPress = millis();
      return;
    }
  }
  
  if (codeScrolling && millis() - lastCodeScroll > 30) {
    lastCodeScroll = millis();
    codeLine++;
    if (codeLine >= 35 - 3) codeLine = 0;
  }
  
  switch (currentState) {
    case MAIN_MENU: runMainMenu(); break;
    case GAMES_MENU: runGamesMenu(); break;
    case SETTINGS_MENU: runSettingsMenu(); break;
    case APPS_MENU: runAppsMenu(); break;
    case SNAKE: runSnake(); break;
    case PONG: runPong(); break;
    case CAR: runCarGame(); break;
    case DINO: runDinoGame(); break;
    case SHOOTER: runShooter(); break;
    case FLAPPY: runFlappy(); break;
    case ARKANOID: runArkanoid(); break;
    case TANKS: runTanks(); break;
    case STOPWATCH: runStopwatch(); break;
    case FLASHLIGHT: runFlashlight(); break;
    case CALCULATOR: runCalculator(); break;
    case RANDOM_GEN: runRandomGen(); break;
    case CODE_VIEWER: runCodeViewer(); break;
    case PET_MENU: runPetMenu(); break;
    case PET_FEED: runFeedGame(); break;
    case ABOUT: runAbout(); break;
    case SHOW_RECORDS: showRecords(); break;
    case THEME_SETTINGS: runThemeSettings(); break;
    default: break;
  }
  delay(10);
}

void changeState(int state) {
  currentState = (MainState)state;
}

// ========== Games Menu ==========
void runGamesMenu() {
  int startY = 14;
  int stepY = 12;
  int itemsPerPage = 4;
  
  if (millis() - lastButtonPress > DEBOUNCE_DELAY) {
    if (digitalRead(BTN_UP) == LOW) { 
      submenuSelection--;
      if (submenuSelection < 0) {
        if (gamesPage == 0) {
          submenuSelection = 3;
          gamesPage = 1;
        } else {
          submenuSelection = 3;
          gamesPage = 0;
        }
      }
      lastButtonPress = millis();
    }
    if (digitalRead(BTN_DOWN) == LOW) { 
      submenuSelection++;
      if (gamesPage == 0 && submenuSelection > 3) {
        submenuSelection = 0;
        gamesPage = 1;
      } else if (gamesPage == 1 && submenuSelection > 3) {
        submenuSelection = 0;
        gamesPage = 0;
      }
      lastButtonPress = millis();
    }
    if (digitalRead(BTN_SELECT) == LOW) {
      int gameIndex = gamesPage * 4 + submenuSelection;
      switch (gameIndex) {
        case 0: initSnake(); changeState(SNAKE); break;
        case 1: initPong(); changeState(PONG); break;
        case 2: initCarGame(); changeState(CAR); break;
        case 3: initDinoGame(); changeState(DINO); break;
        case 4: initShooterGame(); changeState(SHOOTER); break;
        case 5: initFlappy(); changeState(FLAPPY); break;
        case 6: initArkanoid(); changeState(ARKANOID); break;
        case 7: initTanks(); changeState(TANKS); break;
      }
      lastButtonPress = millis();
    }
  }
  
  // Обновляем плавную позицию курсора
  updateSmoothCursor(submenuSelection, startY, stepY);
  
  display.clearDisplay();
  if (whiteTheme) display.fillScreen(WHITE);
  display.setTextSize(1);
  display.setCursor(35, 2);
  setDrawColor();
  display.println("GAMES");
  
  const char* itemsPage0[] = {"SNAKE", "PONG", "RACER", "DINO"};
  const char* itemsPage1[] = {"SHOOTER", "FLAPPY", "ARKANOID", "TANKS"};
  
  const char** currentItems = (gamesPage == 0) ? itemsPage0 : itemsPage1;
  
  // Сначала рисуем плавный прямоугольник (под текстом)
  drawSmoothCursor(5, 118, 10, startY, stepY);
  
  // Потом рисуем текст поверх
  for (int i = 0; i < itemsPerPage; i++) {
    int y = startY + i * stepY;
    const char* gameName = currentItems[i];
    if (strlen(gameName) == 0) continue;
    
    // Инвертируем цвет текста, если он внутри прямоугольника
    int cursorTop = (int)menuSmoothY - 2;
    int cursorBottom = cursorTop + 10;
    
    if (y >= cursorTop && y <= cursorBottom) {
      setInverseColor();
    } else {
      setDrawColor();
    }
    
    display.setCursor(10, y);
    display.print(gameName);
  }
  
  setDrawColor(); // Сброс цвета
  display.display();
}

// ========== Settings Menu ==========
void runSettingsMenu() {
  int startY = 16;
  int stepY = 11;
  
  if (millis() - lastButtonPress > DEBOUNCE_DELAY) {
    if (digitalRead(BTN_UP) == LOW) { 
      submenuSelection = (submenuSelection - 1 + 4) % 4;
      lastButtonPress = millis(); 
    }
    if (digitalRead(BTN_DOWN) == LOW) { 
      submenuSelection = (submenuSelection + 1) % 4;
      lastButtonPress = millis(); 
    }
    if (digitalRead(BTN_SELECT) == LOW) {
      if (submenuSelection == 0) { recordsPage = 0; changeState(SHOW_RECORDS); }
      else if (submenuSelection == 1) changeState(THEME_SETTINGS);
      else if (submenuSelection == 2) changeState(CODE_VIEWER);
      else if (submenuSelection == 3) changeState(ABOUT);
      lastButtonPress = millis();
    }
  }
  
  updateSmoothCursor(submenuSelection, startY, stepY);
  
  display.clearDisplay();
  if (whiteTheme) display.fillScreen(WHITE);
  display.setTextSize(1);
  display.setCursor(35, 2);
  setDrawColor();
  display.println("SETTINGS");
  
  const char* items[] = {"RECORDS", "THEME", "CODE", "ABOUT"};
  
  drawSmoothCursor(5, 118, 10, startY, stepY);
  
  for (int i = 0; i < 4; i++) {
    int y = startY + i * stepY;
    int cursorTop = (int)menuSmoothY - 2;
    int cursorBottom = cursorTop + 10;
    
    if (y >= cursorTop && y <= cursorBottom) {
      setInverseColor();
    } else {
      setDrawColor();
    }
    
    display.setCursor(10, y);
    display.print(items[i]);
  }
  
  setDrawColor();
  display.display();
}

void runThemeSettings() {
  if (millis() - lastButtonPress > DEBOUNCE_DELAY) {
    if (digitalRead(BTN_SELECT) == LOW) {
      whiteTheme = !whiteTheme;
      saveTheme();
      lastButtonPress = millis();
    }
  }
  
  display.clearDisplay();
  if (whiteTheme) display.fillScreen(WHITE);
  display.setTextSize(1);
  display.setCursor(25, 20);
  setDrawColor();
  display.println("THEME SETTINGS");
  display.setCursor(35, 35);
  display.print("MODE: ");
  display.println(whiteTheme ? "WHITE" : "DARK");
  display.display();
}

void showRecords() {
  if (millis() - lastButtonPress > DEBOUNCE_DELAY) {
    if (digitalRead(BTN_UP) == LOW) {
      recordsPage = (recordsPage + 1) % 2;
      lastButtonPress = millis();
    }
    if (digitalRead(BTN_DOWN) == LOW) {
      recordsPage = (recordsPage + 1) % 2;
      lastButtonPress = millis();
    }
  }
  
  display.clearDisplay();
  if (whiteTheme) display.fillScreen(WHITE);
  display.setTextSize(1);
  display.setCursor(10, 2);
  setDrawColor();
  display.println("HIGH SCORES");
  
  if (recordsPage == 0) {
    display.setCursor(10, 14); display.print("SNAKE:    "); display.println(highSnake);
    display.setCursor(10, 24); display.print("PONG:     "); display.println(highPong);
    display.setCursor(10, 34); display.print("RACER:    "); display.println(highCar);
    display.setCursor(10, 44); display.print("DINO:     "); display.println(highDino);
  } else {
    display.setCursor(10, 14); display.print("SHOOTER:  "); display.println(highShooter);
    display.setCursor(10, 24); display.print("FLAPPY:   "); display.println(highFlappy);
    display.setCursor(10, 34); display.print("ARKANOID: "); display.println(highArkanoid);
    display.setCursor(10, 44); display.print("TANKS:    "); display.println(highTanks);
  }
  
  display.display();
}

void runCodeViewer() {
  if (millis() - lastButtonPress > DEBOUNCE_DELAY) {
    if (digitalRead(BTN_SELECT) == LOW) {
      codeScrolling = !codeScrolling;
      lastButtonPress = millis();
    }
  }
  
  display.clearDisplay();
  if (whiteTheme) display.fillScreen(WHITE);
  display.setTextSize(1);
  setDrawColor();
  
  const char* codeLines[] = {
    "#include <Wire.h>",
    "#include <Adafruit_GFX.h>",
    "#include <Adafruit_SSD1306.h>",
    "#include <EEPROM.h>",
    "",
    "#define SCREEN_WIDTH 128",
    "#define SCREEN_HEIGHT 64",
    "",
    "#define BTN_UP 27",
    "#define BTN_DOWN 26",
    "#define BTN_SELECT 25",
    "#define BTN_BACK 14",
    "#define BTN_LEFT 32",
    "#define BTN_RIGHT 33",
    "",
    "// SNL GAMING v7.0",
    "// Created: 22.04.26",
    "// With Virtual Pet!"
  };
  int totalLines = sizeof(codeLines) / sizeof(codeLines[0]);
  
  for (int i = 0; i < 4; i++) {
    int idx = (codeLine + i) % totalLines;
    display.setCursor(0, 2 + i * 15);
    display.print(codeLines[idx]);
  }
  
  display.display();
}

// ========== Apps Menu ==========
void runAppsMenu() {
  int startY = 14;
  int stepY = 12;
  int itemsPerPage = 4;
  
  if (millis() - lastButtonPress > DEBOUNCE_DELAY) {
    if (digitalRead(BTN_UP) == LOW) {
      submenuSelection--;
      if (submenuSelection < 0) {
        submenuSelection = 3;
      }
      lastButtonPress = millis();
    }
    if (digitalRead(BTN_DOWN) == LOW) {
      submenuSelection++;
      if (submenuSelection > 3) {
        submenuSelection = 0;
      }
      lastButtonPress = millis();
    }
    if (digitalRead(BTN_SELECT) == LOW) {
      switch (submenuSelection) {
        case 0: changeState(STOPWATCH); break;
        case 1: changeState(FLASHLIGHT); break;
        case 2: changeState(CALCULATOR); break;
        case 3: changeState(RANDOM_GEN); break;
      }
      lastButtonPress = millis();
    }
  }
  
  updateSmoothCursor(submenuSelection, startY, stepY);
  
  display.clearDisplay();
  if (whiteTheme) display.fillScreen(WHITE);
  display.setTextSize(1);
  display.setCursor(35, 2);
  setDrawColor();
  display.println("APPS");
  
  const char* items[] = {"STOPWATCH", "FLASHLIGHT", "CALCULATOR", "RANDOM GEN"};
  
  drawSmoothCursor(5, 118, 10, startY, stepY);
  
  for (int i = 0; i < itemsPerPage; i++) {
    int y = startY + i * stepY;
    
    if (y >= (int)menuSmoothY - 2 && y <= (int)menuSmoothY + 8) {
      setInverseColor();
    } else {
      setDrawColor();
    }
    
    display.setCursor(10, y);
    display.print(items[i]);
  }
  
  setDrawColor();
  display.display();
}
// ========== Random Generator ==========
void drawRandomUI() {
  display.clearDisplay();
  if (whiteTheme) display.fillScreen(WHITE);
  setDrawColor();
  display.setTextSize(1);
  
  drawRect(0, 0, 128, 16, true);
  setInverseColor();
  display.setCursor(2, 0);
  display.print("RANDOM GENERATOR");
  display.setCursor(2, 8);
  if (randomEditMode == 0) {
    display.print("EDITING MIN VALUE");
  } else {
    display.print("EDITING MAX VALUE");
  }
  setDrawColor();
  
  drawRect(0, 16, 128, 48, false);
  
  display.setCursor(2, 24);
  display.print("MIN: ");
  display.println(randomMin);
  display.setCursor(2, 34);
  display.print("MAX: ");
  display.println(randomMax);
  
  display.setCursor(2, 46);
  display.print("RESULT: ");
  if (randomResult != 0) {
    display.println(randomResult);
  }
  
  display.setCursor(2, 55);
  display.print("SEL:GEN  LEFT:SWITCH  UP/DN:CHANGE");
  
  display.display();
}
 
void runRandomGen() {
  if (millis() - lastButtonPress > DEBOUNCE_DELAY) {
    if (digitalRead(BTN_UP) == LOW) {
      if (randomEditMode == 0 && randomMin < 999) randomMin++;
      if (randomEditMode == 1 && randomMax < 999) randomMax++;
      if (randomMin > randomMax && randomEditMode == 0) randomMin = randomMax;
      lastButtonPress = millis();
    }
    if (digitalRead(BTN_DOWN) == LOW) {
      if (randomEditMode == 0 && randomMin > 1) randomMin--;
      if (randomEditMode == 1 && randomMax > 1) randomMax--;
      if (randomMin > randomMax && randomEditMode == 1) randomMax = randomMin;
      lastButtonPress = millis();
    }
    if (digitalRead(BTN_LEFT) == LOW) {
      randomEditMode = !randomEditMode;
      lastButtonPress = millis();
    }
    if (digitalRead(BTN_SELECT) == LOW) {
      if (randomMin <= randomMax) {
        randomResult = random(randomMin, randomMax + 1);
      }
      lastButtonPress = millis();
    }
    if (digitalRead(BTN_BACK) == LOW) {
      saveRandomRange();
      changeState(APPS_MENU);
      lastButtonPress = millis();
    }
  }
  
  drawRandomUI();
}

// ========== CALCULATOR ==========
void evaluateExpression() {
  String expr = calcExpression;
  if (expr.length() == 0) {
    calcResult = "0";
    return;
  }
  
  int len = expr.length();
  float numbers[10];
  char operators[10];
  int numCount = 0;
  int opCount = 0;
  
  String numStr = "";
  for (int i = 0; i < len; i++) {
    char c = expr[i];
    if ((c >= '0' && c <= '9') || c == '.') {
      numStr += c;
    } else {
      if (numStr.length() > 0) {
        numbers[numCount++] = numStr.toFloat();
        numStr = "";
      }
      operators[opCount++] = c;
    }
  }
  if (numStr.length() > 0) {
    numbers[numCount++] = numStr.toFloat();
  }
  
  float result = numbers[0];
  for (int i = 0; i < opCount; i++) {
    if (operators[i] == '+') result += numbers[i+1];
    else if (operators[i] == '-') result -= numbers[i+1];
    else if (operators[i] == '*') result *= numbers[i+1];
    else if (operators[i] == '/') {
      if (numbers[i+1] != 0) result /= numbers[i+1];
      else {
        calcResult = "ERROR";
        return;
      }
    }
  }
  
  if (result == (int)result) {
    calcResult = String((int)result);
  } else {
    calcResult = String(result, 2);
  }
}

void runCalculator() {
  if (millis() - lastButtonPress > DEBOUNCE_DELAY) {
    // Сохраняем предыдущую позицию курсора для анимации
    if (calcCursorX != calcLastCursorX || calcCursorY != calcLastCursorY) {
      calcLastCursorX = calcCursorX;
      calcLastCursorY = calcCursorY;
    }
    
    if (digitalRead(BTN_UP) == LOW) {
      if (calcCursorY > 0) calcCursorY--;
      lastButtonPress = millis();
    }
    if (digitalRead(BTN_DOWN) == LOW) {
      if (calcCursorY < 3) calcCursorY++;
      lastButtonPress = millis();
    }
    if (digitalRead(BTN_LEFT) == LOW) {
      if (calcCursorX > 0) calcCursorX--;
      lastButtonPress = millis();
    }
    if (digitalRead(BTN_RIGHT) == LOW) {
      if (calcCursorX < 3) calcCursorX++;
      lastButtonPress = millis();
    }
    
    if (digitalRead(BTN_SELECT) == HIGH && !calcButtonPressed) {
      char selected = calcButtons[calcCursorY][calcCursorX];
      
      // Анимация нажатия
      calcButtonPressAnim = calcCursorY * 4 + calcCursorX;
      calcButtonPressTime = millis();
      
      if (selected == 'C') {
        calcExpression = "";
        calcResult = "";
        calcResultDisplay = false;
      }
      else if (selected == '=') {
        evaluateExpression();
        calcResultDisplay = true;
      }
      else {
        if (calcResultDisplay) {
          calcExpression = "";
          calcResultDisplay = false;
        }
        calcExpression += selected;
        calcResult = "";
      }
      calcButtonPressed = true;
      lastButtonPress = millis();
    }
    
    if (digitalRead(BTN_SELECT) == LOW) {
      calcButtonPressed = false;
    }
  }
  
  display.clearDisplay();
  if (whiteTheme) display.fillScreen(WHITE);
  
  drawRect(0, 0, 128, 24, true);
  setInverseColor();
  display.setTextSize(1);
  display.setCursor(2, 2);
  display.print("CALCULATOR");
  
  if (calcResultDisplay && calcResult.length() > 0) {
    display.setCursor(2, 12);
    display.print("RES: ");
    display.print(calcResult);
  } else {
    display.setCursor(2, 12);
    display.print(calcExpression);
  }
  setDrawColor();
  
  drawRect(0, 24, 128, 40, false);
  
  // Анимация нажатия кнопки (мигание/увеличение)
  bool showPressAnim = (calcButtonPressAnim != -1 && 
                        millis() - calcButtonPressTime < 150);
  
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      int btnX = 8 + x * 28;
      int btnY = 28 + y * 8;
      int btnIndex = y * 4 + x;
      
      // Проверяем, нажата ли эта кнопка
      bool isPressed = (showPressAnim && btnIndex == calcButtonPressAnim);
      
      if (calcCursorX == x && calcCursorY == y) {
        if (isPressed) {
          // Эффект нажатия: инвертированные цвета и сдвиг
          drawRect(btnX-2, btnY-2, 24, 10, true);
          setInverseColor();
          display.setCursor(btnX+1, btnY+1);
          display.print(calcButtons[y][x]);
          setDrawColor();
        } else {
          // Обычный выделенный курсор
          drawRect(btnX-2, btnY-2, 24, 10, true);
          setInverseColor();
          display.setCursor(btnX, btnY);
          display.print(calcButtons[y][x]);
          setDrawColor();
        }
      } else {
        if (isPressed) {
          // Нажатая но не выделенная кнопка (инверсия)
          drawRect(btnX-1, btnY-1, 22, 8, true);
          setInverseColor();
          display.setCursor(btnX, btnY);
          display.print(calcButtons[y][x]);
          setDrawColor();
        } else {
          // Обычная кнопка
          display.setCursor(btnX, btnY);
          display.print(calcButtons[y][x]);
        }
      }
    }
  }
  
  // Сброс анимации после завершения
  if (showPressAnim == false && calcButtonPressAnim != -1) {
    calcButtonPressAnim = -1;
  }
  
  display.display();
}

// ========== FLASHLIGHT ==========
void runFlashlight() {
  display.clearDisplay();
  display.fillScreen(WHITE);
  display.display();
}

// ========== STOPWATCH ==========
void runStopwatch() {
  if (millis() - lastButtonPress > DEBOUNCE_DELAY) {
    if (digitalRead(BTN_SELECT) == LOW) {
      if (!stopwatchRunning) {
        stopwatchStartTime = millis() - stopwatchElapsed;
        stopwatchRunning = true;
      } else {
        stopwatchElapsed = millis() - stopwatchStartTime;
        stopwatchRunning = false;
      }
      lastButtonPress = millis();
    }
    if (digitalRead(BTN_UP) == LOW && !stopwatchRunning) {
      stopwatchElapsed = 0;
      lastButtonPress = millis();
    }
  }
  
  if (stopwatchRunning) {
    stopwatchElapsed = millis() - stopwatchStartTime;
  }
  
  unsigned long totalSec = stopwatchElapsed / 1000;
  unsigned long minutes = totalSec / 60;
  unsigned long seconds = totalSec % 60;
  unsigned long millisLeft = stopwatchElapsed % 1000;
  
  display.clearDisplay();
  if (whiteTheme) display.fillScreen(WHITE);
  display.setTextSize(2);
  display.setCursor(8, 8);
  setDrawColor();
  char timeStr[20];
  sprintf(timeStr, "%02lu:%02lu.%03lu", minutes, seconds, millisLeft);
  display.println(timeStr);
  display.setTextSize(1);
  display.setCursor(10, 42);
  display.println("SEL:START/STOP");
  display.setCursor(10, 52);
  display.println("UP:RESET");
  display.display();
}

// ========== ABOUT ==========
void runAbout() {
  display.clearDisplay();
  if (whiteTheme) display.fillScreen(WHITE);
  display.setTextSize(2);
  display.setCursor(12, 5);
  setDrawColor();
  display.println("SNLGAMING");
  display.setTextSize(1);
  display.setCursor(2, 25);
  display.println("Created: 13.04.26");
  display.setCursor(2, 45);
  display.println("by Sonly251");
  display.setCursor(2, 35);
  display.println("snl frimware");
  display.setCursor(82, 40);
  display.println("(^._.^)");
  display.display();
}

// ========== SHOOTER GAME ==========
void initShooterGame() {
  sPlayerX = 60;
  sPlayerHealth = 100;
  sScore = 0;
  sKillCount = 0;
  sGameActive = true;
  sEnemySpeed = 110;
  sEnemySpawnDelay = 2000;
  sEnemyCount = 0;
  
  for (int i = 0; i < S_MAX_BULLETS; i++) {
    sBulletActive[i] = false;
  }
  for (int i = 0; i < S_MAX_ENEMIES; i++) {
    sEnemyActive[i] = false;
  }
  
  sLastEnemySpawn = millis();
  sLastShot = millis();
  sLastEnemyMove = millis();
}

void spawnEnemy() {
  if (sEnemyCount >= S_MAX_ENEMIES) return;
  
  int slot = -1;
  for (int i = 0; i < S_MAX_ENEMIES; i++) {
    if (!sEnemyActive[i]) {
      slot = i;
      break;
    }
  }
  if (slot == -1) return;
  
  sEnemyX[slot] = random(10, 118);
  sEnemyY[slot] = -10;
  sEnemyHealth[slot] = 20;
  sEnemyActive[slot] = true;
  sEnemyCount++;
}

void shootShooter() {
  if (millis() - sLastShot < sShootDelay) return;
  
  int slot = -1;
  for (int i = 0; i < S_MAX_BULLETS; i++) {
    if (!sBulletActive[i]) {
      slot = i;
      break;
    }
  }
  if (slot == -1) return;
  
  sBulletX[slot] = sPlayerX + 5;
  sBulletY[slot] = 50;
  sBulletActive[slot] = true;
  sLastShot = millis();
}

void updateShooterGame() {
  if (!sGameActive) return;
  
  if (millis() - sLastEnemyMove > sEnemySpeed) {
    sLastEnemyMove = millis();
    
    for (int i = 0; i < S_MAX_ENEMIES; i++) {
      if (!sEnemyActive[i]) continue;
      
      sEnemyY[i] += 3;
      
      if (sEnemyY[i] > 64) {
        sEnemyActive[i] = false;
        sEnemyCount--;
      }
      
      if (abs(sEnemyX[i] - sPlayerX) < 12 && abs(sEnemyY[i] - 52) < 12) {
        sPlayerHealth -= 20;
        sEnemyActive[i] = false;
        sEnemyCount--;
        
        if (sPlayerHealth <= 0) {
          gameOver("SHOOTER", sScore, EE_SHOOTER, highShooter);
          return;
        }
      }
    }
  }
  
  for (int i = 0; i < S_MAX_BULLETS; i++) {
    if (!sBulletActive[i]) continue;
    
    sBulletY[i] -= 5;
    
    if (sBulletY[i] < 0) {
      sBulletActive[i] = false;
      continue;
    }
    
    for (int j = 0; j < S_MAX_ENEMIES; j++) {
      if (!sEnemyActive[j]) continue;
      
      if (abs(sBulletX[i] - sEnemyX[j]) < 10 && abs(sBulletY[i] - sEnemyY[j]) < 10) {
        sEnemyHealth[j] -= 10;
        sBulletActive[i] = false;
        
        if (sEnemyHealth[j] <= 0) {
          sEnemyActive[j] = false;
          sEnemyCount--;
          sScore += 10;
          sKillCount++;
          
          if (sKillCount % 5 == 0 && sEnemySpeed > 60) {
            sEnemySpeed -= 2;
          }
          if (sKillCount % 8 == 0 && sEnemySpawnDelay > 800) {
            sEnemySpawnDelay -= 100;
          }
        }
        break;
      }
    }
  }
  
  if (millis() - sLastEnemySpawn > sEnemySpawnDelay && sEnemyCount < S_MAX_ENEMIES) {
    sLastEnemySpawn = millis();
    spawnEnemy();
  }
}

void drawShooterGame() {
  drawTriangle(sPlayerX, 58, sPlayerX + 10, 58, sPlayerX + 5, 48, true);
  drawTriangle(sPlayerX + 2, 54, sPlayerX + 8, 54, sPlayerX + 5, 50, false);
  
  for (int i = 0; i < S_MAX_BULLETS; i++) {
    if (sBulletActive[i]) {
      drawRect(sBulletX[i] - 1, sBulletY[i] - 1, 2, 4, true);
    }
  }
  
  for (int i = 0; i < S_MAX_ENEMIES; i++) {
    if (sEnemyActive[i]) {
      drawRect(sEnemyX[i] - 6, sEnemyY[i] - 4, 12, 8, true);
      drawRect(sEnemyX[i] - 3, sEnemyY[i] - 2, 6, 4, false);
      drawCircle(sEnemyX[i] - 3, sEnemyY[i] - 1, 1, true);
      drawCircle(sEnemyX[i] + 3, sEnemyY[i] - 1, 1, true);
    }
  }
  
  drawRect(0, 0, 128, 10, true);
  setInverseColor();
  display.setTextSize(1);
  display.setCursor(2, 1);
  display.print("SCORE:");
  display.print(sScore);
  display.setCursor(56, 1);
  display.print("HP:");
  display.print(sPlayerHealth);
  display.setCursor(95, 1);
  display.print("K:");
  display.print(sKillCount);
  setDrawColor();
}

void runShooter() {
  if (!sGameActive) {
    initShooterGame();
    return;
  }
  
  if (millis() - lastButtonPress > DEBOUNCE_DELAY) {
    if (digitalRead(BTN_LEFT) == LOW) {
      if (sPlayerX > 10) sPlayerX -= 8;
      lastButtonPress = millis();
    }
    if (digitalRead(BTN_RIGHT) == LOW) {
      if (sPlayerX < 108) sPlayerX += 8;
      lastButtonPress = millis();
    }
    if (digitalRead(BTN_SELECT) == LOW) {
      shootShooter();
      lastButtonPress = millis();
    }
    if (digitalRead(BTN_BACK) == LOW) {
      sGameActive = false;
      gameOver("SHOOTER", sScore, EE_SHOOTER, highShooter);
      return;
    }
  }
  
  updateShooterGame();
  
  if (sPlayerHealth <= 0) {
    sGameActive = false;
    gameOver("SHOOTER", sScore, EE_SHOOTER, highShooter);
    return;
  }
  
  display.clearDisplay();
  if (whiteTheme) display.fillScreen(WHITE);
  
  drawShooterGame();
  
  display.display();
}

// ========== TANKS GAME ==========
void initTanks() {
  tankX = 60;
  tankY = 50;
  tankDirX = 0;
  tankDirY = -1;
  tankHealth = 10;
  tankScore = 0;
  tankGameActive = true;
  tankBulletActive = false;
  waveTransition = false;
  currentWave = 1;
  enemiesInWave = 3;
  enemiesRemaining = 3;
  
  for (int i = 0; i < 10; i++) {
    enemyHealth[i] = 0;
    enemyTanks[i][0] = 0;
    enemyTanks[i][1] = 0;
    enemyBullets[i][2] = 0;
  }
  
  for (int i = 0; i < enemiesInWave; i++) {
    enemyTanks[i][0] = 20 + (i % 3) * 30;
    enemyTanks[i][1] = 15 + (i / 3) * 20;
    enemyTanks[i][2] = random(-1, 2);
    enemyTanks[i][3] = random(-1, 2);
    while (enemyTanks[i][2] == 0 && enemyTanks[i][3] == 0) {
      enemyTanks[i][2] = random(-1, 2);
      enemyTanks[i][3] = random(-1, 2);
    }
    enemyHealth[i] = 1;
    enemyBullets[i][2] = 0;
  }
  
  lastTankMove = millis();
  lastTankShot = millis();
}

void showWaveNotification() {
  display.clearDisplay();
  if (whiteTheme) display.fillScreen(WHITE);
  display.setTextSize(2);
  display.setCursor(30, 20);
  setDrawColor();
  display.print("WAVE");
  display.setCursor(80,20 );
  display.print(currentWave);
  display.setTextSize(1);
  display.setCursor(40, 40);
  display.print("GET READY!");
  display.display();
  delay(1500);
}

void spawnNextWave() {
  waveTransition = false;
  currentWave++;
  
  if (currentWave == 2) {
    enemiesInWave = 5;
  } else {
    enemiesInWave = 7;
  }
  
  enemiesRemaining = enemiesInWave;
  
  for (int i = 0; i < 10; i++) {
    enemyHealth[i] = 0;
    enemyTanks[i][0] = 0;
    enemyTanks[i][1] = 0;
    enemyBullets[i][2] = 0;
  }
  
  for (int i = 0; i < enemiesInWave; i++) {
    enemyTanks[i][0] = 15 + (i % 4) * 25;
    enemyTanks[i][1] = 15 + (i / 4) * 20;
    enemyTanks[i][2] = random(-1, 2);
    enemyTanks[i][3] = random(-1, 2);
    while (enemyTanks[i][2] == 0 && enemyTanks[i][3] == 0) {
      enemyTanks[i][2] = random(-1, 2);
      enemyTanks[i][3] = random(-1, 2);
    }
    enemyHealth[i] = 1;
    enemyBullets[i][2] = 0;
  }
  
  showWaveNotification();
}

void drawTank(int x, int y, int dirX, int dirY) {
  drawRect(x-5, y-5, 10, 10, true);
  drawRect(x-3, y-3, 6, 6, false);
  
  if (dirX == 0 && dirY == -1) {
    drawRect(x-1, y-7, 2, 3, true);
  } else if (dirX == 0 && dirY == 1) {
    drawRect(x-1, y+4, 2, 3, true);
  } else if (dirX == -1 && dirY == 0) {
    drawRect(x-7, y-1, 3, 2, true);
  } else if (dirX == 1 && dirY == 0) {
    drawRect(x+4, y-1, 3, 2, true);
  }
}

void runTanks() {
  if (!tankGameActive) return;
  
  if (!waveTransition && enemiesRemaining <= 0) {
    if (currentWave < 3) {
      waveTransition = true;
      spawnNextWave();
    } else {
      gameOver("TANKS", tankScore, EE_TANKS, highTanks);
      return;
    }
  }
  
  if (millis() - lastButtonPress > DEBOUNCE_DELAY) {
    if (digitalRead(BTN_SELECT) == LOW) {
      if (!tankBulletActive && millis() - lastTankShot > 400) {
        tankBulletX = tankX;
        tankBulletY = tankY;
        tankBulletDirX = tankDirX;
        tankBulletDirY = tankDirY;
        tankBulletActive = true;
        lastTankShot = millis();
      }
      lastButtonPress = millis();
    }
  }
  
  int newX = tankX;
  int newY = tankY;
  
  if (digitalRead(BTN_UP) == LOW) { newY -= 2; tankDirX = 0; tankDirY = -1; }
  if (digitalRead(BTN_DOWN) == LOW) { newY += 2; tankDirX = 0; tankDirY = 1; }
  if (digitalRead(BTN_LEFT) == LOW) { newX -= 2; tankDirX = -1; tankDirY = 0; }
  if (digitalRead(BTN_RIGHT) == LOW) { newX += 2; tankDirX = 1; tankDirY = 0; }
  
  if (newX > 15 && newX < 113 && newY > 15 && newY < 55) {
    tankX = newX;
    tankY = newY;
  }
  
  if (tankBulletActive) {
    tankBulletX += tankBulletDirX * 4;
    tankBulletY += tankBulletDirY * 4;
    if (tankBulletX < 0 || tankBulletX > 128 || tankBulletY < 12 || tankBulletY > 64) {
      tankBulletActive = false;
    }
    for (int i = 0; i < enemiesInWave; i++) {
      if (enemyHealth[i] > 0 && abs(tankBulletX - enemyTanks[i][0]) < 8 && abs(tankBulletY - enemyTanks[i][1]) < 8) {
        enemyHealth[i] = 0;
        tankBulletActive = false;
        tankScore++;
        enemiesRemaining--;
      }
    }
  }
  
  if (millis() - lastTankMove > tankSpeed) {
    lastTankMove = millis();
    for (int i = 0; i < enemiesInWave; i++) {
      if (enemyHealth[i] > 0) {
        int newEnemyX = enemyTanks[i][0] + enemyTanks[i][2] * 1;
        int newEnemyY = enemyTanks[i][1] + enemyTanks[i][3] * 1;
        if (newEnemyX > 15 && newEnemyX < 113 && newEnemyY > 15 && newEnemyY < 55) {
          enemyTanks[i][0] = newEnemyX;
          enemyTanks[i][1] = newEnemyY;
        } else {
          enemyTanks[i][2] = random(-1, 2);
          enemyTanks[i][3] = random(-1, 2);
          while (enemyTanks[i][2] == 0 && enemyTanks[i][3] == 0) {
            enemyTanks[i][2] = random(-1, 2);
            enemyTanks[i][3] = random(-1, 2);
          }
        }
        
        if (random(0, 100) < 2 && !enemyBullets[i][2]) {
          enemyBullets[i][0] = enemyTanks[i][0];
          enemyBullets[i][1] = enemyTanks[i][1];
          enemyBullets[i][2] = 1;
        }
      }
    }
  }
  
  for (int i = 0; i < enemiesInWave; i++) {
    if (enemyBullets[i][2]) {
      int dirX = (tankX - enemyBullets[i][0] > 0) ? 1 : -1;
      int dirY = (tankY - enemyBullets[i][1] > 0) ? 1 : -1;
      if (abs(tankX - enemyBullets[i][0]) > abs(tankY - enemyBullets[i][1])) {
        enemyBullets[i][0] += dirX * 3;
      } else {
        enemyBullets[i][1] += dirY * 3;
      }
      if (enemyBullets[i][0] < 0 || enemyBullets[i][0] > 128 || enemyBullets[i][1] < 12 || enemyBullets[i][1] > 64) {
        enemyBullets[i][2] = 0;
      }
      if (abs(enemyBullets[i][0] - tankX) < 8 && abs(enemyBullets[i][1] - tankY) < 8) {
        tankHealth--;
        enemyBullets[i][2] = 0;
        if (tankHealth <= 0) {
          gameOver("TANKS", tankScore, EE_TANKS, highTanks);
          return;
        }
      }
    }
  }
  
  display.clearDisplay();
  if (whiteTheme) display.fillScreen(WHITE);
  
  drawRect(0, 0, 128, 10, true);
  setInverseColor();
  display.setCursor(2, 1);
  display.print("SCORE:");
  display.print(tankScore);
  display.setCursor(50, 1);
  display.print("WAVE:");
  display.print(currentWave);
  display.setCursor(90, 1);
  display.print("HP:");
  display.print(tankHealth);
  setDrawColor();
  
  drawTank(tankX, tankY, tankDirX, tankDirY);
  
  for (int i = 0; i < enemiesInWave; i++) {
    if (enemyHealth[i] > 0) {
      drawTank(enemyTanks[i][0], enemyTanks[i][1], enemyTanks[i][2], enemyTanks[i][3]);
    }
  }
  
  if (tankBulletActive) {
    drawRect(tankBulletX-1, tankBulletY-1, 2, 2, true);
  }
  
  for (int i = 0; i < enemiesInWave; i++) {
    if (enemyBullets[i][2]) {
      drawRect(enemyBullets[i][0]-1, enemyBullets[i][1]-1, 2, 2, true);
    }
  }
  
  display.display();
}

// ========== SNAKE GAME ==========
void initSnake() {
  snakeLength = 4;
  snakeX[0]=64; snakeY[0]=32; snakeX[1]=60; snakeY[1]=32;
  snakeX[2]=56; snakeY[2]=32; snakeX[3]=52; snakeY[3]=32;
  dirX=1; dirY=0;
  snakeScore=0;
  snakeSpeed=120;
  gameActive=true;
  spawnFood();
}

void spawnFood() { foodX=random(1,31)*4; foodY=random(3,15)*4; }

void runSnake() {
  if(!gameActive) return;
  
  if (millis() - lastButtonPress > DEBOUNCE_DELAY) {
    if (digitalRead(BTN_SELECT) == LOW) { gameOver("SNAKE", snakeScore, EE_SNAKE, highSnake); return; }
    if (digitalRead(BTN_UP) == LOW && dirY == 0) { dirX = 0; dirY = -1; lastButtonPress = millis(); }
    else if (digitalRead(BTN_DOWN) == LOW && dirY == 0) { dirX = 0; dirY = 1; lastButtonPress = millis(); }
    else if (digitalRead(BTN_LEFT) == LOW && dirX == 0) { dirX = -1; dirY = 0; lastButtonPress = millis(); }
    else if (digitalRead(BTN_RIGHT) == LOW && dirX == 0) { dirX = 1; dirY = 0; lastButtonPress = millis(); }
  }
  
  if(millis() - lastMoveTime > snakeSpeed){
    lastMoveTime = millis();
    for(int i = snakeLength-1; i > 0; i--){ snakeX[i] = snakeX[i-1]; snakeY[i] = snakeY[i-1]; }
    snakeX[0] += dirX*4; snakeY[0] += dirY*4;
    
    if(snakeX[0] < 0 || snakeX[0] >= 128 || snakeY[0] < 12 || snakeY[0] >= 64) { gameOver("SNAKE", snakeScore, EE_SNAKE, highSnake); return; }
    for(int i = 1; i < snakeLength; i++) if(snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) { gameOver("SNAKE", snakeScore, EE_SNAKE, highSnake); return; }
    if(abs(snakeX[0]-foodX) < 4 && abs(snakeY[0]-foodY) < 4){
      if(snakeLength < MAX_SNAKE){ snakeLength++; snakeScore++; spawnFood(); if(snakeSpeed > 50) snakeSpeed -= 3; }
    }
  }
  
  display.clearDisplay();
  if (whiteTheme) display.fillScreen(WHITE);
  drawRect(0, 0, 128, 10, true);
  setInverseColor();
  display.setCursor(2,1); display.print("SCORE:"); display.print(snakeScore);
  setDrawColor();
  for(int i = 0; i < snakeLength; i++){ 
    if(i == 0) drawRect(snakeX[i], snakeY[i], 4, 4, true); 
    else drawRect(snakeX[i], snakeY[i], 3, 3, false); 
  }
  if(millis() % 400 < 200) drawCircle(foodX+2, foodY+2, 2, true);
  display.display();
}

// ========== PONG ==========
void initPong() {
  paddleY=24; ballX=64; ballY=32; ballDX=2; ballDY=2; pongScore=0; ballSpeed=30; gameActive=true; pongLastUpdate=millis();
}

void runPong() {
  if(!gameActive) return;
  
  if (millis() - lastButtonPress > DEBOUNCE_DELAY) {
    if (digitalRead(BTN_SELECT) == LOW) { gameOver("PONG", pongScore, EE_PONG, highPong); return; }
    lastButtonPress = millis();
  }
  
  if(digitalRead(BTN_UP) == LOW) paddleY -= 4;
  if(digitalRead(BTN_DOWN) == LOW) paddleY += 4;
  paddleY = constrain(paddleY, 12, 48);
  
  if(millis() - pongLastUpdate > ballSpeed){
    pongLastUpdate = millis();
    ballX += ballDX; ballY += ballDY;
    if(ballY <= 12 || ballY >= 62){ ballDY = -ballDY; }
    if(ballX <= 10 && ballX >= 6 && ballY >= paddleY && ballY <= paddleY+16){ ballDX = abs(ballDX); pongScore++; if(ballSpeed > 15) ballSpeed -= 1; }
    if(ballX >= 126){ ballDX = -abs(ballDX); }
    if(ballX <= 0) { gameOver("PONG", pongScore, EE_PONG, highPong); return; }
  }
  
  display.clearDisplay();
  if (whiteTheme) display.fillScreen(WHITE);
  drawRect(0, 0, 128, 10, true);
  setInverseColor();
  display.setCursor(2,1); display.print("SCORE:"); display.print(pongScore);
  setDrawColor();
  drawRect(4, paddleY, 4, 16, true);
  drawCircle(ballX, ballY, 3, true);
  for(int i = 12; i < 64; i += 6) drawLine(64, i, 64, i+3);
  display.display();
}

// ========== CAR RACER ==========
void initCarGame() {
  playerLane=1; for(int i=0;i<3;i++){ obstacleLane[i]=random(0,3); obstacleY[i]=-20-(i*25); }
  carScore=0; carSpeed=80; carGameActive=true; lastCarMove=millis();
}

void runCarGame() {
  if(!carGameActive) return;
  
  if(millis() - lastButtonPress > DEBOUNCE_DELAY){
    if(digitalRead(BTN_LEFT) == LOW && playerLane > 0){ playerLane--; lastButtonPress = millis(); }
    if(digitalRead(BTN_RIGHT) == LOW && playerLane < 2){ playerLane++; lastButtonPress = millis(); }
    if(digitalRead(BTN_SELECT) == LOW){ gameOver("RACER", carScore, EE_CAR, highCar); return; }
  }
  
  if(millis() - lastCarMove > carSpeed){
    lastCarMove = millis();
    for(int i = 0; i < 3; i++){ 
      obstacleY[i] += 4; 
      if(obstacleY[i] > 64){ obstacleY[i] = -10; obstacleLane[i] = random(0,3); carScore++; if(carSpeed > 30) carSpeed -= 2; }
      if(obstacleY[i] > 40 && obstacleY[i] < 60 && obstacleLane[i] == playerLane){ gameOver("RACER", carScore, EE_CAR, highCar); return; }
    }
  }
  
  display.clearDisplay();
  if (whiteTheme) display.fillScreen(WHITE);
  drawRect(0, 0, 128, 10, true);
  setInverseColor();
  display.setCursor(2,1); display.print("SCORE:"); display.print(carScore);
  setDrawColor();
  
  int laneX[3] = {32, 64, 96};
  for(int i = 0; i < 3; i++){ drawLine(laneX[i]-16, 12, laneX[i]-16, 64); }
  drawLine(laneX[2]+16, 12, laneX[2]+16, 64);
  
  drawRect(laneX[playerLane]-6, 48, 12, 14, true);
  drawRect(laneX[playerLane]-6, 48, 12, 14, false);
  drawRect(laneX[playerLane]-4, 50, 2, 3, true);
  drawRect(laneX[playerLane]+2, 50, 2, 3, true);
  
  for(int i = 0; i < 3; i++){ 
    if(obstacleY[i] > 0 && obstacleY[i] < 64){ 
      drawRect(laneX[obstacleLane[i]]-6, obstacleY[i], 12, 14, true);
      drawRect(laneX[obstacleLane[i]]-4, obstacleY[i]+2, 2, 3, true);
      drawRect(laneX[obstacleLane[i]]+2, obstacleY[i]+2, 2, 3, true);
    }
  }
  display.display();
}

// ========== DINO GAME ==========
void initDinoGame() {
  dinoY=44; dinoVelocity=0; isJumping=false; obstacleX=128; dinoScore=0; dinoSpeed=50; dinoCrouch=false; gameActive=true; lastDinoUpdate=millis();
}

void runDinoGame() {
  if(!gameActive) return;
  
  if(millis() - lastButtonPress > DEBOUNCE_DELAY){
    if(digitalRead(BTN_UP) == LOW && !isJumping && !dinoCrouch){ isJumping=true; dinoVelocity=-8; lastButtonPress=millis(); }
    if(digitalRead(BTN_SELECT) == LOW){ gameOver("DINO", dinoScore, EE_DINO, highDino); return; }
  }
  
  if(digitalRead(BTN_DOWN) == LOW && !isJumping && dinoY >= 44) dinoCrouch = true;
  else dinoCrouch = false;
  
  if(millis() - lastDinoUpdate > dinoSpeed){
    lastDinoUpdate = millis();
    if(isJumping){ dinoY += dinoVelocity; dinoVelocity += 1; if(dinoY >= 44){ dinoY = 44; isJumping = false; dinoVelocity = 0; } }
    obstacleX -= 5;
    if(obstacleX < -10){ obstacleX = 128; dinoScore++; if(dinoSpeed > 25) dinoSpeed -= 1; }
    
    int dinoHeight = dinoCrouch ? 6 : 12;
    int dinoTop = dinoCrouch ? 50 : dinoY;
    if(obstacleX+8 > 12 && obstacleX < 12+(dinoCrouch?12:14) && dinoTop+dinoHeight > 48){ gameOver("DINO", dinoScore, EE_DINO, highDino); return; }
  }
  
  display.clearDisplay();
  if (whiteTheme) display.fillScreen(WHITE);
  drawRect(0, 0, 128, 10, true);
  setInverseColor();
  display.setCursor(2,1); display.print("SCORE:"); display.print(dinoScore);
  setDrawColor();
  drawLine(0, 56, 128, 56);
  if(dinoCrouch){ drawRect(12, 50, 14, 6, true); drawRect(10, 52, 2, 2, true); } 
  else { drawRect(14, dinoY, 8, 12, true); drawRect(12, dinoY+4, 2, 4, true); drawRect(22, dinoY+2, 2, 2, true); }
  drawRect(obstacleX, 48, 8, 8, true);
  display.display();
}

// ========== FLAPPY BIRD ==========
void initFlappy() {
  flappyBirdY = 32;
  flappyBirdVelocity = 0;
  flappyPipeX = 128;
  flappyPipeTopHeight = random(10, 40);
  flappyScore = 0;
  flappySpeed = 60;
  flappyActive = true;
  lastFlappyUpdate = millis();
}

void runFlappy() {
  if (!flappyActive) return;
  
  if (millis() - lastButtonPress > DEBOUNCE_DELAY) {
    if (digitalRead(BTN_SELECT) == LOW || digitalRead(BTN_UP) == LOW) {
      flappyBirdVelocity = -4;
      lastButtonPress = millis();
    }
  }
  
  if (millis() - lastFlappyUpdate > flappySpeed) {
    lastFlappyUpdate = millis();
    
    flappyBirdVelocity += 1;
    flappyBirdY += flappyBirdVelocity;
    
    if (flappyBirdY < 10 || flappyBirdY > 56) {
      gameOver("FLAPPY", flappyScore, EE_FLAPPY, highFlappy);
      return;
    }
    
    flappyPipeX -= 3;
    if (flappyPipeX < -10) {
      flappyPipeX = 128;
      flappyPipeTopHeight = random(10, 40);
      flappyScore++;
      if (flappySpeed > 35) flappySpeed -= 1;
    }
    
    if (flappyPipeX < 25 && flappyPipeX + 10 > 15) {
      if (flappyBirdY < flappyPipeTopHeight || flappyBirdY > flappyPipeTopHeight + flappyPipeGap) {
        gameOver("FLAPPY", flappyScore, EE_FLAPPY, highFlappy);
        return;
      }
    }
  }
  
  display.clearDisplay();
  if (whiteTheme) display.fillScreen(WHITE);
  
  drawRect(flappyPipeX, 0, 10, flappyPipeTopHeight, true);
  drawRect(flappyPipeX, flappyPipeTopHeight + flappyPipeGap, 10, 64 - (flappyPipeTopHeight + flappyPipeGap), true);
  
  drawCircle(20, flappyBirdY, 4, true);
  drawCircle(18, flappyBirdY-2, 1, true);
  
  drawRect(0, 0, 128, 10, true);
  setInverseColor();
  display.setCursor(2, 1);
  display.print("SCORE:"); display.print(flappyScore);
  setDrawColor();
  
  display.display();
}

// ========== ARKANOID ==========
void initArkanoid() {
  arkanoidPaddleX = 54;
  arkanoidBallX = 64;
  arkanoidBallY = 50;
  arkanoidBallDX = 1;
  arkanoidBallDY = -1;
  arkanoidScore = 0;
  arkanoidLives = 3;
  arkanoidActive = true;
  lastArkanoidUpdate = millis();
  
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 12; j++) {
      arkanoidBricks[i][j] = 1;
    }
  }
}

void runArkanoid() {
  if (!arkanoidActive) return;
  
  if (millis() - lastButtonPress > DEBOUNCE_DELAY) {
    if (digitalRead(BTN_LEFT) == LOW) {
      arkanoidPaddleX -= 12;
      if (arkanoidPaddleX < 0) arkanoidPaddleX = 0;
      lastButtonPress = millis();
    }
    if (digitalRead(BTN_RIGHT) == LOW) {
      arkanoidPaddleX += 12;
      if (arkanoidPaddleX > 116) arkanoidPaddleX = 116;
      lastButtonPress = millis();
    }
    if (digitalRead(BTN_SELECT) == LOW) {
      gameOver("ARKANOID", arkanoidScore, EE_ARKANOID, highArkanoid);
      return;
    }
  }
  
  if (millis() - lastArkanoidUpdate > arkanoidSpeed) {
    lastArkanoidUpdate = millis();
    
    arkanoidBallX += arkanoidBallDX;
    arkanoidBallY += arkanoidBallDY;
    
    if (arkanoidBallX <= 2 || arkanoidBallX >= 125) {
      arkanoidBallDX = -arkanoidBallDX;
    }
    if (arkanoidBallY <= 10) {
      arkanoidBallDY = -arkanoidBallDY;
    }
    
    if (arkanoidBallY >= 58 && arkanoidBallY <= 62 && 
        arkanoidBallX >= arkanoidPaddleX && arkanoidBallX <= arkanoidPaddleX + 12) {
      arkanoidBallDY = -arkanoidBallDY;
      int hitPos = arkanoidBallX - arkanoidPaddleX;
      if (hitPos < 4) arkanoidBallDX = -1;
      else if (hitPos > 8) arkanoidBallDX = 1;
      else arkanoidBallDX = (hitPos - 6) * 0.3;
      if (arkanoidBallDX == 0) arkanoidBallDX = 1;
    }
    
    if (arkanoidBallY > 64) {
      arkanoidLives--;
      if (arkanoidLives <= 0) {
        gameOver("ARKANOID", arkanoidScore, EE_ARKANOID, highArkanoid);
        return;
      }
      arkanoidBallX = 64;
      arkanoidBallY = 50;
      arkanoidBallDX = 1;
      arkanoidBallDY = -1;
      delay(500);
    }
    
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 12; j++) {
        if (arkanoidBricks[i][j]) {
          int brickX = j * 10 + 4;
          int brickY = i * 5 + 14;
          if (arkanoidBallX + 3 >= brickX && arkanoidBallX - 3 <= brickX + 8 &&
              arkanoidBallY + 3 >= brickY && arkanoidBallY - 3 <= brickY + 3) {
            arkanoidBricks[i][j] = 0;
            arkanoidBallDY = -arkanoidBallDY;
            arkanoidScore++;
          }
        }
      }
    }
    
    bool allBricksGone = true;
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 12; j++) {
        if (arkanoidBricks[i][j]) allBricksGone = false;
      }
    }
    if (allBricksGone) {
      gameOver("ARKANOID", arkanoidScore, EE_ARKANOID, highArkanoid);
      return;
    }
  }
  
  display.clearDisplay();
  if (whiteTheme) display.fillScreen(WHITE);
  drawRect(0, 0, 128, 10, true);
  setInverseColor();
  display.setCursor(2, 1);
  display.print("SCORE:"); display.print(arkanoidScore);
  display.setCursor(80, 1);
  display.print("LIVES:"); display.print(arkanoidLives);
  setDrawColor();
  
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 12; j++) {
      if (arkanoidBricks[i][j]) {
        drawRect(j * 10 + 4, i * 5 + 14, 8, 3, true);
      }
    }
  }
  
  drawRect(arkanoidPaddleX, 60, 12, 3, true);
  drawCircle(arkanoidBallX, arkanoidBallY, 3, true);
  
  display.display();
}