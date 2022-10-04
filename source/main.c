#include <3ds.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "audio.h"
#include "graphics.h"
#include "level.h"
#include "structs.h"

Configuration cfg;
// doesn't necessarily mean that the game is paused
// could be that we're waiting for removal animation to finish
u8 paused = 0;
// ie. during line removal it's set to 0 so stuff won't break;
u8 controllable = 1;
u8 playable = 1;

u8 mode = MODE_TETRIS;

extern u8 level;
extern u8 ARE_state;

extern u32 high_score;

// controls related variables
u8 start_held = 0;
u8 A_held = 0;
u8 B_held = 0;
u8 HOLD_held = 0;
u8 UP_held = 0;
u8 RIGHT_pressed = 0;
u8 LEFT_pressed = 0;
u32 RIGHT_DAS_count;
u32 LEFT_DAS_count;
u32 RIGHT_DAS_speed_count = 0;
u32 LEFT_DAS_speed_count = 0;

u32 KEY_HOLD = KEY_L;
u32 KEY_DAS = KEY_R;

u8 restartpls = 0;
u8 config_lvl = 1;

char theme_template[64] = "sdmc:/fbwodata/default/%s";

void parse_config(FILE* config_file) {
  char buffer[80];
  char theme_folder_name[60];
  char command[20];

  int val, val2, val3, val4;
  while (fgets(buffer, 79, config_file)) {
    //              level\no.\fpd\rpd\gd
    if (sscanf(buffer, "%20s %d %d %d %d", command, &val, &val2, &val3,
               &val4) == 5) {
      if (!strcmp(command, "level\0") && val <= 20 && val > 0) {
        printf("lvl %d: %d frms, %d rows, %d delay\n", val, val2, val3, val4);
        cfg.frames_per_drop[val - 1] = val2;
        cfg.rows_per_drop[val - 1] = val3;
        cfg.glue_delay[val - 1] = val4;
      }
    } else if (sscanf(buffer, "%20s %d", command, &val) == 2) {
      if (!strcmp(command, "DAS\0")) {
        printf("DAS delay = %d frames\n", val);
        cfg.DAS = val;
      } else if (!strcmp(command, "invisimode\0")) {
        printf("invisimode: ");
        if (val) {
          printf("on\n");
        } else {
          printf("off\n");
        }
        cfg.invisimode = val;
      } else if (!strcmp(command, "hold\0")) {
        printf("hold: ");
        if (val) {
          printf("on\n");
        } else {
          printf("off\n");
        }
        cfg.hold = val;
      } else if (!strcmp(command, "ghost_piece\0")) {
        printf("ghost piece: ");
        if (val) {
          printf("on\n");
        } else {
          printf("off\n");
        }
        cfg.ghost_piece = val;
      } else if (!strcmp(command, "r_hold\0")) {
        printf("Hold button: ");
        if (val) {
          KEY_HOLD = KEY_R;
          KEY_DAS = KEY_L;
          printf("R\n");
        } else {
          printf("L\n");
        }
      } else if (!strcmp(command, "ars\0")) {
        printf("Rotation system: ");
        if (val) {
          cfg.ARS = 1;
          printf("ARS\n");
        } else {
          printf("SRS\n");
        }
      } else if (!strcmp(command, "next_displayed\0")) {
        if (val >= 0 && val < 7) {
          printf("next displayed pieces no. %d\n", val);
          cfg.next_displayed = val;
        }
      }

      else if (!strcmp(command, "are_delay\0")) {
        printf("ARE delay = %d frames\n", val);
        cfg.ARE_delay = val;
      } else if (!strcmp(command, "DAS_speed\0")) {
        printf("DAS speed = %d frames\n", val);
        cfg.DAS_speed = val;
      } else if (!strcmp(command, "line_clear_frms\0")) {
        printf("Line clear lasts = %d frames\n", val);
        cfg.line_clear_frames = val;
      } else if (!strcmp(command, "level\0")) {
        if (level > 0 && level <= 20) {
          printf("level = %d\n", val);
          config_lvl = val;
        }
      } else if (!strcmp(command, "lines_per_lvl\0")) {
        if (val > 0) {
          printf("lines per level = %d\n", val);
          cfg.lines_per_lvl = val;
        }
      }
    } else if (sscanf(buffer, "%15s %60s", command, theme_folder_name) == 2) {
      if (!strcmp("theme", command)) {
        ;
      }
      {
        printf("theme %s\n", theme_folder_name);
        sprintf(theme_template, "sdmc:/fbwodata/%s/%%s", theme_folder_name);
      }
    }
  }
  fclose(config_file);
}

