#include <SPI.h>
#include <XPT2046_Touchscreen.h>
#include <TFT_eSPI.h>
#include <Preferences.h>

// 定义触摸屏引脚
#define XPT2046_IRQ 36
#define XPT2046_MOSI 32
#define XPT2046_MISO 39
#define XPT2046_CLK 25
#define XPT2046_CS 33

// 校准参数
#define TOUCH_MIN_X 200
#define TOUCH_MAX_X 3800
#define TOUCH_MIN_Y 300
#define TOUCH_MAX_Y 3800
// 屏幕宽高
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240


// 定义触摸屏引脚
#define XPT2046_IRQ 36
#define XPT2046_MOSI 32
#define XPT2046_MISO 39
#define XPT2046_CLK 25
#define XPT2046_CS 33

// 校准参数
#define TOUCH_MIN_X 200
#define TOUCH_MAX_X 3800
#define TOUCH_MIN_Y 300
#define TOUCH_MAX_Y 3800
// 屏幕宽高
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

// #define TFT_BLACK 0xdcdc  // 浅灰的反色
// 创建 SPI 和触摸屏对象
SPIClass mySpi = SPIClass(VSPI);
XPT2046_Touchscreen ts(XPT2046_CS, XPT2046_IRQ);
TFT_eSPI tft = TFT_eSPI();
Preferences preferences;
unsigned long lastUpdateTime = 0;

// 全局变量
int savedScore;
const int BOOT_PIN = 0; //boot键
// int randomnum;
#define MAX 100  // 定义蛇的最大长度
int input = 'd'; // 初始方向：右
int page = 0;
int buttonPressed = 0;
int set_chiose = 0;
int init_snake_speed = 180;//初始速度
int init_snake_length = 3;//初始长度
int init_snake_get_food_score = 10;//获取食物得分
int init_snake_get_food_speed = 2;//获取食物加速度
int init_snake_get_food_length = 1;//获取食物加长度
char lastDirection = 'd'; // 上一次的方向，初始为右
int score = 0;            // 分数变量
int current_speed=0;//记录暂停前的速度
// bool change_set = false;
bool gamestart = false;
bool setting = false;

//设置中的
bool direction_line = true;
bool game_diffcult = false;
bool debug = false;
bool reset = false;   // resetscore的确认状态

bool IsPause = false; // 暂停游戏
bool big_food = false;
bool wall = true;
bool Is_egg = false;
bool ineggwall = false;
byte life = 1; // 生命状态
byte colorchoice = 0;
TS_Point p;
 uint16_t colors[8] = {
    0xF800, // 红色
    0xFC00, // 橙色
    0xFFE0, // 黄色
    0x07E0, // 绿色
    0x07FF, // 青色
    0x001F, // 蓝色
    0x401F, // 靛色
    0xF81F  // 紫色
  };
// 颜色
// uint16_t COLOR_BG;         // 背景
// uint16_t COLOR_SNAKE_HEAD; // 蛇头
// uint16_t COLOR_SNAKE_BODY; // 蛇身
// uint16_t COLOR_SNAKE_EYE;  // 蛇眼
// uint16_t COLOR_WALL;       // 墙壁
// uint16_t COLOR_GRID;       // 网格线
// uint16_t COLOR_FOOD;       // 食物
// uint16_t COLOR_TEXT;       // 文字
// uint16_t COLOR_BUTTON;     // 按钮

uint16_t COLOR_BG, COLOR_SNAKE_HEAD, COLOR_SNAKE_BODY, COLOR_SNAKE_EYE;
uint16_t COLOR_WALL, COLOR_GRID, COLOR_FOOD, COLOR_TEXT, COLOR_BUTTON;

void Start();
void FOOD();
void DrawSnake();
void DrawScore();
// void UpdataSnake();
void setup();
void Touch();
void DrawTextWithBorder(int x, int y, const char *text);
void ShowSet();
void ShowStart();
void ShowEnd();
void Theme();
void refreshPage();
void StartGame();
// void readInput();
void Wall_direction();
void Egg();
void handleStartTouch(int x, int y); // 开始界面
void handleSettingTouch(int x, int y); // 设置界面
void handleGameTouch(int x, int y); // 游戏界面

// void Is_boot();

// 蛇的状态
struct Snake
{
    int x[MAX];
    int y[MAX];
    int length;
    int speed;
} snake;

// 食物的位置
struct Food
{
    int x;
    int y;
} food;


// void UpdataSnake();
// void readInput();
// void Wall_direction();
// void Egg();
// void Is_boot();