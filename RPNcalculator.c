// 30/1/2018

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#define SIZE 200
#define RIGHT -1
#define LEFT 0
#define BOTH 1
#define INTEGER 1
#define DECIMAL 2
#define UNARY 1
#define BINARY 2

typedef enum {OPERATOR, OPERAND} tag_t;

struct Operator {
	char operator;
	struct Operator *next;
};
typedef struct Operator operator_t;

typedef union {
	double number;
	char operator;
} role_t;

struct Token {
	role_t role;
	tag_t tag;
	struct Token *next;
	struct Token *prev;
};
typedef struct Token token_t;

operator_t *push(operator_t *stack, char operator);
operator_t *pop(operator_t *stack);
void destroy_stack(operator_t *stack);

//Converting from algebraic to Reverse Polish Notation:
int convert_RPN(char input[], char output[]);
int isoperator(char character);
int precedence(char operator);
int associativity(char operator);

//Calculating postfix expression:
double calculate_RPN(char output[]);
double calculate_intermediate(char operator, double token1, double token2);
token_t *convert_stack(char output[]);
token_t *tok_push_operator_rev(token_t *stack, char operator);
token_t *tok_push_double_rev(token_t *stack, double token);
token_t *tok_pop(token_t *stack);
void destroy_tok_stack(token_t *stack);
void print_stack(token_t *stack);
token_t *find_stack_end(token_t *stack);
token_t *add_interior_token(token_t *stack, token_t *current, double token);
token_t *delete_interior_token(token_t *stack, token_t *current);
void find_operands(token_t *current, double *token1, double *token2);
int arity(char operator);
void print_stack_rev(token_t *stack);

int main(int argc,char *argv[]) {
	char input[SIZE], output[2*SIZE];
	double result;

	printf("Expression: ");
	fgets(input, SIZE, stdin);
	convert_RPN(input, output);
	printf("Postfix: %s\n", output);
	result = calculate_RPN(output);
	printf("Result: %lf\n", result);
	
	return(0);
}

double calculate_RPN(char output[]) {
	double result = 0, token1, token2;
	token_t *current, *next;
	token_t *stack = NULL;
			
	stack = convert_stack(output); 
	if (stack == NULL) {
		printf("In function calculate_RPN, DMA error, exiting...\n");
		exit(-2);
	}
	for (current = stack ; current != NULL ; current = next) {
		next = current->next; 
		if (current->tag == OPERATOR) {
			if (arity(current->role.operator) == BINARY) {
				find_operands(current, &token1, &token2); 
				stack = delete_interior_token(stack, current->prev->prev); 
				stack = delete_interior_token(stack, current->prev); 
				result = calculate_intermediate(current->role.operator, token1, token2);
				stack = add_interior_token(stack, current, result);
				stack = delete_interior_token(stack, current);

			}
		}
	}
	result = stack->role.number;
	destroy_tok_stack(stack);

	return(result);
	
	
}

void find_operands(token_t *current, double *token1, double *token2) { 
	*token1 = current->prev->prev->role.number;
	*token2 = current->prev->role.number;
}

token_t *add_interior_token(token_t *stack, token_t *current, double token) { 
	token_t *new;

	if (current->prev == NULL) {
		new = (token_t*)malloc(sizeof(token_t));
		if (new == NULL) {
			return(NULL);
		}
		new->tag = OPERAND;
		new->role.number = token;
		new->next = stack;
		new->prev = NULL;
		stack->prev = new;
		return(new);
	}

	new = (token_t*)malloc(sizeof(token_t));
	if (new == NULL) {
		return(NULL);
	}
	new->tag = OPERAND;
	new->role.number = token;
	current->prev->next = new;
	new->prev = current->prev;
	current->prev = new;
	new->next = current;
	return(stack);	
}

token_t *delete_interior_token(token_t *stack, token_t *current) { 
	if ((current->prev == NULL) && (current->next == NULL)) {
		free(current);
		return(NULL);
	}
	else if (current->prev == NULL) {
		current->next->prev = NULL;
		stack = current->next;
		free(current);
		return(stack);
	}
	else if (current->next == NULL) {
		current->prev->next = NULL;
		free(current);
		return(stack);
	}
	
	current->prev->next = current->next;
	current->next->prev = current->prev;
	free(current);
	return(stack);
}

int arity(char operator) {
	switch(operator) {
		case '+':
		case '-':
		case '*':
		case '/':
		case '^':
			return(BINARY);
		case '!':
			return(UNARY);
	}
	
	return(999);
}




token_t *tok_pop(token_t *stack) {
	token_t *new;

	if (stack == NULL) {
		return(NULL);
	}
	
	new = stack->next;
	new->prev = NULL;
	free(stack);
	
	return(new);
}

double calculate_intermediate(char operator, double token1, double token2) {
	switch(operator) {
		case '+':
			return(token1+token2);
		case '-':
			return(token1-token2);
		case '*':
			return(token1*token2);
		case '/':
			return(token1/token2);
		case '^':
			return(pow(token1, token2));
	}
	
	return(token1);
}
			
token_t *convert_stack(char output[]) {
	int k, j = 10;
	token_t *stack = NULL;
	int mode = INTEGER;
	double number = 0;
	
	for (k = 0 ; output[k] != '\0' ; k++) {
		if (isoperator(output[k])) {
			stack = tok_push_operator_rev(stack, output[k]);
			if (stack == NULL) {
				return(NULL);
			}
		}
		if (isdigit(output[k])) {
			while(isdigit(output[k]) || (output[k] == '.')) {
				if (output[k] == '.') {
					mode = DECIMAL;
					k++;
					continue;
				}
				if (mode == INTEGER) {
					number = number*10 + (output[k] - '0');
					k++;
				}
				else {
					number += (double)(output[k] - '0')/j;
					j *= 10;
					k++;
				}
			}
			k--;
			mode = INTEGER;
			j = 10;
			stack = tok_push_double_rev(stack, number);
			number = 0;
			if (stack == NULL) {
				return(NULL);
			}
		}
	}
		
	return(stack);
}

