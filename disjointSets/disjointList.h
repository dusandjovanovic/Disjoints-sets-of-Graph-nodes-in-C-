#pragma once
#include <iostream>
#include "graphImplementation.h"
using namespace std;

struct Node
{
	int data, rank;
	Node *parent;
	Node* next = nullptr;
};

Node* newNode(int key)
{
	Node* temp = new Node;
	temp->data = key;
	temp->rank = 0;
	temp->parent = nullptr;
	return temp;
}

class disjointUnion {
private:
	Node* nodes;
	Node** main_parent;
	int size;

	Graph<int, int>* representedGraph;

	void selectParents(int** arr, int n) {
		for (int i = 0; i < n; i++) {
			int x = arr[i][0];
			int y = arr[i][1];
			Union(x, y);
		}
	}

	int findMainParents() {
		// broj predstavnika/roditelja najviseg reda :: disjunktnih skupova
		int count = 0;
		linkedNode<int, int>* ptr = representedGraph->start;
		for (int i = 0; i < size; i++) {
			main_parent[i] = Find(ptr->node); // nalazi predstavnika skupa za svaki clan
			if (main_parent[i]->data != -1 && main_parent[i] == &nodes[i])  // predstavnik skupa je roditelj samom sebi
				count++;
			ptr = ptr->next;
		}
		return count;
	}

public:
	disjointUnion(int n, Graph<int, int>* graph) {
		// size argumenat odgovara broju cvorova grafa
		// size = graph->nodeNumber
		size = n;
		nodes = new Node[size];
		main_parent = new Node*[size];
		representedGraph = graph;
		makeSet();
	}

	void makeSet() {
		// n supova sa jednim elementom :: svaki predstavnik skupa
		// inicijalno n disjunktnih skupova
		linkedNode<int, int>* ptr = representedGraph->start;
		for (int i = 0; i < size; i++) {
			nodes[i].data = ptr->node;
			nodes[i].parent = &nodes[i];
			nodes[i].rank = 0;
			ptr = ptr->next;
		}
	}

	Node* Find(int x) {
		// nalazi predstavnika skupa ciji je x elemenat

		if (x < 0)
			return newNode(-1);

		if (findNode(x)->data == -1 || findNode(x)->parent == nullptr)
			return newNode(-1);

		if (findNode(x)->parent->data != x) {
			// ako x nije svoj roditelj nije predtsavnik skupa
			// rekurzivan poziv za njegovog roditelja
			// kretanje kroz stablo ^
			findNode(x)->parent = Find(findNode(x)->parent->data);
		}

		return findNode(x)->parent;
	}

	Node* findNode(int data) {
		for (int i = 0; i < size; i++) {
			if (nodes[i].data == data)
				return &nodes[i];
		}

		return newNode(-1); // ovo ne bi trebalo nikad da se izvrsi
	}

	void Union(int x, int y) {
		// predstavnici skupova iz kojih su x, y redom

		if (x < 0 || y < 0)
			return;

		Node* xRoot = Find(x);
		Node* yRoot = Find(y);

		//isti prestavnici, isti skup
		if (xRoot == yRoot)
			return;

		if (xRoot->rank < yRoot->rank)
			// dubina X-stabla manja od Y-stabla
			// umetanje X ispod Y :: parent[x] = y, manja dubina rezultujuceg
			xRoot->parent = yRoot;
		else if (xRoot->rank > yRoot->rank)
			// dubina Y-stabla manja od X-stabla
			// umetanje Y ispod X :: parent[y] = x
			yRoot->parent = xRoot->parent;
		else {
			// iste dubine, svejedno..
			yRoot->parent = xRoot;
			xRoot->rank++;
		}
	}

	void bruteUnion(int x, int y) {
		Node* xRoot = Find(x);
		Node* yRoot = Find(y);
		yRoot->parent = xRoot;
		xRoot->rank++;
	}

	void FindComponents(int n) {
		int** arr = new int *[n];
		for (int i = 0; i < n; i++) {
			arr[i] = new int[2];
		}

		linkedNode<int, int>* ptr = representedGraph->start;
		int index = 0;

		while (ptr != nullptr) {

			Edge<int, int>* ptrAdj = ptr->adj;

			while (ptrAdj != nullptr) {
				// za svaki poteg u grafu uredjeni par [0][1] koji predstavlja izvorni i odresini cvor
				arr[index][0] = ptr->node;
				arr[index][1] = ptrAdj->dest->node;
				index++;
				ptrAdj = ptrAdj->link;
			}

			ptr = ptr->next;
		}

		// nalazenje disjunktnih skupova pozivanjem Union(x,y) nad svakim [0][1] parom potega
		selectParents(arr, n);
	}

	void displayUnified() {
		int count_parents = findMainParents();

		// prikaz skupova
		for (int j = 0; j < count_parents; j++) {
			cout << "Component [" << j << "] = {";
			Node* cur_component = newNode(-1);
			int i = 0;

			for (int k = 0; k < size; k++) {
				if (main_parent[k] != nullptr && main_parent[k]->data != -1) {
					// obradjuje predstavnike, nakon prikaza svih elemenata po predstvaniku postavlja se na -1
					// svaki obradjen/prikazan cvor :: main_parent[tog_cvora] = -1 >> preskace se u sledecoj iteraciji gornje for petlje
					cur_component = main_parent[k];
					i = k;
					break;
				}
			}

			for (; i < size; i++) {
				if (main_parent[i] == cur_component) {
					main_parent[i] = nullptr;
					if (nodes[i].data == -1)
						continue;
					cout << " " << nodes[i].data;
				}
			}

			cout << " } " << endl;
		}
	}

	void deleteElements() {
		for (int j = 0; j < 0.2*size; j++) {
			Node* toNull = returnRandomElement();
			int x = toNull->data;

			if (toNull->parent != toNull) { // nije predstavnik skupa
				for (int i = 0; i < size; i++)
					if (nodes[i].parent != nullptr && nodes[i].parent->data == x)
						nodes[i].parent = toNull->parent;
			}
			else {
				Node* newParent;
				for (int i = 0; i < size; i++)
					if (nodes[i].parent != nullptr && nodes[i].parent->data == x) {
						nodes[i].parent = &nodes[i];
						newParent = &nodes[i];
						break;
					}

				for (int i = 0; i < size; i++)
					if (nodes[i].parent != nullptr && nodes[i].parent->data == x) {
						nodes[i].parent = newParent;
					}
			}

			toNull->data = -1;
			toNull->parent = nullptr;
		}
	}

	Node* returnRandomElement() {
		std::uniform_int_distribution<int> uni(0, size - 1); // guaranteed unbiased
		int toReturn = uni(rng);
		Node* node = &nodes[toReturn];

		if (node == NULL || node == nullptr || node->parent == nullptr || node->data == -1)
			returnRandomElement();

		return node;
	}

	void deleteDegradation() {
		for (int j = 0; j < 0.2*size; j++) {
			Node* toNull = returnRandomElement();
			int x = toNull->data;

			// razbijanje skupa n elemenata iz kog je obrisani elemenat na podskupove od n-1
			for (int i = 0; i < size; i++) {
				if (nodes[i].parent == nullptr || nodes[i].parent->data == x) {
					nodes[i].parent = &nodes[i];
					nodes[i].next = nullptr;
					nodes[i].rank = 0;
				}
			}

			toNull->data = -1;
			toNull->parent = nullptr;
		}
	}

	~disjointUnion()
	{
		delete[] nodes;
		delete[] main_parent;
	}
};