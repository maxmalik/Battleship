/*****************************
** Exam 2 Battleship	    **
** Max Malik 		    **
** Spring 2022              **
*****************************/

/* preprocessor directives */
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>	
#include <stdio.h>	
#include <string.h>	
#include <stdbool.h>
#include <time.h>
#define CLS system("cls")
#define PAUSE system("pause")
#define FLUSH myFlush()
#define ROWS 10
#define COLS 10

/* ship struct */
typedef struct {
	char name[20];
	int length;
	char firstLetter;
	int hitsTaken;
	char status[20]; // floating or sunk
}Ship;

/* function prototypes */
void checkForVictory(Ship shipArray[]);
bool doItFit(int direction, int xCoord, int yCoord, Ship s, char shipBoard[ROWS][COLS]);
void fillBoard(char board[ROWS][COLS]);
void getTargetInput(char* letter, int* number);
void hitOrMiss(char letter, int number, char shipBoard[ROWS][COLS], char gameBoard[ROWS][COLS], Ship shipArray[], int* missileCount);
void loadData(Ship shipArray[], char shipBoard[ROWS][COLS], char gameBoard[ROWS][COLS], int *missileCount);
void placeShip(Ship s, char shipBoard[ROWS][COLS]);
void printBoard(char board[ROWS][COLS]);
void printDirections();
void printStatuses(Ship shipArray[]);
void saveData(Ship shipArray[], char shipBoard[ROWS][COLS], char gameBoard[ROWS][COLS], int missileCount);
void myFlush();


int main() {
	// initialize ships with their properties
	Ship Banana = { .name = "Banana Ship", .length = 2, .firstLetter = 'B', .hitsTaken = 0, .status = "Floating" };
	Ship Super = { .name = "Super Ship", .length = 3, .firstLetter = 'S', .hitsTaken = 0, .status = "Floating" };
	Ship Rally = { .name = "Rally Ship", .length = 3, .firstLetter = 'R', .hitsTaken = 0, .status = "Floating" };
	Ship Valencia = { .name = "Valencia Ship", .length = 4, .firstLetter = 'V', .hitsTaken = 0, .status = "Floating" };
	Ship Gold = { .name = "Gold Ship", .length = 5, .firstLetter = 'G', .hitsTaken = 0, .status = "Floating" };

	Ship shipArray[5] = { Banana, Super, Rally, Valencia, Gold };

	char gameBoard[ROWS][COLS]; // gameBoard will store hit, miss, or any fully sunken ships
	char shipBoard[ROWS][COLS];	// shipBoard will store locations of the ships

	int missileCount = 0; // will be incremented after every turn

	// fill both boards with '.'
	fillBoard(gameBoard);
	fillBoard(shipBoard);

	// place ships in random spots and directions
	placeShip(Banana, shipBoard);
	placeShip(Super, shipBoard);
	placeShip(Rally, shipBoard);
	placeShip(Valencia, shipBoard);
	placeShip(Gold, shipBoard);

	// this is where game starts

	loadData(shipArray, shipBoard, gameBoard, &missileCount); // check for previous data 
	CLS;
	printDirections();
	printBoard(gameBoard);
	printStatuses(shipArray);
	printf("Missiles Fired: %i\n\n", missileCount);
	char letter = ' ';
	int number = -1;
	getTargetInput(&letter, &number);
	while (toupper(letter) != 'Q') {
		hitOrMiss(letter, number, shipBoard, gameBoard, shipArray, &missileCount);
		CLS;
		printBoard(gameBoard);
		printStatuses(shipArray);
		printf("Missiles Fired: %i\n\n", missileCount);
		checkForVictory(shipArray);

		getTargetInput(&letter, &number);
	}
	saveData(shipArray, shipBoard, gameBoard, missileCount);
}

