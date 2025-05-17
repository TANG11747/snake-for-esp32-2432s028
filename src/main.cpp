#include <SPI.h>
#include <XPT2046_Touchscreen.h>
#include <TFT_eSPI.h>
#include <Preferences.h>
#include <main.h>

void saveHighScore(int score)
{
  preferences.begin("game", false);
  preferences.putInt("score", score);
  preferences.end();
}

int loadHighScore()
{
  preferences.begin("game", true);
  int score = preferences.getInt("score", 0);
  preferences.end();
  return score;
}

// 初始化游戏
void Start()
{
  snake.x[0] = SCREEN_WIDTH / 2;
  snake.y[0] = SCREEN_HEIGHT / 2;
  snake.length = 3;
  snake.speed = 200; // 初始速度
  input = 'd';       // 初始方向：右
}

// 绘制墙壁
void wall()
{
  tft.fillRect(0, 0, SCREEN_WIDTH, 10, COLOR_WALL);                  // 顶部墙
  tft.fillRect(0, SCREEN_HEIGHT - 10, SCREEN_WIDTH, 10, COLOR_WALL); // 底部墙
  tft.fillRect(0, 0, 10, SCREEN_HEIGHT, COLOR_WALL);                 // 左侧墙
  tft.fillRect(SCREEN_WIDTH - 10, 0, 10, SCREEN_HEIGHT, COLOR_WALL); // 右侧墙
  if (direction_line)
  {
    tft.drawLine(106, 10, 106, 230, COLOR_GRID);  // 左竖
    tft.drawLine(212, 10, 212, 230, COLOR_GRID);  // 右竖
    tft.drawLine(106, 120, 212, 120, COLOR_GRID); // 中横
  }
  tft.fillRect(310, 0, 10, 10, COLOR_BUTTON);
  tft.setCursor(313, 2);
  // tft.setTextColor(COLOR_TEXT);
  tft.setTextSize(1);
  tft.print("X");
}

void FOOD()
{
  food.x = random(1, 30);
  food.y = random(1, 22);
  for (int i = 0; i <= snake.length; i++)
  {
    while (food.x * 10 == snake.x[i] && food.y * 10 == snake.y[i])
    {
      food.x = random(1, 30);
      food.y = random(1, 22);
      break;
    }
  }
  tft.fillRect(food.x * 10, food.y * 10, 10, 10, COLOR_FOOD); // 绘制食物
}

// 绘制蛇
void DrawSnake()
{
  if (direction_line)
  {
    tft.drawLine(106, 10, 106, 230, COLOR_GRID);  // 左竖
    tft.drawLine(212, 10, 212, 230, COLOR_GRID);  // 右竖
    tft.drawLine(106, 120, 212, 120, COLOR_GRID); // 中横
  }

  tft.fillRect(snake.x[0], snake.y[0], 10, 10, COLOR_SNAKE_HEAD);      // 绘制蛇头
  tft.fillRect(snake.x[0] + 2, snake.y[0] + 2, 2, 2, COLOR_SNAKE_EYE); // 绘制蛇头左眼
  tft.fillRect(snake.x[0] + 6, snake.y[0] + 2, 2, 2, COLOR_SNAKE_EYE); // 绘制蛇头右眼
  // tft.fillRect(snake.x[0]+2, snake.y[0]+2, snake.x[0]+4, snake.y[0]+4, TFT_BLACK); // 绘制蛇头
  for (int i = 1; i < snake.length; i++)
  {
    tft.fillRect(snake.x[i], snake.y[i], 10, 10, COLOR_SNAKE_BODY); // 绘制蛇
  }
}

// 绘制分数
void DrawScore()
{
  tft.fillRect(0, 0, 150, 10, COLOR_BG);
  tft.setCursor(0, 0);
  tft.setTextSize(1);
  // tft.setTextColor(COLOR_TEXT);
  tft.print("Score: ");
  tft.print(score);
  tft.print("  highest: ");
  tft.print(savedScore);
  // Debug
  if (debug)
  {
    tft.fillRect(0, 230, 130, 10, COLOR_BG);
    tft.setCursor(0, 232);
    tft.print(food.x);
    tft.print("  ");
    tft.print(food.y);
    tft.print("  ");
    tft.print(snake.x[0]);
    tft.print("  ");
    tft.print(snake.y[0]);
    tft.print("  ");
    tft.print(snake.speed);
  }
}

