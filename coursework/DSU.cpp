#include "DSU.h"



DSU::DSU(int setCount) {
	this->setCount = size = setCount;
	parent = new int[setCount];
	rank = new int[setCount];
	for (int i = 0; i < size; ++i) {
		parent[i] = i;
		rank[i] = 0;
	}
}

DSU::~DSU() {
	delete[] parent;
	delete[] rank;
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
}

int DSU::find(int x) const {
	if (x >= size)
		return -1;
	if (x == parent[x])
		return x;
	return parent[x] = find(parent[x]);
}

DSU::DSUIterator::DSUIterator(const DSU * u, int value)
{
	this->masterUnion = u;
	this->currentSet = value;
}

DSU::DSUIterator & DSU::DSUIterator::operator++()
{
	while (++currentSet < masterUnion->size)
		if (masterUnion->find(currentSet) == currentSet)
			return *this;
	return masterUnion->end();
}

bool DSU::DSUIterator::operator==(const DSUIterator & that)
{
	return (this->masterUnion == that.masterUnion && this->currentSet == that.currentSet);
}

bool DSU::DSUIterator::operator!=(const DSUIterator & that)
{
	return !this->operator==(that);
}

DSU::DSUIterator::DSIterator DSU::DSUIterator::begin() const
{
	for (int i = 0; i < masterUnion->size; ++i)
		if (masterUnion->find(i) == currentSet)
			return DSIterator(this, i);
	return this->end();
}

DSU::DSUIterator::DSIterator DSU::DSUIterator::end() const
{
	return *(new DSIterator(this, -1));
}

DSU::DSUIterator::DSIterator::DSIterator(const DSUIterator * s, int value)
{
	this->masterSet = s;
	this->currentElement = value;
}

DSU::DSUIterator::DSIterator & DSU::DSUIterator::DSIterator::operator++()
{
	while (++currentElement < masterSet->masterUnion->size)
		if (masterSet->masterUnion->find(currentElement) == masterSet->currentSet)
			return *this;
	return masterSet->end();
}

bool DSU::DSUIterator::DSIterator::operator==(const DSIterator & that)
{
	return (this->masterSet == that.masterSet && this->currentElement == that.currentElement);
}

bool DSU::DSUIterator::DSIterator::operator!=(const DSIterator & that)
{
	return !this->operator==(that);
}
