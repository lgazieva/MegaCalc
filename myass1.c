/* Program to perform multi-digit integer arithmetic.

   Skeleton program written by Alistair Moffat, ammoffat@unimelb.edu.au,
   August 2023, with the intention that it be modified by students
   to add functionality, as required by the assignment specification.
   All included code is (c) Copyright University of Melbourne, 2023

   Student Authorship Declaration:

   (1) I certify that except for the code provided in the initial skeleton
   file, the  program contained in this submission is completely my own
   individual work, except where explicitly noted by further comments that
   provide details otherwise.  I understand that work that has been developed
   by another student, or by me in collaboration with other students, or by
   non-students as a result of request, solicitation, or payment, may not be
   submitted for assessment in this subject.  I understand that submitting for
   assessment work developed by or in collaboration with other students or
   non-students constitutes Academic Misconduct, and may be penalized by mark
   deductions, or by other penalties determined via the University of
   Melbourne Academic Honesty Policy, as described at
   https://academicintegrity.unimelb.edu.au.

   (2) I also certify that I have not provided a copy of this work in either
   softcopy or hardcopy or any other form to any other student, and nor will I
   do so until after the marks are released. I understand that providing my
   work to other students, regardless of my intention or any undertakings made
   to me by that other student, is also Academic Misconduct.

   (3) I further understand that providing a copy of the assignment
   specification to any form of code authoring or assignment tutoring service,
   or drawing the attention of others to such services and code that may have
   been made available via such a service, may be regarded as Student General
   Misconduct (interfering with the teaching activities of the University
   and/or inciting others to commit Academic Misconduct).  I understand that
   an allegation of Student General Misconduct may arise regardless of whether
   or not I personally make use of such solutions or sought benefit from such
   actions.

   Signed by: Lola Gazieva 1473456
   Dated:     09/09/2023

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <assert.h>
#include <unistd.h>

/* All necessary #defines provided as part of the initial skeleton */

#define INTSIZE	500	/* max number of digits per integer value */
#define LINELEN	999	/* maximum length of any input line */
#define NVARS	26	/* number of different variables */

#define CH_A     'a'    /* character 'a', first variable name */

#define ERROR	(-1)	/* error return value from some functions */
#define PROMPT	"> "	/* the prompt string for interactive input */

#define PRINT	'?'	/* the print operator */
#define ASSIGN	'='	/* the assignment operator */
#define PLUS	'+'	/* the addition operator */
#define MULT	'*'	/* the multiplication operator */
#define POWR	'^'	/* the power-of operator */
#define DIVS	'/'	/* the division operator */
#define ALLOPS  "?=+*^/"

#define CH_ZERO  '0'    /* character zero */
#define CH_ONE   '1'    /* character one */
#define CH_NINE  '9'    /* character nine */

#define CH_COM   ','    /* character ',' */
#define PUT_COMMAS 3    /* interval between commas in output values */

#define INT_ZERO 0	/* integer 0 */
#define INT_ONE  1  /* integer 1 */
#define INT_TEN  10	/* integer 10 */


typedef int longint_t[INTSIZE+1]; 
#define FALSE 0
#define TRUE 1


/****************************************************************/

int fileno(FILE *);

void print_prompt(void);
void print_tadaa();
void print_error(char *message);
int  read_line(char *line, int maxlen);
void process_line(longint_t vars[], char *line);
int  get_second_value(longint_t vars[], char *rhsarg,
	int *second_value);
int  to_varnum(char ident);
void do_print(int varnum, longint_t *var);
void do_assign(int *var1, int *var2);
void do_plus(int *var, int *var2);
void zero_vars(longint_t vars[]);
void parse_num(char *rhs, int *second_value);
int leading_zeros(char *rhs); 
void do_multiply(int *var, int *var2);
void do_power(int *var1, int *var2);

/****************************************************************/

int
main(int argc, char *argv[]) {
	char line[LINELEN+1] = {INT_ZERO};
    longint_t vars[NVARS];

	zero_vars(vars);
	print_prompt();
	while (read_line(line, LINELEN)) {
		if (strlen(line) > 0)  {
			process_line(vars, line);
		}
		print_prompt();
	}

	print_tadaa();
	return 0;
}

void
print_prompt(void) {
	if (isatty(fileno(stdin))) {
		fprintf(stderr, "> ");
		fflush(stderr);
	}
}

void
print_tadaa() {
	if (isatty(fileno(stdin)) && isatty(fileno(stdout))) {
		printf("\n");
	}
	printf("ta daa!!!\n");
	if (isatty(fileno(stdin)) && !isatty(fileno(stdout))) {
		fprintf(stderr, "\n");
	}
}

void
print_error(char *message) {
	if (isatty(fileno(stdin)) || isatty(fileno(stdout))) {
		fprintf(stderr, "%s\n", message);
		fflush(stderr);
	}
	if (!isatty(fileno(stdout))) {
		printf("%s\n", message);
	}
}

