#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <sf2d.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "level.h"
#include "structs.h"

#define DEFAULT_GRIDX 150
#define DEFAULT_GRIDY 20

#define DEFAULT_NEXT_TEXTX 260
#define DEFAULT_NEXT_TEXTY 55

#define DEFAULT_NEXT_FRAME_1X 260
#define DEFAULT_NEXT_FRAME_1Y 20

#define DEFAULT_NEXT_FRAME_2X 295
#define DEFAULT_NEXT_FRAME_2Y 20

#define DEFAULT_NEXT_FRAME_3X 330
#define DEFAULT_NEXT_FRAME_3Y 20

#define DEFAULT_NEXT_FRAME_4X 365
#define DEFAULT_NEXT_FRAME_4Y 20

#define DEFAULT_NEXT_FRAME_5X 365
#define DEFAULT_NEXT_FRAME_5Y 60

#define DEFAULT_NEXT_FRAME_6X 365
#define DEFAULT_NEXT_FRAME_6Y 100

#define DEFAULT_SCORE_TEXTX 280
#define DEFAULT_SCORE_TEXTY 210

#define DEFAULT_HISCORE_TEXTX 0
#define DEFAULT_HISCORE_TEXTY 210

#define DEFAULT_LINES_FRAMEX 115
#define DEFAULT_LINES_FRAMEY 140

#define DEFAULT_LEVEL_FRAMEX 115
#define DEFAULT_LEVEL_FRAMEY 190

#define DEFAULT_HOLD_FRAMEX 115
#define DEFAULT_HOLD_FRAMEY 20

#define DEFAULT_OFFSET_HOLDX 5
#define DEFAULT_OFFSET_HOLDY 5

#define DEFAULT_OFFSET_NEXTX 5
#define DEFAULT_OFFSET_NEXTY 5

#define DEFAULT_OFFSET_LVLY 10
#define DEFAULT_OFFSET_LNSY 10

#define DEFAULT_POPUPX 270
#define DEFAULT_POPUPY 180

#define DEFAULT_POPUP_FRAME_CFG 40

void render_grid_blocks(int);
void render_block(Tetrimino, bool, bool, int);
void render_frames();
void render_frame(int);
void graphics_parse_config(char*);
void graphics_fini();
void graphics_init();
int load_textures(const char*);

extern int level_grid[DIM_X][DIM_Y];
extern u32 score;
extern u32 high_score;
extern u32 total_lines;
extern u8 paused;
extern u8 controllable;
extern u8 back_to_back_flag;
extern u8 mode;
extern Indicator_to_render indicator;

extern Tetrimino* in_play;
extern Tetrimino* hold;

extern u8* full_lines;
extern u8 ARE_state;

extern const int rotations[6][4][3][3];
extern const int rotation_I[4][5][5];

extern Tetrimino_list* next_blocks;

#endif  // GRAPHICS_H