void checkForVictory(Ship shipArray[]) {
	// if all the ships are sunk, display won message. do this by looping through each status in shipArray and checking for "sunk"
	int sunkCounter = 0;
	for (int i = 0; i < 5; i++) {
		if (strcmp(shipArray[i].status, "Sunk") == 0) {
			sunkCounter = sunkCounter + 1;
		}
	}
	if (sunkCounter == 5) {
		printf("\n\nYOU WON!!!!!\n\n");
		exit(-1);
	}
}

// could have used an int or char, but true/false boolean made more logical sense
bool doItFit(int direction, int xCoord, int yCoord, Ship s, char shipBoard[ROWS][COLS]) { // returns true if it fits, returns false if it doesnt

	// test horizontal ship
	if (direction == 0) {
		for (int i = 0; i < s.length; i++) {
			// if space is already filled with ship, return false
			if (shipBoard[xCoord + i][yCoord] != '.' || shipBoard[xCoord + i][yCoord] == NULL) {
				return false;
				break;
				return false;
			}
			// if the placement is tracking off the grid, return false
			if ((xCoord + i) > 10) {
				return false;
				break;
				return false;
			}
		}
		return true;
	}

	// test vertical ship
	if (direction == 1) {
		for (int i = 0; i < s.length; i++) {
			// if space is already filled with ship or ship is tracking off the grid, return false.
			if (shipBoard[xCoord][yCoord + i] != '.' || shipBoard[xCoord][yCoord + i] == NULL || (yCoord + i) > 10) {
				return false;
				break;
				return false;
			}
			// prevent vertical ships at the bottom of grid from continuning into top of next column
			for (int i = 0; i < 4; i++) {
				if ((s.length == (i + 2)) && (yCoord > (8 - i))) {
					return false;
					break;
					return false;
				}
			}
		}
		return true;
	}
}  

void fillBoard(char board[ROWS][COLS]) {
	int i, j;
	for (i = 0; i < ROWS; i++) {
		for (j = 0; j < COLS; j++) {
			board[i][j] = '.';
		}
	}
}   

void getTargetInput(char* letter, int* number) {
	char l = ' ';
	int n = -1;
	printf("Enter a space: ");
	scanf("%c%i", &l, &n); FLUSH;
	// execute Quit Return now to avoid unintentional outcomes during validation
	if (toupper(l) == 'Q') {
		*letter = 'Q';
		*number = 0;
		return;
	}
	// validate input
	while (n < 0 || n > 9 || toupper(l) < 'A' || toupper(l) > 'J') {
		printf("\nPlease enter a valid space.\n");
		printf("\nEnter a space: ");
		scanf("%c%i", &l, &n); FLUSH;
	}
	*letter = l;
	*number = n;
}  

