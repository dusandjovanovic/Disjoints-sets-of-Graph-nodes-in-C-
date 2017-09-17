#pragma once
#include <iostream>
#include <random>

std::random_device rd;     // only used once to initialise (seed) engine
std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)

using namespace std;

template <class t, class w>
class linkedNode;

template <class t, class w>
class Edge {
public:
	linkedNode<t, w>* dest;
	Edge<t, w>* link;
	w weight;

	Edge() {
		dest = link = nullptr;
	}

	Edge(linkedNode<t, w>* newDest, Edge<t, w>* newLink) {
		dest = newDest;
		link = newLink;
	}
};

template <class t, class w>
class linkedNode {
public:
	t node;
	Edge<t, w>* adj;
	linkedNode<t, w>* next;
	int status;

	linkedNode() {
		adj = next = nullptr;
		status = 0;
	}

	linkedNode(t newNode) {
		node = newNode;
		adj = next = nullptr;
		status = 0;
	}

	linkedNode(t newNode, Edge<t, w>* newAdj, linkedNode<t, w>* newNext, int newStatus) {
		node = newNode;
		adj = newAdj;
		next = newNext;
		status = newStatus;
	}

	void visitNode() {
		cout << endl << node << endl;
	}
};

template <class t, class w>
class Graph {
public:
	linkedNode<t, w>* start;
	long nodeNumber;

public:
	Graph() {
		start = nullptr;
		nodeNumber = 0;
	}

	~Graph() {

	}

	void displayGraph() {
		linkedNode<t, w>* ptr = start;
		while (ptr != nullptr) {
			cout << ptr->node << " --> ";

			Edge<t, w>* ptrAdj = ptr->adj;
			while (ptrAdj != nullptr) {
				cout << ptrAdj->dest->node << " | ";
				ptrAdj = ptrAdj->link;
			}

			cout << endl;
			ptr = ptr->next;
		}
	}

	linkedNode<t, w>* findNode(t nodeValue) {
		linkedNode<t, w>* ptr = start;
		while (ptr != nullptr && ptr->node != nodeValue)
			ptr = ptr->next;
		return ptr;
	}

	Edge<t, w>* findEdge(t nodeA, t nodeB) {
		linkedNode<t, w>* pa = findNode(nodeA);
		linkedNode<t, w>* pb = findNode(nodeB);

		if (pa == nullptr || pb == nullptr)
			return nullptr;

		Edge<p, w>* ptr = pa->adj;
		while (ptr != nullptr && ptr->dest != pb)
			ptr = ptr->link;
		return ptr;
	}

	bool insertNode(t nodeValue) {
		linkedNode<t, w>* newNode = new linkedNode<t, w>(nodeValue, nullptr, start, 0); // start (next) :: dodavanje na pocetak liste cvorova, adj lista inicijalno prazna
		if (newNode == nullptr)
			return false;
		start = newNode;
		nodeNumber++;
		return true;
	}

	bool insertEdge(t nodeA, t nodeB) {
		linkedNode<t, w>* pa = findNode(nodeA);
		linkedNode<t, w>* pb = findNode(nodeB);
		if (pa == nullptr || pb == nullptr)
			return false;

		Edge<t, w>* ptr = new Edge<t, w>(pb, pa->adj); // link polje potega ukazuje na pocetak Adj liste prvog cvora
		if (ptr == nullptr)
			return false;
		pa->adj = ptr; // prelancavanje :: adj lista pocinje novim potegom
		return true;
	}

	bool deleteNode(t toDelete) {
		linkedNode<t, w>* ptr;
		if (start == nullptr)
			return false;
		if (start->node == toDelete) {
			ptr = start;
			Edge<t, w>* pot = start->adj;
			while (pot != nullptr) {
				Edge<t, w>* tpot = pot->link;
				delete pot;
				pot = tpot;
			}
			start->adj = nullptr;
			deleteEdgeToNode(start);
			ptr = start;
			start = start->next;
			delete ptr;
			nodeNumber--;
			return true;
		}
		else {
			ptr = start->next;
			linkedNode<t, w>* par = start;
			while (ptr != nullptr && ptr->node != toDelete) {
				par = ptr;
				ptr = ptr->next;
			}

			if (ptr == nullptr)
				return false;

			par->next = ptr->next;
			Edge<t, w>* pot = ptr->adj;
			while (pot != nullptr) {
				Edge<t, w>* tpot = pot->link;
				delete pot;
				pot = tpot;
			}

			ptr->adj = nullptr;
			deleteEdgeToNode(ptr);
			delete ptr;
			nodeNumber--;
			return true;
		}
	}
		
	void deleteEdgeToNode(linkedNode<t, w>* toNode) {
		linkedNode<t, w>* ptr = start;
		while (ptr != nullptr) {

			Edge<t, w>* ppot = ptr->adj;

			if (ppot == nullptr) {
				ptr = ptr->next;
				continue;
			}

			if (ppot->dest == toNode && ppot->link == nullptr) {
				delete ppot;
				ptr->adj = nullptr;
			}
			else if (ppot->dest == toNode) {
				Edge<t, w>* tmp = ppot;
				ppot = ppot->link;
				delete tmp;
			}
			else {
				Edge<t, w>* pred = ppot;
				Edge<t, w>* tmp = ppot->link;

				for (pred, tmp; tmp != nullptr && !(tmp->dest == toNode); pred = pred->link, tmp = tmp->link);

				if (tmp != nullptr) {
					pred->link = tmp->link;
					delete tmp;
				}
			}

			ptr = ptr->next;
		}
	}

	t returnRandomElement() {
		std::uniform_int_distribution<int> uni(0, nodeNumber-1); // guaranteed unbiased
		int toReturn = uni(rng);
		linkedNode<t, w>* node = start;

		for (int i = 0; i < toReturn; i++)
			node = node->next;

		if (node->node == NULL || node == nullptr)
			returnRandomElement();

		return node->node;
	}
};