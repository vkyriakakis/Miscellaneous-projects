#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Na spasw kathe synarthsh generate se polles mikres
//Na ftiaxw synarthsh generate volcanos h opoia ftiaxnei to vlocano biome, kai bazei obsidian (glitwnw to flag)

#define WORLD_SIZE_X 130 //TEMP, pio meta tha desmeuetai dynamika
#define WORLD_SIZE_Y 60
#define PEOPLE_SIZE 10
#define COLOR_SIZE 10
#define RACE_MAX 6

#define WATER_RACES 4 //SAHAGIN, LIZARDMEN, MERFOLK, ELEMENTAL_WATER
#define PLAIN_RACES_EUR 15 //HUMAN, ELF, GNOME, HALFING, ORC, GNOLL, SAHAGIN, HALF_ELF, HALF_ORC, TIEFLING, BUGBEAR, DWARF
//OGRE, ELEMENTAL_EARTH, ELEMENTAL_WATER, ELEMENTAL_EARTH, DRAGON_WHITE
#define FOREST_RACES 9 //ELF, GOBLIN, FAE, GNOLL, OGRE, ELEMENTAL, DRAGON, KOBOLD, GNOME
#define MOUNTAIN_RACES 8 //DWARF, HUMAN, ORC, HALF_ORC, ELEMENTAL, DRAGON, OGRE, MINOTAUR, BEHOLDER, MIND_FLAYER, 
#define DESERT_RACES  //HUMAN, ELF, HALF_ELF, HALFING, GNOME, ELEMENTAL, DRAGON, LAMIA, 
#define VOLCANO_RACES
#define MUSHROOM_RACES
 

//ANSI ESCAPE SEQUENCES:

//COLORS:
#define RED "\e[0;91m"
#define GREEN "\e[0;32m"
#define YELLOW "\e[0;93m"
#define BLUE "\e[0;34m"
#define CYAN "\e[0;36m"
#define BROWN "\e[0;33m"
#define HI_GREEN "\e[0;92m"
#define HI_CYAN "\e[1;96m"
#define GREY "\e[0;30m"
#define HI_GREY "\e[0;90m"
#define RESET "\e[0m"

typedef enum {FALSE, TRUE};

typedef enum {EVIL, GOOD};

typedef enum {WATER, PLAIN, DESERT, FOREST, MOUNTAIN, SNOW_MOUNTAIN, SNOW, ASH, LAVA, CORAL, METEORITE, MUSHROOM} biomeT;

typedef enum {STONE, IRON, COPPER, GOLD, SILVER, MYTHRIL, OBSIDIAN, METEOR, ADAMANTITE, TITANIUM, THORIUM, URANIUM} depositT;

typedef enum {HUMAN, GOBLIN, ELF, HALF_ELF, HALF_ORC, GNOME, HALFING, ORC, DWARF, GNOLL, SAHAGIN, LIZARDMAN, BEHOLDER, OGRE, ELEMENTAL_FIRE, ELEMENTAL_ICE, ELEMENTAL_AIR, ELEMENTAL_EARTH, ELEMENTAL_WATER, ELEMENTAL_ELECTRIC, ELEMENTAL_LIGHT, ELEMENTAL_SHADOW, MIND_FLAYER, SALAMANDER, KOBOLD, BUGBEAR, DRAGON_BLUE, DRAGON_GREEN, DRAGON_RED, DRAGON_BLACK, DRAGON_WHITE, DRAGON_GOLD, DRAGON_SILVER, DRAGON_COPPER, DRAGON_MYTHRIL, TIEFLING, MERFOLK, MINOTAUR, LAMIA} raceT;

typedef struct {
	char *NAME;
	char *TITLE;
	raceT RACE;
} world_evil_t; 

typedef struct {
	raceT *RACE;
	unsigned POPULATION;
} population_t;

typedef struct {
	char *NAME;
	char COLOR[COLOR_SIZE];
	population_t *RACES;
} peopleT;

typedef struct {
	biomeT BIOME;
	depositT DEPOSIT;
	peopleT *PEOPLE;
	int people_flag;
} squareT; //One game "square" == 1 world array position

