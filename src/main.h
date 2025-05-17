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

// #define COLOR_BG      0xAEDC  // 薄荷蓝背景
// #define COLOR_SNAKE   0xF81F  // 粉红蛇身
// #define COLOR_HEAD    0xFD20  // 蜜桃橙蛇头
// #define COLOR_FOOD    0xFFE0  // 檬檬黄食物
// #define COLOR_WALL    0xB81F  // 软糖紫墙壁
// #define COLOR_SCORE   0xFFFF  // 奶油白得分文字

// #define TFT_BLACK 0xdcdc  // 浅灰的反色
// 创建 SPI 和触摸屏对象
SPIClass mySpi = SPIClass(VSPI);
XPT2046_Touchscreen ts(XPT2046_CS, XPT2046_IRQ);
TFT_eSPI tft = TFT_eSPI();
Preferences preferences;
unsigned long lastUpdateTime = 0;

// 全局变量
int savedScore;
// int randomnum;
#define MAX 100           // 定义蛇的最大长度
int input = 'd';          // 初始方向：右
int life = 1;             // 生命状态
char lastDirection = 'd'; // 上一次的方向，初始为右
int score = 0;            // 分数变量
bool gamestart = false;
bool setting = false;
bool direction_line = false;
bool debug = false;
byte colorchoice = 0;
TS_Point p;

// 颜色
uint16_t COLOR_BG;         // 背景
uint16_t COLOR_SNAKE_HEAD; // 蛇头
uint16_t COLOR_SNAKE_BODY; // 蛇身
uint16_t COLOR_SNAKE_EYE; // 蛇眼
uint16_t COLOR_WALL;       // 墙壁
uint16_t COLOR_GRID;       // 网格线
uint16_t COLOR_FOOD;       // 食物
uint16_t COLOR_TEXT;       // 文字
uint16_t COLOR_BUTTON;     //按钮

void Start();
void wall();
void FOOD();
void DrawSnake();
void DrawScore();
void JIANPAN();
void setup();
void Touch();
void DrawTextWithBorder(int x, int y, const char *text);
void ShowSet();
void ShowStart();
void ShowEnd();
void Theme();

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