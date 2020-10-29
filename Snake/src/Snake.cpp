#include "Snake.h"

Snake::Snake(int ix, int ivel_tabele){
	x = ix;
	y = ix;
	score = 2;
	vel_tabele = ivel_tabele;
}

void Snake::changeDir(bool* keys){
	if ((keys['W'] || keys[265]) && (direct != DOWN || score == 2))
		direct = UP;

	else if ((keys['S'] || keys[264]) && (direct != UP || score == 2))
		direct = DOWN;

	else if ((keys['A'] || keys[263]) && (direct != RIGHT || score == 2))
		direct = LEFT;

	else if ((keys['D'] || keys[262]) && (direct != LEFT || score == 2))
		direct = RIGHT;
}

void Snake::move(){
	switch (direct) {
		case UP:
			y--;
			break;
		case DOWN:
			y++;
			break;
		case LEFT:
			x--;
			break;
		case RIGHT:
			x++;
			break;
	}
}

void Snake::upScore(){
	score++;
}

Snake::~Snake()
{
}