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
  if (IsPause)
  {
    DrawSnake();
  }
  else
  {
    snake.x[0] = SCREEN_WIDTH / 2;
    snake.y[0] = SCREEN_HEIGHT / 2;
    snake.length = init_snake_length;
    snake.speed = init_snake_speed; // 初始速度
    current_speed = snake.speed;
    input = 'd'; // 初始方向：右
  }
  if (wall)
  {
    tft.fillRect(0, 0, SCREEN_WIDTH, 10, COLOR_WALL);                  // 顶部墙
    tft.fillRect(0, SCREEN_HEIGHT - 10, SCREEN_WIDTH, 10, COLOR_WALL); // 底部墙
    tft.fillRect(0, 0, 10, SCREEN_HEIGHT, COLOR_WALL);                 // 左侧墙
    tft.fillRect(SCREEN_WIDTH - 10, 0, 10, SCREEN_HEIGHT, COLOR_WALL); // 右侧墙
  }
}

// 绘制墙壁
void Wall()
{
  if (wall)
  {
    tft.fillRect(0, 0, SCREEN_WIDTH, 10, COLOR_WALL);                  // 顶部墙
    tft.fillRect(0, SCREEN_HEIGHT - 10, SCREEN_WIDTH, 10, COLOR_WALL); // 底部墙
    tft.fillRect(0, 0, 10, SCREEN_HEIGHT, COLOR_WALL);                 // 左侧墙
    tft.fillRect(SCREEN_WIDTH - 10, 0, 10, SCREEN_HEIGHT, COLOR_WALL); // 右侧墙
  }
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

  bool valid = false;
  while (!valid)
  {
    food.x = random(1, 30);
    food.y = random(1, 22);
    valid = true;
    for (int i = 0; i < snake.length; i++)
    {
      if (food.x * 10 == snake.x[i] && food.y * 10 == snake.y[i])
      {
        valid = false;
        break;
      }
    }
  }

  tft.fillRect(food.x * 10, food.y * 10, 10, 10, COLOR_FOOD); // 绘制食物
}

void DrawSnake()
{
  // 绘制蛇头
  if (Is_egg)
    tft.fillRect(snake.x[0], snake.y[0], 10, 10, colors[7]);
  else
    tft.fillRect(snake.x[0], snake.y[0], 10, 10, COLOR_SNAKE_HEAD);
  // 绘制蛇头表情
  tft.fillCircle(snake.x[0] + 3, snake.y[0] + 3, 1, COLOR_SNAKE_EYE); // 左眼
  tft.fillCircle(snake.x[0] + 7, snake.y[0] + 3, 1, COLOR_SNAKE_EYE); // 右眼

  // 微笑嘴巴
  tft.fillRect(snake.x[0] + 2, snake.y[0] + 6, 2, 2, COLOR_SNAKE_EYE); // 左嘴角
  tft.fillRect(snake.x[0] + 6, snake.y[0] + 6, 2, 2, COLOR_SNAKE_EYE); // 右嘴角
  tft.fillRect(snake.x[0] + 4, snake.y[0] + 7, 2, 2, COLOR_SNAKE_EYE);

  // 绘制蛇身
  for (int i = 1; i < snake.length - 1; i++)
  {
    if (Is_egg)
    {
      int colorIndex = (i % 8); // 8种颜色循环使用
      tft.fillRect(snake.x[i], snake.y[i], 10, 10, colors[colorIndex]);
    }
    else
    {
      tft.fillRect(snake.x[i], snake.y[i], 10, 10, COLOR_SNAKE_BODY);
    }
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
  savedScore = loadHighScore();
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
    tft.print((200 - snake.speed) / 2);
  }
}

