#pragma once
#include <vector>
#include "Node.h"
#include "Element.h"
#include <iostream>

//Ksi = X, Eta = Y

struct Grid {
	int size_X = 0;
	int size_Y = 0;
	vector<Node> Nodes;                 //tablica punktow
	vector<Element> elements;           //tablica elementow (tablica tablic 4-elementowych)
	vector<int> bc;
	Grid() {}
	//Konstruktor z list¹
	Grid(vector<Node> Nodes, vector<Element> elements, vector<int> bc) {
		this->Nodes = Nodes;
		this->elements = elements;
		this->size_X = sqrt(elements.size());
		this->size_Y = size_X;
		this->bc = bc;
	}

	void add_node(Node newNode) {
		this->Nodes.push_back(newNode);
	}
	void add_element(Element newElement) {
		this->elements.push_back(newElement);
	}

	void activate() {
		for (Element el : elements)
			el.setUp();
	}

	friend ostream& operator<<(ostream& os, const Grid& grid) {
		cout << "\n\NODES LIST\n";
		for (int i = 0; i < grid.Nodes.size(); i++)
			cout << "\t" << grid.Nodes[i] << endl;

		cout << "\nELEMENTS LIST\n";
		for (int i = 0; i < grid.elements.size(); i++) {
			cout << "\tElement" << grid.elements[i].ID << "\t";
			for (Node x : grid.elements[i].Nodes)
				cout << x.id << " ";
			cout << endl;
		}

		//cout << "BC:\n\t" << grid.bc;
		return os;
	}
};