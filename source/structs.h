#ifndef STRUCTS_H
#define STRUCTS_H

// actually not only structs
#define O_TYPE 0
#define T_TYPE 1
#define S_TYPE 2
#define Z_TYPE 3
#define J_TYPE 4
#define L_TYPE 5
#define I_TYPE 6

#define DIM_X 10
#define DIM_Y 24

#define LINE_FULL 1

#define MODE_TETRIS 1
#define MODE_MENU 2
#define MODE_SETTINGS 3

#include <sf2d.h>
#include <stdio.h>
#include <tremor/ivorbisfile.h>

typedef enum {
  NONE,
  TETRIS,
  TSPIN,
  TSPINSINGLE,
  TSPINDOUBLE,
  TSPINTRIPLE
} Indicator_to_render;

typedef struct {
  u32 posx;  // top left corner postition
  u32 posy;
  u8 type;
  u8 rotation;
} Tetrimino;

typedef struct Tetrimino_list {
  Tetrimino* tetrimino;
  struct Tetrimino_list* next;
} Tetrimino_list;

typedef struct {
  u32 DAS;  // delay (in frames) between pressing the button and moving the
            // block automatically
  u32 DAS_speed;       // delay (in frames) between moving the block
  u32 glue_delay[20];  // lock delay - time (in frames) between block making it
                       // to the bottom and locking it in place
  u8 hold;             // hold functionality
  u8 ghost_piece;      // rendering of the ghost piece
  u8 next_displayed;   // no. of next blocks displayed, 0-6
  u8 invisimode;       // blocks aren't rendered.
  u32 line_clear_frames;  // how many frames does the line clear animation last
  u32 frames_per_drop[20];  // how many frames per drop
  u32 rows_per_drop[20];    // how big the drop is
  u32 lines_per_lvl;  // how many lines should be cleared before the level is
                      // incremented
  u32 ARS;            // 0 for SRS, 1 for ARS (TGM rotation system)
  u32 ARE_delay;      // frames between spawning another block
} Configuration;

typedef struct {
  sf2d_texture* texture;
  s32 posx;
  s32 posy;
} image;

typedef struct {
  u32 chnl;  // channel used to play
  u32 channels;
  u32 sample_rate;
  u32 bits_per_sample;
  u32 data_size;
  u32 total_buffer_size;
  u32 last_check;
  u16 bytes_per_sample;
  u16 ndsp_format;
  ndspWaveBuf* first;
  ndspWaveBuf* second;
  u8* first_data;
  u8* second_data;
  OggVorbis_File* file;
} Music;
#endif
