#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <config.h>
#include <bao.h>
#include <math.h>

#define MAX(X, Y) (((X) >= (Y)) ? (X) : (Y))
#define MIN(X, Y) (((X) <= (Y)) ? (X) : (Y))

static inline size_t opposite_homerow(size_t row) {
  if (row == PLAYER1_HOMEROW)
    return PLAYER2_HOMEROW;
  else if (row == PLAYER2_HOMEROW)
    return PLAYER1_HOMEROW;
  return 0;
}

void bao_print(board_t *board) {
  for (size_t i = 0; i < BOARD_COLUMNS + 1; i++)
    fputs("---", stdout);
  putc('\n', stdout);
  for (size_t i = 0; i < BOARD_ROWS; i++) {
    putc('|', stdout);
    for (size_t j = 0; j < BOARD_COLUMNS; j++)
      printf("%3u", board->cboard[i][j]);
    fputs(" |", stdout);
    printf("\n");
    if (i == 1) {
      putc('|', stdout);
      for (size_t i = 0; i < BOARD_COLUMNS; i++)
        fputs("   ", stdout);
      fputs(" |", stdout);
      printf("\n");
    }
  }
  for (size_t i = 0; i < BOARD_COLUMNS + 1; i++)
    fputs("---", stdout);
  putc('\n', stdout);
}

void bao_print_highlight(board_t *board, size_t row, size_t col) {
  for (size_t i = 0; i < BOARD_COLUMNS + 1; i++)
    fputs("---", stdout);
  putc('\n', stdout);
  for (size_t i = 0; i < BOARD_ROWS; i++) {
    putc('|', stdout);
    for (size_t j = 0; j < BOARD_COLUMNS; j++)
      printf("%s%3u%s", (i == row && j == col) ? "\033[7m" : "", board->cboard[i][j], (i == row && j == col) ? "\033[m" : "");
    fputs(" |", stdout);
    printf("\n");
    if (i == 1) {
      putc('|', stdout);
      for (size_t i = 0; i < BOARD_COLUMNS; i++)
        fputs("   ", stdout);
      fputs(" |", stdout);
      printf("\n");
    }
  }
  for (size_t i = 0; i < BOARD_COLUMNS + 1; i++)
    fputs("---", stdout);
  putc('\n', stdout);
}

void bao_fen(board_t *board) {
  for (size_t i = 0; i < BOARD_ROWS * BOARD_COLUMNS; i++)
    printf("%u%s", board->lboard[i], (i != BOARD_ROWS * BOARD_COLUMNS - 1) ? "," : "");
  printf("\n");
}

void bao_load_fen(board_t *board, char *_fen) {
  size_t i = 0;
  char *save = NULL;
  char *__fen = strdup(_fen);
  char *fen = __fen;
  for (char *tok = strtok_r(fen, ",", &save); i < 40; tok = (tok) ? strtok_r(NULL, ",", &save) : NULL, i++)
    board->lboard[i] = (tok && *tok) ? atoi(tok) : 0;
  free(__fen);
}

int bao_init(board_t *board) {
  memset(board->lboard, BEADS_INIT, sizeof(board->lboard));
  return 0;
}

int bao_zero(board_t *board) {
  memset(board->lboard, 0, sizeof(board->lboard));
  return 0;
}

int bao_check_win(board_t *board) {
  size_t total = 0;
  for (size_t i = 0; i < BOARD_COLUMNS; i++)
    total += board->cboard[PLAYER1_HOMEROW][i];
  if (!total)
    return PLAYER2;

  total = 0;
  for (size_t i = 0; i < BOARD_COLUMNS; i++)
    total += board->cboard[PLAYER2_HOMEROW][i];
  if (!total)
    return PLAYER1;

  size_t max = 0;
  for (size_t i = 0; i < BOARD_COLUMNS; i++)
    for (size_t j = 0; j < BOARD_ROWS / 2; j++)
      max = MAX(max, board->cboard[j][i]);
  if (max < 2)
    return PLAYER2;
  
  max = 0;
  for (size_t i = 0; i < BOARD_COLUMNS; i++)
    for (size_t j = BOARD_ROWS / 2; j < BOARD_ROWS; j++)
      max = MAX(max, board->cboard[j][i]);
  if (max < 2)
    return PLAYER1;

  return 0;
}

