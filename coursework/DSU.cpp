#include "DSU.h"

DSU::DSU(int setCount) {
	this->setCount = size = setCount;
	parent = new int[size];
	rank = new int[size];
	for (int i = 0; i < size; ++i) {
		parent[i] = i;
		rank[i] = 0;
	}
}

DSU::DSU(const DSU & rhs)
{
	*this = rhs;
}

DSU::~DSU() {
	if (parent != nullptr)
		delete[] parent;
	if (rank != nullptr)
		delete[] rank;
}

DSU & DSU::operator=(const DSU & rhs)
{
	if (parent != nullptr)
		delete[] parent;
	if (rank != nullptr)
		delete[] rank;
	this->setCount = rhs.setCount;
	this->size = rhs.size;
	parent = new int[size];
	rank = new int[size];
	for (int i = 0; i < size; ++i) {
		parent[i] = rhs.parent[i];
		rank[i] = rhs.rank[i];
	}
	return *this;
}

void DSU::unite(int x, int y) {
	// поиск вершин деревьев
	x = find(x);
	y = find(y);
	if (x == y) return;
	// меньшее дерево X, большее Y
	if (rank[x] > rank[y])
		std::swap(x, y);
	parent[x] = y;
	if (rank[x] == rank[y]) // высота может измениться
		rank[y]++;
	--setCount;
}

int DSU::find(int x) const {
	if (x >= size)
		return -1;
	if (x == parent[x])
		return x;
	return parent[x] = find(parent[x]);
}

int * DSU::getLeaders() const
{
	int * result = new int[setCount];
	int resultIndex = 0;
	for (int x = 0; x < size; ++x)
		if (parent[x] == x)
			result[resultIndex++] = x;
	return result;
}
