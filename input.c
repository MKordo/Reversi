#include "input.h"

extern int n;
extern int dif;
extern int color;
extern int option;

/* Information from arguments in command line */
void parse_args(int argc, char **argv) {

	char *s;
	n = 8;

	if (argc>1) {
		while (--argc) { 				  /* Loop while arguments are present */
			*++argv;
			s = *argv;
			if (**argv == '-') {
				s++;
				
				if (*s == 'n') {
					s = *(argv + 1);
					while (*s == ' ' || *s == '\t') s++;
					if (*s == '-') n = 8;
					if (*s>='0' && *s<='9') n = atoi(*(argv + 1));
					if (n%2 == 1 || !(n>=4 && n<=26)) {
						printf("\"%d\" is an invalid dimension of game table.\n", n);
						printf("Automatic setting of dimension to 8.\n");
						n = 8;
					}
				}

				if (*s == 'd') {
					s = *(argv + 1);
					while (*s == ' ' || *s == '\t') s++;
					if (*s == '-') dif = 1;
					if (*s>='0' && *s<='9') dif = atoi(*(argv + 1));
					if (dif < 1) {
						printf ("Difficulty level has to be greater than one\n");
						printf("Automatic setting of level to 1.\n");
						dif=1;
					}
 				}

				if (*s == 'l')
					option = ON;
	
				if (*s == 'w')
					color = WHITE;
			}
		}
	}
	fflush(stdout);
}

int compare_strings( const char *input, const char *s ) {
	int i=0;

	if (strncmp(input,s,strlen(s)))
		return 0;
	while(i<strlen(s) && *input>='a' && *input<='z') {
		input++;
		i++;
	}
	if (*input != '\n' && *input!=' ' && *input!='\t')	 /* if input given is */
	    return 0;					 /* ex. "newgamegfkj 10" command is wrong */
	return 1;
}

/* Avoid cases like: (ex.) cont e5 */
int after_command(const char *input){
	int i = 0;
	while (*input>='a' && *input<='z') {
		input++;
		i++;
	}
	while (i<40 && (*input==' ' || *input=='\t')) {
		input++;						   /* Eat all empty space			  */
		i++;
	}
	if (*input == '\n')
		return 1;						   /* if input have finished return 1 */
	else
		return 0;						   /* if input continues return 0	  */
}