static int __int_bao_move(board_t *board, ssize_t row, ssize_t col, int dir) {
  size_t c = board->cboard[row][col];
  board->cboard[row][col] = 0;

  col += dir;

  if (col >= BOARD_COLUMNS || col < 0) {
    col -= dir;
    if (row % 2)
      row--;
    else
      row++;
    dir *= -1;
  }

  for (size_t i = 0; i < c; i++) {
    board->cboard[row][col]++;

    col += dir;
    if (col >= BOARD_COLUMNS || col < 0) {
      col -= dir;
      if (row % 2)
        row--;
      else
        row++;
      dir *= -1;
    }
  }

  col -= dir;

  if (col >= BOARD_COLUMNS || col < 0) {
    col += dir;
    if (row % 2)
      row--;
    else
      row++;
    dir *= -1;
  }

  if (board->cboard[row][col] == 1)
    return 0;

  while (row == PLAYER1_HOMEROW || row == PLAYER2_HOMEROW) {
    size_t opp = opposite_homerow(row);

    if (board->cboard[opp][col]) {
      size_t c = board->cboard[opp][col];
      board->cboard[opp][col] = 0;
  
      if (col >= 6 && dir > 0)
        dir *= -1;
      else if (col < 2 && dir < 0)
        dir *= -1;
      
      if (dir > 0)
        col = 0;
      else if (dir < 0)
        col = 7;
      
      for (size_t i = 0; i < c; i++) {
        board->cboard[row][col]++;

        col += dir;
        if (col >= BOARD_COLUMNS || col < 0) {
          col -= dir;
          if (row % 2)
            row--;
          else
            row++;
          dir *= -1;
        }
      }
  
      col -= dir;

      if (col >= BOARD_COLUMNS || col < 0) {
        col += dir;
        if (row % 2)
          row--;
        else
          row++;
        dir *= -1;
      }
  
      if (board->cboard[row][col] == 1)
        return 0;
    }
    else 
      break;
  }
  
  return __int_bao_move(board, row, col, dir);
}

int bao_move(board_t *board, ssize_t row, ssize_t col, int dir) {
  if (!dir)
    return -1;
  if (row >= BOARD_ROWS || col >= BOARD_COLUMNS)
    return -1;
  if (board->cboard[row][col] == 1 || board->cboard[row][col] == 0)
    return -1;

  return __int_bao_move(board, row, col, dir);
}

int bao_is_legal(board_t *board, ssize_t row, ssize_t col) {
  if (row >= BOARD_ROWS / 2 || col >= BOARD_COLUMNS)
    return 0;
  if (board->cboard[row][col] == 1 || board->cboard[row][col] == 0)
    return 0;

  return 1;
}

int bao_is_all_legal(board_t *board, ssize_t row, ssize_t col) {
  if (row >= BOARD_ROWS || col >= BOARD_COLUMNS)
    return 0;
  if (board->cboard[row][col] == 1 || board->cboard[row][col] == 0)
    return 0;

  return 1;
}

void bao_flip(board_t *board) {
  typeof(*board) b;
  memcpy(b.lboard, board->lboard, sizeof(*board));
  for (size_t i = 0; i < BOARD_COLUMNS * BOARD_ROWS; i++)
    board->lboard[i] = b.lboard[BOARD_ROWS * BOARD_COLUMNS - 1 - i];
}