// 更新蛇的位置
void JIANPAN()
{
  for (int i = 1; i < snake.length; i++)
  {
    if (snake.x[0] == snake.x[i] && snake.y[0] == snake.y[i])
    {
      life = 0;
      tft.setCursor(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2);
      tft.setTextSize(3);
      // tft.setTextColor(TFT_RED);
      tft.println("GAME OVER!TAT");
      if (score > savedScore)
        saveHighScore(score);
      return; // 游戏结束
    }
  }
  // 检查是否撞墙
  if (snake.x[0] <= 0 || snake.x[0] >= SCREEN_WIDTH - 10 || snake.y[0] <= 0 || snake.y[0] >= SCREEN_HEIGHT - 10)
  {
    // 撞墙，游戏结束
    life = 0; // 设置死亡标志
    tft.setCursor(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2);
    tft.setTextSize(3);
    // tft.setTextColor(TFT_RED);
    tft.println("GAME OVER!TAT");
    if (score > savedScore)
      saveHighScore(score);
    return; // 结束函数，游戏结束
  }

  // 防止蛇不能回头：如果新方向与旧方向相反，则不更新
  if ((input == 'w' && lastDirection != 's') ||
      (input == 's' && lastDirection != 'w') ||
      (input == 'a' && lastDirection != 'd') ||
      (input == 'd' && lastDirection != 'a'))
  {
    lastDirection = input; // 更新当前方向
  }

  // 更新蛇的身体位置
  for (int i = snake.length; i > 0; i--)
  {
    snake.x[i] = snake.x[i - 1];
    snake.y[i] = snake.y[i - 1];
  }

  // 根据输入更新蛇头位置
  switch (lastDirection)
  {
  case 'w':
    snake.y[0] -= 10; // 向上移动
    break;
  case 's':
    snake.y[0] += 10; // 向下移动
    break;
  case 'a':
    snake.x[0] -= 10; // 向左移动
    break;
  case 'd':
    snake.x[0] += 10; // 向右移动
    break;
  }

  // 检查是否吃到食物：区域判定
  if (snake.x[0] == food.x * 10 && snake.y[0] == food.y * 10)
  {
    snake.length++; // 增加蛇的长度
    score += 10;
    // 每吃到一个食物，分数增加
    if (score > savedScore)
      saveHighScore(score);
    snake.speed -= 2;
    tft.fillRect(food.x * 10, food.y * 10, 10, 10, COLOR_BG); // 清除食物
    FOOD();                                                   // 重新生成食物
  }

  // 绘制蛇
  DrawSnake();

  // 清除蛇的尾部
  tft.fillRect(snake.x[snake.length], snake.y[snake.length], 10, 10, COLOR_BG);
  // 更新分数
  DrawScore();
}

// 初始化
void setup()
{
  Serial.begin(115200);
  tft.setSwapBytes(true);
  savedScore = loadHighScore();

  tft.writecommand(ILI9341_MADCTL);
  tft.writedata(ILI9341_MADCTL_MX | ILI9341_MADCTL_MY | ILI9341_MADCTL_BGR); // 改为 BGR

  // 初始化触摸屏和显示屏
  mySpi.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  ts.begin(mySpi);
  ts.setRotation(1);

  preferences.begin("color", false);
  colorchoice = preferences.getInt("color", 0);
  preferences.end();

  tft.init();
  tft.setRotation(1);
  Theme();
  tft.fillScreen(COLOR_BG);
  tft.setTextColor(COLOR_TEXT);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  // Start(); // 初始化蛇
  // wall();  // 绘制墙壁
  // FOOD();  // 生成食物
}

