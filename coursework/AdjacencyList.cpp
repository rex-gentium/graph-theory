#include "AdjacencyList.h"
#include <fstream>
#include <sstream>


AdjacencyList::AdjacencyList() {}

AdjacencyList::AdjacencyList(int vertexCount, bool isWeigted, bool isDirected) {
	this->isWeighted = isWeighted;
	this->isDirected = isDirected;
	this->vertexCount = vertexCount;
	if (isWeighted)
		this->weightedAdjacencyList.resize(vertexCount);
	else
		this->adjacencyList.resize(vertexCount);
}


AdjacencyList::~AdjacencyList() {}

void AdjacencyList::read(istream & inpFile) {
	int orientationFlag, weightFlag;
	inpFile >> this->vertexCount;
	inpFile >> orientationFlag;
	this->isDirected = (orientationFlag > 0);
	inpFile >> weightFlag;
	this->isWeighted = (weightFlag > 0);
	string line;
	getline(inpFile, line); // force jump to next line
	if (isWeighted) {
		for (int vertex = 0; vertex < vertexCount; vertex++) {
			set<pair<int, int>> listContent;
			getline(inpFile, line);
			int to, weight;
			istringstream iss(line);
			while (iss >> to) {
				--to; // поправка на нумерацию с нуля
				iss >> weight;
				listContent.insert(make_pair(to, weight));
			}
			weightedAdjacencyList.push_back(listContent);
		}
	}
	else {
		for (int vertex = 0; vertex < vertexCount; vertex++) {
			set<int> listContent;
			getline(inpFile, line);
			//cout << line;
			istringstream iss(line);
			int to;
			while (iss >> to) {
				--to; // поправка на нумерацию с нуля
				listContent.insert(to);
			}
			adjacencyList.push_back(listContent);
		}
	}
}

void AdjacencyList::write(ostream & outFile) {
	outFile << 'L' << ' ' << vertexCount << '\n'
		<< ((isDirected) ? 1 : 0) << ' ' << ((isWeighted) ? 1 : 0) << '\n';
	if (isWeighted) {
		for (int vertex = 0; vertex < vertexCount; vertex++) {
			for (const auto & adjacency : weightedAdjacencyList[vertex])
				outFile << (adjacency.first + 1) << ' ' << adjacency.second << ' ';
			outFile << '\n';
		}
	}
	else {
		for (int vertex = 0; vertex < vertexCount; vertex++) {
			for (const int & adjacent : adjacencyList[vertex])
				outFile << (adjacent + 1) << ' ';
			outFile << '\n';
		}
	}
}

void AdjacencyList::addEdge(int from, int to, int weight) {
	if (isWeighted) {
		weightedAdjacencyList[from].insert(make_pair(to, weight));
		if (!isDirected) // неориентированный граф дублирует смежности
			weightedAdjacencyList[to].insert(make_pair(from, weight));
	}
	else {
		adjacencyList[from].insert(to);
		if (!isDirected) // неориентированный граф дублирует смежности
			adjacencyList[to].insert(from);
	}
}

int AdjacencyList::changeEdge(int from, int to, int newWeight) {
	int oldWeight = 0;
	set<pair<int, int>> * vertexAdjList = &(weightedAdjacencyList[from]);
	auto it = vertexAdjList->lower_bound(make_pair(to, 0));
	if (it != vertexAdjList->end()) {
		oldWeight = it->second;
		auto insertionHint = it; // should I really be doing this?
		++insertionHint;
		vertexAdjList->erase(it);
		vertexAdjList->insert(insertionHint, make_pair(to, newWeight));
	}
	if (!isDirected) {
		// неориентированный граф дублирует списки
		vertexAdjList = &(weightedAdjacencyList[to]);
		it = vertexAdjList->lower_bound(make_pair(from, 0));
		if (it != vertexAdjList->end()) {
			oldWeight = it->second;
			auto insertionHint = it;
			++insertionHint;
			vertexAdjList->erase(it);
			vertexAdjList->insert(insertionHint, make_pair(from, newWeight));
		}
	}
	return oldWeight;
}

void AdjacencyList::removeEdge(int from, int to) {
	if (isWeighted) {
		set<pair<int, int>> * vertexAdjList = &(weightedAdjacencyList[from]);
		// вес не имеет значения, сравнение происходит по первому элементу пары
		auto it = vertexAdjList->lower_bound(make_pair(to, 0));
		if (it != vertexAdjList->end())
			vertexAdjList->erase(it);
		if (!isDirected) { // в неориентированном графе списки дублируются
			vertexAdjList = &(weightedAdjacencyList[to]);
			it = vertexAdjList->lower_bound(make_pair(from, 0));
			if (it != vertexAdjList->end())
				vertexAdjList->erase(it);
		}
	}
	else {
		set<int> * vertexAdjList = &(adjacencyList[from]);
		auto it = vertexAdjList->find(to);
		if (it != vertexAdjList->end())
			vertexAdjList->erase(it);
		if (!isDirected) { // в неориентированном графе списки дублируются
			vertexAdjList = &(adjacencyList[to]);
			it = vertexAdjList->find(from);
			if (it != vertexAdjList->end())
				vertexAdjList->erase(it);
		}
	}
}

