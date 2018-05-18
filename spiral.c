#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define RED "\e[0;31m"
#define RESET "\e[0;37m"
#define BLUE "\e[0;34m"
#define GREEN "\e[0;32m"
#define CYAN "\e[0;36m"
#define PURPLE "\e[0;35m"
#define YELLOW "\e[0;33m"

typedef enum {RHOR, LHOR, UVER, DVER} state_t;

void fill_array(int **array, int rows, int cols);
void print_array(int **array, int rows, int cols);

int main(int argc, char *argv[]) {
	int **array;
	int rows, cols;

	if (argc < 3) {
		fprintf(stderr, "error!\n");
		return(1);
	}
	rows = atoi(argv[1]);
	cols = atoi(argv[2]);

	array = malloc(rows*sizeof(int*));
	for (int k = 0 ; k < rows ; k++) {
		array[k] = malloc(cols*sizeof(int));
	}

	fill_array(array, rows, cols);
	print_array(array, rows, cols);

	for (int k = 0 ; k < rows ; k++) {
		free(array[k]);
	}

	free(array);
}

void fill_array(int **array, int rows, int cols) {
	state_t state = RHOR;
	int rhor_limit = cols, dver_limit = rows, lhor_limit = 0, uver_limit = 1;
	int total = 0;
	int x_pos = 0, y_pos = 0;

	array[0][0] = 0;

	do {
		if (total == rows*cols) {
			return;
		}
		switch(state) {
			case RHOR:
				if (x_pos+1 == rhor_limit) {
					state = DVER;
					rhor_limit--;
					continue;
				}
				else {
					x_pos++;
				}
				break;
			case LHOR:
				if (x_pos == lhor_limit) {
					state = UVER;
					lhor_limit++;
					continue;
				}
				else {
					x_pos--;
				}
				break;
			case DVER:
				if (y_pos+1 == dver_limit) {
					state = LHOR;
					dver_limit--;
					continue;
				}
				else {
					y_pos++;
				}
				break;
			case UVER:
				if (y_pos == uver_limit) {
					state = RHOR;
					uver_limit++;
					continue;
				}
				else {
					y_pos--;
				}
				break;
		}
		
			total++;
		if (total < rows*cols) {
			array[y_pos][x_pos] = total;
		}
	} while (total < rows*cols);
}

void print_array(int **array, int rows, int cols) {
	int k, j;

	for (k = 0 ; k < rows ; k++) {
		for (j = 0 ; j < cols ; j++) {
			switch((array[k][j]) % 6) {
				case 0:
					printf(RED"%6d "RESET, array[k][j]);
					break;
				case 1:
					printf(GREEN"%6d "RESET, array[k][j]);
					break;
				case 2:
					printf(BLUE"%6d "RESET, array[k][j]);
					break;
				case 3:
					printf(CYAN"%6d "RESET, array[k][j]);
					break;
				case 4:
					printf(PURPLE"%6d "RESET, array[k][j]);
					break;
				case 5:
					printf(YELLOW"%6d "RESET, array[k][j]);
					break;

			}
			fflush(stdout);
			usleep(100000);
		}
		putchar('\n');
		putchar('\n');
	}
}
