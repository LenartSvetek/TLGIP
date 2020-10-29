#include "Fruit.h"

Fruit::Fruit(int vel_tabele, std::vector<Cell>& CellList){
	srand(time(NULL));
	x = rand() % (vel_tabele);
	y = rand() % (vel_tabele);
	if (CellList[x + y * vel_tabele].getMode() == "Snake") {
		x = rand() % (vel_tabele);
		y = rand() % (vel_tabele);
		if (CellList[x + y * vel_tabele].getMode() == "Snake") {
			bool sort = false;
			for (int i = 0; i < CellList.size(); i++) {
				if (CellList[i].getMode() == "Nada") {
					CellList[i].setMode("Fruit");
					break;
				}
			}
			return;
		}
	}
	CellList[x + y * vel_tabele].setMode("Fruit");
}

Fruit::~Fruit()
{
}
