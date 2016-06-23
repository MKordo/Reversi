/* better but evaluate move needs work */

#include <stdio.h>
#include <stdlib.h>

#include "minmax.h"
#include "input.h"

extern int n;
extern int dif;
extern int color;
extern int option;

int boardindex(int i, int j) {
	return i*n + j;
}

int checkbounds(int i, int j) {	  		  /* returns 0 if we get out of board */
	return (0 <= i && i < n && 0 <= j && j < n);
}

/* checks if cell (a,b) on the board, is a legal move for the provided turn.  */
int checkiflegalmove(int a, int b, char *board, int turn) {

	int i, j;
	char mycolor, othercolor;
	int rowstep, colstep;

	if(turn == WHITE) {
		mycolor = 'w';
		othercolor = 'b';
	}
	else {
		mycolor = 'b';
		othercolor = 'w';
	}
/*
A = move to check
     -1   0   1
    +---+---+---+
 -1 |   |   |   |
    +---+---+---+ 	In order to find out if a potential move is valid or
  0 |   | A |   |  	not, the algorithm will move towards 8 directories
    +---+---+---+   [(0,0)->(0,1), (0,0)->(1,1), (0,0)->(-1,1),...].
  1 |   |   |   |
    +---+---+---+
*/
							 			 /* if the square that you want to	  */
							 			 /* play the move isn't empty or it is*/
							 			 /* off bounds your move is invalid	  */
	if (board[boardindex(a, b)] != 'n' || !checkbounds(a, b))
	    return 0;

	for(rowstep = -1; rowstep <= 1; ++rowstep)					  /* row step */
		for(colstep = -1; colstep <= 1; ++colstep) {		   /* column step */

			i = a;
			j = b;

			if(rowstep == 0 && colstep == 0)
				continue;
			
														/* choose a direction */
			if(checkbounds(i + rowstep, j + colstep) &&
				board[boardindex(i + rowstep, j + colstep)] == othercolor) {
				i += rowstep;				/* move towards in this direction */
				j += colstep;
				
				while(checkbounds(i + rowstep, j + colstep) &&
				board[boardindex(i + rowstep, j + colstep)] == othercolor) {
					i += rowstep;		/* and don't stop as long as you meet */
					j += colstep;						  /* opponent's color */
				}
				
				if(checkbounds(i + rowstep, j + colstep) &&
                    board[boardindex(i + rowstep, j + colstep)] == mycolor)
                    return 1;
			}
		}
	return 0;
}

/* reverses color of squares */
int playmove(int a, int b, char *board, int turn) {

	int i, j;
	char mycolor, othercolor;
	int rowstep, colstep;

	if(turn == WHITE) {
		mycolor = 'w';
		othercolor = 'b';
	}
	else {
		mycolor = 'b';
		othercolor = 'w';
	}

	for(rowstep = -1; rowstep <= 1; ++rowstep)
		for(colstep = -1; colstep <= 1; ++colstep) {
			i = a;
			j = b;
			if (rowstep == 0 && colstep == 0)
				    continue;
			if(checkbounds(i + rowstep, j + colstep) &&
				board[boardindex(i + rowstep, j + colstep)] == othercolor) {
				i += rowstep;
				j += colstep;
				while(checkbounds(i + rowstep, j + colstep) &&
				board[boardindex(i + rowstep, j + colstep)] == othercolor) {
					i += rowstep;
					j += colstep;
				}
			}
			else
				continue;
			if(checkbounds(i + rowstep, j + colstep) &&
					board[boardindex(i + rowstep, j + colstep)] == mycolor) {
				/* reaching this point means that we just checked a direction */
							/* in which, color of opponent should be reversed */
				i = a;				  /* so get at start of this direction... */
				j = b;
				board[boardindex(i, j)] = mycolor;				/* play move, */
				while(board[boardindex(i+rowstep, j+colstep)] == othercolor){
					board[boardindex(i+rowstep, j+colstep)] = mycolor;
					i += rowstep;
					j += colstep;
				}						 /* and reverse your rival's color!!! */
			}
		}
	return 1;
}

int player_has_legal_moves (int player, char *board) {

	int i, j, k;

	for (i=0, k=0; i<n; i++)
		for (j=0; j<n; j++)
			if ( checkiflegalmove(i, j, board, player) )
				k++;
				
	if (k)
	    return 1;			/* if player has at least one legal move return 1 */
	else
	    return 0;											 /* else return 0 */
}