void world_init(squareT world[][WORLD_SIZE_X], peopleT *people_matrix[], int *people_size);
void generate_landmasses(squareT world[][WORLD_SIZE_X]);
void generate_climates(squareT world[][WORLD_SIZE_X]);
void generate_biomes(squareT world[][WORLD_SIZE_X], int *volcano_flag);
void generate_deposits(squareT world[][WORLD_SIZE_X], int volcano_flag);
void generate_underground(squareT world[][WORLD_SIZE_X]);
void generate_animals(squareT world[][WORLD_SIZE_X]);
void genearate_peoples(squareT world[][WORLD_SIZE_X], peopleT *people_matrix[], int *people_size);
void print_world_geographical(squareT world[][WORLD_SIZE_X]);
void print_world_minerals(squareT world[][WORLD_SIZE_X]);
void clear_people(peopleT *people_matrix[], int people_size);
void fprint_geographical(squareT world[][WORLD_SIZE_X]);

int main(int argc, char *argv[]) {
	int seed;
	int people_size = 0;
	int volcano_flag = FALSE;
	squareT world[WORLD_SIZE_Y][WORLD_SIZE_X];
	peopleT **people_matrix = NULL;
	
	printf("Enter seed: ");
	scanf("%d", &seed);
	srand(seed);
	generate_landmasses(world);
	generate_climates(world);
	generate_biomes(world, &volcano_flag);
	print_world_geographical(world);
	generate_deposits(world, volcano_flag);
	fprint_geographical(world);
	clear_people(people_matrix, people_size);
	
	return(0);
}

void world_init(squareT world[][WORLD_SIZE_X], peopleT *people_matrix[], int *people_size){
}

void generate_landmasses(squareT world[][WORLD_SIZE_X]) {
	int k, j;
	int check;
	int random_y, random_x;
	
	for (k = 0 ; k < WORLD_SIZE_Y ; k++) {
		for (j = 0 ; j < WORLD_SIZE_X ; j++) {
			world[k][j].BIOME = WATER;
		}
	}
	
	for (k = 0 ; k < WORLD_SIZE_Y/3 ; k++) {
		do {
			random_y = rand() % WORLD_SIZE_Y;
			random_x = rand() % WORLD_SIZE_X;
		} while (world[random_y][random_x].BIOME == PLAIN);
		world[random_y][random_x].BIOME = PLAIN;
	}
	
	for (k = 0 ; k < WORLD_SIZE_X*100 ; k++) {
		do {
			random_y = rand() % WORLD_SIZE_Y;
			random_x = rand() % WORLD_SIZE_X;
		} while (world[random_y][random_x].BIOME != PLAIN);
		
		if (random_y < WORLD_SIZE_Y-1) {
			check = rand() % 2;
			if (check) {
				world[random_y+1][random_x].BIOME = PLAIN;
			}
		}
		if (random_y > 0) {
			check = rand() % 2;
			if (check) {
				world[random_y-1][random_x].BIOME = PLAIN;
			}
		}
		if (random_x < WORLD_SIZE_X - 1) {
			check = rand() % 2;
			if (check) {
				world[random_y][random_x+1].BIOME = PLAIN;
			}
		}
		if (random_x > 0) {
			check = rand() % 2;
			if (check) {
				world[random_y][random_x-1].BIOME = PLAIN;
			}
		}
	}
}
	
void generate_climates(squareT world[][WORLD_SIZE_X]) {
	int k, j;
	
	for (k = 0 ; k < WORLD_SIZE_Y/6 ; k++) {
		for (j = 0 ; j < WORLD_SIZE_X ; j++) {
			if (world[k][j].BIOME == PLAIN) {
				if (k == WORLD_SIZE_Y/6 - 1) {
					if (rand() % 2) 
						world[k][j].BIOME = SNOW;
				}
				else {
					world[k][j].BIOME = SNOW;
				}
			}
		}
	}

	for (k = WORLD_SIZE_Y/2 ; k < 3*WORLD_SIZE_Y/4 ; k++) {
		for (j = 0 ; j < WORLD_SIZE_X ; j++) {
			if (world[k][j].BIOME == PLAIN) {
				if ((k == WORLD_SIZE_Y/2) || (k == 3*WORLD_SIZE_Y/4 - 1)) {
					if (rand() % 2) 
						world[k][j].BIOME = DESERT;
				}
				else {
					world[k][j].BIOME = DESERT;
				}
			}
		}
	}

	for (k = 15*WORLD_SIZE_Y/16 ; k < WORLD_SIZE_Y ; k++) {
		for (j = 0 ; j < WORLD_SIZE_X ; j++) {
			if (world[k][j].BIOME == PLAIN) {
				if (k == 15*WORLD_SIZE_Y/16) {
					if (rand() % 2) 
						world[k][j].BIOME = SNOW;
				}
				else {
					world[k][j].BIOME = SNOW;
				}
			}
		}
	}

	
}

