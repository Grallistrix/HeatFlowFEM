#pragma once
#include <vector>
#include "Node.h"
#include "SurfaceOperator.h"
//Ksi = X, Eta = Y
struct Element{
public:
	int ID;
	vector<Node> Nodes;

	//Stale Tablice
	vector<vector<double>>H_BC = {
								{0,0,0,0},
								{0,0,0,0},
								{0,0,0,0},
								{0,0,0,0},
	};
	vector<vector<double>>H = {
									{0,0,0,0},
									{0,0,0,0},
									{0,0,0,0},
									{0,0,0,0},
	};
	vector<vector<double>>H_PLUS_BC;
	vector<double>wektorP = { 0,0,0,0 };
	vector<vector<double>> macierzC;
	//Nie zmienono, tylko ORG
	vector<vector<vector<double>>> matrixyJakobiego;
	vector<vector<vector<double>>> odwrJakob;
	vector<vector<vector<double>>> przeliczonyJakob;
	vector<vector<double>> N_XX;
	vector<vector<double>> N_YY;
	vector<double> detJ;
	vector<vector<double>> matrixC;
	SurfaceOperator Powierzchnia;


	Element(int id, Node a, Node b, Node c, Node d) {
		ID = id;
		Nodes.insert(Nodes.begin(), { a,b,c,d });
	}

	void setUp() {
		getMacierzH();

		SurfaceOperator temp(Nodes);
		temp.Obliczenia();

		H_BC = temp.H_BC;
		Powierzchnia = temp;
		wektorP = temp.getVectorP();
		H_PLUS_BC = (H + H_BC);
		matrixC=getMacierzC();
		//printData();
	}

	//********************Funkcje do obliczenia H [Macierz Jakobiego]
	//Obliczanie Jakobiego, wyznacznika J, odwrotnoœci Jakobiego i wymno¿enie
	void setupMacierzeJakobiego() {
		for (int i = 0; i < ILE_PKT; i++) {
			
			double x_ksi = getX_Po_Ksi(GLOBAL_N_PO_KSI[i], this->Nodes);
			double y_ksi = getY_Po_Ksi(GLOBAL_N_PO_KSI[i], this->Nodes);
			double x_eta = getX_Po_Eta(GLOBAL_N_PO_ETA[i], this->Nodes);
			double y_eta = getY_Po_Eta(GLOBAL_N_PO_ETA[i], this->Nodes);
			vector<vector<double>> Jakob({
					{x_ksi,y_ksi},
					{x_eta,y_eta}
				});
			matrixyJakobiego.push_back(Jakob);
			detJ.push_back(determinant(Jakob));
			odwrJakob.push_back(OdwrocJakobiego(Jakob));
			przeliczonyJakob.push_back(odwrJakob[i] * (1.0/detJ[i]));

			//cout << "ELEMENT:"<<ID<<"\nPKT CALK:"<<i<< "\nMacierz J:\n" << Jakob << "DetJ=" << detJ[i] ;
		}
	}
	//Obliczanie Wszystkich N po X, N po Y
	void obliczN_XY() {
		for (int i = 0; i < ILE_PKT; i++) {
			double y_ksi, y_eta, x_ksi, x_eta;
			y_eta = przeliczonyJakob[i][0][0];
			y_ksi = przeliczonyJakob[i][0][1];
			x_eta = przeliczonyJakob[i][1][0];
			x_ksi = przeliczonyJakob[i][1][1];

			vector<double> liniaX;
			vector<double> liniaY;
			for (int j = 0; j < 4; j++) {
				double N_X = (y_eta*GLOBAL_N_PO_KSI[i][j])+(y_ksi*GLOBAL_N_PO_ETA[i][j]);
				double N_Y = (x_eta* GLOBAL_N_PO_KSI[i][j])+(x_ksi* GLOBAL_N_PO_ETA[i][j]);
				liniaX.push_back(N_X);
				liniaY.push_back(N_Y);
			}
			N_XX.push_back(liniaX);
			N_YY.push_back(liniaY);
			//cout << "N_X:" << N_XX[i] << "\nN_Y:" << N_YY[i] << endl;
		}
	}
	//Obliczenie wszystkich macierzy H
	vector<vector<double>> getMacierzH() {
		setupMacierzeJakobiego();
		obliczN_XY();

		vector<vector<double>> result(4, vector<double>(4, 0));//Tablica 4x4, pe³na zer

		for (int i = 0; i < ILE_PKT; i++) {
			vector<double> liniaX = N_XX[i];
			vector<double> liniaY = N_YY[i];

			vector<vector<double>> X_squared = liniaX * liniaX;
			vector<vector<double>> Y_squared = liniaY * liniaY;
			vector<vector<double>> suma = X_squared + Y_squared;

			suma = suma *  gCond * GLOBAL_WAGI_CALK[i] * detJ[i];
			result = result + suma;
		}
		H = result;
		return result;
	}
	//Obliczenia Macierzy C
	vector<vector<double>> getMacierzC() {
		vector<vector<double>> all_N;
		vector<vector<vector<double>>> all_PC_C;
		vector<vector<double>> result(4, vector<double>(4, 0));
		//Petla po punktach calkowania
		for (int i = 0; i < ILE_PKT; i++) {
			vector<double> PUNKT = GLOBAL_PKT_CALK[i];
			double ksi = PUNKT[0]; double eta = PUNKT[1];
			vector<double> N = get_N_For_Point(ksi,eta);
			all_N.push_back(N);
			//cout << "PKT_Calk="<<i<<"\t"<<N;
		}

		//Mnozenie i sumowanie
		for (int i = 0; i < all_N.size(); i++) {
			vector<vector<double>> localC;
			localC = (all_N[i] * all_N[i]) * gDensity * gSpecHeat*detJ[i]*GLOBAL_WAGI_CALK[i];
			all_PC_C.push_back(localC);
			result = result + localC;
			//cout << localC<< endl << endl;

		}
		//cout << "RESULTAT:\n" << result;
		macierzC = result;
		return result;
	}

	//Funkcje do wypisywania
	void printData() {
		cout << "\t\tELEMENT: " << ID << endl;
		cout << "***\tMACIERZ H:\n" << H << endl;
		cout << "&&&\tMACIERZ HBC:\n" << H_BC << endl;
		cout << "@@@\tMACIERZ H+BC:\n" << H_PLUS_BC << endl;
		cout << "!!!\tWEKTOR P:\t" << wektorP << endl;
		cout << "~~~\tMACIERZ C:\n" << matrixC << endl;
	}

	void printDataCalk() {
		cout << "ELEMENT: " << ID << endl;
		for (int i = 0; i < ILE_PKT; i++) {
			cout << "ELEMENT:" << ID << "\nPKT CALK:" << i << "\nMacierz J:\n" << matrixyJakobiego[i] 
				<< "DetJ=" << detJ[i] << "N_X:" << N_XX[i] << "\nN_Y:" << N_YY[i] << endl;;
		}
	}

	friend ostream& operator<<(ostream& os, const Element el) {

		os << "Element " << el.ID << "\t Nodes:[";
		for (int i = 0; i < el.Nodes.size(); i++) {
			os << el.Nodes[i].id;
			if (i != 3)
				os << ",";
		}
			

		os << "]";

		return os;
	}

};

