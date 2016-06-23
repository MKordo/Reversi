#ifndef MINMAX_H
#define MINMAX_H

#include <time.h>

int boardindex (int i, int j);
int checkbounds (int i, int j);
int checkiflegalmove (int a, int b, char *board, int turn);
int playmove (int a, int b, char *board, int turn);
int player_has_legal_moves (int player, char *board);
int evaluate_positions_of_board (int *matrix);
int count_checkers (char *board, int *numwhite, int *numblack);
int evaluatemove (char *board, int* valueBoard, int mycolor);
int examine_deeper (int level, char *board, int* valueBoard, int turn, int player);
int best_move (char *board, int* valueBoard, int turn, int *a, int *b);
char *makeBoard (char *board);

#endif /* MINMAX_H */
