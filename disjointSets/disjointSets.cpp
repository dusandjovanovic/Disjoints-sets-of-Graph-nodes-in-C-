// disjointSets.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include "disjointTree.h"
#include "disjointList.h"

int main()
{
	Graph<int, int> graph = Graph<int, int>();
	graph.insertNode(1);
	graph.insertNode(2);
	graph.insertNode(3);
	graph.insertNode(4);
	graph.insertNode(5);
	graph.insertNode(6);
	graph.insertNode(7);
	graph.insertNode(8);
	graph.insertNode(9);
	graph.insertNode(10);

	graph.insertEdge(1, 2);
	graph.insertEdge(1, 3);
	graph.insertEdge(2, 3);
	graph.insertEdge(2, 4);

	graph.insertEdge(5, 6);
	graph.insertEdge(6, 7);

	graph.insertEdge(8, 9);

	graph.displayGraph();
	disjointUnion disjoint(10, &graph);
	disjoint.FindComponents(7);
	cout << endl;
	disjoint.displayUnified();
	cout << endl;

	system("pause");

	//////////////////////////////////////

	cout << endl;

	std::uniform_int_distribution<int> uni(1, 10000);

	for (int nodeNumber = 10; nodeNumber <= 1000000; nodeNumber *= 10) {
		
		Graph<int, int> _graph = Graph<int, int>();

		for (int i = 0; i < nodeNumber; i++)
			if (_graph.insertNode(uni(rng)))
				continue;
			else
				--i;

		for (int i = 0; i < nodeNumber; i++)
			_graph.insertEdge(_graph.returnRandomElement(), _graph.returnRandomElement());

		_graph.displayGraph();

		disjointUnion dSet(nodeNumber, &_graph);
		dSet.FindComponents(nodeNumber);

		dSet.displayUnified();

		dSet.deleteElements();
		//dSet.deleteDegradation();

		cout << endl;

		dSet.displayUnified();

		cout << endl;
		system("pause");
	}

    return 0;
}

