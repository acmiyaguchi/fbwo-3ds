#ifndef LEVEL_H
#define LEVEL_H

#include <3ds.h>
#include "structs.h"

// function declaration
void save_highscore();
void load_highscore();
int check_collision(const Tetrimino);
void initialize_game();
u32 gravity_drop();
void increase_ticks();
void soft_drop();
Tetrimino get_ghost_piece();
int go_all_down();
void do_gravity();
void go_left();
void go_right();
void rotate_clockwise();
void rotate_counterclockwise();
void apply_rotation();
void glue();
void update_level();
void do_gameover();
void iterative_list_cleanup(Tetrimino_list*);
void deploy_next(bool);
void do_hold();
void deploy_hold();
int check_lines();
int check_gameover();
void clear_lines();
void move_down();
Tetrimino_list* generate_bag();
int rand_lim(int);
void ARE_tick();
void ARE_hold();
void ARE_cw();
void ARE_ccw();
void ARE_finish();
u32 T_corners_occupied();

extern Configuration cfg;

extern Tetrimino* in_play;
extern Tetrimino* hold;
extern Tetrimino* last_deployed;
extern Tetrimino_list* next_blocks;

extern u32 ticks_before_glue;
extern u32 score;
extern u32 gravity_frame_counter;
extern u32 total_lines;
extern u32 high_score;
extern u32 ARE_frames;

extern u8 back_to_back_flag;
extern u8 render_line_clear;
extern u8 next_counter;
extern u8 level;
extern u8 gameover;
extern u8 hold_last;  // flag to disallow infinite holding
extern u8 ARE_state;
extern u8 last_T_rotation;
extern u8 last_T_kick;
extern u8 back_to_back_flag_old;

extern Indicator_to_render indicator;

extern image background;
extern image next_text;
extern image next_frame[6];
extern image grid;
extern image score_text;
extern image hiscore_text;
extern image lines_frame;
extern image level_frame;
extern image hold_frame;

extern sf2d_texture* tetriminos[7];
extern sf2d_texture* blocks[7];
extern sf2d_texture* score_num[10];
extern sf2d_texture* misc_num[10];
extern sf2d_texture* gameover_text;
extern sf2d_texture* paused_text;

extern sf2d_texture* tetris_indicator;
extern sf2d_texture* tspin_indicator;
extern sf2d_texture* tspinsingle_indicator;
extern sf2d_texture* tspindouble_indicator;
extern sf2d_texture* tspintriple_indicator;
extern sf2d_texture* backtoback_indicator;

extern s32 block_offset_holdx;
extern s32 block_offset_holdy;

extern s32 block_offset_nextx;
extern s32 block_offset_nexty;

extern s32 remove_line_count;

extern s32 digit_offset_linesy;
extern s32 digit_offset_levely;

extern s32 indicatorx;
extern s32 indicatory;

extern s32 indicator_frames;
extern s32 indicator_frame_config;

extern const int ARS_rotations[6][4][3][3];
extern const int ARS_rotation_I[4][4][4];

#endif  // LEVEL_H
