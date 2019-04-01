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

#include "../../inc/MarlinConfigPre.h"

#if HAS_LCD_MENU && ANY(MARLIN_BRICKOUT, MARLIN_INVADERS, MARLIN_SNAKE)

#include "menu.h"
#include "../dogm/ultralcd_DOGM.h"
#include "../lcdprint.h"
#define SCREEN_M ((LCD_PIXEL_WIDTH) / 2)
#define _BUZZ(D,F) BUZZ(D,F)
//#define _BUZZ(D,F) NOOP

// Simple 8:8 fixed-point
typedef int16_t fixed_t;
#define FTOP(F) fixed_t((F)*256.0f)
#define PTOF(P) (float(P)*(1.0f/256.0f))
#define BTOF(X) (fixed_t(X)<<8)
#define FTOB(X) int8_t(fixed_t(X)>>8)

int score;
uint8_t game_state;
millis_t next_frame;

inline void draw_game_over() {
  constexpr int8_t gowide = (MENU_FONT_WIDTH) * 9,
                   gohigh = MENU_FONT_ASCENT - 3,
                       lx = (LCD_PIXEL_WIDTH - gowide) / 2,
                       ly = (LCD_PIXEL_HEIGHT + gohigh) / 2;
  if (PAGE_CONTAINS(ly - gohigh - 1, ly + 1)) {
    u8g.setColorIndex(0);
    u8g.drawBox(lx - 1, ly - gohigh - 1, gowide + 2, gohigh + 2);
    u8g.setColorIndex(1);
    if (ui.get_blink()) {
      lcd_moveto(lx, ly);
      lcd_put_u8str_P(PSTR("GAME OVER"));
    }
  }
}

