#pragma once
#include "Node.h"
#include "Constants.h"
#include "Functions.h"
#include <iostream>

//Ksi = X, Eta = Y
struct SurfaceOperator
{
	Node n1, n2, n3, n4;
	vector<Node> Nodes;
	vector<vector<double>> H_BC;
	vector<vector<vector<double>>> H_BC_SKLADOWE;
	vector<double> len;
	vector<double> wektorP;



	SurfaceOperator() {}

	vector<double> getVectorP() {	
		vector<double> P_holder(4,0);
		vector<vector<double>> holder;
		int weightITerator = 0;
		//petla po scianach
		for (int i = 0; i < 4; i++) {
			if (Nodes[i].BC != 0 and Nodes[(i + 1) % 4].BC != 0) {
				vector<vector<double>> POINTS = GLOBAL_PKT_SCIANA[i];

				//Pêtla po punktach na œcianie
				vector<double> tempSum={0, 0, 0, 0};
				for (int j = 0; j < SCHEMAT; j++) {
					vector<double> N = get_N_For_Point(POINTS[j][0], POINTS[j][1]);
	
					tempSum = tempSum + (N * GLOBAL_WAGI_SCIANA[weightITerator]);
					weightITerator++;
				}
				
				tempSum = tempSum * gTot * gAlfa * (len[i] / 2);

				holder.push_back(tempSum);
	
			}
			else
			{
					holder.push_back(vector<double>(4, 0));
					weightITerator += SCHEMAT;
			}
			
		}

		//Sumowanie i dalsze mnozenie
		for (int i = 0; i < holder.size(); i++) {
			//cout << holder[i] << endl;
			P_holder = P_holder + holder[i];

		}
		//cout << "\n\n"<<P_holder;
		wektorP = P_holder;
		return wektorP;
	}

	SurfaceOperator(vector<Node> Nodess) {
		Nodes = Nodess;
		n1 = Nodes[0];
		n2 = Nodes[1];
		n3 = Nodes[2];
		n4 = Nodes[3];

		//Wokraoround, chyba sie FPU zepsul bo wg niego sqrt(0.625)=0.17 wg niego, TODO
		len.push_back(nodeDistance(n1, n2));
		len.push_back(nodeDistance(n2, n3));
		len.push_back(nodeDistance(n3, n4));
		len.push_back(nodeDistance(n4, n1));


		//cout << "HBC PELNE:\n" << H_BC;

	}


	//Obliczenie wszystkich N*N, 
	void Obliczenia() {
		setUp();
		getVectorP();
		//Sumowanie
		H_BC = vector<vector<double>>(4, vector<double>(4, 0));
		for (vector<vector<double>> part : H_BC_SKLADOWE)
			H_BC = H_BC + part;
	}

	//Oblicza N*N dla danej sciany i sumowanie ich
	void setUp() {

		int weightIterator = 0;
		//Petla po scianach
		for (int i = 0; i < 4; i++) {
			vector<vector<double>> suma = vector<vector<double>>(4, vector<double>(4, 0));//wektor sumy z przemnozonych (wagi, alfa, detJ)
			if (Nodes[i].BC != 0 and Nodes[(i + 1) % 4].BC != 0) {
				//cout << "Porownuje PKT:" << i <<"-"<< (i + 1) % 4 << endl;

				vector<vector<vector<double>>> NN_na_scianie = getScianaBC(GLOBAL_PKT_SCIANA[i]);

				double detJ = len[i] / 2;

				//Petla po NN sciany
				for (int j = 0; j < NN_na_scianie.size(); j++) {
					vector<vector<double>>temp = NN_na_scianie[j];
									
					temp = temp * gAlfa * GLOBAL_WAGI_SCIANA[weightIterator] * detJ;
					
					weightIterator++;
					suma = suma + temp;
					//cout << Nodes[i] <<"\t"<< Nodes[(i + 1) % 4]<<endl;
					//cout << "Dodaje:\n" << temp << endl;
				}
				//cout << "Suma na scianie " << i << endl << suma;
				H_BC_SKLADOWE.push_back(suma);

			}
			else {
				//cout << "Suma na Scianie:\n"<< vector<vector<double>>(4, vector<double>(4, 0));
				H_BC_SKLADOWE.push_back(vector<vector<double>>(4, vector<double>(4, 0)));
				weightIterator += SCHEMAT;
			}
		}
	}
	//Zwraca tablice wszystkich N*N na danej scianie
	vector<vector<vector<double>>> getScianaBC(vector<vector<double>> scianaPkt) {
		vector<vector<vector<double>>> ScianaN_N;

		for (int i = 0; i < scianaPkt.size(); i++) {
			double ksi=scianaPkt[i][0];
			double eta=scianaPkt[i][1];
			vector<double> N_in_point;
			N_in_point = get_N_For_Point(ksi, eta);

			ScianaN_N.push_back(N_in_point*N_in_point);
		}

		//cout << ScianaN_N <<"\n";
		return ScianaN_N;//Zwraca WSZYSTKIE wektory 4x4
	}
	
	
};