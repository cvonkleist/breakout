#include "xmas.h"
#include "icons.h"
#include "levels.h"

const int width = 10;
const int height = 10;

const int strand_count = 2;
const int strand_pins[] = {5, 6};

const int strand_pin_lookup_table[] = {
  1, 0, 1, 0, 1, 0, 1, 0, 1, 0,
  0, 1, 0, 1, 0, 1, 0, 1, 0, 1,
  1, 0, 1, 0, 1, 0, 1, 0, 1, 0,
  0, 1, 0, 1, 0, 1, 0, 1, 0, 1,
  1, 0, 1, 0, 1, 0, 1, 0, 1, 0,
  0, 1, 0, 1, 0, 1, 0, 1, 0, 1,
  1, 0, 1, 0, 1, 0, 1, 0, 1, 0,
  0, 1, 0, 1, 0, 1, 0, 1, 0, 1,
  1, 0, 1, 0, 1, 0, 1, 0, 1, 0,
  0, 1, 0, 1, 0, 1, 0, 1, 0, 1,
};

const int bulb_lookup_table[] = {
  4, 5, 14, 15, 24, 25, 34, 35, 44, 45,
  4, 5, 14, 15, 24, 25, 34, 35, 44, 45,
  3, 6, 13, 16, 23, 26, 33, 36, 43, 46,
  3, 6, 13, 16, 23, 26, 33, 36, 43, 46,
  2, 7, 12, 17, 22, 27, 32, 37, 42, 47,
  2, 7, 12, 17, 22, 27, 32, 37, 42, 47,
  1, 8, 11, 18, 21, 28, 31, 38, 41, 48,
  1, 8, 11, 18, 21, 28, 31, 38, 41, 48,
  0, 9, 10, 19, 20, 29, 30, 39, 40, 49,
  0, 9, 10, 19, 20, 29, 30, 39, 40, 49,
};

#define INTENSITY 0x33

uint16_t *level_icons[] = {icon_1, icon_2, icon_3, icon_4, icon_5};

// this buffer stores the state of the screen
xmas_color_t screen[width][height];


int level;
int bricks_on;
int ball_x, ball_y;
int ball_speed_x, ball_speed_y;
int paddle_x;

void loop() {
  clear_ball();

  // update ball position
  ball_x += ball_speed_x;
  ball_y += ball_speed_y;

  // bounce paddle of walls and ceiling
  if(ball_x >= width || ball_x < 0) {
    ball_speed_x = -ball_speed_x;
    ball_x += 2 * ball_speed_x;
  }
  if(ball_y < 0) {
    ball_speed_y = -ball_speed_y;
    ball_y += 2 * ball_speed_y;
  }

  // see what the ball is touching now
  xmas_color_t pixel = screen[ball_x][ball_y];
  if(pixel != COLOR_BLACK) {
    if(pixel == COLOR_YELLOW) { // paddle
      ball_speed_y = -ball_speed_y;
      ball_y += ball_speed_y;
      if(ball_x <= paddle_x + 1) {
        ball_speed_x = -1;
      } else {
        ball_speed_x = 1;
      }
    } else { // brick
      set_screen_pixel(ball_x, ball_y, INTENSITY, COLOR_BLACK);
      bricks_on--;
      ball_speed_y = -ball_speed_y;
      ball_speed_x = -ball_speed_x;
    }
  }
  
  // did the ball make it off the bottom of the screen?
  if(ball_y >= height) {
    // demo cheat
    level = (level + 1) % LEVEL_COUNT;
    win_level();
    next_level();
//    if(1) {
//      game_over();
//    } else {
//      // for demo video. to advance convincingly to next level, let ball die.
//      int bricks_to_leave = random(3) + 1;
//      ball_y = 8;
//      ball_speed_y = -1;
//      for(int x = 0; x < 9; x++) {
//        for(int y = 0; y < line_count[level] && bricks_on > bricks_to_leave; y++) {
//          if(screen[x][y] != COLOR_BLACK && screen[x][y] != COLOR_YELLOW) {
//            set_screen_pixel(x, y, INTENSITY, COLOR_BLACK);
//            bricks_on--;
//          }
//        }
//      }
//    }
  }

  draw_ball();

  // update paddle only if it moved (to avoid flicker)
  int new_paddle_x = read_paddle();
  //new_paddle_x = max(0, min(ball_x - 1, 7));
  if(new_paddle_x != paddle_x) {
    clear_paddle();
    paddle_x = new_paddle_x;
  }
  draw_paddle();

  touch_up();

  if(bricks_on == 0) {
    level = (level + 1) % LEVEL_COUNT;
    win_level();
    next_level();
  }

  // game speed
  delay(100);
}

