#pragma once

#include <vector>
#include "Cell.h"

#include <stdlib.h>
#include <ctime>

class Fruit{
public:
	Fruit(int vel_tabele, std::vector<Cell>& CellList);

	~Fruit();
private:
	int x, y;
};