#if ENABLED(MARLIN_BRICKOUT)

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

  void game_screen_brickout() {
    static int8_t slew;
    ui.refresh(LCDVIEW_CALL_NO_REDRAW); // Call as often as possible

    if (ui.first_page) slew = 2;

    if (slew-- > 0) {                   // Logic runs twice for finer resolution
      // Update Paddle Position
      paddle_x = (int8_t)ui.encoderPosition;
      paddle_x = constrain(paddle_x, 0, (LCD_PIXEL_WIDTH - (PADDLE_W)) / (PADDLE_VEL));
      ui.encoderPosition = paddle_x;
      paddle_x *= (PADDLE_VEL);

      // Run the ball logic
      if (game_state) do {

        // Provisionally update the position
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
    if (ui.use_click()) ui.goto_previous_screen();
  }

  void lcd_goto_brickout() {
    constexpr uint8_t paddle_start = SCREEN_M - (PADDLE_W) / 2;
    paddle_x = paddle_start;
    game_state = 2; // reset bricks on paddle hit
    score = 0;
    balls_left = 3;
    reset_bricks(0x0000);
    reset_ball();
    ui.goto_screen(game_screen_brickout, paddle_start / (PADDLE_VEL));
    ui.defer_status_screen();
  }

#endif // MARLIN_BRICKOUT

#if ENABLED(MARLIN_INVADERS)

  // 11x8
  const unsigned char invader[3][2][16] PROGMEM = {
    { { B00000110,B00000000,
        B00001111,B00000000,
        B00011111,B10000000,
        B00110110,B11000000,
        B00111111,B11000000,
        B00001001,B00000000,
        B00010110,B10000000,
        B00101001,B01000000
      }, {
        B00000110,B00000000,
        B00001111,B00000000,
        B00011111,B10000000,
        B00110110,B11000000,
        B00111111,B11000000,
        B00010110,B10000000,
        B00100000,B01000000,
        B00010000,B10000000
      }
    }, {
      { B00010000,B01000000,
        B00001000,B10000000,
        B00011111,B11000000,
        B00110111,B01100000,
        B01111111,B11110000,
        B01011111,B11010000,
        B01010000,B01010000,
        B00001101,B10000000
      }, {
        B00010000,B01000000,
        B01001000,B10010000,
        B01011111,B11010000,
        B01110111,B01110000,
        B01111111,B11110000,
        B00011111,B11000000,
        B00010000,B01000000,
        B00100000,B00100000 
      }
    }, {
      { B00001111,B00000000,
        B01111111,B11100000,
        B11111111,B11110000,
        B11100110,B01110000,
        B11111111,B11110000,
        B00011001,B10000000,
        B00110110,B11000000,
        B11000000,B00110000
      }, {
        B00001111,B00000000,
        B01111111,B11100000,
        B11111111,B11110000,
        B11100110,B01110000,
        B11111111,B11110000,
        B00011001,B10000000,
        B00110110,B11000000,
        B00011001,B10000000
      }
    }
  };
  const unsigned char cannon[] PROGMEM = {
    B00000100,B00000000,
    B00001110,B00000000,
    B00001110,B00000000,
    B01111111,B11000000,
    B11111111,B11100000,
    B11111111,B11100000,
    B11111111,B11100000,
    B11111111,B11100000
  };
  const unsigned char life[] PROGMEM = {
    B00010000,
    B01111100,
    B11111110,
    B11111110,
    B11111110
  };
  const unsigned char explosion[] PROGMEM = {
    B01000100,B01000000,
    B00100100,B10000000,
    B00000000,B00000000,
    B00110001,B10000000,
    B00000000,B00000000,
    B00100100,B10000000,
    B01000100,B01000000
  };
  const unsigned char ufo[] PROGMEM = {
    B00011111,B11000000,
    B01111111,B11110000,
    B11011101,B11011000,
    B11111111,B11111000,
    B01111111,B11110000
  };

  #define INVASION_SIZE 3

  #if INVASION_SIZE == 3
    #define INVADER_COLS   5
  #elif INVASION_SIZE == 4
    #define INVADER_COLS   6
  #else
    #define INVADER_COLS   8
    #undef INVASION_SIZE
    #define INVASION_SIZE  5
  #endif

  #define INVADER_ROWS INVASION_SIZE

  constexpr uint8_t inv_type[] = {
    #if INVADER_ROWS == 5
      0, 1, 1, 2, 2
    #elif INVADER_ROWS == 4
      0, 1, 1, 2
    #elif INVADER_ROWS == 3
      0, 1, 2
    #else
      #error "INVASION_SIZE must be 3, 4, or 5."
    #endif
  };

  #define INVADER_RIGHT ((INVADER_COLS) * (COL_W))

  #define CANNON_W      11
  #define CANNON_H       8
  #define CANNON_VEL     4
  #define CANNON_Y      (LCD_PIXEL_HEIGHT - 1 - CANNON_H)

  #define COL_W         14
  #define INVADER_H      8
  #define ROW_H         (INVADER_H + 2)
  #define INVADER_VEL    3

  #define INVADER_TOP   MENU_FONT_ASCENT
  #define INVADERS_WIDE ((COL_W) * (INVADER_COLS))
  #define INVADERS_HIGH ((ROW_H) * (INVADER_ROWS))

  #define UFO_H          5
  #define UFO_W         13

  #define LASER_H        4
  #define SHOT_H         3
  #define EXPL_W        11
  #define LIFE_W         8
  #define LIFE_H         5

  #define INVADER_COL(X) ((X - invaders_x) / (COL_W))
  #define INVADER_ROW(Y) ((Y - invaders_y + 2) / (ROW_H))

  #define INV_X_LEFT(C,T) (invaders_x + (C) * (COL_W) + inv_off[T])
  #define INV_X_CTR(C,T)  (INV_X_LEFT(C,T) + inv_wide[T] / 2)
  #define INV_Y_BOT(R)    (invaders_y + (R + 1) * (ROW_H) - 2)

  uint8_t cannons_left;
  int8_t cannon_x;
  typedef struct { int8_t x, y, v; } laser_t;
  laser_t laser, expl, bullet[10];
  constexpr uint8_t inv_off[] = { 2, 1, 0 }, inv_wide[] = { 8, 11, 12 };
  int8_t invaders_x, invaders_y, invaders_dir, leftmost, rightmost, botmost;
  uint8_t invader_count, invaders[INVADER_ROWS], shooters[(INVADER_ROWS) * (INVADER_COLS)];

  inline void update_invader_data() {
    uint8_t inv_mask = 0;
    // Get a list of all active invaders
    uint8_t sc = 0;
    LOOP_L_N(y, INVADER_ROWS) {
      uint8_t m = invaders[y];
      if (m) botmost = y + 1;
      inv_mask |= m;
      for (uint8_t x = 0; x < INVADER_COLS; ++x)
        if (TEST(m, x)) shooters[sc++] = (y << 4) | x;
    }
    leftmost = 0;
    LOOP_L_N(i, INVADER_COLS)            { if (TEST(inv_mask, i)) break; leftmost -= COL_W; }
    rightmost = LCD_PIXEL_WIDTH - (INVADERS_WIDE);
    for (uint8_t i = INVADER_COLS; i--;) { if (TEST(inv_mask, i)) break; rightmost += COL_W; }
    if (invader_count == 2) invaders_dir = invaders_dir > 0 ? INVADER_VEL + 1 : -(INVADER_VEL + 1);
  }

  inline void reset_bullets() {
    LOOP_L_N(i, COUNT(bullet)) bullet[i].v = 0;
  }

  inline void reset_invaders() {
    invaders_x = 0; invaders_y = INVADER_TOP;
    invaders_dir = INVADER_VEL;
    invader_count = (INVADER_COLS) * (INVADER_ROWS);
    LOOP_L_N(i, INVADER_ROWS) invaders[i] = _BV(INVADER_COLS) - 1;
    update_invader_data();
    reset_bullets();
  }

  int8_t ufox, ufov;
  inline void spawn_ufo() {
    ufov = random(0, 2) ? 1 : -1;
    ufox = ufov > 0 ? -(UFO_W) : LCD_PIXEL_WIDTH - 1;
  }

  void reset_player() {
    cannon_x = 0;
    ui.encoderPosition = 0;
  }

  inline void fire_cannon() {
    laser.x = cannon_x + CANNON_W / 2;
    laser.y = LCD_PIXEL_HEIGHT - CANNON_H - (LASER_H);
    laser.v = -(LASER_H);
  }

  inline void explode(const int8_t x, const int8_t y, const int8_t v=4) {
    expl.x = x - (EXPL_W) / 2; expl.y = y; expl.v = v;
  }

  inline void kill_cannon(const uint8_t st) {
    reset_bullets();
    explode(cannon_x + (CANNON_W) / 2, CANNON_Y, 6);
    _BUZZ(1000, 10);
    if (--cannons_left) {
      laser.v = 0;
      game_state = st;
      reset_player();
    }
    else
      game_state = 0;
  }

  void game_screen_invaders() {
    static bool game_blink;

    ui.refresh(LCDVIEW_CALL_NO_REDRAW); // Call as often as possible

    // Run game logic once per full screen
    if (ui.first_page) {

      // Update Cannon Position
      int32_t ep = (int32_t)ui.encoderPosition;
      ep = constrain(ep, 0, (LCD_PIXEL_WIDTH - (CANNON_W)) / (CANNON_VEL));
      ui.encoderPosition = ep;

      ep *= (CANNON_VEL);
      if (ep > cannon_x) { cannon_x += CANNON_VEL - 1; if (ep - cannon_x < 2) cannon_x = ep; }
      if (ep < cannon_x) { cannon_x -= CANNON_VEL - 1; if (cannon_x - ep < 2) cannon_x = ep; }

      // Run the game logic
      if (game_state) do {

        // Move the UFO, if any
        if (ufov) { ufox += ufov; if (!WITHIN(ufox, -(UFO_W), LCD_PIXEL_WIDTH - 1)) ufov = 0; }

        if (game_state > 1) { if (--game_state == 2) { reset_invaders(); } else if (game_state == 100) { game_state = 1; } break; }

        static uint8_t blink_count;
        const bool did_blink = (++blink_count > invader_count >> 1);
        if (did_blink) {
          game_blink = !game_blink;
          blink_count = 0;
        }

        if (invader_count && did_blink) {
          const int8_t newx = invaders_x + invaders_dir;
          if (!WITHIN(newx, leftmost, rightmost)) {             // Invaders reached the edge?
            invaders_dir *= -1;                                 // Invaders change direction
            invaders_y += (ROW_H) / 2;                          // Invaders move down
            invaders_x -= invaders_dir;                         // ...and only move down this time.
            if (invaders_y + botmost * (ROW_H) - 2 >= CANNON_Y) // Invaders reached the bottom?
              kill_cannon(20);                                  // Kill the cannon. Reset invaders.
          }

          invaders_x += invaders_dir;               // Invaders take one step left/right

          // Randomly shoot if invaders are listed
          if (invader_count && !random(0, 20)) {

            // Find a free bullet
            laser_t *b = NULL;
            LOOP_L_N(i, COUNT(bullet)) if (!bullet[i].v) { b = &bullet[i]; break; }
            if (b) {
              // Pick a random shooter and update the bullet
              //SERIAL_ECHOLNPGM("free bullet found");
              const uint8_t inv = shooters[random(0, invader_count + 1)], col = inv & 0x0F, row = inv >> 4, type = inv_type[row];
              b->x = INV_X_CTR(col, type);
              b->y = INV_Y_BOT(row);
              b->v = 2 + random(0, 2);
            }
          }
        }

        // Update the laser position
        if (laser.v) {
          laser.y += laser.v;
          if (laser.y < 0) laser.v = 0;
        }

        // Did the laser collide with an invader?
        if (laser.v && WITHIN(laser.y, invaders_y, invaders_y + INVADERS_HIGH - 1)) {
          const int8_t col = INVADER_COL(laser.x);
          if (WITHIN(col, 0, INVADER_COLS - 1)) {
            const int8_t row = INVADER_ROW(laser.y);
            if (WITHIN(row, 0, INVADER_ROWS - 1)) {
              const uint8_t mask = _BV(col);
              if (invaders[row] & mask) {
                const uint8_t type = inv_type[row];
                const int8_t invx = INV_X_LEFT(col, type);
                if (WITHIN(laser.x, invx, invx + inv_wide[type] - 1)) {
                  // Turn off laser
                  laser.v = 0;
                  // Remove the invader!
                  invaders[row] &= ~mask;
                  // Score!
                  score += INVADER_ROWS - row;
                  // Explode sound!
                  _BUZZ(40, 10);
                  // Explosion bitmap!
                  explode(invx + inv_wide[type] / 2, invaders_y + row * (ROW_H));
                  // If invaders are gone, go to reset invaders state
                  if (--invader_count) update_invader_data(); else { game_state = 20; reset_bullets(); }
                } // laser x hit
              } // invader exists
            } // good row
          } // good col
        } // laser in invader zone

        // Handle alien bullets
        LOOP_L_N(s, COUNT(bullet)) {
          laser_t *b = &bullet[s];
          if (b->v) {
            // Update alien bullet position
            b->y += b->v;
            if (b->y >= LCD_PIXEL_HEIGHT)
              b->v = 0; // Offscreen
            else if (b->y >= CANNON_Y && WITHIN(b->x, cannon_x, cannon_x + CANNON_W - 1))
              kill_cannon(120); // Hit the cannon
          }
        }

        // Randomly spawn a UFO
        if (!ufov && !random(0,500)) spawn_ufo();

        // Did the laser hit a ufo?
        if (laser.v && ufov && laser.y < UFO_H + 2 && WITHIN(laser.x, ufox, ufox + UFO_W - 1)) {
          // Turn off laser and UFO
          laser.v = ufov = 0;
          // Score!
          score += 10;
          // Explode!
          _BUZZ(40, 10);
          // Explosion bitmap
          explode(ufox + (UFO_W) / 2, 1);
        }

      } while (false);

    }

    // Click to fire or exit
    if (ui.use_click()) {
      if (!game_state)
        ui.goto_previous_screen();
      else if (game_state == 1 && !laser.v)
        fire_cannon();
    }

    u8g.setColorIndex(1);

    // Draw invaders
    if (PAGE_CONTAINS(invaders_y, invaders_y + botmost * (ROW_H) - 2 - 1)) {
      int8_t yy = invaders_y;
      for (uint8_t y = 0; y < INVADER_ROWS; ++y) {
        const uint8_t type = inv_type[y];
        if (PAGE_CONTAINS(yy, yy + INVADER_H - 1)) {
          int8_t xx = invaders_x;
          for (uint8_t x = 0; x < INVADER_COLS; ++x) {
            if (TEST(invaders[y], x))
              u8g.drawBitmapP(xx, yy, 2, INVADER_H, invader[type][game_blink]);
            xx += COL_W;
          }
        }
        yy += ROW_H;
      }
    }

    // Draw UFO
    if (ufov && PAGE_UNDER(UFO_H + 2))
      u8g.drawBitmapP(ufox, 2, 2, UFO_H, ufo);

    // Draw cannon
    if (game_state && PAGE_CONTAINS(CANNON_Y, CANNON_Y + CANNON_H - 1) && (game_state < 2 || (game_state & 0x02)))
      u8g.drawBitmapP(cannon_x, CANNON_Y, 2, CANNON_H, cannon);

    // Draw laser
    if (laser.v && PAGE_CONTAINS(laser.y, laser.y + LASER_H - 1))
      u8g.drawVLine(laser.x, laser.y, LASER_H);

    // Draw invader bullets
    LOOP_L_N (i, COUNT(bullet)) {
      if (bullet[i].v && PAGE_CONTAINS(bullet[i].y - (SHOT_H - 1), bullet[i].y))
        u8g.drawVLine(bullet[i].x, bullet[i].y - (SHOT_H - 1), SHOT_H);
    }

    // Draw explosion
    if (expl.v && PAGE_CONTAINS(expl.y, expl.y + 7 - 1)) {
      u8g.drawBitmapP(expl.x, expl.y, 2, 7, explosion);
      --expl.v;
    }

    // Blink GAME OVER when game is over
    if (!game_state) draw_game_over();

    if (PAGE_UNDER(MENU_FONT_ASCENT - 1)) {
      // Draw Score
      //const uint8_t sx = (LCD_PIXEL_WIDTH - (score >= 10 ? score >= 100 ? score >= 1000 ? 4 : 3 : 2 : 1) * MENU_FONT_WIDTH) / 2;
      constexpr uint8_t sx = 0;
      lcd_moveto(sx, MENU_FONT_ASCENT - 1);
      lcd_put_int(score);

      // Draw lives
      if (cannons_left)
        for (uint8_t i = 1; i <= cannons_left; ++i)
          u8g.drawBitmapP(LCD_PIXEL_WIDTH - i * (LIFE_W), 6 - (LIFE_H), 1, LIFE_H, life);
    }

  }

  void lcd_goto_invaders() {
    game_state = 20; // countdown to reset invaders
    score = 0;
    cannons_left = 3;
    laser.v = 0;
    reset_invaders();
    reset_player();
    ui.goto_screen(game_screen_invaders, 0);
    ui.defer_status_screen();
  }

#endif // MARLIN_INVADERS

#if ENABLED(MARLIN_SNAKE)

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
        if ((foodx == p.x - 1 || foodx == p.x) && WITHIN(foody, MIN(p.y, q.y), MAX(p.y, q.y)))
          return true;
      }
      else if ((foody == p.y - 1 || foody == p.y) && WITHIN(foodx, MIN(p.x, q.x), MAX(p.x, q.x)))
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
          if (WITHIN(h1.x, MIN(p.x, q.x), MAX(p.x, q.x)) && (h1.y <= p.y) == (h2.y >= p.y)) return true;
        } // Overlapping vertical segment
        else if (h1.x == p.x && MIN(h1.y, h2.y) <= MAX(p.y, q.y) && MAX(h1.y, h2.y) >= MIN(p.y, q.y)) return true;
      }
    }
    else {
      // Loop from oldest to segment two away from head
      for (uint8_t n = 0; n < head_ind - 2; ++n) {
        // Segment p to q
        const pos_t &p = snake_tail[n], &q = snake_tail[n + 1];
        if (p.y != q.y) {
          // Crossing vertical segment
          if (WITHIN(h1.y, MIN(p.y, q.y), MAX(p.y, q.y)) && (h1.x <= p.x) == (h2.x >= p.x)) return true;
        } // Overlapping horizontal segment
        else if (h1.y == p.y && MIN(h1.x, h2.x) <= MAX(p.x, q.x) && MAX(h1.x, h2.x) >= MIN(p.x, q.x)) return true;
      }
    }
    return false;
  }

  void game_screen_snake() {
    static int8_t slew;
    if (ui.first_page) slew = 2;
    ui.refresh(LCDVIEW_CALL_NO_REDRAW);   // Call as often as possible

    // Run the snake logic
    if (game_state && slew-- > 0) do {    // Run logic twice for finer resolution

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
          const int8_t y1 = GAMEY(MIN(p.y, q.y)), y2 = GAMEY(MAX(p.y, q.y));
          if (PAGE_CONTAINS(y1, y2))
            u8g.drawVLine(GAMEX(p.x), y1, y2 - y1 + 1);
        }
        else if (PAGE_CONTAINS(GAMEY(p.y), GAMEY(p.y))) {
          const int8_t x1 = GAMEX(MIN(p.x, q.x)), x2 = GAMEX(MAX(p.x, q.x));
          u8g.drawHLine(x1, GAMEY(p.y), x2 - x1 + 1);
        }
      }

    #elif SNAKE_WH == 2

      // At this scale draw two lines
      for (uint8_t n = 0; n < head_ind; ++n) {
        const pos_t &p = snake_tail[n], &q = snake_tail[n + 1];
        if (p.x == q.x) {
          const int8_t y1 = GAMEY(MIN(p.y, q.y)), y2 = GAMEY(MAX(p.y, q.y));
          if (PAGE_CONTAINS(y1, y2 + 1))
            u8g.drawFrame(GAMEX(p.x), y1, 2, y2 - y1 + 1 + 1);
        }
        else {
          const int8_t py = GAMEY(p.y);
          if (PAGE_CONTAINS(py, py + 1)) {
            const int8_t x1 = GAMEX(MIN(p.x, q.x)), x2 = GAMEX(MAX(p.x, q.x));
            u8g.drawFrame(x1, py, x2 - x1 + 1 + 1, 2);
          }
        }
      }

    #else

      // Draw a series of boxes
      for (uint8_t n = 0; n < head_ind; ++n) {
        const pos_t &p = snake_tail[n], &q = snake_tail[n + 1];
        if (p.x == q.x) {
          const int8_t y1 = MIN(p.y, q.y), y2 = MAX(p.y, q.y);
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
            const int8_t x1 = MIN(p.x, q.x), x2 = MAX(p.x, q.x);
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
    if (ui.use_click()) ui.goto_previous_screen();
  }

  void lcd_goto_snake() {
    game_state = 1; // Game running
    score = 0;
    snake_reset();
    food_reset();
    ui.encoder_direction_normal();
    ui.goto_screen(game_screen_snake);
    ui.defer_status_screen();
  }

#endif // MARLIN_SNAKE

#if HAS_GAME_MENU

  void menu_game() {
    START_MENU();
    MENU_BACK(MSG_MAIN);
    #if ENABLED(MARLIN_BRICKOUT)
      MENU_ITEM(submenu, MSG_BRICKOUT, lcd_goto_brickout);
    #endif
    #if ENABLED(MARLIN_INVADERS)
      MENU_ITEM(submenu, MSG_INVADERS, lcd_goto_invaders);
    #endif
    #if ENABLED(MARLIN_SNAKE)
      MENU_ITEM(submenu, MSG_SNAKE, lcd_goto_snake);
    #endif
    END_MENU();
  }

#endif

#endif // HAS_LCD_MENU && (MARLIN_BRICKOUT || MARLIN_INVADERS || MARLIN_SNAKE)
