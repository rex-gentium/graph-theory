#pragma once
#include <vector>
/*
	Система непересекающихся множеств.
	Не является полноценной реализацией: возможно только создать систему из N 
	пронумерованных элементов, каждый из которых образует множество. Добавить
	новое множество нельзя.
*/
class DSU {
public:
	struct Element {
		int parent = -1; // индекс в массиве, можно заменить на указатель
		int rank = 0;	// высота дерева от этого элемента
	};

	/* создаёт систему из setCount непересекающихся множеств, id множества является порядковый номер*/
	DSU(int setCount);
	~DSU();

	/*создаёт новое множество с указанным лидером*/
	//void makeSet(int x); // для реализации потребуется заменить массив предков на карту

	/*объединеняет два множества указанных представителей. Амортизированная O(1)*/
	void unite(int x, int y);
	/*возвращает лидера множества указанного представителя. Амортизированная O(1)*/
	int find(int x) const;
	/* возвращает количество множеств */
	int getSetCount() const { return setCount; }
	
	/* итераторная часть */
	static class DSUIterator {
		const DSU * masterUnion;
		int currentSet;
	public:
		DSUIterator(const DSU * u, int value);
		DSUIterator& operator++();
		bool operator==(const DSUIterator& that);
		bool operator!=(const DSUIterator& that);
		static class DSIterator {
			const DSUIterator * masterSet;
			int currentElement;
		public:
			DSIterator(const DSUIterator * s, int value);
			DSIterator& operator++();
			bool operator==(const DSIterator& that);
			bool operator!=(const DSIterator& that);
			int operator*();
		};
		DSIterator begin() const;
		DSIterator end() const;

	};
	DSUIterator begin() const { return *(new DSUIterator(this, 0)); }
	DSUIterator end() const { return *(new DSUIterator(this, -1)); }

private:
	int * parent;
	int * rank;
	int size;
	int setCount;
};

