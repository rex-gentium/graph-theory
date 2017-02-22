#include "DSU.h"



DSU::DSU(int setCount) {
	forest.resize(setCount);
}

DSU::~DSU() {
	forest.clear();
}

void DSU::unite(int x, int y) {
	// поиск вершин деревьев
	x = find(x);
	y = find(y);
	if (x == y) return;
	// меньшее дерево X, большее Y
	if (forest[x].rank > forest[y].rank) std::swap(x, y);
	
	forest[x].parent = y;
	if (forest[y].rank == 0) // высота может измениться
		forest[y].rank++;
}

int DSU::find(int x) const {
	while (forest[x].parent != -1)
		x = forest[x].parent;
	return x;
}