//NA TO SPASW SE TREIS SYNARTHSEIS!!!!!!!!!!!!!!!!!!!!1

void generate_biomes(squareT world[][WORLD_SIZE_X], int *volcano_flag) {
	int k;
	int random_x, random_y;
	int check;
	//MOUNTAINS: 
	
	
	for (k = 0 ; k < WORLD_SIZE_Y/10 ; k++) {
		do {
			random_x = rand() % WORLD_SIZE_X;
			random_y = rand() % WORLD_SIZE_Y;
		} while (world[random_y][random_x].BIOME == WATER);
		
		if (world[random_y][random_x].BIOME == SNOW) {
			world[random_y][random_x].BIOME = SNOW_MOUNTAIN;
		}
		else {
			world[random_y][random_x].BIOME = MOUNTAIN;
		}
	}

	
	for (k = 0 ; k < WORLD_SIZE_X*10 ; k++) {
		do {
			random_y = rand() % WORLD_SIZE_Y;
			random_x = rand() % WORLD_SIZE_X;
		} while ((world[random_y][random_x].BIOME != MOUNTAIN) && (world[random_y][random_x].BIOME != SNOW_MOUNTAIN));
		
		if (random_y < WORLD_SIZE_Y-1) {
			check = rand() % 2;
			if (check) {
				if (world[random_y+1][random_x].BIOME == SNOW) {
					world[random_y+1][random_x].BIOME = SNOW_MOUNTAIN;
				}
				else if ((world[random_y+1][random_x].BIOME != WATER) && 
					(world[random_y+1][random_x].BIOME != MOUNTAIN) &&
					(world[random_y+1][random_x].BIOME != SNOW_MOUNTAIN)) {
					if (!(rand() % 5)) {
						world[random_y+1][random_x].BIOME = SNOW_MOUNTAIN;
					}
					else {
						world[random_y+1][random_x].BIOME = MOUNTAIN;
					}
				}
			}
		}
		
		if (random_y > 0) {
			check = rand() % 2;
			if (check) {
				if (world[random_y-1][random_x].BIOME == SNOW) {
					world[random_y-1][random_x].BIOME = SNOW_MOUNTAIN;
				}
				else if ((world[random_y-1][random_x].BIOME != WATER) && 
					(world[random_y-1][random_x].BIOME != MOUNTAIN) &&
					(world[random_y-1][random_x].BIOME != SNOW_MOUNTAIN)) {
					if (!(rand() % 5)) {
						world[random_y-1][random_x].BIOME = SNOW_MOUNTAIN;
					}
					else {
						world[random_y-1][random_x].BIOME = MOUNTAIN;
					}
				}
			}
		}
		if (random_x < WORLD_SIZE_X - 1) {
			check = rand() % 2;
			if (check) {
				if (world[random_y][random_x+1].BIOME == SNOW) {
					world[random_y][random_x+1].BIOME = SNOW_MOUNTAIN;
				}
				else if ((world[random_y][random_x+1].BIOME != WATER) && 
					(world[random_y][random_x+1].BIOME != MOUNTAIN) &&
					(world[random_y][random_x+1].BIOME != SNOW_MOUNTAIN)) {
					if (!(rand() % 5)) {
						world[random_y][random_x+1].BIOME = SNOW_MOUNTAIN;
					}
					else {
						world[random_y][random_x+1].BIOME = MOUNTAIN;
					}
				}
			}
		}
		if (random_x > 0) {
			check = rand() % 2;
			if (check) {
				if (world[random_y][random_x-1].BIOME == SNOW) {
					world[random_y][random_x-1].BIOME = SNOW_MOUNTAIN;
				}
				else if ((world[random_y][random_x-1].BIOME != WATER) && 
					(world[random_y][random_x-1].BIOME != MOUNTAIN) &&
					(world[random_y][random_x-1].BIOME != SNOW_MOUNTAIN)) {
					if (!(rand() % 5)) {
						world[random_y][random_x-1].BIOME = SNOW_MOUNTAIN;
					}
					else {
						world[random_y][random_x-1].BIOME = MOUNTAIN;
					}
				}
			}
		}
	
	}
	
	//MUSHROOM:
	
	if (!(rand() % 5)) { //optional
		for (k = 0 ; k < WORLD_SIZE_Y/30 ; k++) {
			do {
				random_x = rand() % WORLD_SIZE_X;
				random_y = rand() % WORLD_SIZE_Y;
			} while (world[random_y][random_x].BIOME != PLAIN);
		
			world[random_y][random_x].BIOME = MUSHROOM;
		}
	
		for (k = 0 ; k < WORLD_SIZE_X/2 ; k++) {
			do {
				random_y = rand() % WORLD_SIZE_Y;
				random_x = rand() % WORLD_SIZE_X;
			} while (world[random_y][random_x].BIOME != MUSHROOM);
		
			if (random_y < WORLD_SIZE_Y-1) {
				check = rand() % 2;
				if (check) {
					if (world[random_y+1][random_x].BIOME == PLAIN) {
						world[random_y+1][random_x].BIOME = MUSHROOM;
					}
				}
			}
			if (random_y > 0) {
				check = rand() % 2;
				if (check) {
					if (world[random_y-1][random_x].BIOME == PLAIN) {
						world[random_y-1][random_x].BIOME = MUSHROOM;
					}
				}
			}
			if (random_x < WORLD_SIZE_X - 1) {
				check = rand() % 2;
				if (check) {
					if (world[random_y][random_x+1].BIOME == PLAIN) {
						world[random_y][random_x+1].BIOME = MUSHROOM;
					}
				}
			}
			if (random_x > 0) {
				check = rand() % 2;
				if (check) {
					if (world[random_y][random_x-1].BIOME == PLAIN) {
						world[random_y][random_x-1].BIOME = MUSHROOM;
					}
				}
			}
		}
	}
	
	//FORESTS:
	
	for (k = 0 ; k < WORLD_SIZE_Y/10 ; k++) {
		do {
			random_x = rand() % WORLD_SIZE_X;
			random_y = rand() % WORLD_SIZE_Y;
		} while (world[random_y][random_x].BIOME != PLAIN);
		
		world[random_y][random_x].BIOME = FOREST;
	}
	
	for (k = 0 ; k < WORLD_SIZE_X*10 ; k++) {
		do {
			random_y = rand() % WORLD_SIZE_Y;
			random_x = rand() % WORLD_SIZE_X;
		} while (world[random_y][random_x].BIOME != FOREST);
		
		if (random_y < WORLD_SIZE_Y-1) {
			check = rand() % 2;
			if (check) {
				if (world[random_y+1][random_x].BIOME == PLAIN) {
					world[random_y+1][random_x].BIOME = FOREST;
				}
			}
		}
		if (random_y > 0) {
			check = rand() % 2;
			if (check) {
				if (world[random_y-1][random_x].BIOME == PLAIN) {
					world[random_y-1][random_x].BIOME = FOREST;
				}
			}
		}
		if (random_x < WORLD_SIZE_X - 1) {
			check = rand() % 2;
			if (check) {
				if (world[random_y][random_x+1].BIOME == PLAIN) {
					world[random_y][random_x+1].BIOME = FOREST;
				}
			}
		}
		if (random_x > 0) {
			check = rand() % 2;
			if (check) {
				if (world[random_y][random_x-1].BIOME == PLAIN) {
					world[random_y][random_x-1].BIOME = FOREST;
				}
			}
		}
	}

	//VOLCANOS:
	if (!(rand() % 5)) {
		do {
			random_x = rand() % WORLD_SIZE_X + 1;
			random_y = rand() % WORLD_SIZE_Y + 1;
		} while ((random_x >= WORLD_SIZE_X-5) || (random_y >= WORLD_SIZE_Y-3));
		world[random_y][random_x].BIOME = LAVA;

		for (k = 0 ; k < WORLD_SIZE_X*2 ; k++) {
			do {
				random_y = rand() % WORLD_SIZE_Y;
				random_x = rand() % WORLD_SIZE_X;
			} while ((world[random_y][random_x].BIOME != LAVA) && (world[random_y][random_x].BIOME != ASH));
		
			if (random_y < WORLD_SIZE_Y-1) {
				check = rand() % 2;
				if (check) {
					if (rand() % 4) {
						world[random_y+1][random_x].BIOME = ASH;
					}
					else {
						world[random_y+1][random_x].BIOME = LAVA;
					}
				}
			}
			if (random_y > 0) {
				check = rand() % 2;
				if (check) {
					if (rand() % 4) {
						world[random_y-1][random_x].BIOME = ASH;
					}
					else {
						world[random_y-1][random_x].BIOME = LAVA;
					}
				}
			}
			if (random_x < WORLD_SIZE_X - 1) {
				check = rand() % 2;
				if (check) {
					if (rand() % 4) {
						world[random_y][random_x+1].BIOME = ASH;
					}
					else {
						world[random_y][random_x+1].BIOME = LAVA;
					}
				}
			}
			if (random_x > 0) {
				check = rand() % 2;
				if (check) {
					if (rand() % 4) {
						world[random_y][random_x-1].BIOME = ASH;
					}
					else {
						world[random_y][random_x-1].BIOME = LAVA;
					}
				}
			}
		}
		*volcano_flag = TRUE;
		
	}	
}