int evaluate_positions_of_board(int *matrix) {

/*
     a   b   c   d   e   f   g   h
   +---+---+---+---+---+---+---+---+
 1 |100|   |   |   |   |   |   |   |
   +---+---+---+---+---+---+---+---+
 2 |-8 |-25|   |   |   |   |   |   |
   +---+---+---+---+---+---+---+---+
 3 | 8 |-4 | 7 |   |   |   |   |   |
   +---+---+---+---+---+---+---+---+
 4 | 6 |-3 | 4 | 0 |   |   |   |   |
   +---+---+---+---+---+---+---+---+
 5 |   |   |   |   |   |   |   |   |
   +---+---+---+---+---+---+---+---+
 6 |   |   |   |   |   |   |   |   |
   +---+---+---+---+---+---+---+---+
 7 |   |   |   |   |   |   |   |   |
   +---+---+---+---+---+---+---+---+
 8 |   |   |   |   |   |   |   |   |
   +---+---+---+---+---+---+---+---+
*/
	int i,j;

	for (i=0; i<n; i++)
	    for (j=0; j<n; j++)
	    	matrix[boardindex(i, j)] = 0;

	if (n>=8) {
		matrix[boardindex(0, 0)] = 100;
		matrix[boardindex(1, 0)] = -8;
		matrix[boardindex(1, 1)] = -25;
		matrix[boardindex(2, 0)] = 8;
		matrix[boardindex(2, 1)] = -4;
		matrix[boardindex(2, 2)] = 7;
		for (i=3, j=0; i<(n/2); i++)
			matrix[boardindex(i,j)] = 6;
		for (i=3, j=1; i<(n/2); i++)
			matrix[boardindex(i,j)] = -3;
		for (i=3; i<(n/2); i++)
		    for (j=2; j<(n/2)-1; j++)
				matrix[boardindex(i,j)] = 4;
		matrix[boardindex((n/2)-1, (n/2-1))] = 0;

		for (i=0; i<n/2; i++)
		    for (j=0; j<i; j++)
		    	matrix[boardindex(j,i)] = matrix[boardindex(i,j)];

		for (i=0; i<n/2; i++)
		    for (j=0; j<n/2; j++)
		    	matrix[boardindex(n-1-i,j)] = matrix[boardindex(i,j)];
	    	
		 for (i=0; i<n; i++)
		    for (j=0; j<n/2; j++)
		    	matrix[boardindex(i,n-1-j)] = matrix[boardindex(i,j)];
	}

/*	printf("     ");
	for (j=0; j<n; j++)
		printf("%c   ", 'a' + j);
	printf("\n   ");
	for (i=0; i<n; i++)
		printf("+---");
	printf("+\n");
	for (i=0; i<n; i++) {
		printf("%2d |", i+1);
		for (j=0; j<n; j++) {
			if (matrix[boardindex(i,j)] == 100 || matrix[boardindex(i,j)] < 0)
				printf("%-3d|", matrix[boardindex(i,j)]);
			else
				printf(" %-2d|", matrix[boardindex(i,j)]);

		}
		printf("\n   ");
		for (j=0; j<n; j++)
		    printf("+---");
			printf("+\n");
	}*/
	return 1;
}

/* counts how many black and white checkers are on the board 				  */
int count_checkers (char *board, int *numwhite, int *numblack) {

	int i, j;
	
	for (i=0; i<n; i++)
	    for (j=0; j<n; j++)
			switch ( board[boardindex(i,j)] ) {
				case 'w': ++*numwhite;
				    break;
				case 'b': ++*numblack;
				    break;
				default:				 /* case that content of board is 'n' */
					break;
			}

	if ( *numwhite == 2 && *numblack == 2 )			   /* if state is newgame */
	    return 0;
	else
	    return 1;
}