int
read_line(char *line, int maxlen) {
	int i=0, c;
	while (((c=getchar())!=EOF) && (c!='\n')) {
		if (i<maxlen && !isspace(c)) {
			line[i++] = c;
		}
	}
	line[i] = '\0';
	if (!isatty(fileno(stdin)) || !isatty(fileno(stdout))) {
		printf("%s%s\n", PROMPT, line);
	}
	return ((i>0) || (c!=EOF));
}


void
process_line(longint_t vars[], char *line) {
	int varnum, optype, status;
	longint_t second_value;

	varnum = to_varnum(line[0]);
	if (varnum==ERROR) {
		print_error("invalid LHS variable");
		return;
	}

	if (strlen(line)<2) {
		print_error("no operator supplied");
		return;
	}

	optype = line[1];
	if (strchr(ALLOPS, optype) == NULL) {
		print_error("unknown operator\n");
		return;
	}

	if (optype != PRINT) {
		if (strlen(line) < 3) {
			print_error("no RHS supplied");
			return;
		}
		status = get_second_value(vars, line+2, second_value);
		if (status==ERROR) {
			print_error("RHS argument is invalid");
			return;
		}
	}

    if (optype == PRINT) {
		do_print(varnum, vars+varnum);
	} else if (optype == ASSIGN) {     
		do_assign(vars[varnum], second_value);
	} else if (optype == PLUS) {
		do_plus(vars[varnum], second_value);
	} else if (optype == MULT) {
		do_multiply(vars[varnum], second_value);
    } else if (optype == POWR) {
		do_power(vars[varnum], second_value);
	} else {
		print_error("operation not available yet");
		return;
	}
	return;
}


int
to_varnum(char ident) {
	int varnum;
	varnum = ident - CH_A;
	if (INT_ZERO <=varnum && varnum<NVARS) {
		return varnum;
	} else {
		return ERROR;
	}
}


int
get_second_value(longint_t vars[], char *rhsarg,
			int *second_value) {
	char *p;
	int varnum2;
	if (isdigit(*rhsarg)) {
		for (p=rhsarg+1; *p; p++) {
			if (!isdigit(*p)) {
				return ERROR;
			}
		}
        // converts string to an array of integers, which is second_value
		parse_num(rhsarg, second_value);
		return !ERROR;
	} else {
		varnum2 = to_varnum(*rhsarg);
		if (varnum2==ERROR || strlen(rhsarg)!=INT_ONE) {
			return ERROR;
		}
        do_assign(second_value, vars[varnum2]);
		return !ERROR;
	}
	return ERROR;
}

void
zero_vars(longint_t vars[]) {
    // at first, initialises all values in a 2d array to zero 
    for (int i = 0; i < NVARS; i++) {
        for (int j = 0; j < INTSIZE; j++) {
            vars[i][j] = INT_ZERO;
        }
    }
    // then initialise the 1st value in all 26 rows to one
    // so that input such as 'a?' returns zero
    for (int i = 0; i < NVARS; i++) {
        vars[i][0] = INT_ONE;
    }
}



void
parse_num(char *rhs, int *second_value) {
    int num_of_vals = strlen(rhs), s_value_ind=1;
    // since we store a reverse array, end is at the first non-zero element
    int end_index = leading_zeros(rhs);
    // subtracting leading zeros from the total number of digits 
    second_value[0] = num_of_vals - end_index;
    if (second_value[0] > INTSIZE) {
        print_error("your number is too big");
		return;
    }
    for (int i = num_of_vals-1; i >= end_index; i--) {
        // converting char to int digit by digit
		second_value[s_value_ind] = rhs[i] - CH_ZERO;
        s_value_ind++;
	}
}

void do_print(int varnum, longint_t *var) {
    printf("register %c: ", varnum + CH_A);
    int len = (*var)[0];
    // since the array is reversed, we start from the end
    for (int i = len; i >= 1; i--) {
        printf("%d", (*var)[i]);
        if (i > INT_ONE && (i - INT_ONE) % PUT_COMMAS == INT_ZERO) {
            printf("%c", CH_COM);
    	}
    }
	printf("\n");
}


void do_assign(int *var1, int *var2) {
    for (int i = 0; i <= var2[0]; i++) {
        var1[i]= var2[i];
    }
}

