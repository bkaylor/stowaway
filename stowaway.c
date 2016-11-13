#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

//
// This program finds the solution to the card game Stowaway 52
// Start with any card
// Each card leads to one of two other cards
// The game ends when you reach a "dead end"- both options are already-used cards
// Only one solution uses all 56 cards (?)
// Some cards are "item cards"
// Some next-card options require an "item card" to have already been visited
//

//
// Card ID's will be 0-55: the order being...
// 0-13: 	Blue 1 - Blue 14
// 14-27:	Purple 1 - Purple 14
// 28-41:	Green 1 - Green 14
// 42-55:	Red 1 - Red 14 
//

// 
// The game has 5 items, which are assigned item ID's:
// 0: N/A
// 1: FREEZE RAY
// 2: COMPUTER SPHERE
// 3: YELLOW KEYCARD
// 4: BLUE KEYCARD
// 5: RED KEYCARD
//

struct Card 
{
	int id;
	int option_1;
	int option_1_requires;
	int option_2;
	int option_2_requires;
	int item_id;
	int points;
};

const struct Card deck[56] = {
	{0, 40, 0, 36, 0, 0,  1},  {1, 18, 0, 28, 0, 0, 1},   {2, 17, 0, 30, 0, 0, 1},   {3, 6, 0, 42, 0, 0, 20},
	{4, 48, 0, 54, 0, 0,  1},  {5,  4, 0, 31, 0, 4, 5},   {6, 15, 3, 20, 0, 0, 1},   {7, 46, 0, 14, 0, 0, 1},
	{8, 53, 0, 46, 0, 0, 15},  {9, 37, 0, 1, 0, 0, 20},   {10, 0, 0, 8, 1, 0, 1},    {11, 26, 0, 0, 0, 0, 1},
	{12, 25, 0, 15, 3, 0, 1},  {13, 47, 0, 55, 0, 0, 15},

	{14, 24, 0, 6, 0, 0, 1},   {15, 40, 0, 29, 0, 0, 15}, {16, 35, 0, 54, 0, 0, 1},  {17, 50, 0, 36, 0, 2, 5},
	{18, 5, 0, 11, 0, 0, 1},   {19, 3, 0, 23, 2, 0, 1},   {20, 16, 0, 37, 0, 0, 20}, {21, 12, 0, 52, 0, 0, 1},
	{22, 2, 0, 55, 0, 0, 1},   {23, 8, 1, 21, 0, 0, 1},   {24, 41, 0, 47, 0, 0, 2},  {25, 43, 0, 31, 0, 0, 20},
	{26, 53, 0, 22, 0, 0, 1},  {27, 38, 5, 44, 0, 0, 1},

	{28, 1, 0, 9, 0, 0, 2},    {29, 13, 2, 35, 0, 0, 1},  {30, 29, 0, 10, 0, 5, 5},  {31, 24, 0, 27, 0, 0, 1},
	{32, 45, 0, 51, 0, 0, 20}, {33, 12, 0, 19, 0, 0, 1},  {34, 2, 0, 7, 0, 0, 20},   {35, 17, 0, 22, 0, 0, 1},
	{36, 49, 4, 43, 0, 0, 1},  {37, 28, 0, 9, 0, 0, 1},   {38, 32, 0, 51, 0, 0, 15}, {39, 27, 0, 7, 0, 0, 1},
	{40, 42, 0, 37, 0, 0, 1},  {41, 4, 0, 49, 4, 0, 1},
	
	{42, 16, 0, 14, 0, 0, 1},  {43, 34, 0, 39, 0, 0, 1},  {44, 13, 0, 52, 0, 0, 20}, {45, 34, 0, 3, 0, 0, 1},
	{46, 32, 0, 33, 0, 3, 5},  {47, 50, 0, 25, 0, 0, 1},  {48, 19, 0, 44, 0, 0, 1},  {49, 38, 0, 20, 0, 0, 15},
	{50, 30, 0, 18, 0, 0, 1},  {51, 21, 0, 26, 0, 0, 20}, {52, 23, 0, 45, 0, 0, 1},  {53, 41, 0, 33, 0, 0, 1},
	{54, 10, 0, 48, 0, 1, 5},  {55, 5, 0, 11, 0, 0, 1}
};

const int map_item_to_id[6] = {-1, 54, 17, 46, 5, 30};

FILE *fp;
int solutions = 1;

void print_card_id(int id)
{
	int suit = id / 14;
	int num = (id % 14) + 1;

	switch (suit)
	{
		case 0:
			fprintf(fp, "B");
			break;
		case 1:
			fprintf(fp, "P");
			break;
		case 2:
			fprintf(fp, "G");
			break;
		case 3:
			fprintf(fp, "R");
			break;
	}

	fprintf(fp, "%i ", num);
}

int visited[56] = {0};

void zero_visited()
{
	for (int i = 0; i < 56; ++i)
		visited[i] = 0;
}

void print_path()
{
	// Number in path
	int i = 1;

	// Number in array
	int j = 0;

	while (i <= 56)
	{
		for (j = 0; j < 56; ++j)
		{
			if (visited[j] == i)
				print_card_id(j);
		}
		++i;
	}
}

void dfs(int v, int pathsum, int order)
{
	// Visit node
	visited[v] = order;
	pathsum += deck[v].points;

#ifdef DEBUG
	for (int i = 0; i < order; ++i)
		fprintf(fp, "-");
	print_card_id(v);
	fprintf(fp, " %i", order);
	fprintf(fp, "\n");
#endif

	int have_item_1 = 1;
	int have_item_2 = 1;

	if (deck[v].option_1_requires != 0 && !visited[map_item_to_id[deck[v].option_1_requires]])
		have_item_1 = 0;

	if (deck[v].option_2_requires != 0 && !visited[map_item_to_id[deck[v].option_2_requires]])
		have_item_2 = 0;

	order++;
	// Visit adjacent nodes
	if (!visited[deck[v].option_1] && have_item_1)
	{
		dfs(deck[v].option_1, pathsum, order);
	}
	if (!visited[deck[v].option_2] && have_item_2)
	{
		dfs(deck[v].option_2, pathsum, order);
	}

	// If we reach an end, check our score
	if (visited[deck[v].option_1] && visited[deck[v].option_2])
	{
		if (pathsum == 300)
		{
			fprintf(fp, "\nSolution %i:\n", solutions);
			++solutions;
			print_path(); 
			fprintf(fp, "\n");
		}
	}

	// Unvisit this node, so we can go back up the tree
	visited[v] = 0;
}

int main(int argc, char *argv[])
{
	fp = fopen("solutions.txt", "w");

	for (int i = 0; i < 56; ++i)
	{
		zero_visited();
		dfs(i, 0, 1);
	}

	return 0;
}