void generate_deposits(squareT world[][WORLD_SIZE_X], int volcano_flag) {
	int k, j;
	int random_x, random_y;
	

	//STONE:
	
	for (k = 0 ; k < WORLD_SIZE_Y ; k++) {
		for (j = 0 ; j < WORLD_SIZE_X ; j++) {
			world[k][j].DEPOSIT = STONE;
		}
	}

	//COPPER:
	
	for (k = 0 ; k < WORLD_SIZE_Y*10 ; k++) {
		if (!(rand() % 5)) {
			random_x = rand() % WORLD_SIZE_X;
			random_y = rand() % WORLD_SIZE_Y;
			world[random_y][random_x].DEPOSIT = COPPER;
		}
	}

	//IRON:
	
	for (k = 0 ; k < WORLD_SIZE_Y*10 ; k++) {
		if (!(rand() % 5)) {
			random_x = rand() % WORLD_SIZE_X;
			random_y = rand() % WORLD_SIZE_Y;
			world[random_y][random_x].DEPOSIT = IRON;
		}
	}
	
	//SILVER:
	
	for (k = 0 ; k < WORLD_SIZE_Y*10 ; k++) {
		if (!(rand() % 6)) {
			random_x = rand() % WORLD_SIZE_X;
			random_y = rand() % WORLD_SIZE_Y;
			world[random_y][random_x].DEPOSIT = SILVER;
		}
	}

	//GOLD:
	
	for (k = 0 ; k < WORLD_SIZE_Y*10 ; k++) {
		if (!(rand() % 6)) {
			random_x = rand() % WORLD_SIZE_X;
			random_y = rand() % WORLD_SIZE_Y;
			world[random_y][random_x].DEPOSIT = IRON;
		}
	}
	
	//OBSIDIAN:
	if (volcano_flag) {
		for (k = 0 ; k < WORLD_SIZE_Y*10 ; k++) {
			if (!(rand() % 4)) {
				do {
					random_x = rand() % WORLD_SIZE_X;
					random_y = rand() % WORLD_SIZE_Y;
				} while ((world[random_y][random_x].BIOME != LAVA) && (world[random_y][random_x].BIOME != ASH));
				world[random_y][random_x].DEPOSIT = OBSIDIAN;
			}
		}
	}
	
	//Choose at random between mythril and adamantite:
	
	if (rand() % 2) {
		for (k = 0 ; k < WORLD_SIZE_Y*10 ; k++) {
			if (!(rand() % 7)) {
				random_x = rand() % WORLD_SIZE_X;
				random_y = rand() % WORLD_SIZE_Y;
				world[random_y][random_x].DEPOSIT = MYTHRIL;
			}
		}
	}
	else {
		for (k = 0 ; k < WORLD_SIZE_Y*10 ; k++) {
			if (!(rand() % 7)) {
				random_x = rand() % WORLD_SIZE_X;
				random_y = rand() % WORLD_SIZE_Y;
				world[random_y][random_x].DEPOSIT = ADAMANTITE;
			}
		}
	}
	
	//Choose at random between uranium and thorium:
	
	if (rand() % 2) {
		for (k = 0 ; k < WORLD_SIZE_Y*10 ; k++) {
			if (!(rand() % 8)) {
				random_x = rand() % WORLD_SIZE_X;
				random_y = rand() % WORLD_SIZE_Y;
				world[random_y][random_x].DEPOSIT = URANIUM;
			}
		}
	}
	else {
		for (k = 0 ; k < WORLD_SIZE_Y*10 ; k++) {
			if (!(rand() % 8)) {
				random_x = rand() % WORLD_SIZE_X;
				random_y = rand() % WORLD_SIZE_Y;
				world[random_y][random_x].DEPOSIT = THORIUM;
			}
		}
	}

}
	
	
	
	
	
	
void print_world_geographical(squareT world[][WORLD_SIZE_X]) {
	int k, j;
	
	for (k = 0 ; k < WORLD_SIZE_Y ; k++) {
		for (j = 0 ; j < WORLD_SIZE_X ; j++) {
			switch(world[k][j].BIOME) {
				case WATER:
					printf(BLUE"~"RESET);
					break;
				case PLAIN:
					printf(HI_GREEN"="RESET);
					break;
				case SNOW:
					printf("=");
					break;
				case DESERT:
					printf(YELLOW"~"RESET);
					break;
				case MOUNTAIN:
					printf(BROWN"^"RESET);
					break;
				case SNOW_MOUNTAIN:
					printf("^");
					break;
				case FOREST:
					printf(GREEN"F"RESET);
					break;
				case MUSHROOM:
					printf(HI_CYAN"T"RESET);
					break;
				case ASH:
					printf(GREY"="RESET);
					break;
				case LAVA:
					printf(RED"~"RESET);
					break;
			}
			
		}
		putchar('\n');
	}
}