void Theme()
{
  switch (colorchoice)
  {
  case 0:
    //
    COLOR_BG = 0x18C3;         // 深夜蓝背景
    COLOR_SNAKE_HEAD = 0x7BFF; // 星辰紫蛇头
    COLOR_SNAKE_BODY = 0x4A7F; // 银河蓝蛇身
    COLOR_SNAKE_EYE = 0xFFFF;  // 亮白蛇眼
    COLOR_WALL = 0x2945;       // 金属灰墙壁
    COLOR_GRID = 0x18C3;       // 暗蓝网格
    COLOR_FOOD = 0xFB20;       // 彗星红食物
    COLOR_TEXT = 0xC618;       // 浅灰文字
    COLOR_BUTTON = 0x5BFF;     // 电光蓝按钮

    break;
  case 1:
    // 暖秋
    COLOR_BG = 0xFEA0;         // 浅杏色背景
    COLOR_SNAKE_HEAD = 0xD020; // 红棕蛇头
    COLOR_SNAKE_BODY = 0xA260; // 棕橙蛇身
    COLOR_SNAKE_EYE = 0x0000;  // 纯黑蛇眼
    COLOR_WALL = 0x738E;       // 灰褐墙壁
    COLOR_GRID = 0xB596;       // 米黄网格
    COLOR_FOOD = 0xF800;       // 经典红食物
    COLOR_TEXT = 0x4208;       // 暗棕文字
    COLOR_BUTTON = 0xFD20;     // 橙黄色按钮
    break;
  case 2:
    // 夜间
    COLOR_BG = 0xEFFD;         // 薄荷白背景
    COLOR_SNAKE_HEAD = 0x07E0; // 明亮绿蛇头
    COLOR_SNAKE_BODY = 0x05B0; // 柔和青蛇身
    COLOR_SNAKE_EYE = 0x001F;  // 深蓝蛇眼
    COLOR_WALL = 0xAD55;       // 浅灰墙壁
    COLOR_GRID = 0xC618;       // 米黄网格
    COLOR_FOOD = 0xFBE0;       // 淡粉食物
    COLOR_TEXT = 0x3186;       // 藏青文字
    COLOR_BUTTON = 0x07FF;     // 天青按钮

    break;
  case 3:
    COLOR_BG = 0x0000;         // 纯黑背景
    COLOR_SNAKE_HEAD = 0x4208; // 黑曜石蛇头
    COLOR_SNAKE_BODY = 0x3186; // 暗青蛇身
    COLOR_SNAKE_EYE = 0xFFFF;  // 白色蛇眼
    COLOR_WALL = 0x8410;       // 深灰墙壁
    COLOR_GRID = 0x2104;       // 暗铁色网格
    COLOR_FOOD = 0xF800;       // 红色食物
    COLOR_TEXT = 0xC618;       // 浅灰文字
    COLOR_BUTTON = 0x07E0;     // 亮绿按钮

    break;
  case 4:
    COLOR_BG = 0xFFFF;         // 雪白背景
    COLOR_SNAKE_HEAD = 0xF81F; // 荧光粉蛇头
    COLOR_SNAKE_BODY = 0xFD20; // 糖果橙蛇身
    COLOR_SNAKE_EYE = 0x0010;  // 深蓝蛇眼
    COLOR_WALL = 0xC618;       // 浅灰墙壁
    COLOR_GRID = 0xFFE0;       // 米黄网格
    COLOR_FOOD = 0xF800;       // 樱桃红食物
    COLOR_TEXT = 0x8410;       // 深棕文字
    COLOR_BUTTON = 0x07FF;     // 天蓝按钮

    break;
  }
}

void Touch() // 更改方向
{
  // TS_Point p = ts.getPoint();
  p = ts.getPoint();
  // 映射触摸点坐标到屏幕范围
  int mapX = map(p.x, TOUCH_MIN_X, TOUCH_MAX_X, 0, tft.width());
  int mapY = map(p.y, TOUCH_MIN_Y, TOUCH_MAX_Y, 0, tft.height());

  // if (!gamestart && mapX > SCREEN_WIDTH - 10 && mapY > SCREEN_HEIGHT - 10)
  // {
  //  preferences.putInt("score", 0);
  // }

  // 判断触摸区域来改变蛇的方向
  if (mapX > SCREEN_WIDTH / 3 && mapX < SCREEN_WIDTH / 3 * 2)
  {
    if (mapY < SCREEN_HEIGHT / 2)
    {
      input = 'w'; // 向上
    }
    else if (mapY > SCREEN_HEIGHT / 2)
    {
      input = 's'; // 向下
    }
  }
  else
  {
    if (mapX < SCREEN_WIDTH / 2)
    {
      input = 'a'; // 向左
    }
    else if (mapX > SCREEN_WIDTH / 2)
    {
      input = 'd'; // 向右
    }
  }
}

void DrawTextWithBorder(int x, int y, const char *text)
{
  // 设置文本大小
  tft.setTextSize(2);
  // 绘制四周8个方向的边
  tft.setTextColor(TFT_LIGHTGREY);
  tft.setCursor(x - 1, y);
  tft.print(text);
  tft.setCursor(x + 1, y);
  tft.print(text);
  tft.setCursor(x, y - 1);
  tft.print(text);
  tft.setCursor(x, y + 1);
  tft.print(text);

  tft.setCursor(x - 1, y - 1);
  tft.print(text);
  tft.setCursor(x + 1, y - 1);
  tft.print(text);
  tft.setCursor(x - 1, y + 1);
  tft.print(text);
  tft.setCursor(x + 1, y + 1);
  tft.print(text);

  // 最后绘制正文
  tft.setTextColor(COLOR_TEXT);
  tft.setCursor(x, y);
  tft.print(text);
}

void ShowStart()
{
  // 初始显示界面
  // tft.fillScreen(TFT_BLACK);
  tft.setTextSize(3);
  // tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2 - 20);
  tft.println("HELLO?qwq");
  tft.fillRect(0, 0, 40, 20, COLOR_BUTTON);
  tft.setTextSize(2);
  tft.setCursor(0, 0);
  tft.print("SET");
}