void tetris_control(u32 kDown) {
  if (kDown & KEY_START) {
    if (!start_held) {
      if (gameover) {
        playable = 0;
        return;
      }
      paused = !paused;
      if (!paused) audio_music_play();
      start_held = 1;
    }
  } else
    start_held = 0;
  if (kDown & KEY_SELECT) {
    if (!gameover && (kDown & KEY_START)) playable = 0;
    if (gameover && playable) restartpls = 1;
  }
  if (!paused && controllable && !gameover) {
    if (kDown & KEY_A) {
      if (!A_held) {
        rotate_clockwise();
        A_held = 1;
      }
      if (ARE_state) {
        ARE_cw();
      }
    } else
      A_held = 0;
    if (kDown & KEY_B) {
      if (!B_held) {
        rotate_counterclockwise();
        B_held = 1;
      }
      if (ARE_state) {
        ARE_ccw();
      }
    } else
      B_held = 0;
    if (kDown & KEY_HOLD && cfg.hold) {
      if (!HOLD_held && !cfg.ARS) {
        if (!ARE_state) do_hold();
        HOLD_held = 1;
      } else if (ARE_state) {
        ARE_hold();
      }
    } else
      HOLD_held = 0;
    if (kDown & KEY_UP) {
      if (!UP_held) {
        go_all_down();
        UP_held = 1;
      }
    } else
      UP_held = 0;
    if (kDown & KEY_DOWN && !ARE_state) {
      soft_drop();
    }
    if (kDown & KEY_RIGHT) {
      if (!RIGHT_pressed) {
        if (!ARE_state) go_right();
        RIGHT_DAS_count--;
        RIGHT_pressed = 1;
      } else {
        if (RIGHT_DAS_count <= 0) {
          // yes, it can go below 0
          if (RIGHT_DAS_speed_count == 0 && !ARE_state) {
            go_right();
          }
          if (kDown & KEY_DAS)
            if (RIGHT_DAS_speed_count > (cfg.DAS_speed >> 1))
              RIGHT_DAS_speed_count = 0;
            else
              // boost the DAS!
              RIGHT_DAS_speed_count =
                  (RIGHT_DAS_speed_count + 1) % (cfg.DAS_speed >> 1);
          else
            RIGHT_DAS_speed_count = (RIGHT_DAS_speed_count + 1) % cfg.DAS_speed;
        } else {
          // boost the DAS!
          if (kDown & KEY_DAS) RIGHT_DAS_count--;
          RIGHT_DAS_count--;
        }  // end RIGHT_DAS_count if
      }    // end right pressed if
    } else {
      // reset values
      RIGHT_DAS_count = cfg.DAS;
      RIGHT_DAS_speed_count = 0;
      RIGHT_pressed = 0;
    }  // end right if
    if (kDown & KEY_LEFT) {
      if (!LEFT_pressed) {
        if (!ARE_state) go_left();
        LEFT_DAS_count--;
        LEFT_pressed = 1;
      } else {
        if (LEFT_DAS_count <= 0) {
          if (LEFT_DAS_speed_count == 0 && !ARE_state) {
            go_left();
          }
          if (kDown & KEY_DAS)
            if (LEFT_DAS_speed_count > (cfg.DAS_speed >> 1))
              LEFT_DAS_speed_count = 0;
            else
              // boost the DAS!
              LEFT_DAS_speed_count =
                  (LEFT_DAS_speed_count + 1) % (cfg.DAS_speed >> 1);
          else
            LEFT_DAS_speed_count = (LEFT_DAS_speed_count + 1) % cfg.DAS_speed;
        } else {
          if (kDown & KEY_DAS)
            // boost the DAS!
            LEFT_DAS_count--;
          LEFT_DAS_count--;
        }  // end LEFT_DAS_count if
      }    // end LEFT pressed if
    } else {
      // reset values
      LEFT_DAS_count = cfg.DAS;
      LEFT_DAS_speed_count = 0;
      LEFT_pressed = 0;
    }  // end LEFT if
  }
}