void print_world_minerals(squareT world[][WORLD_SIZE_X]) {
	int k, j;
	
	for (k = 0 ; k < WORLD_SIZE_Y ; k++) {
		for (j = 0 ; j < WORLD_SIZE_X ; j++) {
			switch(world[k][j].DEPOSIT) {
				case COPPER:
					printf(RED"C"RESET);
					break;
				case STONE:
					printf(HI_GREY"S"RESET);
					break;
				case GOLD:
					printf(YELLOW"G"RESET);
					break;
				case SILVER:
					printf("s");
					break;
				case IRON:
					printf(BROWN"I"RESET);
					break;
				case OBSIDIAN:
					printf(GREY"O"RESET);
					break;
				case MYTHRIL:
					printf(CYAN"M"RESET);
					break;
				case ADAMANTITE:
					printf(BLUE"A"RESET);
					break;
				case THORIUM:
					printf(HI_CYAN"T"RESET);
					break;
				case URANIUM:
					printf(HI_GREEN"U"RESET);
					break;
			}
			
		}
		putchar('\n');
	}
}

char tochar(biomeT biome) {
	switch(biome) {
				case WATER:
					return('W');
					break;
				case PLAIN:
					return('=');
					break;
				case SNOW:
					return('*');
					break;
				case DESERT:
					return('~');
				case MOUNTAIN:
				case SNOW_MOUNTAIN:
					return('^');
				case FOREST:
					return('F');
				case MUSHROOM:
					return('T');
				case ASH:
					return('A');
			}
	return('L');
}

void fprint_geographical(squareT world[][WORLD_SIZE_X]) {
	int k, j;
	char name[100], filepath[120];
	FILE *printed;
	
	printf("Enter world name: ");
	scanf("%99s", name);
	strcpy(filepath, name);
	strcat(filepath, ".txt");

	printed = fopen(filepath, "w");
	for (k = 0 ; k < WORLD_SIZE_Y ; k++) {
		for (j = 0 ; j < WORLD_SIZE_X ; j++) {
			fputc(tochar(world[k][j].BIOME), printed);
		}
		fputc('\n', printed);
	}
	
	fclose(printed);
}

void clear_people(peopleT *people_matrix[], int people_size){
}
	
