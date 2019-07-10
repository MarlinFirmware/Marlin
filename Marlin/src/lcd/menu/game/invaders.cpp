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

#if ENABLED(MARLIN_INVADERS)

#include "game.h"

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

typedef struct { int8_t x, y, v; } laser_t;

uint8_t cannons_left;
int8_t cannon_x;
laser_t explod, laser, bullet[10];
constexpr uint8_t inv_off[] = { 2, 1, 0 }, inv_wide[] = { 8, 11, 12 };
int8_t invaders_x, invaders_y, invaders_dir, leftmost, rightmost, botmost;
uint8_t invader_count, quit_count, bugs[INVADER_ROWS], shooters[(INVADER_ROWS) * (INVADER_COLS)];

inline void update_invader_data() {
  uint8_t inv_mask = 0;
  // Get a list of all active invaders
  uint8_t sc = 0;
  LOOP_L_N(y, INVADER_ROWS) {
    uint8_t m = bugs[y];
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
  LOOP_L_N(i, INVADER_ROWS) bugs[i] = _BV(INVADER_COLS) - 1;
  update_invader_data();
  reset_bullets();
}

int8_t ufox, ufov;
inline void spawn_ufo() {
  ufov = random(0, 2) ? 1 : -1;
  ufox = ufov > 0 ? -(UFO_W) : LCD_PIXEL_WIDTH - 1;
}

inline void reset_player() {
  cannon_x = 0;
  ui.encoderPosition = 0;
}

inline void fire_cannon() {
  laser.x = cannon_x + CANNON_W / 2;
  laser.y = LCD_PIXEL_HEIGHT - CANNON_H - (LASER_H);
  laser.v = -(LASER_H);
}

inline void explode(const int8_t x, const int8_t y, const int8_t v=4) {
  explod.x = x - (EXPL_W) / 2;
  explod.y = y;
  explod.v = v;
}

inline void kill_cannon(uint8_t &game_state, const uint8_t st) {
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

void InvadersGame::game_screen() {
  static bool game_blink;

  ui.refresh(LCDVIEW_CALL_NO_REDRAW); // Call as often as possible

  // Run game logic once per full screen
  if (ui.first_page) {

    // Update Cannon Position
    int16_t ep = constrain(int16_t(ui.encoderPosition), 0, (LCD_PIXEL_WIDTH - (CANNON_W)) / (CANNON_VEL));
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
            kill_cannon(game_state, 20);                      // Kill the cannon. Reset invaders.
        }

        invaders_x += invaders_dir;               // Invaders take one step left/right

        // Randomly shoot if invaders are listed
        if (invader_count && !random(0, 20)) {

          // Find a free bullet
          laser_t *b = nullptr;
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
            if (bugs[row] & mask) {
              const uint8_t type = inv_type[row];
              const int8_t invx = INV_X_LEFT(col, type);
              if (WITHIN(laser.x, invx, invx + inv_wide[type] - 1)) {
                // Turn off laser
                laser.v = 0;
                // Remove the invader!
                bugs[row] &= ~mask;
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
            kill_cannon(game_state, 120); // Hit the cannon
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

  // Click-and-hold to abort
  if (ui.button_pressed()) --quit_count; else quit_count = 10;

  // Click to fire or exit
  if (ui.use_click()) {
    if (!game_state)
      quit_count = 0;
    else if (game_state == 1 && !laser.v)
      fire_cannon();
  }

  if (!quit_count) exit_game();

  u8g.setColorIndex(1);

  // Draw invaders
  if (PAGE_CONTAINS(invaders_y, invaders_y + botmost * (ROW_H) - 2 - 1)) {
    int8_t yy = invaders_y;
    for (uint8_t y = 0; y < INVADER_ROWS; ++y) {
      const uint8_t type = inv_type[y];
      if (PAGE_CONTAINS(yy, yy + INVADER_H - 1)) {
        int8_t xx = invaders_x;
        for (uint8_t x = 0; x < INVADER_COLS; ++x) {
          if (TEST(bugs[y], x))
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
  if (explod.v && PAGE_CONTAINS(explod.y, explod.y + 7 - 1)) {
    u8g.drawBitmapP(explod.x, explod.y, 2, 7, explosion);
    --explod.v;
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

void InvadersGame::enter_game() {
  init_game(20, game_screen); // countdown to reset invaders
  cannons_left = 3;
  quit_count = 10;
  laser.v = 0;
  reset_invaders();
  reset_player();
}

#endif // MARLIN_INVADERS
