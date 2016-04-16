// drzewo.cpp : main project file.

#include "stdafx.h"

using namespace System;

//int main(array<System::String ^> ^args)
//{
//    Console::WriteLine(L"Hello World");
//    return 0;
//}
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>

typedef int ElType;

typedef struct Tree {
	ElType key;
	struct Tree *left;
	struct Tree *right;
} Tree;

Tree* InsertBST(Tree* t, int k)
{
	if (t == NULL) {
		Tree* w = (Tree*)malloc(sizeof(Tree));
		w->key = k;
		w->left = NULL;
		w->right = NULL;
		return w;
	}

	if (k <= t->key)
		t->left = InsertBST(t->left, k);
	else
		t->right = InsertBST(t->right, k);

	return t;
}

int NumberOfLeaves(Tree* t)
{
	if (t == NULL)
		return 0;

	if (t->left == NULL && t->right == NULL)
		return 1;

	return NumberOfLeaves(t->left) + NumberOfLeaves(t->right);
}

#define SCREEN_HEIGHT  100
#define SCREEN_WIDTH    80

typedef struct {
	int numberOfLines;
	char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 2]; // 100 linii, kaşda linia ma 80 znaków plus \n i \0
} TreeDisplay;

/* For a better display:
#define DOWN                    179
#define RIGHT_DOWN              191
#define DOWN_LEFT_AND_RIGHT     193
#define LEFT_DOWN               218
#define DASH                    196
*/

// For a solid display:
#define DOWN                    '|'
#define RIGHT_DOWN              '.'
#define DOWN_LEFT_AND_RIGHT     '^'
#define LEFT_DOWN               '.'
#define DASH                    '-'

void WriteSymbolToTreeDisplay(TreeDisplay* td, char c, int row, int col)
{
	td->screen[row][col] = c;

	if (row + 1 > td->numberOfLines)
		td->numberOfLines = row + 1;
}

void WriteKeyToTreeDisplay(TreeDisplay* td, int key, int row, int col)
{
	if (key >= 1000)
		td->screen[row][col - 1] = td->screen[row][col] = td->screen[row][col + 1] = '.'; // za duza wartosc
	else if (key >= 100) {
		td->screen[row][col - 1] = (key / 100) + '0';
		td->screen[row][col] = ((key / 10) % 10) + '0';
		td->screen[row][col + 1] = (key % 10) + '0';
	}
	else if (key >= 10) {
		td->screen[row][col] = (key / 10) + '0';
		td->screen[row][col + 1] = (key % 10) + '0';
	}
	else if (key >= 0)
		td->screen[row][col] = key + '0';
	else {
		td->screen[row][col] = '-';
		td->screen[row][col + 1] = 'X';
	}

	if (row + 1 > td->numberOfLines)
		td->numberOfLines = row + 1;
}

int UpdateTreeDisplay(TreeDisplay* td, Tree* t, int depth, int left, int right)
// wypisz drzewo t do ekranu td w obszarze: pionowo od depth do SCREEN_HEIGHT, a poziomowo od left do right;
// funkcja zwraca pozimowa pozycje korzenia drzewa
{
	if (t == NULL)
		return -1;

	int left_width = NumberOfLeaves(t->left);
	int right_width = NumberOfLeaves(t->right);

	if (left_width + right_width == 0) {
		int middle = (left + right) / 2;
		WriteKeyToTreeDisplay(td, t->key, depth, middle);
		return middle;
	}

	if (left_width == 0) {
		int middle = UpdateTreeDisplay(td, t->right, depth + 2, left, right);
		WriteKeyToTreeDisplay(td, t->key, depth, middle);
		WriteSymbolToTreeDisplay(td, DOWN, depth + 1, middle);
		return middle;
	}

	if (right_width == 0) {
		int middle = UpdateTreeDisplay(td, t->left, depth + 2, left, right);
		WriteKeyToTreeDisplay(td, t->key, depth, middle);
		WriteSymbolToTreeDisplay(td, DOWN, depth + 1, middle);
		return middle;
	}

	int k = (right - left - 2) / (left_width + right_width);
	int l = UpdateTreeDisplay(td, t->left, depth + 2, left, left + left_width*k - 1);
	int r = UpdateTreeDisplay(td, t->right, depth + 2, left + left_width*k + 1, right - 1);

	int middle = (l + r) / 2;
	WriteKeyToTreeDisplay(td, t->key, depth, middle);

	WriteSymbolToTreeDisplay(td, DOWN_LEFT_AND_RIGHT, depth + 1, middle);
	WriteSymbolToTreeDisplay(td, LEFT_DOWN, depth + 1, l);
	WriteSymbolToTreeDisplay(td, RIGHT_DOWN, depth + 1, r);

	int i;
	for (i = l + 1; i < middle; i++)
		WriteSymbolToTreeDisplay(td, DASH, depth + 1, i);
	for (i = middle + 1; i < r; i++)
		WriteSymbolToTreeDisplay(td, DASH, depth + 1, i);

	return middle;
}


void DisplayTree(Tree* t) //kg
{
	TreeDisplay TD;
	TD.numberOfLines = 0;
	int i, j;
	for (i = 0; i < SCREEN_HEIGHT; i++) {
		for (j = 0; j < SCREEN_WIDTH; j++)
			TD.screen[i][j] = ' '; // spacja
		TD.screen[i][SCREEN_WIDTH] = '\n';
		TD.screen[i][SCREEN_WIDTH + 1] = '\0';
	}

	UpdateTreeDisplay(&TD, t, 0, 1, SCREEN_WIDTH - 1);

	for (i = 0; i < TD.numberOfLines; i++)
		printf("%s", TD.screen[i]);
	printf("-------------\n");
}

int main()
{
	int i;
	srand(time(NULL));

	Tree* t = NULL;
	for (i = 0; i < 20; i++)
		t = InsertBST(t, rand() % 1000);

	DisplayTree(t);
	getchar();

	return 0;
}
