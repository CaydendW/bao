#include <bao.h>
#include <stddef.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <config.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdint.h>
#include <sys/resource.h>

int main() {
  board_t board;
  bao_init(&board);
  size_t depth;
  scanf("%zu", &depth);
  printf("Search depth: %zu\n", depth);

  double val = bao_alphabeta_minimax_search(&board, depth, depth, -VALUE_INF, VALUE_INF, 1);
  if (val > 64 && val <= VALUE_WIN)
    printf("#%li\n", (ssize_t)((double)VALUE_WIN - val));
  else if (val < -64 && val >= -VALUE_WIN)
    printf("#-%li\n", (ssize_t)((double)VALUE_WIN + val));
  else
    printf("%lf\n", val);

  return 0;
}