void hitOrMiss(char letter, int number, char shipBoard[ROWS][COLS], char gameBoard[ROWS][COLS], Ship shipArray[], int *missileCount) {
	int row = -1;
	int column = -1;
	column = number;
	// convert letter to number
	switch (toupper(letter)) {
	case 'A':
		row = 0;
		break;
	case 'B':
		row = 1;
		break;
	case 'C':
		row = 2;
		break;
	case 'D':
		row = 3;
		break;
	case 'E':
		row = 4;
		break;
	case 'F':
		row = 5;
		break;
	case 'G':
		row = 6;
		break;
	case 'H':
		row = 7;
		break;
	case 'I':
		row = 8;
		break;
	case 'J':
		row = 9;
		break;
	}
	if ((gameBoard[column][row] != '.')) {
		printf("\nYou have already aimed here. Please aim somewhere else.\n\n");
		PAUSE;
		return;
	}
	// hit Banana Ship
	if (shipBoard[column][row] == 'B') {
		gameBoard[column][row] = 'H';
		shipArray[0].hitsTaken += 1; // increment hits taken on Banana ship
		if (shipArray[0].hitsTaken == shipArray[0].length) { // if the ship is fully sunk
			strcpy(shipArray[0].status, "Sunk"); // update status to "Sunk"
			// display ship first letter in game board for ship spaces
			for (int i = 0; i < ROWS; i++) {
				for (int j = 0; j < COLS; j++) {
					if (shipBoard[i][j] == 'B') {
						gameBoard[i][j] = 'B';
					}
				}
			}
		}
		*missileCount += 1;
	}
	// hit Super Ship
	else if (shipBoard[column][row] == 'S') {
		gameBoard[column][row] = 'H';
		shipArray[1].hitsTaken += 1; // increment hits taken on Super ship
		if (shipArray[1].hitsTaken == shipArray[1].length) { // if the ship is fully sunk
			strcpy(shipArray[1].status, "Sunk"); // update status to "Sunk"
			// display ship first letter in game board for ship spaces
			for (int i = 0; i < ROWS; i++) {
				for (int j = 0; j < COLS; j++) {
					if (shipBoard[i][j] == 'S') {
						gameBoard[i][j] = 'S';
					}
				}
			}
		}
		*missileCount += 1;
	}
	// hit Rally Ship
	else if (shipBoard[column][row] == 'R') {
		gameBoard[column][row] = 'H';
		shipArray[2].hitsTaken += 1; // increment hits taken on Rally ship
		// if the ship is fully sunk
		if (shipArray[2].hitsTaken == shipArray[2].length) {
			strcpy(shipArray[2].status, "Sunk"); // update status to "Sunk"
			// display ship first letter in game board for ship spaces
			for (int i = 0; i < ROWS; i++) {
				for (int j = 0; j < COLS; j++) {
					if (shipBoard[i][j] == 'R') {
						gameBoard[i][j] = 'R';
					}
				}
			}
		}
		*missileCount += 1;
	}
	// hit Valencia Ship
	else if (shipBoard[column][row] == 'V') {
		gameBoard[column][row] = 'H';
		shipArray[3].hitsTaken += 1; // increment hits taken on Valencia ship
		if (shipArray[3].hitsTaken == shipArray[3].length) { // if the ship is fully sunk
			strcpy(shipArray[3].status, "Sunk"); // update status to "Sunk"
			// display ship first letter in game board for ship spaces
			for (int i = 0; i < ROWS; i++) {
				for (int j = 0; j < COLS; j++) {
					if (shipBoard[i][j] == 'V') {
						gameBoard[i][j] = 'V';
					}
				}
			}
		}
		*missileCount += 1;
	}
	// hit Gold Ship
	else if (shipBoard[column][row] == 'G') {
		gameBoard[column][row] = 'H';
		shipArray[4].hitsTaken += 1; // increment hits taken on Gold ship
		if (shipArray[4].hitsTaken == shipArray[4].length) { // if the ship is fully sunk
			strcpy(shipArray[4].status, "Sunk"); // update status to "Sunk"
			// display ship first letter in game board for ship spaces
			for (int i = 0; i < ROWS; i++) {
				for (int j = 0; j < COLS; j++) {
					if (shipBoard[i][j] == 'G') {
						gameBoard[i][j] = 'G';
					}
				}
			}
		}
		*missileCount += 1;
	}
	// miss
	else if (shipBoard[column][row] == '.') {
		gameBoard[column][row] = 'M';
		*missileCount += 1;
	}
}

void loadData(Ship shipArray[], char shipBoard[ROWS][COLS], char gameBoard[ROWS][COLS], int* missileCount) {
	// if there is nothing to open, just return.
	FILE* ptr = fopen("battleshipSaves.bin", "rb");
	if (ptr == NULL) {
		return;
	}
	// ask if user wants to open saved game or start a new one
	char choice;
	printf("[N]ew Game\n[L]oad Game\n\nSelect an Option: ");
	scanf("%c", &choice); FLUSH;
	while ((toupper(choice) != 'N') && (toupper(choice) != 'L')) {
		printf("Please enter a valid choice.\n\n");
		printf("\nSelect an Option: ");
		scanf("%c", &choice); FLUSH;
	}
	// return, effectively starting a new game
	if (toupper(choice) == 'N') {
		return;
	}
	// load previous game from saves
	if (toupper(choice) == 'L') {
		fread(missileCount, sizeof(int), 1, ptr);
		fread(shipArray, sizeof(Ship), 5, ptr);
		fread(shipBoard, sizeof(char), ROWS * COLS, ptr);
		fread(gameBoard, sizeof(char), ROWS * COLS, ptr);
	}
	fclose(ptr);
}   