int evaluatemove(char *board, int *valueBoard, int mycolor) {

	int player = 0;
	int opponent = 0;
	int i,j;

	if (n>=8){
		if(mycolor == WHITE)
			for (i=0; i<n; i++)
			    for (j=0; j<n; j++)
					switch ( board[boardindex(i,j)] ) {
						case 'w': player += valueBoard[boardindex(i,j)];
						    break;
						case 'b': opponent += valueBoard[boardindex(i,j)];
						    break;
						default:				 /* case that content of board is 'n' */
							break;
					}
		else
			for (i=0; i<n; i++)
		    	for (j=0; j<n; j++)
					switch ( board[boardindex(i,j)] ) {
						case 'w': opponent += valueBoard[boardindex(i,j)];
						    break;
						case 'b': player += valueBoard[boardindex(i,j)];
					    	break;
						default:				 /* case that content of board is 'n' */
							break;
					}
	}
	else if (n==4 || n==6) {
        if(mycolor == WHITE)
        	count_checkers (board, &player, &opponent);
		else
            count_checkers (board, &opponent, &player);
	}
	
	return (player-opponent);
}

/* recursive function that predicts the possible moves   */
/* after the move that the player played.                */
/* Examine_deeper is quite similar to function best_move */
int examine_deeper (int level, char *board, int* valueBoard, int turn, int player)
{
	int i, j, k=0;
	int numblack=0;
	int numwhite=0;
	int new_score = 0;							  /* score of each legal move */
	int score = 1000;						  /* the worse record from a move */
	char *array;							/* this is gonna be a trial array */
											/* and it will be a copy of board */
	for (k=0, i=0; i<n; i++)
		for (j=0; j<n; j++)
			if ( checkiflegalmove (i, j, board, turn) )
			{	k++;						   /* count number of valid moves */
				array = makeBoard(board);
				playmove(i,j,array,turn);

				if (level > 1)
					new_score = examine_deeper (level-1,array,valueBoard,1-turn,player);
				else
					new_score = evaluatemove(array,valueBoard,player);

				if ( score > new_score )
					score = new_score;
					
				free(array);
   			}
	if (k==0) {						   /* if current player can't play a move */
													/* maybe the opponent can */
		if ( player_has_legal_moves (1-turn, board) ) {				/* yes??? */
		    if ( level>1 )
				score = examine_deeper (level-1,board,valueBoard,1-turn,player);
													   /* examine deeper then */
			else
				score = evaluatemove(board,valueBoard,player);
		}
		else {														 /* no??? */
													/* then game has finished */
			count_checkers (board,&numwhite,&numblack);
				if ( player == WHITE ) {						  /* who won? */
					if ( numwhite > numblack )				   /* was it you? */
						return 900;								   /* yeah!!! */
					else								   /* what? you lose? */
					    return -900;	 /* you'd better avoid this move then */
				}
				else {
					if ( numblack > numwhite )
					    return 900;
					else
					    return -900;
				}
		}
	}

	return score;
}

/* decides which is the best move for the player in provided turn 			  */
int best_move (char *board, int* valueBoard, int turn, int *a, int *b)
{
	int i, j, random = -1;
	int level;
	int new_score = 0;						/* score of each legal move		  */
	int score = -1000;						/* the best record from a move	  */
	char *array;							/* this is gonna be a trial array */
											/* and it will be a copy of board */
	level = dif;
	for (i=0; i<n; i++)
		for (j=0; j<n; j++)
			if ( checkiflegalmove (i, j, board, turn) )
			{
				array = makeBoard(board);
				playmove(i,j,array,turn);

				if (level > 1)
					new_score = examine_deeper (level-1,array,valueBoard,1-turn,turn);
				if( level==1 || new_score == 1000 ) {
					new_score = evaluatemove(array,valueBoard,turn);
				}
										 /* to compare it with rest of scores */
				if( score < new_score )
      	 		{
					score = new_score;
					*a = i;
					*b = j;
      			}
						  /* in case that two legal moves have the same score *
						   * we will decide by luck which one will be played  */
       			else if ( score == new_score ) {
       			    random = rand() % 10;
					if ( random < 5 ) {
					    score = new_score;
						*a = i;
						*b = j;
					}
				}
				free(array);
			}
	return 1;
}

/* makes a new board each time that game's state changes,   */
/* in order not to change the content of the previous board */
char *makeBoard(char *board) {

	char *newMatrix;
	int i, j;

	newMatrix = malloc (n * n * sizeof(char));			 /* create new matrix */
		if (newMatrix == NULL)
			return NULL;

	for (i=0; i<n; i++)
		for (j=0; j<n; j++)
			newMatrix[boardindex(i,j)] = board[boardindex(i,j)];
return newMatrix;
}
