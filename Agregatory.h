#pragma once
#include <vector>
#include "Constants.h"

extern vector<vector<double>> agregatorH;
extern vector<vector<double>> agregatorC_2D;
extern vector<double> agregatorWektoraP;
extern vector<double> agregatorC_1D;
extern vector<double> agregatorH_1D;


struct AgregatWektoraP {
	AgregatWektoraP() {
		const int rows = gNodSize;
		vector<double> vector1D(gNodSize, 0);
		agregatorWektoraP = vector1D;
	}

	void ammendAgregacja(Element El) {
		vector<double> wekP = El.Powierzchnia.wektorP;
		vector<int> nodeID;


		for (Node x : El.Nodes)
			nodeID.push_back(x.id);
		//cout << wekP;
		//cout << nodeID;
		for (int i = 0; i < 4; i++) {
			double dodajWektor = wekP[i];
			double jakieID = nodeID[i] - 1;//i-1, bo tablica od 0, a index'y node'ów od 1
			agregatorWektoraP[jakieID] += dodajWektor;
		}

	}
	void showData() {
		cout << "Po dokonaniu Agregacji WektoraP:\n" << agregatorWektoraP;
	}
};

struct AgreagatMacierzyH {


	AgreagatMacierzyH() {
		//Stwóz pust¹ macierz IloœcNode x IloœæNode
		vector<vector<double>> vector2D(gNodSize, vector<double>(gNodSize, 0));
		agregatorH = vector2D;
		agregatorH_1D = vector<double>(gNodSize, 0);
		//cout << agregatorH;
	}

	void ammendAgregacja(Element El) {
		vector<double> wekP = El.Powierzchnia.getVectorP();
		vector<int> nodeID;
		for (Node x : El.Nodes) {
			nodeID.push_back(x.id - 1);//-1 bo index od 1
			//cout << "Pushuje na NodeID, node'a o numerze" << x.ID << endl;
		}

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				int pierwszaWsp = nodeID[i];
				int drugaWsp = nodeID[j];
				agregatorH[pierwszaWsp][drugaWsp] += El.H_PLUS_BC[i][j];
				//cout << "Agreguje [" << pierwszaWsp << "," << drugaWsp << "]\n";
				//cout << "Wysylam dane:" << El.H_PLUS_BC[i][j]<<endl;
			}
		}
	}

	void prepare1D() {
		for (int i = 0; i < gNodSize; i++) {
			double temp = 0;
			for (int j = 0; j < gNodSize; j++) {
				temp += agregatorC_2D[j][i];
			}
			agregatorH_1D[i] = temp;
		}

	}

};

struct AgregatMacierzyC {

	AgregatMacierzyC() {
		//Stwóz pust¹ macierz IloœcNode x IloœæNode
		vector<vector<double>> vector2D(gNodSize, vector<double>(gNodSize, 0));
		agregatorC_2D = vector2D;
		agregatorC_1D = vector<double>(gNodSize, 0);
		//cout << "MacC\n" << agregatorC;
	}
	void ammendAgregacja(Element el) {
		vector<vector<double>> localC = el.macierzC;

		vector<int> nodeID;
		for (Node x : el.Nodes) {
			nodeID.push_back(x.id - 1);//-1 bo index od 1
			//cout << "Pushuje na NodeID, node'a o numerze" << x.ID << endl;
		}
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				int pierwszaWsp = nodeID[i];
				int drugaWsp = nodeID[j];
				agregatorC_2D[pierwszaWsp][drugaWsp] += el.macierzC[i][j];
				//cout << "Agreguje [" << pierwszaWsp << "," << drugaWsp << "]\n";
				//cout << "Wysylam dane:" << El.H_PLUS_BC[i][j]<<endl;
			}
		}


	}


	//Uzywac tylko jak 2D sie przygotuje
	void prepare1D() {
		for (int i = 0; i < gNodSize; i++) {
			double temp = 0;
			for (int j = 0; j < gNodSize; j++) {
				temp += agregatorC_2D[j][i];
			}
			agregatorC_1D[i] = temp;
		}

	}
};