int main() {
  graphics_init();
  // init config w/ def. values
  cfg.DAS = 11;
  cfg.DAS_speed = 6;
  cfg.next_displayed = 5;
  cfg.invisimode = 0;
  cfg.hold = 1;
  cfg.line_clear_frames = 40;
  cfg.lines_per_lvl = 10;
  cfg.ARS = 0;
  cfg.ARE_delay = 0;
  level = 1;
  // level:             1   2   3   4   5   6   7   8  9  10 11 12 13 14 15 16
  // 17 19 20
  static const u32 fpd[] = {30, 28, 27, 24, 20, 15, 10, 8, 5, 3,
                            2,  1,  1,  1,  1,  1,  1,  1, 1};
  memcpy(cfg.frames_per_drop, fpd, sizeof(u32) * 20);
  // level:             1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17  18
  // 19  20
  static const u32 rpd[] = {1, 1, 1, 1, 1, 1, 1,  1,  1,  1,
                            1, 1, 2, 4, 6, 8, 10, 12, 15, 20};
  memcpy(cfg.rows_per_drop, rpd, sizeof(u32) * 20);

  static const u32 gd[] = {30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
                           30, 30, 30, 30, 30, 30, 30, 30, 30, 30};
  memcpy(cfg.glue_delay, gd, sizeof(u32) * 20);

  // load textures

  printf("reading config...\n");
  FILE* config = fopen("sdmc:/fbwodata/config.cfg", "r");
  if (config != NULL) {
    parse_config(config);
  } else {
    printf("failed to read config! default values will be used.\n");
  }
  // some of the loading operations will be in
  // another thread so maybe it will be a bit faster
  audio_init(theme_template);

  if (!load_textures(theme_template)) {
    goto texture_error;
  }
  RIGHT_DAS_count = cfg.DAS;
  LEFT_DAS_count = cfg.DAS;

  graphics_parse_config(theme_template);

// game init
init:
  initialize_game();
  paused = 1;
  playable = 1;
  restartpls = 0;
  level = config_lvl;
  load_highscore();
  gameover = 0;

  printf("Game ready! Press START to uh.. start?\n");

  while (aptMainLoop() && playable) {
    // controls
    hidScanInput();
    u32 kDown = hidKeysHeld();
    switch (mode) {
      case MODE_TETRIS:
        tetris_control(kDown);
        if (!paused && !gameover) {
          audio_music_check();
          if (controllable) do_gravity();
        } else
          audio_music_pause();
        render_frames();

        break;
      // the following are still in "to-do" - not critical to gameplay
      case MODE_MENU:
        break;
      case MODE_SETTINGS:
        break;
    }
    sf2d_swapbuffers();
  }
  if (restartpls) goto init;
  goto exit;

texture_error:
  printf("error loading textures! missing files?\n");

  printf("press START to exit.\n");
  while (aptMainLoop()) {
    hidScanInput();
    u32 kDown = hidKeysDown();
    if (kDown & KEY_START) break;  // break in order to return to hbmenu
  }

exit:
  save_highscore();
  printf("exitting...\n");
  graphics_fini();
  audio_fini();
  return 0;
}