void win_level() {
  delay(2000);
  clear_screen();
  draw_icon(icon_win, 0, 0, INTENSITY, COLOR_BLUE);
  delay(2000);
}

// sets up the pegboard for the current level
void next_level() {
  clear_screen();

  paddle_x = 3;
  ball_x = 4; ball_y = 8;
  ball_speed_x = 1;
  ball_speed_y = -1;

  clear_screen();
  draw_level_icon();
  delay(2000);

  clear_screen();
  draw_bricks();
}

// this is hand-calibrated to my joystick
int read_paddle() {
  int v = analogRead(0);
  if(v < 930) v = 930;
  v = 9 - (v - 930.0) / (1024 - 930) * 9;
  return v;
}

void setup() {
  // wait for light processors to start up
  delay(3000);

  Serial.begin(9600);
  randomSeed(analogRead(0));

  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);

  level = 0;

  next_level();
}

void draw_level_icon() {
  draw_icon(level_icons[level], 0, 0, INTENSITY, COLOR_BLUE);
}

void game_over() {
  delay(2000);

  // fill screen with blood!!11
  for(int y = 9; y >= 0; y--) {
    for(int x = 0; x < 10; x++)
      set_pixel(x, y, INTENSITY, COLOR_RED);
    delay(y * 10);
  }
  delay(2000);

  clear();
  draw_icon(icon_sad_face, 0, 0, INTENSITY, COLOR_RED);
  delay(2000);

  setup();
}

// just like set_pixel, but also updates the screen buffer
inline void set_screen_pixel(uint8_t x, uint8_t y, uint8_t intensity, xmas_color_t color) {
  set_pixel(x, y, intensity, color);
  screen[x][y] = color;
}

// draws the icon represented by the data in +nums+ to the pegboard
void draw_icon(uint16_t nums[], uint8_t x, uint8_t y, uint8_t intensity, xmas_color_t color) {
  uint8_t w = nums[0], h = nums[1];
  uint16_t *rows = &nums[2];

  for(int i = w; i >= 0; i--) {
    for(int j = 0; j < h; j++) {
      if(((rows[j] >> i) & 1) == 1)
        set_screen_pixel(x + w - i - 1, y + j, intensity, color);
    }
  }
}

void touch_up() {
  int x = random(10), y = random(10);
  for(int i = 0; i < 20; i++) {
    set_pixel(x, y, INTENSITY, screen[x][y]);
  }
}

// paints the whole pegboard black and also resets the screen buffer to the same
void clear_screen() {
  clear();
  for(int y = 0; y < height; y++)
    for(int x = 0; x < width; x++)
      screen[x][y] = COLOR_BLACK;
}

// paddle drawing and erasing
void clear_paddle() {
  paddle(COLOR_BLACK);
}
void draw_paddle() {
  paddle(COLOR_YELLOW);
}
void paddle(xmas_color_t color) {
  for(int x = paddle_x; x < paddle_x + 4; x++) {
    if(x < 10)
      set_screen_pixel(x, 9, INTENSITY, color);
  }
}

void draw_bricks() {
  xmas_color_t color;
  bricks_on = 0;
  for(int l = 0; l < line_count[level]; l++) {
    for(int c = 0; c < width; c++) {
      color = brick_color(bricks[level][l][c]);
      set_screen_pixel(c, l, INTENSITY, color);
      if(color != COLOR_BLACK && color != COLOR_YELLOW)
        bricks_on++;
    }
  }
  //bricks_on = 3; // cheat mode
}

// converts char values (e.g., "r" or "g" etc.) in brick map into a color
xmas_color_t brick_color(char c) {
  switch(c) {
    case 'R': return COLOR_RED;
    case 'G': return COLOR_GREEN;
    case 'B': return COLOR_BLUE;
    case 'C': return COLOR_CYAN;
    case 'Y': return COLOR_YELLOW;
    case 'M': return COLOR_MAGENTA;
    default: return COLOR_BLACK;
  }
}

// ball drawing and erasing
void draw_ball() {
  set_screen_pixel(ball_x, ball_y, INTENSITY, COLOR_WHITE);
}
void clear_ball() {
  set_screen_pixel(ball_x, ball_y, INTENSITY, COLOR_BLACK);
}