void
do_plus(int *var1, int *var2) {
    // if the second value has more digits, we need to increase the number of
    // digits in the first value
    if (var2[0] > var1[0]) {
        var1[0] += (var2[0]-var1[0]);
    }
    for (int i = 1; i <= var2[0]; i++) {
        var1[i] += var2[i];
    }
    int len = var1[0];
    for (int i = 1; i <= len; i++) {
        if (var1[i] >= INT_TEN) {
            // adds carry to the next digit
            var1[i+1] +=1;
            // reduces the value at position i by 10
            var1[i] = var1[i] % INT_TEN;
            // if the result has more digits than 1st value
            // increase the number of digits in the 1st value by one
            if ((i+1) > var1[0]) {
                var1[0] +=1;
                if (var1[0] > INTSIZE) {
                    print_error("the result is too big");
		            return;
                }
            }
            // checks if the number has correct number of digits stored at
            // position 0
			if (var1[i+2] >0 && (i+2) > var1[0]) {
					var1[0] += 1;
                    if (var1[0] > INTSIZE) {
                        print_error("the result is too big");
		                return;
                    }
				}
        }
    }
}


/*****************************************************************
******************************************************************

My new functions

******************************************************************
*****************************************************************/

void
do_multiply(int *var1, int *var2) {
	int toright= 0;
    // temporarily store the result in an empty array
	longint_t calc = {0};
	calc[0] = var1[0];
	for (int i=1; i <= var2[0]; i++) {
		for (int j=1; j <= var1[0]; j++) {
			calc[j+toright] += var2[i] * var1[j];
            // carry-over condition
			if ((j+toright) > calc[0]) {
				calc[0] += 1;
                // Increase the number of digits if carry-over occurs
                if (calc[0] > INTSIZE) {
                    print_error("the result is too big");
		            return;
                    }
			}
			if (calc[j+toright] >= INT_TEN) {
                // Add the tens place to the next digit
				calc[j+toright+1] += calc[j+toright] / INT_TEN;
                // checks if the number has correct number of digits stored at
                // position 0
				if (calc[j+toright+1] > INT_ZERO && (j+toright+1) > calc[0]) {
					calc[0] += 1;
                    if (calc[0] > INTSIZE) {
                        print_error("the result is too big");
		                return;
                    }
				}
                // keep only the last digit of a number
				calc[j+toright] %= INT_TEN;
			}
		}
        // for every digit we need to move addition to the right
		toright++;
	}
    // Update the first element of the first number with the final length of
    // the result
	var1[0] = calc[0];
    // Copy the result back to the first number
    for (int i=1; i <= calc[0]; i++) {
        var1[i] = calc[i];
    }
    int all_zeros = TRUE;
    // Check if there are any non-zero digits in the first number
    for (int i=1; i <= var1[0]; i++) {
        if (var1[i] != INT_ZERO) {
            all_zeros = FALSE;
        }
    }
    // if all_zeros, the number is 0
    if (all_zeros) {
        var1[0] = INT_ONE;
    }
}

//counts and returns the number of leading zeros
int leading_zeros(char *rhs) {
    int end_index=0;
    int size = strlen(rhs);
    for (int i=0; i < size; i++) {
        if (rhs[i] - CH_ZERO != INT_ZERO) {
            break;
        } else {
            end_index++;
        }
    }
    if (end_index==size) {
        end_index-=1;
    }
    return end_index;
}
        
void do_power(int *var1, int *var2) {
    int power_is_zero = TRUE;
    for (int i = 1; i <= var2[0]; i++) {
        if (var2[i] != INT_ZERO) {
            power_is_zero=FALSE;
        }
    }
    // if power is 0, then returns 1
    if (power_is_zero) {
        var1[0] = INT_ONE;
        var1[1] = INT_ONE;
        return;
    }
    // if power is 1, then returns the number itself
    if (var2[0]== INT_ONE && var2[1]== INT_ONE) {
        return;
    }
    // if number is 0, then returns the number itself
    // unless it's to the power of 0 - then returns 1 earlier in the function
    if (var1[0]== INT_ONE && var1[1]== INT_ZERO) {
        return;
    }
    // if number is 1, then returns the number itself
    if (var1[0]== INT_ONE && var1[1]== INT_ONE) {
        return;
    }
    longint_t copy_var1 = {0};
    for (int i=0; i<= var1[0]; i++) {
        copy_var1[i]=var1[i];
    }
    int add_one[2] = {1, 1};
    longint_t updating_array = {0};
    updating_array[1] = INT_ONE;
    updating_array[0] = INT_ONE;
    int are_the_same = TRUE;
    while (TRUE) {
        are_the_same = TRUE;
        // adds 1 to an updating array which initially only contains 1 until
        // it becomes equal to the power array
        do_plus(updating_array, add_one);
        // multiplies the increasing number by the copy of its initial form
        // throughout the duration of the loop
        do_multiply(var1, copy_var1);
        // checks whether updating array and power array are equal
        for (int i=0; i <= var2[0]; i++) {
            if (updating_array[i] != var2[i]) {
                are_the_same = FALSE;
            } 
        }
        // once that happens, the loop stops
        if (are_the_same) {
            break;
        }
    }
} 

// algorithms are fun
// but this assignment made me cry ngl
