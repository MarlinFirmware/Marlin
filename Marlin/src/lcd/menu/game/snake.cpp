/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(MARLIN_SNAKE)

#include "game.h"

#define SNAKE_BOX 4

#define HEADER_H  (MENU_FONT_ASCENT - 2)
#define SNAKE_WH  (SNAKE_BOX + 1)

#define IDEAL_L   2
#define IDEAL_R   (LCD_PIXEL_WIDTH - 1 - 2)
#define IDEAL_T   (HEADER_H + 2)
#define IDEAL_B   (LCD_PIXEL_HEIGHT - 1 - 2)
#define IDEAL_W   (IDEAL_R - (IDEAL_L) + 1)
#define IDEAL_H   (IDEAL_B - (IDEAL_T) + 1)

#define GAME_W    int((IDEAL_W) / (SNAKE_WH))
#define GAME_H    int((IDEAL_H) / (SNAKE_WH))

#define BOARD_W   ((SNAKE_WH) * (GAME_W) + 1)
#define BOARD_H   ((SNAKE_WH) * (GAME_H) + 1)
#define BOARD_L   ((LCD_PIXEL_WIDTH - (BOARD_W) + 1) / 2)
#define BOARD_R   (BOARD_L + BOARD_W - 1)
#define BOARD_T   (((LCD_PIXEL_HEIGHT + IDEAL_T) - (BOARD_H)) / 2)
#define BOARD_B   (BOARD_T + BOARD_H - 1)

#define GAMEX(X)  (BOARD_L + ((X) * (SNAKE_WH)))
#define GAMEY(Y)  (BOARD_T + ((Y) * (SNAKE_WH)))

#if SNAKE_BOX > 2
  #define FOOD_WH SNAKE_BOX
#else
  #define FOOD_WH 2
#endif

#if SNAKE_BOX < 1
  #define SNAKE_SIZ 1
#else
  #define SNAKE_SIZ SNAKE_BOX
#endif

constexpr fixed_t snakev = FTOP(0.20);

int8_t snake_dir, // NESW
       foodx, foody, food_cnt,
       old_encoder;
fixed_t snakex, snakey;

// Up to 50 lines, then you win!
typedef struct { int8_t x, y; } pos_t;
uint8_t head_ind;
pos_t snake_tail[50];

// Remove the first pixel from the tail.
// If needed, shift out the first segment.
void shorten_tail() {
  pos_t &p = snake_tail[0], &q = snake_tail[1];
  bool shift = false;
  if (p.x == q.x) {
    // Vertical line
    p.y += (q.y > p.y) ? 1 : -1;
    shift = p.y == q.y;
  }
  else {
    // Horizontal line
    p.x += (q.x > p.x) ? 1 : -1;
    shift = p.x == q.x;
  }
  if (shift) {
    head_ind--;
    for (uint8_t i = 0; i <= head_ind; ++i)
      snake_tail[i] = snake_tail[i + 1];
  }
}

// The food is on a line
inline bool food_on_line() {
  for (uint8_t n = 0; n < head_ind; ++n) {
    pos_t &p = snake_tail[n], &q = snake_tail[n + 1];
    if (p.x == q.x) {
      if ((foodx == p.x - 1 || foodx == p.x) && WITHIN(foody, _MIN(p.y, q.y), _MAX(p.y, q.y)))
        return true;
    }
    else if ((foody == p.y - 1 || foody == p.y) && WITHIN(foodx, _MIN(p.x, q.x), _MAX(p.x, q.x)))
      return true;
  }
  return false;
}

// Add a new food blob
void food_reset() {
  do {
    foodx = random(0, GAME_W);
    foody = random(0, GAME_H);
  } while (food_on_line());
}

// Turn the snake cw or ccw
inline void turn_snake(const bool cw) {
  snake_dir += cw ? 1 : -1;
  snake_dir &= 0x03;
  head_ind++;
  snake_tail[head_ind].x = FTOB(snakex);
  snake_tail[head_ind].y = FTOB(snakey);
}

