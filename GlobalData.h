#pragma once
#include "Grid.h"
#include <iostream>
#include <fstream>
#include <string>
//Ksi = X, Eta = Y


extern double gTot;
extern double gCond;
extern double gAlfa;
extern int gElSize;
extern int gNodSize;
extern double gSpecHeat;
extern double gDensity;
extern double gSimStepTime;
extern double gSimTime;
extern double gInitTemp;


struct GlobalData {
	int SimTime = 0;
	int SimStepTime = 0;
	int Conductivty = 0;
	int Alfa = 0;
	int Tot = 0;
	int InitTemp = 0;
	int Density = 0;
	int SpecHeat = 0;
	int NodeNumb = 0;
	int ElemNumb = 0;
	Grid grid;

	GlobalData(string filename) {
		loadData(filename);
	}
	//Funkcjao golna do zaladowania danych
	void loadData(string filename) {
		ifstream dataFile(filename);
		string line;
		vector<string> firstTen;
		vector<string> nodesString;
		vector<string> elementString;
		vector<string> bcString;
		bool hasLoadedTen = false;
		unsigned long int i = 0;    //unsigned long int - nie powinno byæ overflow

		while (!dataFile.eof())
		{
			//Zacznij odczyt z pliku jeœli istnieje
			if (!dataFile) {
				cout << "ERROR, brakuje pliku .txt";
				exit(-1337);
			}

			//Pierwsze 10 pozycji - zczytaj info i za³aduj
			getline(dataFile, line);
			if (i < 10) {                   //10 pierwszych pozycji - reszta nie obchodzi wiêc nastêpna iteracja
				firstTen.push_back(line);
				i++;
				continue;
			}
			if (!hasLoadedTen)sanitizeAndLoad10(firstTen);



			//Zczytaj wszystkie Node i za³aduj
			if (line.find("*Node") != std::string::npos) {
				for (int j = 0; j < NodeNumb; j++) {
					getline(dataFile, line);
					nodesString.push_back(line);
				}
				sanitizeLoadNodes(nodesString);
				//setupMacierzC_2D_1D();//Setup macierzy C 2D tutaj, bo wiemy ju¿ ile Node'ów;
			}

			//Zczytaj wszystkie Elementy i za³aduj UWAGA, LOAD POTEM, BO NIE MAMY BC
			if (line.find("*Element") != std::string::npos) {
				for (int j = 0; j < ElemNumb; j++) {
					getline(dataFile, line);
					elementString.push_back(line);
				}
				//sanitizeLoadElements(elementString);
			}

			//Zczytaj BC i za³aduj
			if (line.find("*BC") != std::string::npos) {
				getline(dataFile, line);
				bcString.push_back(line);
				sanitizeLoadBC(bcString);
			}
		}
		sanitizeLoadElements(elementString);
		dataFile.close();
	}
	//Zaladowanie i ustawienie danych globalnych dal programu (Temperatura, wspolczynniki etc.)
	void sanitizeAndLoad10(vector<string> dataVecc) {
		for (int i = 0; i < dataVecc.size(); i++) {
			string dataLine = dataVecc[i];//cout << dataVecc[i] << endl;

			//Sanityzacja - zostawiamy tylko liczby
			string sanitized = "";
			for (auto letter : dataVecc[i]) {
				if (letter >= '0' && letter <= '9')
					sanitized += letter;
			}

			//cout << "DODAWANY REZULTAT \""<<sanitized <<"\"" << endl;
			int dataNumber = stoi(sanitized);

			switch (i) {            //Dane s¹ ju¿ w rzêdzie, wiêc mo¿na je z automatu wpisaæ
			case 0:
				SimTime = dataNumber;
				gSimTime = SimTime;
				break;
			case 1:
				SimStepTime = dataNumber;
				gSimStepTime = SimStepTime;
				break;
			case 2:
				Conductivty = dataNumber;
				gCond = dataNumber;
				break;
			case 3:
				Alfa = dataNumber;
				gAlfa = Alfa;
				break;
			case 4:
				Tot = dataNumber;
				gTot = Tot;
				break;
			case 5:
				InitTemp = dataNumber;
				gInitTemp = InitTemp;
				break;
			case 6:
				Density = dataNumber;
				gDensity = Density;
				break;
			case 7:
				SpecHeat = dataNumber;
				gSpecHeat = SpecHeat;
				break;
			case 8:
				NodeNumb = dataNumber;
				gNodSize = NodeNumb;
				break;
			case 9:
				ElemNumb = dataNumber;
				gElSize = ElemNumb;
				break;
			default:
				cout << "\n\nERROR - dataVecc.size()>10 - FIRST TEN";
				exit(-2);
			}
		}
	}
	//Zaladowanie i utworzenie Node'ow
	void sanitizeLoadNodes(vector<string> dataVecc) {


		//Pêtla po wszystkich elementach Nodes
		int jkl = 1;
		for (string str : dataVecc) {
			//Usuñ wszystkie spacje - zostaje tylko CSV
			vector<string> separated;
			str.erase(std::remove(str.begin(), str.end(), ' '), str.end());

			//Pêtla po wszystkich znakach linii - po ca³oœci jednego elementu
			string temp = "";
			for (char x : str) {
				if (x != ',')
					temp += x;
				else {
					separated.push_back(temp);
					temp = "";
				}
			}
			separated.push_back(temp);//Dodaj ostatni element, bo pêtla urywa
			double xxx = stod(separated[1]);
			double yyy = stod(separated[2]);
			//cout << xxx << "\t" << yyy << endl << endl << endl;
			grid.Nodes.push_back(Node(jkl, xxx, yyy));
			jkl++;
			separated.clear();
		}
	}
	//Fragemnt odpowiedzlany za zaladowanie i utworzenie Elementow
	void sanitizeLoadElements(vector<string> dataVecc) {
		//Pêtla po elementach [jako string]
		for (string str : dataVecc) {

			//Pêtla po [string] konwersja na [vector<int>]
			str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
			vector<int> sep;
			string temp = "";
			for (char x : str) {
				if (x != ',')
					temp += x;
				else {
					sep.push_back(stoi(temp));
					temp = "";
				}
			}sep.push_back(stoi(temp));
			//cout << sep;
			//Konwerja [vector<int>] na Element i pushowanie do g³ównego
			Element tempElem(sep[0], grid.Nodes[sep[1] - 1], grid.Nodes[sep[2] - 1], grid.Nodes[sep[3] - 1], grid.Nodes[sep[4] - 1]);
			//cout << tempElem;
			grid.elements.push_back(tempElem);
		}
	}
	//Fragemnt odpowiedzlany za zaladowanie czesci BC i jej ustawienie
	void sanitizeLoadBC(vector<string> dataVecc) {

		string str = dataVecc[0];
		str.erase(std::remove(str.begin(), str.end(), ' '), str.end());//Usuñ spacje
		//cout << dataVecc;
		//cout << str << endl;

		//Odseparuj CSV i za³aduj
		vector<int> separated;
		string temp = "";
		for (char x : str) {
			if (x != ',')
				temp += x;
			else {
				separated.push_back(stoi(temp));
				temp = "";
			}
		}separated.push_back(stoi(temp));
		grid.bc = separated;

		//Pêtla po node'ach
		for (int i = 0; i < grid.Nodes.size(); i++) {
			//Pêtla po BC - sprawdza czy jest czy nie i ustawia
			for (int j = 0; j < grid.bc.size(); j++) {
				if (grid.Nodes[i].id == grid.bc[j])
					grid.Nodes[i].BC = 1;
			}
		}
		/*
		cout << "BC=\n";
		for (int k : separated) {
			cout << k << "\t";
		}
		cout <<"\n\n\n";
		*/
	}

	friend ostream& operator<<(ostream& os, const GlobalData& globDat) {
		cout << "\nSimulationTime: " << globDat.SimTime;
		cout << "\nSimulationStepTime: " << globDat.SimStepTime;
		cout << "\nConductivity: " << globDat.Conductivty;
		cout << "\nAlfa: " << globDat.Alfa;
		cout << "\nTot: " << globDat.Tot;
		cout << "\nInitial Temperature: " << globDat.InitTemp;
		cout << "\nDensity: " << globDat.Density;
		cout << "\nSpecificHeat: " << globDat.SpecHeat;
		cout << "\nNodes Number: " << globDat.NodeNumb;
		cout << "\nElements Number: " << globDat.ElemNumb;

		cout << globDat.grid;

		return os;
	}

};