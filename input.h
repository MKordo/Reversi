#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define OFF 0
#define ON 1
#define BLACK 0
#define WHITE 1

enum game_commands {NEWGM = 1, PLAY, CONT, UNDO, SUG, SELE,
			   		SHOWL, LEVEL, SAVE, LOAD, SHOWS, QUIT};
												   /* commands of game are 12 */
void parse_args(int argc, char **argv);
int recognize_command (const char *input, int *a, int *b, char **file);

#endif /* INPUT_H */