list<tuple<int, int, int>> AdjacencyList::getWeightedEdgesList() const
{
	list<tuple<int, int, int>> result;
	for (int from = 0; from < vertexCount; ++from)
		for (const auto & adjacency : weightedAdjacencyList[from]) {
			int to = adjacency.first;
			int weight = adjacency.second;
			if (isDirected || from <= to) // избегаем дублей в неориентированном графе
				result.push_back(make_tuple(from, to, weight));
		}
	return result;
}

tuple<int, int, int> AdjacencyList::findMinEdge(bool * isMarked) const
{
	int minWeight = INT_MAX, minI = -1, minJ = -1;
	for (int i = 0; i < vertexCount; i++) {
		if (!isMarked[i]) continue;
		for (const auto & adjacency : weightedAdjacencyList[i]) {
			int j = adjacency.first;
			if (isMarked[j]) continue;
			int weight = adjacency.second;
			if (weight < minWeight) {
				minWeight = weight;
				minI = i;
				minJ = j;
			}
		}
	}
	return make_tuple(minI, minJ, minWeight);
}

GraphContent::ConstEdgeIterator& AdjacencyList::edgeBegin() const
{
	if (isWeighted) {
		for (int from = 0; from < vertexCount; ++from)
			if (!weightedAdjacencyList[from].empty()) {
				return *new AdjListEdgeIterator(this, from,
					AdjListEdgeIterator::InnerWeightedIterator(weightedAdjacencyList[from].cbegin()));
			}
	}
	else {
		for (int from = 0; from < vertexCount; ++from)
			if (!adjacencyList[from].empty())
				return *new AdjListEdgeIterator(this, from, 
					AdjListEdgeIterator::InnerIterator(adjacencyList[from].cbegin()));
	}
	return edgeEnd();
}

GraphContent::ConstEdgeIterator& AdjacencyList::edgeEnd() const
{
	return (isWeighted) 
		? *new AdjListEdgeIterator(this, -1, AdjListEdgeIterator::InnerWeightedIterator::null())
		: *new AdjListEdgeIterator(this, -1, AdjListEdgeIterator::InnerIterator::null());
}

tuple<int, int, int> AdjacencyList::nextEdge(ConstEdgeIterator & iterator) const
{
	AdjListEdgeIterator& edgeIt = dynamic_cast<AdjListEdgeIterator&>(iterator);
	AdjListEdgeIterator::InnerIterator innerIt = edgeIt.inner();
	if (!innerIt.isNull())
		return nextEdge(iterator.from(), innerIt);
	AdjListEdgeIterator::InnerWeightedIterator& innerWIt = edgeIt.innerWeighted();
	if (!innerWIt.isNull())
		return nextEdge(iterator.from(), innerWIt);
	return make_tuple(-1, -1, -1);
}

tuple<int, int, int> AdjacencyList::nextEdge(int fromV, AdjListEdgeIterator::InnerIterator & iterator) const
{
	// идем дальше по текущему списку смежности
	if (++iterator != adjacencyList[fromV].cend())
		return make_tuple(fromV, *iterator, -1);
	else { // переходим в следующий список смежности
		if (++fromV < vertexCount) {
			iterator = adjacencyList[fromV].cbegin();
			return make_tuple(fromV, *iterator, -1);
		}
		else { // конец коллекции
			iterator = AdjListEdgeIterator::InnerIterator::null();
			return make_tuple(-1, -1, -1);
		}
	}
}

tuple<int, int, int> AdjacencyList::nextEdge(int fromV, AdjListEdgeIterator::InnerWeightedIterator & iterator) const
{
	// идем дальше по текущему списку смежности
	if (++iterator != weightedAdjacencyList[fromV].cend())
		return make_tuple(fromV, iterator->first, iterator->second);
	else { // переходим в следующий список смежности
		if (++fromV < vertexCount) {
			iterator = weightedAdjacencyList[fromV].cbegin();
			return make_tuple(fromV, iterator->first, iterator->second);
		}
		else { // конец коллекции
			iterator = AdjListEdgeIterator::InnerWeightedIterator::null();
			return make_tuple(-1, -1, -1);
		}
	}
}

AdjacencyList::AdjListEdgeIterator::AdjListEdgeIterator(const AdjacencyList * graph, int fromV, InnerIterator it) :
	ConstEdgeIterator(graph, fromV, !it.isNull() ? *it : -1),
	adjList(graph),
	it(it),
	wIt(AdjListEdgeIterator::InnerWeightedIterator::null())
{}

AdjacencyList::AdjListEdgeIterator::AdjListEdgeIterator(const AdjacencyList * graph, int fromV, InnerWeightedIterator it) :
	ConstEdgeIterator(graph, fromV,
		(!it.isNull()) ? it->first : -1,
		(!it.isNull()) ? it->second : -1),
	adjList(graph),
	it(AdjListEdgeIterator::InnerIterator::null()),
	wIt(it)
{}

/*void AdjacencyList::AdjListEdgeIterator::operator++()
{
	if (*this != graph->edgeEnd())
		edge = (graph->isWeighted)
			? adjList->nextEdge(get<0>(edge), wIt)
			: adjList->nextEdge(get<0>(edge), it);
}*/