// Reset the snake for a new game
void snake_reset() {
  // Init the head and velocity
  snakex = BTOF(1);
  snakey = BTOF(GAME_H / 2);
  //snakev = FTOP(0.25);

  // Init the tail with a cw turn
  snake_dir = 0;
  head_ind = 0;
  snake_tail[0].x = 0;
  snake_tail[0].y = GAME_H / 2;
  turn_snake(true);

  // Clear food flag
  food_cnt = 5;

  // Clear the controls
  ui.encoderPosition = 0;
  old_encoder = 0;
}

// Check if head segment overlaps another
bool snake_overlap() {
  // 4 lines must exist before a collision is possible
  if (head_ind < 4) return false;
  // Is the last segment crossing any others?
  const pos_t &h1 = snake_tail[head_ind - 1], &h2 = snake_tail[head_ind];
  // VERTICAL head segment?
  if (h1.x == h2.x) {
    // Loop from oldest to segment two away from head
    for (uint8_t n = 0; n < head_ind - 2; ++n) {
      // Segment p to q
      const pos_t &p = snake_tail[n], &q = snake_tail[n + 1];
      if (p.x != q.x) {
        // Crossing horizontal segment
        if (WITHIN(h1.x, _MIN(p.x, q.x), _MAX(p.x, q.x)) && (h1.y <= p.y) == (h2.y >= p.y)) return true;
      } // Overlapping vertical segment
      else if (h1.x == p.x && _MIN(h1.y, h2.y) <= _MAX(p.y, q.y) && _MAX(h1.y, h2.y) >= _MIN(p.y, q.y)) return true;
    }
  }
  else {
    // Loop from oldest to segment two away from head
    for (uint8_t n = 0; n < head_ind - 2; ++n) {
      // Segment p to q
      const pos_t &p = snake_tail[n], &q = snake_tail[n + 1];
      if (p.y != q.y) {
        // Crossing vertical segment
        if (WITHIN(h1.y, _MIN(p.y, q.y), _MAX(p.y, q.y)) && (h1.x <= p.x) == (h2.x >= p.x)) return true;
      } // Overlapping horizontal segment
      else if (h1.y == p.y && _MIN(h1.x, h2.x) <= _MAX(p.x, q.x) && _MAX(h1.x, h2.x) >= _MIN(p.x, q.x)) return true;
    }
  }
  return false;
}