static double bao_eval(board_t *board) {
  int w = bao_check_win(board);
  if (w == PLAYER1)
    return VALUE_WIN;
  else if (w == PLAYER2) 
    return -VALUE_WIN;

  double ret = 0;
  for (size_t i = 0; i < BOARD_COLUMNS * BOARD_ROWS / 2; i++)
    ret += board->lboard[i];
  for (size_t i = BOARD_COLUMNS * BOARD_ROWS / 2; i < BOARD_COLUMNS * BOARD_ROWS; i++)
    ret -= board->lboard[i];
  return ret;
}

double bao_alphabeta_minimax_search(board_t *board, size_t depth, size_t orig_depth, double a, double b, int north) {
  int e = bao_check_win(board);
  if (e) {
    double r = (double)VALUE_WIN - orig_depth + depth;
    if (e == PLAYER2)
      r *= -1;
    return r;
  }
  if (!depth)
    return bao_eval(board);

  /*a = MAX(-((double)VALUE_WIN - orig_depth + depth), a);*/
  /*b = MIN(((double)VALUE_WIN - orig_depth + depth + 1), b);*/
  /*if (a >= b)*/
    /*return a;*/

  if (north) {
    double val = -VALUE_INF;
    typeof(*board) bo;
    memcpy(bo.lboard, board->lboard, sizeof(board->lboard));

    for (size_t i = 0; i < BOARD_COLUMNS * BOARD_ROWS; i++) {
      if (!bao_is_all_legal(board, (i / 2) / BOARD_COLUMNS, (i / 2) % BOARD_COLUMNS))
        continue;

      bao_move(board, (i / 2) / BOARD_COLUMNS, (i / 2) % BOARD_COLUMNS, (i % 2) * 2 - 1);
      val = MAX(val, bao_alphabeta_minimax_search(board, depth - 1, orig_depth, a, b, 0));
      memcpy(board->lboard, bo.lboard, sizeof(board->lboard));
      
      a = MAX(a, val);
      if (val >= b)
        break;
    }

    return val;
  }
  else {
    double val = VALUE_INF;
    typeof(*board) bo;
    memcpy(bo.lboard, board->lboard, sizeof(board->lboard));

    for (size_t i = 0; i < BOARD_COLUMNS * BOARD_ROWS; i++) {
      if (!bao_is_all_legal(board, (i / 2) / BOARD_COLUMNS + (BOARD_ROWS / 2), (i / 2) % BOARD_COLUMNS))
        continue;

      bao_move(board, (i / 2) / BOARD_COLUMNS + (BOARD_ROWS / 2), (i / 2) % BOARD_COLUMNS, (i % 2) * 2 - 1);
      val = MIN(val, bao_alphabeta_minimax_search(board, depth - 1, orig_depth, a, b, 1));
      memcpy(board->lboard, bo.lboard, sizeof(board->lboard));
      
      b = MIN(b, val);
      if (val <= a)
        break;
    }
    
    return val;
  }
}

/*
function pvs(node, depth, α, β, color) is
    if depth = 0 or node is a terminal node then
        return color × the heuristic value of node
    for each child of node do
        if child is first child then
            score := −pvs(child, depth − 1, −β, −α, −color)
        else
            score := −pvs(child, depth − 1, −α − 1, −α, −color) (* search with a null window *)
            if α < score < β then
                score := −pvs(child, depth − 1, −β, −score, −color) (* if it failed high, do a full re-search *)
        α := max(α, score)
        if α ≥ β then
            break (* beta cut-off *)
    return α
 */

/*typedef struct compact_moves {*/
  /*uint16_t dir : 1;*/
  /*uint16_t lboard_move : 5;*/
  /*double eval;*/
/*} compact_moves_t;*/

/*int move_compr(const void *a0, const void *a1) {*/
  /*if (((compact_moves_t *)a0)->eval == ((compact_moves_t *)a1)->eval)*/
    /*return 0;*/
  /*if (((compact_moves_t *)a0)->eval < ((compact_moves_t *)a1)->eval)*/
    /*return 1;*/
  /*return -1;*/
