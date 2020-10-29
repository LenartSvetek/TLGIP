#include "Cell.h"

Cell::Cell(GLfloat ix, GLfloat iy, GLfloat iwidth) : Rectangle(ix, iy, iwidth, iwidth) {}

void Cell::setMode(std::string imode){
	mode = imode;
	
	if (mode == "Nada")
		setColour(0, 0, 0);
	if (mode == "Snake")
		setColour(255, 0, 128);
	if (mode == "Fruit")
		setColour(0, 255, 0);
}

void Cell::setHP(int score){
	HP = score; 
}

void Cell::update(){
	if (HP > 0) {
		
		HP--;
		if (HP <= 0) {
			setMode("Nada");
			HP = 0;
		}
	}
}

Cell::~Cell()
{
}
