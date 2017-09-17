#pragma once

#include "graphImplementation.h"

class disjointUnion {
private:
	int* parent;
	int *main_parent;
	int* rank;
	int size;

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
		for (int i = 0; i < size; i++) {
			main_parent[i] = Find(i); // nalazi predstavnika skupa za svaki clan
			if (main_parent[i] == i)  // predstavnik skupa je roditelj samom sebi
				count++;
		}
		return count;
	}

public:
	disjointUnion(int n) {
		size = n;
		parent = new int[size];
		main_parent = new int[size];
		rank = new int[size];
		makeSet();
	}

	void makeSet() {
		// n supova sa jednim elementom :: svaki predtsavnik skupa
		// inicijalno n disjunktnih skupova
		for (int i = 0; i < size; i++) {
			parent[i] = i;
			rank[i] = 0;
		}
	}

	int Find(int x) {
		// nalazi prestavnika skupa ciji je x elemenat

		if (x < 0)
			return -2;

		if (parent[x] != x) {
			// ako x nije svoj roditelj nije predtsavnik skupa
			// rekurzivan poziv za njegovog roditelja
			// kretanje kroz stablo ^
			parent[x] = Find(parent[x]);
		}

		return parent[x];
	}

	void Union(int x, int y) {
		// predstavnici skupova iz kojih su x, y redom

		if (x < 0 || y < 0)
			return;

		int xRoot = Find(x);
		int yRoot = Find(y);

		//isti prestavnici, isti skup
		if (xRoot == yRoot)
			return;

		if (rank[xRoot] < rank[yRoot])
			// dubina X-stabla manja od Y-stabla
			// umetanje X ispod Y :: parent[x] = y, manja dubina rezultujuceg
			parent[xRoot] = yRoot;
		else if (rank[yRoot] < rank[xRoot])
			// dubina Y-stabla manja od X-stabla
			// umetanje Y ispod X :: parent[y] = x
			parent[yRoot] = xRoot;
		else {
			// iste dubine, svejedno..
			parent[yRoot] = xRoot;
			rank[xRoot] = rank[xRoot] + 1;
		}
	}

	void bruteUnion(int x, int y) {
		int xRoot = Find(x);
		int yRoot = Find(y);
		parent[xRoot] = yRoot;
		rank[xRoot] = rank[xRoot] + 1;
	}

	void FindComponents(Graph<int, int> graph, int n) {
		int** arr = new int *[n];
		for (int i = 0; i < n; i++) {
			arr[i] = new int[2];
		}

		linkedNode<int, int>* ptr = graph.start;
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
			int cur_component = -1;
			int i = 0;

			for (int k = 0; k < size; k++) {
				if (main_parent[k] != -1) {
					// obradjuje predstavnike, nakon prikaza svih elemenata po predstvaniku postavlja se na -1
					// svaki obradjen/prikazan cvor :: main_parent[tog_cvora] = -1 >> preskace se u sledecoj iteraciji gornje for petlje
					cur_component = main_parent[k];
					i = k;
					break;
				}
			}

			for (; i < size; i++) {
				if (main_parent[i] == cur_component) {
					main_parent[i] = -1;
					cout << " " << i;
				}
			}

			cout << " } " << endl;
		}
	}

	void deleteElements(Graph<int, int> graph) {
		for (int j = 0; j < 0.2*size; j++) {
			int x = graph.returnRandomElement();
			if (parent[x] != x) {
				for (int i = 0; i < size; i++) {
					if (parent[i] == x)
						parent[i] = parent[x];
				}
				parent[x] = x;
			}
			else {
				int newParent;
				for (int i = 0; i < size; i++)
					if (parent[i] == x) {
						parent[i] = i;
						main_parent[i] = i;
						newParent = i;
						break;
					}

				for (int i = 0; i < size; i++)
					if (parent[i] == x) {
						parent[i] = newParent;
						main_parent[i] = newParent;
					}
			}
		}
	}


	void deleteDegradation(Graph<int, int> graph) {
		int parents = findMainParents();

		for (int j = 0; j < 0.2*size; j++) {
			int x = graph.returnRandomElement();
			// razbijanje skupa n elemenata iz kog je obrisani elemenat na podskupove od n-1
			for (int i = 0; i < size; i++) {
				if (main_parent[i] == x)
					main_parent[i] = i;
				if (parent[i] == x)
					parent[i] = i;
			}
		}
	}

	~disjointUnion()
	{
		delete[] parent;
		delete[] rank;
		delete[] main_parent;
	}
};