/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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

#if ENABLED(MARLIN_BRICKOUT)

#include "game.h"

#define BRICK_H      5
#define BRICK_TOP    MENU_FONT_ASCENT
#define BRICK_ROWS   4
#define BRICK_COLS  16

#define PADDLE_H     2
#define PADDLE_VEL   3
#define PADDLE_W    ((LCD_PIXEL_WIDTH) / 8)
#define PADDLE_Y    (LCD_PIXEL_HEIGHT - 1 - PADDLE_H)

#define BRICK_W     ((LCD_PIXEL_WIDTH) / (BRICK_COLS))
#define BRICK_BOT   (BRICK_TOP + BRICK_H * BRICK_ROWS - 1)

#define BRICK_COL(X) ((X) / (BRICK_W))
#define BRICK_ROW(Y) ((Y - (BRICK_TOP)) / (BRICK_H))

uint8_t balls_left, brick_count;
uint16_t bricks[BRICK_ROWS];

inline void reset_bricks(const uint16_t v) {
  brick_count = (BRICK_COLS) * (BRICK_ROWS);
  LOOP_L_N(i, BRICK_ROWS) bricks[i] = v;
}

int8_t paddle_x, hit_dir;
fixed_t ballx, bally, ballh, ballv;

void reset_ball() {
  constexpr uint8_t ball_dist = 24;
  bally = BTOF(PADDLE_Y - ball_dist);
  ballv = FTOP(1.3f);
  ballh = -FTOP(1.25f);
  uint8_t bx = paddle_x + (PADDLE_W) / 2 + ball_dist;
  if (bx >= LCD_PIXEL_WIDTH - 10) { bx -= ball_dist * 2; ballh = -ballh; }
  ballx = BTOF(bx);
  hit_dir = -1;
}