void placeShip(Ship s, char shipBoard[ROWS][COLS]) {
	int xCoord, yCoord, direction;
	bool test = false;

	// generate new coordinates if ship doesn't fit
	while (test == false) {
		srand(time(0));
		xCoord = (rand() % (9 - 0 + 1)) + 0;
		yCoord = (rand() % (9 - 0 + 1)) + 0;
		direction = (rand() % (1 - 0 + 1)) + 0; // 0 = horizontal, 1 = vertical;
		test = doItFit(direction, xCoord, yCoord, s, shipBoard);
	}

	// enter ship horizontally into shipBoard array
	if (direction == 0) {
		for (int i = 0; i < s.length; i++) {
			shipBoard[xCoord + i][yCoord] = s.firstLetter;
		}
	}

	// enter ship vertically into shipBoard array
	if (direction == 1) {
		for (int i = 0; i < s.length; ++i) {
			shipBoard[xCoord][yCoord + i] = s.firstLetter;
		}
	}
}  

void printBoard(char board[ROWS][COLS]) {
	int i, j;
	int rowCounter = 0;
	printf("    0 1 2 3 4 5 6 7 8 9 \n");
	printf("   --------------------");
	for (i = 0; i < COLS; i++) {
		printf("                       \n");
		// switch is simply for printing A - J on left side of grid
		switch (rowCounter) {
			case 0:
				printf(" A|");
				break;
			case 1:
				printf(" B|");
				break;
			case 2:
				printf(" C|");
				break;
			case 3:
				printf(" D|");
				break;
			case 4:
				printf(" E|");
				break;
			case 5:
				printf(" F|");
				break;
			case 6:
				printf(" G|");
				break;
			case 7:
				printf(" H|");
				break;
			case 8:
				printf(" I|");
				break;
			case 9:
				printf(" J|");
				break;
		}
		rowCounter++;
		for (j = 0; j < ROWS; j++) {
			printf(" %c", board[j][i]);
			printf("");
		}
		printf(" \n");
	}
}  

void printDirections() {
	printf("Directions:\n");
	printf("5 ships with varying lengths are randomly scattered throughout the board.\n");
	printf("Enter a row letter and a column number to fire a missile. Ex: G4 or QQ to quit.\n");
	printf("Goal: Sink all ships\n\n");
}

void printStatuses(Ship shipArray[]) {
	printf("\n");
	for (int i = 0; i < 5; i++) {
		printf("%s: %s\n", shipArray[i].name, shipArray[i].status);
	}
	printf("\n");
}   

void saveData(Ship shipArray[], char shipBoard[ROWS][COLS], char gameBoard[ROWS][COLS], int missileCount) {
	FILE* ptr = fopen("battleshipSaves.bin", "wb");
	if (ptr == NULL) {
		printf("\nUnable to save data.\n\n");
		return;
	}
	fwrite(&missileCount, sizeof(int), 1, ptr);
	fwrite(shipArray, sizeof(Ship), 5, ptr);
	fwrite(shipBoard, sizeof(char), ROWS * COLS, ptr);
	fwrite(gameBoard, sizeof(char), ROWS * COLS, ptr);
	printf("\nThe data has been saved.\n\n");
	fclose(ptr);
}   

void myFlush() {
	while (getchar() != '\n');
}