void Egg()
{
  COLOR_BG = ~COLOR_BG;
  COLOR_SNAKE_HEAD = ~COLOR_SNAKE_HEAD;
  COLOR_SNAKE_BODY = ~COLOR_SNAKE_BODY;
  COLOR_SNAKE_EYE = ~COLOR_SNAKE_EYE;
  COLOR_WALL = ~COLOR_WALL;
  COLOR_GRID = ~COLOR_GRID;
  COLOR_FOOD = ~COLOR_FOOD;
  COLOR_TEXT = ~COLOR_TEXT;
  COLOR_BUTTON = ~COLOR_BUTTON;
  refreshPage();
  tft.setCursor(10, SCREEN_HEIGHT - 20);
  tft.setTextSize(1);
  tft.println("HELLO?");
  if (wall)
  {
    tft.fillRect(0, 0, SCREEN_WIDTH, 10, COLOR_WALL);                  // 顶部墙
    tft.fillRect(0, SCREEN_HEIGHT - 10, SCREEN_WIDTH, 10, COLOR_WALL); // 底部墙
    tft.fillRect(0, 0, 10, SCREEN_HEIGHT, COLOR_WALL);                 // 左侧墙
    tft.fillRect(SCREEN_WIDTH - 10, 0, 10, SCREEN_HEIGHT, COLOR_WALL); // 右侧墙
  }
  tft.fillRect(food.x * 10, food.y * 10, 10, 10, COLOR_FOOD); // 绘制食物
  Is_egg = !Is_egg;
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
    if (wall)
    {
      if (debug && snake.x[0] < 130 && snake.y[0] > 230 - 10) // 彩蛋
      {
        snake.x[0] = SCREEN_WIDTH / 2;
        snake.y[0] = SCREEN_HEIGHT / 2;
        Egg();
      }
      else
      {
        // 撞墙，游戏结束
        life = 0; // 设置死亡标志
        tft.setCursor(SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2);
        tft.setTextSize(3);
        // tft.setTextColor(TFT_RED);
        tft.println("GAME OVER!TAT");
        if (score > savedScore)
        {
          saveHighScore(score);
          savedScore = loadHighScore();
        }
        delay(300);
        return; // 结束函数，游戏结束
      }
    }
    else
    {
      if (debug && snake.x[0] < 130 && snake.y[0] > 230 - 10 && !ineggwall) // 彩蛋
      {
        Egg();
        ineggwall = true;
      }
      Wall_direction();
    }
  }
  else // 没撞墙
  {
    ineggwall = false;
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

  // // 检查是否吃到食物：区域判定
  // if (snake.x[0] == food.x * 10 && snake.y[0] == food.y * 10)
  // {
  //   snake.length++; // 增加蛇的长度
  //   score += 10;
  //   // 每吃到一个食物，分数增加
  //   if (score > savedScore)
  //     saveHighScore(score);
  //   snake.speed -= 2;
  //   tft.fillRect(food.x * 10, food.y * 10, 10, 10, COLOR_BG); // 清除食物
  //   FOOD();                                                   // 重新生成食物
  // }

  // 检查是否吃到食物：区域判定
  if (snake.x[0] == food.x * 10 && snake.y[0] == food.y * 10)
  {
    snake.length += init_snake_get_food_length; // 增加蛇的长度
    score += init_snake_get_food_score;
    // 每吃到一个食物，分数增加
    if (score > savedScore)
    {
      saveHighScore(score);
      savedScore = loadHighScore();
    }
    if (game_diffcult && snake.speed < 100 && snake.speed > 30) // 加速
      snake.speed -= init_snake_get_food_speed - 3;
    else if (snake.speed > 50)
      snake.speed -= init_snake_get_food_speed;
    current_speed = snake.speed;
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

// 无墙时位置变化
void Wall_direction()
{
  if (snake.x[0] < 0 || snake.x[0] >= 320)
  {
    snake.x[0] < 0 ? snake.x[0] = 320 : snake.x[0] = 0;
  }
  else if (snake.y[0] < 0 || snake.y[0] >= 240)
  {
    snake.y[0] < 0 ? snake.y[0] = 240 : snake.y[0] = 0;
  }
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
  // Wall();  // 绘制墙壁
  // FOOD();  // 生成食物
}

void Theme()
{
  switch (colorchoice)
  {
  case 0:
    //
    COLOR_BG = 0x0000;         // 黑色背景
    COLOR_SNAKE_HEAD = 0x07E0; // 明亮绿蛇头
    COLOR_SNAKE_BODY = 0x03E0; // 深绿蛇身
    COLOR_SNAKE_EYE = 0xFFFF;  // 白色蛇眼
    COLOR_WALL = 0x4208;       // 深灰墙壁
    COLOR_GRID = 0x2104;       // 暗灰网格线
    COLOR_FOOD = 0xF800;       // 鲜红食物
    COLOR_TEXT = 0x07E0;       // 绿色文字
    COLOR_BUTTON = 0x07E0;     // 绿色按钮

    break;
  case 1:
    // 暖秋
    COLOR_BG = 0xFFFF;         // 纯白背景
    COLOR_SNAKE_HEAD = 0xFBCE; // 樱粉蛇头
    COLOR_SNAKE_BODY = 0xFBAE; // 淡粉蛇身
    COLOR_SNAKE_EYE = 0x0010;  // 深蓝蛇眼
    COLOR_WALL = 0xC638;       // 米灰墙壁
    COLOR_GRID = 0xDEFB;       // 淡米网格
    COLOR_FOOD = 0xF800;       // 红色草莓食物
    COLOR_TEXT = 0x8410;       // 深灰文字
    COLOR_BUTTON = 0x7D7C;     // 淡紫按钮

    break;
  case 2:
    // 夜间
    COLOR_BG = 0x001F;         // 深蓝夜背景
    COLOR_SNAKE_HEAD = 0xF81F; // 荧光粉蛇头
    COLOR_SNAKE_BODY = 0x07FF; // 青蓝蛇身
    COLOR_SNAKE_EYE = 0xFFFF;  // 白色蛇眼
    COLOR_WALL = 0x8410;       // 暗墙壁
    COLOR_GRID = 0x3186;       // 荧光蓝网格
    COLOR_FOOD = 0xFFE0;       // 荧黄食物
    COLOR_TEXT = 0xFFFF;       // 白文字
    COLOR_BUTTON = 0xFBE0;     // 橙粉按钮

    break;
  case 3:

    // 秋日柔和配色方案（变量赋值版）
    COLOR_BG = 0xF7D0;         // 柔米白
    COLOR_SNAKE_HEAD = 0xD580; // 枫红橙
    COLOR_SNAKE_BODY = 0xB3A0; // 枯玫瑰棕
                               //  COLOR_SNAKE_BODY_TAIL= 0x8C61;  // 深橘褐
    COLOR_SNAKE_EYE = 0xFFFF;  // 纯白

    COLOR_WALL = 0x7320; // 树干褐
    COLOR_GRID = 0xC638; // 枯叶土黄

    COLOR_FOOD = 0xFA60; // 柿子红
    COLOR_TEXT = 0x0000; // 黑色文字

    COLOR_BUTTON = 0xDD22; // 枫叶红
                           //  COLOR_BUTTON_HOVER   = 0xAA00;  // 深栗红
                           //  COLOR_ACCENT         = 0xFFE0;  // 柔金色

    break;
  case 4:
    COLOR_BG = 0xC618;         // 浅灰背景
    COLOR_SNAKE_HEAD = 0x0000; // 黑蛇头
    COLOR_SNAKE_BODY = 0x8410; // 深灰蛇身
    COLOR_SNAKE_EYE = 0xFFFF;  // 白蛇眼
    COLOR_WALL = 0x8410;       // 深灰墙壁
    COLOR_GRID = 0xA514;       // 中灰网格
    COLOR_FOOD = 0xFFFF;       // 白色食物（点缀）
    COLOR_TEXT = 0x0000;       // 黑文字
    COLOR_BUTTON = 0x8410;     // 深灰按钮

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
  tft.setCursor(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 10);
  if (IsPause && life)
  {
    tft.setTextSize(1);
    tft.print("You have a game");
  }
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

  DrawTextWithBorder(0, 0 * 40, "Line: ");
  direction_line ? tft.print("YES!") : tft.print("NO!");

  DrawTextWithBorder(0, 1 * 40, "Debug: ");
  debug ? tft.print("YES!") : tft.print("NO!");

  DrawTextWithBorder(0, 2 * 40, "SnakeColor: ");
  switch (colorchoice)
  {
  case 0:
    tft.print("Matrix");
    break;
  case 1:
    tft.print("Sakura❀");
    break;
  case 2:
    tft.print("Neon Night");
    break;
  case 3:
    tft.print("Autumn");
    break;
  case 4:
    tft.print("Monochrome");
    break;
  }
  tft.fillRect(0, 118, 320, 20, COLOR_BG);
  if (!reset)
  {
    DrawTextWithBorder(0, 3 * 40, "ResetScore ");
    tft.print(savedScore);
  }
  else
  {
    DrawTextWithBorder(0, 3 * 40, "Are You Sure To Reset? ");
  }

  DrawTextWithBorder(0, 4 * 40, "WALL? ");
  wall ? tft.print("YES!") : tft.print("NO!");

  DrawTextWithBorder(0, 5 * 40, "Diffcult: ");
  game_diffcult ? tft.print("Very!") : tft.print("Easy!");
}

void handleTouch()
{
  p = ts.getPoint();
  int mapX = map(p.x, TOUCH_MIN_X, TOUCH_MAX_X, 0, tft.width());
  int mapY = map(p.y, TOUCH_MIN_Y, TOUCH_MAX_Y, 0, tft.height());

  switch (page)
  {
  case 0:
    handleStartTouch(mapX, mapY);
    break;
  case 1:
    handleSettingTouch(mapX, mapY);
    break;
  case 2:
    handleGameTouch(mapX, mapY);
    break;
  }
}

void handleStartTouch(int x, int y) // 开始界面
{
  if (x < 40 && y < 20)
  {
    page = 1;
    refreshPage();
    delay(100);
  }
  else
  {
    if (IsPause && life)
    { // 游戏时返回暂停游戏
      tft.fillScreen(COLOR_BG);
      page = 2;
      Wall();
      tft.fillRect(food.x * 10, food.y * 10, 10, 10, COLOR_FOOD); // 绘制食物FD
      IsPause = false;
      snake.speed = 200;
    }
    else
    {
      page = 2;
      StartGame();
    }
  }
}

void handleSettingTouch(int x, int y) // 设置界面
{
  if (x > 300 && y > 220)
  {
    page = 0;
    reset = false;
    refreshPage();
  }
  else if (y <= 40)
  {
    direction_line = !direction_line;
    refreshPage();
  }
  else if (y <= 40 * 2)
  {
    debug = !debug;
    refreshPage();
  }
  else if (y <= 40 * 3)
  {
    colorchoice = (colorchoice + 1) % 5;
    Theme();
    preferences.begin("color", false);
    preferences.putInt("color", colorchoice);
    preferences.end();
    refreshPage();
  }
  else if (y <= 40 * 4)
  {
    if (!reset)
    {
      reset = true;
      ShowSet();
    }
    else
    {
      saveHighScore(0);
      savedScore = loadHighScore();
      reset = false;
      ShowSet();
    }
  }
  else if (y <= 40 * 5)
  {
    wall = !wall;
    refreshPage();
  }
  else if (y <= 40 * 6)
  {
    game_diffcult = !game_diffcult;
    if (game_diffcult)
    {
      init_snake_speed = 150;
      init_snake_length = 3;
      init_snake_get_food_score = 20;
      init_snake_get_food_speed = 6;
      init_snake_get_food_length = 2;
    }
    else
    {
      init_snake_speed = 180;
      init_snake_length = 3;
      init_snake_get_food_score = 10;
      init_snake_get_food_speed = 2;
      init_snake_get_food_length = 1;
    }
    refreshPage();
  }
  delay(200);
}

void handleGameTouch(int x, int y) // 游戏界面
{
  if (x > 310 && y < 10)
  {
    page = 0;
    // score = 0;
    IsPause = true;
    snake.speed = 100000; // 速度无限大
    refreshPage();
    delay(300);
  }
  else
  {
    Touch();
  }
}

void handleGameOver(bool touched)
{
  ShowEnd();
  if (!touched)
    return;

  p = ts.getPoint();
  int x = map(p.x, TOUCH_MIN_X, TOUCH_MAX_X, 0, tft.width());
  int y = map(p.y, TOUCH_MIN_Y, TOUCH_MAX_Y, 0, tft.height());

  if (x > 310 && y < 10)
  {
    page = 0;
    score = 0;
    refreshPage();
    delay(300);
  }
  else
  {
    StartGame(); // 重启游戏
    delay(300);
  }
}

void refreshPage()
{
  tft.fillScreen(COLOR_BG);
  switch (page)
  {
  case 0:
    ShowStart();
    break;
  case 1:
    ShowSet();
    break;
  }
}

void StartGame()
{
  tft.fillScreen(COLOR_BG);
  Start();
  Wall();
  FOOD();
  gamestart = 1;
  life = 1;
  score = 0;
  lastDirection = 'd';
  snake.speed = 200;
}

// 0开始
// 1设置
// 2游戏

void loop()
{
  bool touched = ts.tirqTouched() && ts.touched();
  unsigned long currentTime = millis();

  if (touched)
    handleTouch(); // 独立处理触摸事件

  switch (page)
  {
  case 0:
    ShowStart();
    break;
  case 1:
    ShowSet();
    break;
  case 2:
    if (life)
    {
      if (currentTime - lastUpdateTime > snake.speed)
      {
        lastUpdateTime = currentTime;
        JIANPAN();
      }
    }
    else
      handleGameOver(touched);
    break;
  }
}
