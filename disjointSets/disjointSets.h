#pragma once
#include "graphImplementation.h"

class disjointSet {
private:
	int *parent;
	int *main_parent;
	int *rank;
	int size;

	void select_parents(int** arr, int n) {
		for (int i = 0; i < n; i++) {
			int x = arr[i][0];
			int y = arr[i][1];
			Union(x, y);
		}
	}

	int find_main_parents() {
		int count = 0;
		for (int i = 0; i < size; i++) {
			main_parent[i] = Find(i);
			if (main_parent[i] == i) {
				count++;
			}
		}
		return count;
	}


public:
	disjointSet(int s)
	{
		size = s;
		parent = new int[size];
		rank = new int[size];
		main_parent = new int[size];
		for (int i = 0; i < size; i++)
		{
			parent[i] = i;
			rank[i] = 0;
		}
	}

	int Find(int x_) {
		int x = x_;
		int p = parent[x_];

		while (p != x) {
			x = p;
			p = parent[p];
		}
		while (x_ != p)
		{
			x = parent[x_];
			parent[x_] = p;
			x_ = x;
		}
		return p;
	}

	void Union(int x, int y)
	{
		int s1 = Find(x);
		int s2 = Find(y);
		int r1 = rank[s1];
		int r2 = rank[s2];
		if (r1<r2) {
			parent[s1] = s2;
		}
		else {
			parent[s2] = s1;
			if (r1 == r2)
				rank[s1] = rank[s2] + 1;
		}
	}

	void FindComponents(Graph<int, int> graph, int n)
	{
		int** arr = new int *[n];
		for (int i = 0; i < n; i++) {
			arr[i] = new int[2];
		}

		linkedNode<int, int>* ptr = graph.start;
		int index = 0;

		while (ptr != nullptr) {

			Edge<int, int>* ptrAdj = ptr->adj;

			while (ptrAdj != nullptr) {
				arr[index][0] = ptr->node;
				arr[index][1] = ptrAdj->dest->node;
				index++;
				ptrAdj = ptrAdj->link;
			}

			ptr = ptr->next;
		}


		select_parents(arr, n);
		int count_parents = find_main_parents();
		for (int j = 0; j < count_parents; j++)
		{
			cout << "Component [" << j << "] = {";
			int cur_component = -1;
			int i = 0;
			for (int k = 0; k < size; k++)
			{
				if (main_parent[k] != -1)
				{
					cur_component = main_parent[k];
					i = k;
					break;
				}
			}
			for (; i < size; i++)
			{
				if (main_parent[i] == cur_component)
				{
					main_parent[i] = -1;
					cout << " " << i;
				}
			}
			cout << " } " << endl;
		}

	}

	~disjointSet()
	{
		delete[] parent;
		delete[] rank;
		delete[] main_parent;
	}
};