/*}*/

/*double bao_alphabeta_negascout_search(board_t *board, size_t depth, size_t orig_depth, double a, double b, int color) {*/
  /*if (!depth || bao_check_win(board)) {*/
    /*double e = bao_eval(board);*/
    /*printf("Ret %lf\n", e);*/
    /*return e;*/
  /*}*/

  /*[>size_t ply = orig_depth - depth + 1;<]*/
  /*[>a = MAX(VALUE_WIN - ply, a);<]*/
  /*[>b = MIN(-VALUE_WIN + ply + 1, b);<]*/
  /*[>if (a >= b)<]*/
    /*[>return a;<]*/

  /*typeof(*board) bo;*/
  /*memcpy(bo.lboard, board->lboard, sizeof(board->lboard));*/

  /*compact_moves_t moves[32] = {0};*/
  /*size_t off = 0;*/
  /*size_t len = 0;*/
  /*for (size_t i = 0; i < BOARD_COLUMNS * BOARD_ROWS; i++) {*/
    /*size_t row_inc = (color == -1) ? 2 : 0;*/
    /*if (!bao_is_all_legal(board, (i / 2) / BOARD_COLUMNS + row_inc, (i / 2) % BOARD_COLUMNS)) {*/
      /*off++;*/
      /*continue;*/
    /*}*/
    /*bao_move(board, (i / 2) / BOARD_COLUMNS + row_inc, (i / 2) % BOARD_COLUMNS, i % 2);*/
    /*moves[i - off].eval = bao_eval(board);*/
    /*moves[i - off].dir = (i >= BOARD_COLUMNS * 2);*/
    /*moves[i - off].lboard_move = (i >= BOARD_COLUMNS * 2) ? i - BOARD_COLUMNS * 2 : i;*/
    /*memcpy(board->lboard, bo.lboard, sizeof(board->lboard));*/
    /*len++;*/
  /*}*/

  /*qsort(moves, len, sizeof(compact_moves_t), move_compr);*/
    
  /*memcpy(board->lboard, bo.lboard, sizeof(board->lboard));*/

  /*for (size_t inc = 0; inc < len; inc++) {*/
    /*double val = 0;*/

    /*[>size_t row_inc = (color == -1) ? 2 : 0;<]*/
    /*[>if (!bao_is_all_legal(board, (i / 2) / BOARD_COLUMNS + row_inc, (i / 2) % BOARD_COLUMNS))<]*/
      /*[>continue;<]*/
    /*[>bao_move(board, (i / 2) / BOARD_COLUMNS + row_inc, (i / 2) % BOARD_COLUMNS, i % 2);<]*/

    /*size_t i = (color == 1) ? inc : len - inc;*/
    /*size_t row = moves[i].lboard_move / BOARD_COLUMNS; */
    /*size_t col = moves[i].lboard_move % BOARD_COLUMNS;*/
    /*int dir = (moves[i].dir) ? 1 : -1;*/

    /*bao_move(board, row, col, dir);*/

    /*printf("Trying: %lu:%lu:%d\n", row, col, dir);*/

    /*if (!i)*/
      /*val = -bao_alphabeta_negascout_search(board, depth -1, orig_depth, -b, -a, -color);*/
    /*else {*/
      /*val = -bao_alphabeta_negascout_search(board, depth -1, orig_depth, -a - 1, -a, -color);*/
      /*memcpy(board->lboard, bo.lboard, sizeof(board->lboard));*/
      /*if (a < val && val < b)*/
        /*val = -bao_alphabeta_negascout_search(board, depth -1, orig_depth, -b, -val, -color);*/
    /*}*/
    /*a = MAX(a, val);*/

    /*memcpy(board->lboard, bo.lboard, sizeof(board->lboard));*/

    /*if (a >= b)*/
      /*break;*/
  /*}*/

  /*return a;*/
/*}*/
