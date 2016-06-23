/* File: main.c */
/* na valw EOG extern int */

#include "minmax.h"
#include "input.h"

typedef struct linkedlist ll;

struct linkedlist {
	char *data;
	int i;
	int j;
	int move;
	struct linkedlist *next;
};

int n = 8;										 /* for size of game		  */
int dif = 1;								     /* for dificulty of game	  */
int color = BLACK;								 /* for color of human player */
int option = OFF;		 /* decides if legal moves of h. player will be shown */
int game_over = 0;		 				 		   /* a flag for: END OF GAME */
int player_loses_turn = 0;

void insert_at_start(ll **list, char *data, int i, int j, int move);
void printboard(char *board, int turn, int a, int b);

int main(int argc, char **argv)
{
	ll *history = NULL, *tmp = NULL;
	FILE *statefile = NULL;
	char *board=NULL;	  		 /* martix where state of game will be kept   */
	char input[40];	  			 /* will receive the commands from stdin	  */
	char *file = NULL;	    /* will keep name of a file to be saved or loaded */
	char answer[20];
	int turn = BLACK; 			 /* decides turn of player					  */
	int command = 0;  			 /* decides which command will be executed	  */
	int i, j, k=0;	  									/* counters			  */
	int game = OFF;		  /* flag that shows if a game is being played or not */
	int *board_with_values = NULL;

	srand ( (unsigned int) time(NULL) );
						  /*  random numbers will be needed at the minmax.c file
						   *  library time.h is included by minmax.h		  */
	parse_args(argc,argv);
								 /* Management of commands during the game	  */
	while (command != QUIT){

		for (i=0;i<40;i++)
			input[i]='\0';
		fgets (input, 40, stdin);
		command = recognize_command(input,&i,&j,&file);
		
		switch ( command ) {
			case NEWGM:											   /* newgame */

/* if a new game is started while the previous one hasn't finished,			  */
/* erase history of the previous game and reset n, level, etc...			  */
				if (game == ON) {
					printf("\nYour current game hasn't finished yet.\n");
					printf("Are you sure that you want to start a new game?\n");
					printf("Type \"y\" for yes or \"n\" for no to answer.\n");
					fgets (answer, 20, stdin);
					if ( answer[0] == 'y' ) {
						while(history != NULL) {
							tmp = history;
							free(history->data);
							history = history->next;
							free(tmp);
						}
						dif = 1; color = BLACK; option = OFF; turn = BLACK;
						command = 0; k=0; file = NULL;
						printf("\nAlright then!\n");
						printf("You'll be the black player and the level ");
						printf("of difficulty will be one.\n\n");
					}
					else {
						if ( answer[0] != 'n' || answer[1] != '\n' )
							printf("Didn't I say that you should type \"y\" or \"n\"?\n");
					    printf("A new game will not be started.\n");
						break;
					}
				}
				if (n>=8) {
					board_with_values = (int *) malloc (n*n *sizeof(int));
					if (board_with_values==NULL)
	    				return -1;
					evaluate_positions_of_board(board_with_values);
				}
												 /* time to set a new game... */
				board = malloc (n*n * sizeof(char));  /* create 1 dimensional */
				if (board == NULL)					  /* matrix named board   */
					return -1;

				for (i=0; i<n; i++)					  /* Initialise matrix    */
					for (j=0; j<n; j++)
						board[boardindex(i,j)] = 'n';
				i = j = n/2;
						  /* Find the four middle squares and initialise them */
				board[boardindex(i,j)] = board[boardindex(i-1,j-1)] = 'w';
				board[boardindex(i,j-1)] = board[boardindex(i-1,j)] = 'b';
													  /* print the board	  */
				printboard(board, turn, i, j);
				insert_at_start(&history,board,0,0,0);		  /* prepend data */
				fflush(stdout);
		
		        game=ON;
				break;
			case PLAY:												  /* play */
					if (turn != color) {
					    printf ("Sorry, it's not your turn to play\n");
						break;
					}
							  /* If player's move is legal, allow him to play */
					if (checkiflegalmove(i, j, board, color)) {
						board = makeBoard(board);
						playmove(i, j, board, turn);		  /* update board */
					}
					else {
						printf("Your move is not legal\n");
						break;
					}
					turn = 1-color;
											 /* if opponent can't play a move */
					if (!player_has_legal_moves(1-color, board)) {
													  /* check if you have at */
													  /* least one legal move */
						if (!player_has_legal_moves(color, board))
						{		    		/* if none of you can play a move */
   							insert_at_start(&history,board,i,j,history->move);
							game_over = 1;			/* then game has finished */
							printboard(board, turn, i, j);
						    printf("\nEND OF GAME\n");
							break;
						}
						else{				  /* else it's your turn to play  */
							turn = color;
							player_loses_turn = 1;
						}
					}
					printboard(board, turn, i, j);
					insert_at_start(&history,board,i,j,history->move+1);
															  /* prepend data */
					fflush(stdout);

				break;
			case CONT:
					if (turn == color) {
					    printf ("It's your turn to play\n");
						break;
					}

	  /* call function that will evaluate which move is the best for computer */
     				best_move (board,board_with_values,turn,&i,&j);

					if (checkiflegalmove(i, j, board, turn)) {
						board = makeBoard(board);
						playmove(i, j, board, turn);
					}
					turn = color;
											 /* if opponent can't play a move */
					if (!player_has_legal_moves(color, board)) {
													  /* check if you have at */
													  /* least one legal move */
						if (!player_has_legal_moves(1-color, board))
						{		    		/* if none of you can play a move */
   							insert_at_start(&history,board,i,j,history->move);
							game_over = 1;			/* then game has finished */
							printboard(board, turn, i, j);
						    printf("\nEND OF GAME\n");
						    break;
						}
						else{				  /* else it's your turn to play  */
							turn = 1-color;
							player_loses_turn = 1;
						}
					}
                    printboard(board, turn, i, j);
					insert_at_start(&history,board,i,j,history->move+1);
															  /* prepend data */
                    fflush(stdout);
					break;
			case UNDO:                                                          
					tmp = history;
					if (turn == color) {		/* if computer made it's move */
						if(history != NULL && history->next !=NULL) {

							history = history->next;  /* undo computer's move */
							board = history->data;
							i = history->i;
							j = history->j;
							free(tmp->data);
							free(tmp);
							turn = 1 - color;		  /* it's computer's turn */
								 /* unless we've reached the state of newgame */
							if(history != NULL && history->next !=NULL) {
								tmp = history;			/* undo player's move */
								history = history->next;
								board = history->data;
								i = history->i;
								j = history->j;
								free(tmp->data);
								free(tmp);
								turn = color;
							}
							printboard(board, turn, i, j);
						}
						else {
							printf("You can not undo.\n");
							printboard(board, turn, i, j);
						}
					}
					else {					/* if computer hasn't made a move */
						if(history != NULL && history->next !=NULL) {
							history = history->next;	/* undo player's move */
							board = history->data;
							i = history->i;
							j = history->j;
							free(tmp->data);
							free(tmp);
							turn = color;
							printboard(board, turn, i, j);
						}
						else {
							printf("You can not undo\n");
							printboard(board, turn, i, j);
						}
					}
					fflush(stdout);
					break;
			case SUG:
					if (turn != color) {
					    printf("We can't suggest you a move,");
						printf("because it's not your turn to play\n");
						fflush(stdout);
						break;
					}
     				if (best_move (board,board_with_values,color,&i,&j)) {
												  /* if there are legal moves */
						printf("Suggested move: %c%d\n\n", 'a'+ j, i+1);
					}
     				else
     				    printf("You have no moves to play\n");
					fflush(stdout);
					break;

/*cases SELE (for selectcolor),
		SHOWL (for showlegal),
		LEVEL (for selection of level), aren't really necessary as we've already
taken the informarion that we need from function "recognize command".		  */

			case SAVE:

					if (game!=ON) {
						printf("You have to start a game in order to save its state\n");
						fflush(stdout);
					    break;
					}
					i = history->i;
					j = history->j;
															 /* time to save! */
					statefile = fopen(file, "wb");
					if (statefile==NULL)
					    return -1;
					fwrite(&n, sizeof(int), 1, statefile);
					fwrite(&turn, sizeof(int), 1, statefile);
					fwrite(&i, sizeof(int), 1, statefile);
					fwrite(&j, sizeof(int), 1, statefile);
					fwrite(board, sizeof(char), n*n, statefile);
					fclose(statefile);

					free(file);
					break;
			case LOAD:
												/* open file you want to load */
					statefile = fopen(file, "rb");
					if (statefile == NULL) {
						printf("File \"%s\" could not open.\n", file);
						printf("Please, check if the file that you want to open exists or not.\n");
						fflush(stdout);
						break;
					}

					while(history != NULL) {   /* if the file we want to load */
						tmp = history;		   /* exists, erase history		  */
						free(history->data);
						history = history->next;
						free(tmp);
					}
									 /* take all needed information from file */
					fread(&n, sizeof(int), 1, statefile);
									 /* create space for board into statefile */
					board = (char *)malloc(n*n* sizeof(char));
					fread(&turn, sizeof(int), 1, statefile);
					fread(&i, sizeof(int), 1, statefile);
					fread(&j, sizeof(int), 1, statefile);
					fread(board, sizeof(char), n*n, statefile);

					insert_at_start(&history,board,i,j,0);
							   /* prepend data that were saved into statefile */
					printboard(board, turn, i, j);
					fflush(stdout);
					fclose(statefile);
					free(file);
					game = ON;

					break;
			case SHOWS:						/* it shows current state of game */
					printboard(history->data,turn,history->i,history->j);
					fflush(stdout);
					break;
			default:
					break;
		}

/* if command is quit */
		if (game_over == 1 || command == QUIT) {

			while(history != NULL) {   /* if game finished, erase the history */
				tmp = history;
				free(history->data);
				history = history->next;
				free(tmp);
			}
			game_over = 0;
			game = OFF;
			if (command == QUIT)
				continue;				  /* do not initialise command with 0 */
		}
		command = 0;
	}
	free(board_with_values);
	printf("---------------------------\n\n\nbye...\n\n\n");
	fflush(stdout);
}