void SnakeGame::game_screen() {
  // Run the snake logic
  if (game_frame()) do {    // Run logic twice for finer resolution

    // Move the snake's head one unit in the current direction
    const int8_t oldx = FTOB(snakex), oldy = FTOB(snakey);
    switch (snake_dir) {
      case 0: snakey -= snakev; break;
      case 1: snakex += snakev; break;
      case 2: snakey += snakev; break;
      case 3: snakex -= snakev; break;
    }
    const int8_t x = FTOB(snakex), y = FTOB(snakey);

    // If movement took place...
    if (oldx != x || oldy != y) {

      if (!WITHIN(x, 0, GAME_W - 1) || !WITHIN(y, 0, GAME_H - 1)) {
        game_state = 0; // Game Over
        _BUZZ(400, 40); // Bzzzt!
        break;          // ...out of do-while
      }

      snake_tail[head_ind].x = x;
      snake_tail[head_ind].y = y;

      // Change snake direction if set
      const int8_t enc = int8_t(ui.encoderPosition), diff = enc - old_encoder;
      if (diff) {
        old_encoder = enc;
        turn_snake(diff > 0);
      }

      if (food_cnt) --food_cnt; else shorten_tail();

      // Did the snake collide with itself or go out of bounds?
      if (snake_overlap()) {
        game_state = 0; // Game Over
        _BUZZ(400, 40); // Bzzzt!
      }
      // Is the snake at the food?
      else if (x == foodx && y == foody) {
        _BUZZ(5, 220);
        _BUZZ(5, 280);
        score++;
        food_cnt = 2;
        food_reset();
      }
    }

  } while(0);

  u8g.setColorIndex(1);

  // Draw Score
  if (PAGE_UNDER(HEADER_H)) {
    lcd_moveto(0, HEADER_H - 1);
    lcd_put_int(score);
  }

  // DRAW THE PLAYFIELD BORDER
  u8g.drawFrame(BOARD_L - 2, BOARD_T - 2, BOARD_R - BOARD_L + 4, BOARD_B - BOARD_T + 4);

  // Draw the snake (tail)
  #if SNAKE_WH < 2

    // At this scale just draw a line
    for (uint8_t n = 0; n < head_ind; ++n) {
      const pos_t &p = snake_tail[n], &q = snake_tail[n + 1];
      if (p.x == q.x) {
        const int8_t y1 = GAMEY(_MIN(p.y, q.y)), y2 = GAMEY(_MAX(p.y, q.y));
        if (PAGE_CONTAINS(y1, y2))
          u8g.drawVLine(GAMEX(p.x), y1, y2 - y1 + 1);
      }
      else if (PAGE_CONTAINS(GAMEY(p.y), GAMEY(p.y))) {
        const int8_t x1 = GAMEX(_MIN(p.x, q.x)), x2 = GAMEX(_MAX(p.x, q.x));
        u8g.drawHLine(x1, GAMEY(p.y), x2 - x1 + 1);
      }
    }

  #elif SNAKE_WH == 2

    // At this scale draw two lines
    for (uint8_t n = 0; n < head_ind; ++n) {
      const pos_t &p = snake_tail[n], &q = snake_tail[n + 1];
      if (p.x == q.x) {
        const int8_t y1 = GAMEY(_MIN(p.y, q.y)), y2 = GAMEY(_MAX(p.y, q.y));
        if (PAGE_CONTAINS(y1, y2 + 1))
          u8g.drawFrame(GAMEX(p.x), y1, 2, y2 - y1 + 1 + 1);
      }
      else {
        const int8_t py = GAMEY(p.y);
        if (PAGE_CONTAINS(py, py + 1)) {
          const int8_t x1 = GAMEX(_MIN(p.x, q.x)), x2 = GAMEX(_MAX(p.x, q.x));
          u8g.drawFrame(x1, py, x2 - x1 + 1 + 1, 2);
        }
      }
    }

  #else

    // Draw a series of boxes
    for (uint8_t n = 0; n < head_ind; ++n) {
      const pos_t &p = snake_tail[n], &q = snake_tail[n + 1];
      if (p.x == q.x) {
        const int8_t y1 = _MIN(p.y, q.y), y2 = _MAX(p.y, q.y);
        if (PAGE_CONTAINS(GAMEY(y1), GAMEY(y2) + SNAKE_SIZ - 1)) {
          for (int8_t i = y1; i <= y2; ++i) {
            const int8_t y = GAMEY(i);
            if (PAGE_CONTAINS(y, y + SNAKE_SIZ - 1))
              u8g.drawBox(GAMEX(p.x), y, SNAKE_SIZ, SNAKE_SIZ);
          }
        }
      }
      else {
        const int8_t py = GAMEY(p.y);
        if (PAGE_CONTAINS(py, py + SNAKE_SIZ - 1)) {
          const int8_t x1 = _MIN(p.x, q.x), x2 = _MAX(p.x, q.x);
          for (int8_t i = x1; i <= x2; ++i)
            u8g.drawBox(GAMEX(i), py, SNAKE_SIZ, SNAKE_SIZ);
        }
      }
    }

  #endif

  // Draw food
  const int8_t fy = GAMEY(foody);
  if (PAGE_CONTAINS(fy, fy + FOOD_WH - 1)) {
    const int8_t fx = GAMEX(foodx);
    u8g.drawFrame(fx, fy, FOOD_WH, FOOD_WH);
    if (FOOD_WH == 5) u8g.drawPixel(fx + 2, fy + 2);
  }

  // Draw GAME OVER
  if (!game_state) draw_game_over();

  // A click always exits this game
  if (ui.use_click()) exit_game();
}

void SnakeGame::enter_game() {
  init_game(1, game_screen); // 1 = Game running
  snake_reset();
  food_reset();
}

#endif // MARLIN_SNAKE
