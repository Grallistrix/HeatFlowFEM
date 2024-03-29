#pragma once
#include <iostream> 
#include <string>

using namespace std;
//Ksi = X, Eta = Y

struct Node
{
	int id;
	int BC=0;
	double x, y;

	Node() { x = -999; y = -999; id = -999; }

	Node(int id, double x, double y) {
		this->x = x;
		this->y = y;
		this->id = id;
	}
	Node(double x, double y) {
		this->x = x; this->y = y; id = -999;
	}
	bool operator==(const Node& other) const {
		if (this->x == other.x && this->y == other.y)
			return true;

		return false;
	};
	friend ostream& operator<<(std::ostream& os, const Node& nod) {
		os << "Node " << nod.id << "\tX:" << nod.x << "\tY:" << nod.y << "\t BC=" << nod.BC;
		return os;
	}

	string fetchParaData() {
		string x_d = to_string(this->x);
		string y_d = to_string(this->y);
		string z_d = "0";

		string result = x_d + " " + y_d + " " + z_d;
		return result;
	}
};