int recognize_command (const char *input, int *a, int *b, char **file)
{
	int tmp=0, i=0, j=0, wrong = 1, notexist = 0;
	char c;

	while ( *input==' ' || *input=='\t' )			   /* Eat all empty space */
	    input++;

	switch (*input) {
		case 'n':
/* if command is newgame */
				if ( compare_strings(input,"newgame") != 1 )
					break;

				while (*input>='a' && *input<='z')	 /* pass string "newgame" */
					input++;
				while (*input==' ' || *input=='\t' )   /* Eat all empty space */
					input++;
				if (*input>='0' && *input<='9'){	 /* if a number was given */
					tmp = atoi(input); /* and it is valid to be the dimension */
					if ( tmp%2 == 0 && tmp>=4 && tmp<=26 )	   /* of board... */
						n = tmp;						   /* then n = number */
					else {
						printf ("Given value of size for the game table is invalid\n");
						printf ("Please, repeat command inserting an even number between 4 and 26\n");
						wrong = 0;
						break;
					}
				}
				else if (*input != '\n')		  /* if a number wasn't given */
					break;			  /* accept only the ascci character '\n' */

				return NEWGM;
				break;
		case 'p':
/* if command is play */
				if ( compare_strings(input,"play") != 1 )
					break;

				while (*input>='a' && *input<='z') {
					input++;
					i++;
				}
				while (i<40 && (*input==' ' || *input=='\t')) {
					input++;						   /* Eat all empty space */
					i++;
				}

				if (*input == '\n') {	/* If player haven't played a move... */
					printf("You have to decide where to play.\n");
					wrong = 0;
					break;
				}
				else if ( *input >= 'a' && *input <= 'a'+ (n-1) ) {
					c = *input;
					*b = c - 'a';
					input++;
				}
				else
					notexist = 1;

				if ( *input >= '1' && *input <= '9' ) {
					tmp = atoi(input);
					if ( tmp >=1 && tmp <= n )
						*a = tmp-1;
					else
					    notexist = 1;
				}
				else
				    notexist = 1;

				if (notexist == 1) {
					printf("Your move is not legal\n");
					wrong = 0;
					break;
				}

				return PLAY;
				break;
		case 'c':
/* if command is cont */
				if ( compare_strings(input,"cont") != 1 )
					break;

				if ( after_command(input) )
				    return CONT;
				else
				    break;
		case 'u':
/* if command is undo */
				if ( compare_strings(input,"undo") != 1 )
					break;

				if ( after_command(input) )
					return UNDO;
				else
					break;
			case 's':
/* if command is save */
				if ( compare_strings(input,"save") == 1 ) {

					while (*input>='a' && *input<='z') {
						input++;
						i++;
					}
					while (i<40 && (*input==' ' || *input=='\t')) {
						input++;					   /* Eat all empty space */
						i++;
					}
									  /* If player haven't given file to open */
					if (*input == '\n') { 
						printf("You have to decide the name of the file you");
						printf(" want to save the current state of game.\n");
						wrong=0;
						break;
					}		   /* read name of file and return string to file */
					while ( (*input >= 'a' && *input <= 'z') || (*input >= '0' && *input <= '9') )
					{
						tmp++;
						input++;
					}
					input -= tmp;
					*file = malloc ((tmp+1)*sizeof(int));
					for (i=0;i<tmp+1;i++)
						(*file)[i]='\0';
					strncpy(*file, input, tmp);

						return SAVE;
						break;
				}
/* if command is selectcolor */
				if ( compare_strings(input,"selectcolor") == 1 ) {

					while (*input>='a' && *input<='z') {
						input++;
						i++;
					}
					while (i<40 && (*input==' ' || *input=='\t')) {
						input++;					   /* Eat all empty space */
						i++;
					}
									   /* If player haven't selected color... */
					if (*input == '\n') {
						printf("You have to decide your color, or else it will not change.\n");
						wrong = 0;
						break;
					}
					else if (*input == 'b') {
						if (compare_strings(input,"black")==1)
						    color = BLACK;
					}
					else if (*input == 'w') {
						if (compare_strings(input,"white")==1)
						    color = WHITE;
					}
					else
						break;
printf("You are the %s player\n", (color==WHITE)?"white":"black");
					return SELE;
					break;
				}
/* if command is suggest */
				if ( compare_strings(input,"suggest") == 1 ) {

					if ( after_command(input) )
						return SUG;
					else
						break;
				}
/* if command is showlegal */
				if ( compare_strings(input,"showlegal") == 1 ) {

					while (*input>='a' && *input<='z') {
						input++;
						i++;
					}
					while (i<40 && (*input==' ' || *input=='\t')) {
						input++;					   /* Eat all empty space */
						i++;
					}

					if (*input == '\n') {/* If player haven't selected option */
						printf("You have to decide if you want to be shown your legal moves.\n");
						wrong = 0;
						break;
					}
					else if (*input == 'o') {
						input++;
						if (compare_strings(input,"n")==1)
						    option = ON;
						if (compare_strings(input,"ff")==1)
						    option = OFF;
					}
					else
						break;

					return SHOWL;
					break;
				}
/* if command is showstate */
				if ( compare_strings(input,"showstate") == 1 ) {

					if ( after_command(input) )
						return SHOWS;
					else
						break;
				}
				wrong = 1;
				break;
		case 'l':
/* if command is load */
				if ( compare_strings(input,"load") == 1 ) {

					while (*input>='a' && *input<='z') {
						input++;
						i++;
					}
					while (i<40 && (*input==' ' || *input=='\t')) {
						input++;					   /* Eat all empty space */
						i++;
					}
						/* If player haven't given the name of a file to open */
					if (*input == '\n') {
						printf("You have to decide which file you want to load.\n");
						wrong=0;
						break;
					}
							   /* read name of file and return string to file */
					while ( (*input >= 'a' && *input <= 'z') || (*input >= '0' && *input <= '9') )
					{
						tmp++;
						input++;
					}
					input -= tmp;
					*file = malloc ((tmp+1)*sizeof(int));
					for (i=0;i<tmp+1;i++)
						(*file)[i]='\0';
					strncpy(*file, input, tmp);

						return LOAD;
						break;
				}
/* if command is level */
				if ( compare_strings(input,"level") == 1 ) {

					while (*input>='a' && *input<='z')
						input++;
						
					while (*input==' ' || *input=='\t' )/*Eat all empty space */
						input++;
					if (*input>='0' && *input<='9'){ /* if a number was given */
						tmp = atoi(input); /* and it is valid to be the level */
						if ( tmp>=1 )
							dif = tmp;					 /* then dif = number */
						else {
							printf ("Given value for difficulty of game is invalid\n");
							printf ("Try again inserting a number greater than 1\n");
							wrong = 0;
							break;
						}
					}
					else if (*input == '\n') {
						printf("Level did not change\n");
						printf("Current level is %d\n", dif);
					}

					return LEVEL;
					break;
				}

				break;
		case 'q':
/* if command is quit */
				if ( compare_strings(input,"quit") != 1 )
					break;
					
					if ( after_command(input) )
						return QUIT;
					else
						break;
		case '\n':
				wrong = 0;
				break;
		default:
				wrong = 1;
				break;
	}
	if ( wrong == 1 )
	    printf("Command can't be recognised. Please try again\n\n");
	fflush(stdout);
	return 0;
}