void BrickoutGame::game_screen() {
  if (game_frame()) {     // Run logic twice for finer resolution
    // Update Paddle Position
    paddle_x = (int8_t)ui.encoderPosition;
    paddle_x = constrain(paddle_x, 0, (LCD_PIXEL_WIDTH - (PADDLE_W)) / (PADDLE_VEL));
    ui.encoderPosition = paddle_x;
    paddle_x *= (PADDLE_VEL);

    // Run the ball logic
    if (game_state) do {

      // Provisionally update the ball position
      const fixed_t newx = ballx + ballh, newy = bally + ballv;  // current next position
      if (!WITHIN(newx, 0, BTOF(LCD_PIXEL_WIDTH - 1))) {    // out in x?
        ballh = -ballh; _BUZZ(5, 220);                      // bounce x
      }
      if (newy < 0) {                                       // out in y?
        ballv = -ballv; _BUZZ(5, 280);                      // bounce v
        hit_dir = 1;
      }
      // Did the ball go below the bottom?
      else if (newy > BTOF(LCD_PIXEL_HEIGHT)) {
        BUZZ(500, 75);
        if (--balls_left) reset_ball(); else game_state = 0;
        break; // done
      }

      // Is the ball colliding with a brick?
      if (WITHIN(newy, BTOF(BRICK_TOP), BTOF(BRICK_BOT))) {
        const int8_t bit = BRICK_COL(FTOB(newx)), row = BRICK_ROW(FTOB(newy));
        const uint16_t mask = _BV(bit);
        if (bricks[row] & mask) {
          // Yes. Remove it!
          bricks[row] &= ~mask;
          // Score!
          score += BRICK_ROWS - row;
          // If bricks are gone, go to reset state
          if (!--brick_count) game_state = 2;
          // Bounce the ball cleverly
          if ((ballv < 0) == (hit_dir < 0)) { ballv = -ballv; ballh += fixed_t(random(-16, 16)); _BUZZ(5, 880); }
                                       else { ballh = -ballh; ballv += fixed_t(random(-16, 16)); _BUZZ(5, 640); }
        }
      }
      // Is the ball moving down and in paddle range?
      else if (ballv > 0 && WITHIN(newy, BTOF(PADDLE_Y), BTOF(PADDLE_Y + PADDLE_H))) {
        // Ball actually hitting paddle
        const int8_t diff = FTOB(newx) - paddle_x;
        if (WITHIN(diff, 0, PADDLE_W - 1)) {

          // Reverse Y direction
          ballv = -ballv; _BUZZ(3, 880);
          hit_dir = -1;

          // Near edges affects X velocity
          const bool is_left_edge = (diff <= 1);
          if (is_left_edge || diff >= PADDLE_W-1 - 1) {
            if ((ballh > 0) == is_left_edge) ballh = -ballh;
          }
          else if (diff <= 3) {
            ballh += fixed_t(random(-64, 0));
            NOLESS(ballh, BTOF(-2));
            NOMORE(ballh, BTOF(2));
          }
          else if (diff >= PADDLE_W-1 - 3) {
            ballh += fixed_t(random( 0, 64));
            NOLESS(ballh, BTOF(-2));
            NOMORE(ballh, BTOF(2));
          }

          // Paddle hit after clearing the board? Reset the board.
          if (game_state == 2) { reset_bricks(0xFFFF); game_state = 1; }
        }
      }

      ballx += ballh; bally += ballv;   // update with new velocity

    } while (false);
  }

  u8g.setColorIndex(1);

  // Draw bricks
  if (PAGE_CONTAINS(BRICK_TOP, BRICK_BOT)) {
    for (uint8_t y = 0; y < BRICK_ROWS; ++y) {
      const uint8_t yy = y * BRICK_H + BRICK_TOP;
      if (PAGE_CONTAINS(yy, yy + BRICK_H - 1)) {
        for (uint8_t x = 0; x < BRICK_COLS; ++x) {
          if (TEST(bricks[y], x)) {
            const uint8_t xx = x * BRICK_W;
            for (uint8_t v = 0; v < BRICK_H - 1; ++v)
              if (PAGE_CONTAINS(yy + v, yy + v))
                u8g.drawHLine(xx, yy + v, BRICK_W - 1);
          }
        }
      }
    }
  }

  // Draw paddle
  if (PAGE_CONTAINS(PADDLE_Y-1, PADDLE_Y)) {
    u8g.drawHLine(paddle_x, PADDLE_Y, PADDLE_W);
    #if PADDLE_H > 1
      u8g.drawHLine(paddle_x, PADDLE_Y-1, PADDLE_W);
      #if PADDLE_H > 2
        u8g.drawHLine(paddle_x, PADDLE_Y-2, PADDLE_W);
      #endif
    #endif
  }

  // Draw ball while game is running
  if (game_state) {
    const uint8_t by = FTOB(bally);
    if (PAGE_CONTAINS(by, by+1))
      u8g.drawFrame(FTOB(ballx), by, 2, 2);
  }
  // Or draw GAME OVER
  else
    draw_game_over();

  if (PAGE_UNDER(MENU_FONT_ASCENT)) {
    // Score Digits
    //const uint8_t sx = (LCD_PIXEL_WIDTH - (score >= 10 ? score >= 100 ? score >= 1000 ? 4 : 3 : 2 : 1) * MENU_FONT_WIDTH) / 2;
    constexpr uint8_t sx = 0;
    lcd_moveto(sx, MENU_FONT_ASCENT - 1);
    lcd_put_int(score);

    // Balls Left
    lcd_moveto(LCD_PIXEL_WIDTH - MENU_FONT_WIDTH * 3, MENU_FONT_ASCENT - 1);
    PGM_P const ohs = PSTR("ooo\0\0");
    lcd_put_u8str_P(ohs + 3 - balls_left);
  }

  // A click always exits this game
  if (ui.use_click()) exit_game();
}

void BrickoutGame::enter_game() {
  init_game(2, game_screen); // 2 = reset bricks on paddle hit
  constexpr uint8_t paddle_start = SCREEN_M - (PADDLE_W) / 2;
  paddle_x = paddle_start;
  balls_left = 3;
  reset_bricks(0x0000);
  reset_ball();
  ui.encoderPosition = paddle_start / (PADDLE_VEL);
}

#endif // MARLIN_BRICKOUT