/* create a new ll node which will be used by function: insert_at_start */
ll *makeNewNode(char *data, int i, int j, int move) {
	ll *newnode = (ll *)malloc(1 * sizeof(ll));
	newnode->data = data;
	newnode->i = i;
	newnode->j = j;
	newnode->move = move;
	newnode->next = NULL;
	return newnode;
}
/* a function for prepending data of a new state at the *
 * beginning of history list							*/
void insert_at_start(ll **list, char *data, int i, int j, int move) {
	ll *tmp = makeNewNode(data,i,j,move);
	tmp->next = *list;
	*list = tmp;
}

void printboard(char *board, int turn, int a, int b) {
/*
extern int n;
extern int color;
extern int option;*/
int i, j;						  /*	      i, j are counters for the board */
int numwhite = 0;				  /* counter for the number of white checkers */
int numblack = 0;				  /* counter for the number of black checkers */

	printf("     ");
	for (j=0; j<n; j++) 		  /*  print the latin letters that correspond */
		printf("%c   ", 'a' + j); /*				    to the vertical lines */
	printf("\n   ");
	for (i=0; i<n; i++)			  /*     print 1st horizontal line of contour */
		printf("+---");
	printf("+\n");
	for (i=0; i<n; i++) {
		printf("%2d |", i+1);	  /* 			  number of line is needed at */
		for (j=0; j<n; j++) {	  /* 			   the beginning of each line */
			if (board[boardindex(i,j)] == 'n') {
								  /* Instead of 'n' type empty space or '*'	  */
				if(option == ON && turn == color && checkiflegalmove(i, j, board, turn))
					printf(" * |");
				else
					printf("   |");
			}
			if (board[boardindex(i,j)] == 'w') { /* Instead of "w" type "W" */
				printf(" W |");
			}
			if (board[boardindex(i,j)] == 'b') { /* Instead of "b" type "B" */
				printf(" B |");
			}
		}
		printf("\n   ");
		for (j=0; j<n; j++)
		    printf("+---");
			printf("+\n");
	}						 /* if state isn't new game print the move played */
	if ( count_checkers (board, &numwhite, &numblack) )
		printf("Move played : %c%d\n", 'a'+ b, a+1);

	if ( player_loses_turn ) {
		switch (turn) {
			case WHITE:
				printf ("\nBlack player has no legal moves, and loses his/her turn\n");
				break;
			case BLACK:
				printf ("\nBlack player has no legal moves, and loses his/her turn\n");
				break;
		}
	player_loses_turn = 0;
	}

	if (game_over == 0) {						   /* if game hasn't finished */
		switch (turn) {						/* print whose player is the turn */
			case BLACK:
				printf ("Black player %s plays now\n\n",
						(color == BLACK) ? "(human)" : "(computer)");
				break;
			case WHITE:
				printf ("White player %s plays now\n\n",
						(color == WHITE) ? "(human)" : "(computer)");
				break;
			default:
				printf("oops, something really bad happened\n");
				break;
		}
	}

	printf("White : %d - Black : %d\n", numwhite, numblack);
	fflush(stdout);
}
