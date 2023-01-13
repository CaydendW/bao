#ifndef __BAO_H__
#define __BAO_H__

#include <stddef.h>
#include <sys/types.h>
#include <stdint.h>
#include <config.h>

#define VALUE_INF (double)((size_t)-1) 
#define VALUE_WIN 32767

typedef union board{
  uint8_t cboard[BOARD_ROWS][BOARD_COLUMNS];
  uint8_t lboard[BOARD_ROWS * BOARD_COLUMNS];
} board_t;

void bao_print(board_t *board);
void bao_print_highlight(board_t *board, size_t row, size_t col);
void bao_fen(board_t *board);
void bao_load_fen(board_t *board, char *_fen);
int bao_init(board_t *board);
int bao_zero(board_t *board);
int bao_check_win(board_t *board);
int bao_move(board_t *board, ssize_t row, ssize_t col, int dir);
int bao_is_legal(board_t *board, ssize_t row, ssize_t col);
void bao_flip(board_t *board);
double bao_alphabeta_minimax_search(board_t *board, size_t depth, size_t orig_depth, double a, double b, int north);
double bao_alphabeta_negascout_search(board_t *board, size_t depth, size_t orig_depth, double a, double b, int color);

#endif
