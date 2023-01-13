#ifndef __CONFIG_H__
#define __CONFIG_H__

#define BOARD_ROWS 4
#define BOARD_COLUMNS 8

#define BEADS_INIT 2

#define MAX_BEADS BOARD_ROWS *BOARD_COLUMNS *BEADS_INIT

#define PLAYER1 1
#define PLAYER2 2

#define PLAYER1_HOMEROW 1
#define PLAYER2_HOMEROW 2

#define ANN_SAVE_NAME "./ann"

#define MEMBERS_PER_GEN 16

#define HIDDEN_LAYERS 16
#define NODES_PER_HIDDEN_LAYER 1024

#endif