void ShowEnd()
{
  // 游戏结束后的等待
  tft.setTextSize(2);
  // tft.setTextColor(TFT_GREENYELLOW, TFT_BLACK);
  tft.setCursor(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2 + 40);
  tft.println("Restart!awa");
}

void ShowSet()
{
  tft.fillRect(300, 220, 20, 20, COLOR_BUTTON); // 关闭
  tft.setCursor(310, 230);
  tft.setTextSize(2);
  tft.print("X");
  setting = 1;

  DrawTextWithBorder(0, 0, "Line: ");
  direction_line ? tft.print("YES!") : tft.print("NO!");

  DrawTextWithBorder(0, 40, "Debug: ");
  debug ? tft.print("YES!") : tft.print("NO!");

  DrawTextWithBorder(0, 80, "SnakeColor: ");
  switch (colorchoice)
  {
  case 0:
    tft.print("modern");
    break;
  case 1:
    tft.print("Autumn");
    break;
  case 2:
    tft.print("Night");
    break;
  case 3:
    tft.print("awa");
    break;
  case 4:
    tft.print("qwq");
    break;
  }
}

void loop()
{
  bool touched = ts.tirqTouched() && ts.touched();
  unsigned long currentTime = millis();

  if (touched) // 触摸了
  {
    p = ts.getPoint();
    // 映射触摸点坐标到屏幕范围
    int mapX = map(p.x, TOUCH_MIN_X, TOUCH_MAX_X, 0, tft.width());
    int mapY = map(p.y, TOUCH_MIN_Y, TOUCH_MAX_Y, 0, tft.height());
    if (!gamestart)
    {
      if (setting)
      {
        if (mapX > 300 && mapY > 220)
        {
          setting = 0;
          tft.fillScreen(COLOR_BG);
        }
        else if (mapY < 20 && mapY > 0)
        {
          direction_line = !direction_line;
          tft.fillScreen(COLOR_BG);
          ShowSet();
        }
        else if (mapY > 40 && mapY < 60)
        {
          debug = !debug;
          tft.fillScreen(COLOR_BG);
          ShowSet();
        }
        else if (mapY > 60 && mapY < 90)
        {
          if (colorchoice == 4)
            colorchoice = 0;
          else
            colorchoice += 1;
          Theme();
          preferences.begin("color", false);
          int score = preferences.putInt("color", colorchoice);
          preferences.end();
          tft.fillScreen(COLOR_BG);
          ShowSet();
        }
        delay(200); // 轻微防抖
      }
      else
      {
        if (mapX < 40 && mapY < 20)
        {
          tft.fillScreen(COLOR_BG);
          ShowSet();
        }
        else
        {
          // 第一次触摸，开始游戏
          tft.fillScreen(COLOR_BG);
          Start();
          wall();
          FOOD();
          gamestart = 1;
          life = 1;
          // score = 0;
          // lastDirection = 'd';
          // snake.speed = 200;
          // delay(300); // 防止误触
        }
      }
    }
    else if (life) // 游戏中
    {
      if (mapX > 310 && mapY < 10) // 返回主页
      {
        tft.fillScreen(COLOR_BG);
        // Start();
        gamestart = 0;
        score = 0;
        ShowStart();
        delay(300); // 防抖
      }
      else
      {
        Touch();
      }
    }
  }

  if (gamestart && life) // 开始了也活着
  {
    if (currentTime - lastUpdateTime > snake.speed)
    {
      lastUpdateTime = currentTime;
      JIANPAN();
    }
  }
  else if (!gamestart) // 没开始
  {
    if (setting)
    {
      ShowSet();
    }
    else
    {
      ShowStart();
    }
  }
  else if (!life) // 死了
  {
    ShowEnd();

    if (touched)
    {
      p = ts.getPoint();
      // 映射触摸点坐标到屏幕范围
      int mapX = map(p.x, TOUCH_MIN_X, TOUCH_MAX_X, 0, tft.width());
      int mapY = map(p.y, TOUCH_MIN_Y, TOUCH_MAX_Y, 0, tft.height());
      if (mapX > 310 && mapY < 10) // 返回主页
      {
        tft.fillScreen(COLOR_BG);
        // Start();
        gamestart = 0;
        score = 0;
        ShowStart();
        delay(300); // 防抖
      }
      else
      {
        tft.fillScreen(COLOR_BG);
        Start();
        wall();
        FOOD();
        gamestart = 1;
        life = 1;
        score = 0;
        lastDirection = 'd';
        snake.speed = 200;
        delay(300);
      }
    }
  }
}