void print_stack(token_t *stack) {
	token_t *current;
	
	for (current = stack ; current != NULL ; current = current->next) {
		if (current->tag == OPERATOR) {
			printf("%c ", current->role.operator);
		}
		else {
			printf("%lf ", current->role.number);
		}
	}
	putchar('\n');
}

token_t *find_stack_end(token_t *stack) {
	token_t *current;

	for (current = stack ; current->next != NULL ; current = current->next);
	
	return(current);
}


token_t *tok_push_operator_rev(token_t *stack, char operator) {
	token_t *new, *last;

	if (stack == NULL) {
		stack = (token_t*)malloc(sizeof(token_t));
		if (stack == NULL) {
			return(NULL);
		}
		stack->next = stack->prev = NULL;
		stack->tag = OPERATOR;
		stack->role.operator = operator;
		return(stack);
	}
	
	new = (token_t*)malloc(sizeof(token_t));
	if (new == NULL) {
		destroy_tok_stack(stack);
		return(NULL);
	}
	new->tag = OPERATOR;
	new->role.operator = operator;
	last = find_stack_end(stack);
	last->next = new;
	new->next = NULL;
	new->prev = last;
	return(stack);
}

token_t *tok_push_double_rev(token_t *stack, double number) {
	token_t *new, *last;

	if (stack == NULL) {
		stack = (token_t*)malloc(sizeof(token_t));
		if (stack == NULL) {
			return(NULL);
		}
		stack->next = stack->prev = NULL;
		stack->tag = OPERAND;
		stack->role.number = number;
		return(stack);
	}
	
	new = (token_t*)malloc(sizeof(token_t));
	if (new == NULL) {
		destroy_tok_stack(stack);
		return(NULL);
	}
	new->tag = OPERAND;
	new->role.number = number;
	last = find_stack_end(stack);
	last->next = new;
	new->next = NULL;
	new->prev = last;
	
	return(stack);
}

void destroy_tok_stack(token_t *stack) {
	if (stack == NULL) {
		return;
	}
	
	destroy_tok_stack(stack->next);
	free(stack);
}
	
int convert_RPN(char input[], char output[]) {
	operator_t *stack = NULL;
	int k, size = 0;
	
	for (k = 0 ; input[k] != '\n' ; k++) {
		if (isoperator(input[k])) {
			if (stack != NULL) {
				while ((stack != NULL) && ((precedence(input[k]) < precedence(stack->operator)) || 
				      ((precedence(input[k]) == precedence(stack->operator)) && (associativity(stack->operator) == LEFT))) 
					&& (stack->operator != '(')) {
						output[size] = stack->operator;
						size++;
						output[size] = ' ';
						size++;
						stack = pop(stack);
				}
			}
			stack = push(stack, input[k]);
		}
		else if (input[k] == '(') {
			stack = push(stack, input[k]);
		}
		else if (input[k] == ')') {
			while (stack->operator != '(') {
				output[size] = stack->operator;
				size++;
				output[size] = ' ';
				size++;
				stack = pop(stack);
			}
			stack = pop(stack);
		}
		else if ((isdigit(input[k])) || (input[k] == '.')) {
			output[size] = input[k];
			size++;
			if ((!isdigit(input[k+1])) && (input[k+1] != '.')) {
				output[size] = ' ';
				size++;
			}
		}
		else {
			printf("Invalid expression!\n");
			return(1);
		}
	}

	if (stack != NULL) {
		while (stack != NULL) {
			output[size] = stack->operator;
			size++;
			output[size] = ' ';
			size++;
			stack = pop(stack);
		}
	}
	
	output[size] = '\0';
	destroy_stack(stack);
	
	return(0);
}

int associativity(char operator) {
	switch(operator) {
		case '+':
		case '*':
			return(BOTH);
		case '-':
		case '/':
			return(LEFT);
		case '^':
			return(RIGHT);
	}
	
	return(9999); 
}

int precedence(char operator) {
	switch(operator) {
		case '+':
		case '-':
			return(0);
		case '*':
		case '/':
			return(1);
		case '^':
			return(2);
	}
	
	return(9999); 
}

operator_t *push(operator_t *stack, char operator) {
	operator_t *new;

	if (stack == NULL) {
		stack = (operator_t*)malloc(sizeof(operator_t));
		if (stack == NULL) {
			return(NULL);
		}
		stack->next = NULL;
		stack->operator = operator;
		return(stack);
	}

	new = (operator_t*)malloc(sizeof(operator_t));
	if (new == NULL) {
		destroy_stack(stack);
		return(NULL);
	}
	new->operator = operator;
	new->next = stack;
		
	return(new);
}
	
operator_t *pop(operator_t *stack) {	
	operator_t *new;
	
	if (stack->next == NULL) {
		free(stack);
		return(NULL);
	}
	
	new = stack->next;
	free(stack);
	
	return(new);
}

void destroy_stack(operator_t *stack) {
	if (stack == NULL) {
		return;
	}
	
	destroy_stack(stack->next);
	free(stack);
}

int isoperator(char character) {
	switch(character) {
		case '!':
		case '^':
		case '*':
		case '-':
		case '+':
		case '%':
		case '/':
			return(1);
	}
	
	return(0);
}

void print_stack_rev(token_t *stack) {
	if (stack == NULL) {
		return;
	}
	
	print_stack_rev(stack->next);
		printf("%lf ", stack->role.number);
		printf("%p\n", stack->prev);
	
}

