#pragma once
#include "Constants.h"
#include "Node.h"

//Ksi = X, Eta = Y


extern int SCHEMAT;
extern int ILE_PKT;
extern vector<vector<double>> GLOBAL_N_PO_KSI;
extern vector<vector<double>> GLOBAL_N_PO_ETA;
extern vector<double> P_C;
extern vector<vector<double>> Saved_Temperatura;
extern vector<double> temperatures;

//***************************************************************************	Obliczanie Funkcji Kszta³tu N - EtaKsi, PoKsi, PoEta
double get_N1(double ksi, double eta) {	return 0.25 * (1 - ksi) * (1 - eta);}
double get_N2(double ksi, double eta) {	return 0.25 * (1 + ksi) * (1 - eta);}
double get_N3(double ksi, double eta) {	return 0.25 * (1 + ksi) * (1 + eta);}
double get_N4(double ksi, double eta) {	return 0.25 * (1 - ksi) * (1 + eta);}
vector<double> get_N_For_Point(double ksi, double eta) { return vector<double>{get_N1(ksi, eta), get_N2(ksi, eta), get_N3(ksi, eta), get_N4(ksi, eta)};}
vector<vector<double>> getN_For_Schemat() {
	vector<vector<double>> points;
	switch (SCHEMAT) {
	case 2:
		points = PktCalk2D;
		break;
	case 3:
		points = PktCalk3D;
		break;
	case 4:
		points = PktCalk4D;
		break;
	defualt:
		cout << "ERR, NIE OBSLUGUJE TAKIEGO\n";
		exit(SCHEMAT);
	}
	vector<vector<double>> temp;
	for (auto point : points) {
		double ksi = point[0];
		double eta = point[1];
		temp.push_back(get_N_For_Point(ksi, eta));
	}
	return temp;
}

double get_N1_Po_Ksi(double eta) {return -0.25 * (1 - eta);}
double get_N2_Po_Ksi(double eta) {return  0.25 * (1 - eta);}
double get_N3_Po_Ksi(double eta) {return  0.25 * (1 + eta);}
double get_N4_Po_Ksi(double eta) {return -0.25 * (1 + eta);}
vector<double> getN_Po_Ksi_For_Point(double eta) {return vector<double>{get_N1_Po_Ksi(eta), get_N2_Po_Ksi(eta), get_N3_Po_Ksi(eta), get_N4_Po_Ksi(eta)};}
vector<vector<double>> getN_Po_Ksi_For_Schemat() {

	//Ksi = X, Eta = Y
	vector<vector<double>> temp;
	for (auto point : GLOBAL_PKT_CALK) {
		double eta = point[1];
		temp.push_back(getN_Po_Ksi_For_Point(eta));
	}
	return temp;
}

double get_N1_Po_Eta(double ksi) { return -0.25 * (1 - ksi); }
double get_N2_Po_Eta(double ksi) { return -0.25 * (1 + ksi); }
double get_N3_Po_Eta(double ksi) { return  0.25 * (1 + ksi); }
double get_N4_Po_Eta(double ksi) { return  0.25 * (1 - ksi); }
vector<double> getN_Po_Eta_For_Point(double ksi) {return vector<double>{ get_N1_Po_Eta(ksi), get_N2_Po_Eta(ksi) , get_N3_Po_Eta(ksi), get_N4_Po_Eta(ksi)};}
vector<vector<double>> getN_Po_Eta_For_Schemat() {
	//Ksi = X, Eta = Y
	vector<vector<double>> temp;
	for (auto point : GLOBAL_PKT_CALK) {
		double eta = point[0];
		temp.push_back(getN_Po_Eta_For_Point(eta));
	}
	return temp;
}

void setup_global_N() {	GLOBAL_N_PO_ETA = getN_Po_Eta_For_Schemat();GLOBAL_N_PO_KSI = getN_Po_Ksi_For_Schemat();}

//***************************************************************************	Macierz Jakobiego
double getX_Po_Ksi(vector<double> N_KSI, vector<Node> Nodes) {
	vector<double> x;
	double X = 0;
	//Find Y
	for (Node nod : Nodes)
		x.push_back(nod.x);
	//Oblicz N
	for (int i = 0; i < 4; i++)
		X += x[i] * N_KSI[i];

	return X;
}
double getY_Po_Ksi(vector<double> N_KSI, vector<Node> Nodes) {
	vector<double> y;
	double Y = 0;
	//Find Y
	for (Node nod : Nodes)
		y.push_back(nod.y);

	//Oblicz N
	for (int i = 0; i < 4; i++)
		Y += y[i] * N_KSI[i];

	return Y;
}
double getX_Po_Eta(vector<double> N_Eta, vector<Node> Nodes) {
	vector<double> x;
	double X = 0;
	//Find Y
	for (Node nod : Nodes)
		x.push_back(nod.x);

	//Oblicz N
	for (int i = 0; i < 4; i++)
		X += x[i] * N_Eta[i];

	return X;
}
double getY_Po_Eta(vector<double> N_Eta, vector<Node> Nodes) {
	vector<double> y;
	double Y = 0;
	//Find Y
	for (Node nod : Nodes)
		y.push_back(nod.y);

	//Oblicz N
	for (int i = 0; i < 4; i++)
		Y += y[i] * N_Eta[i];

	return Y;
}
vector<vector<double>> OdwrocJakobiego(vector<vector<double>> Jakob) {
	double x_ksi = Jakob[0][0];
	double y_eta = Jakob[1][1];

	vector<vector<double>> odwr(
		{ { y_eta,-Jakob[0][1] },
		{ -Jakob[1][0],x_ksi } });
	return odwr;
}


//***************************************************************************	Przeci¹¿enia Operatorów
template <typename T>
vector<vector<T>> operator+(const vector<vector<T>>& vec1, const vector<vector<T>>& vec2) {
	// Pod warunkiem, ¿e maj¹ takie same Rozmiary
	vector<vector<T>> result(vec1.size(), vector<T>(vec1[0].size()));
	for (int i = 0; i < vec1.size(); i++)
		for (int j = 0; j < vec1[i].size(); j++)
			result[i][j] = vec1[i][j] + vec2[i][j];

	return result;
}
vector<double> operator+(const vector<double>& vec1, const vector<double>& vec2) {
	// Pod warunkiem, ¿e maj¹ takie same Rozmiary
	vector<double> result(vec1.size(), 0);
	for (int i = 0; i < vec1.size(); i++)
			result[i] = vec1[i] + vec2[i];

	return result;
}
vector<vector<double>> operator*(const vector<vector<double>>& matrix, double scalar) {
	//Nowy wektor o wielkoœci starego
	vector<vector<double>> result(matrix.size(), vector<double>(matrix[0].size()));

	// Mno¿enie
	for (int i = 0; i < matrix.size(); ++i) {
		for (int j = 0; j < matrix[i].size(); ++j) {
			result[i][j] = matrix[i][j] * scalar;
		}
	}

	return result;
}
vector<vector<double>> operator*(const vector<double> matrix, const vector<double> vec2) {


	// Create a new TwoDVector to store the result
	vector<vector<double>> result;

	// Perform vector multiplication
	for (size_t i = 0; i < matrix.size(); ++i) {
		vector<double> row;
		for (size_t j = 0; j < matrix.size(); ++j) {
			row.push_back(matrix[i] * vec2[j]);
		}
		result.push_back(row);
	}

	return result;
}
vector<double> operator*(const vector<double>& matrix, double scalar) {
	//Nowy wektor o wielkoœci starego
	vector<double> result(matrix.size(), 0);

	for (int i = 0; i < matrix.size(); ++i) 
			result[i] = matrix[i] * scalar;
	
	return result;
}
template <typename T>
ostream& operator<<(ostream& os, vector<vector<T>> vecc) {

	for (auto dwaD : vecc) {
		for (auto x : dwaD) {
			cout << x << "  ";
		}
		cout << endl;
	}
	return os;
}
template <typename T>
ostream& operator<<(ostream& os, vector<T> vecc) {
	for (int i = 0; i < vecc.size(); i++)
		os << vecc[i] << " ";
	os << endl;
	return os;
}


//***************************************************************************	Algorytm rozwiazywania rownan
//Bazowany na kodzie motody LU
void algorithmSimulation() {

	cout << "\n\n\n\t\tPOCZATEK SYMULACJI\n";


	//Init
	P_C = vector<double>(gNodSize, 0);
	temperatures = vector<double>(gNodSize, 0);

	//obliczenie na podstawie  {t0} = initTemp
	for (int g = 0; g < gNodSize; g++)
		P_C[g] = agregatorWektoraP[g] + gInitTemp * (agregatorC_1D[g] / gSimStepTime);


	
	//Pêtla po krokach czasowych, dalsze iteracj
	for (double currentTime = gSimStepTime; currentTime <= gSimTime; currentTime += gSimStepTime)
	{
		vector<double> Y(gNodSize, 0);
		vector<vector<double>> L(gNodSize, vector<double>(gNodSize, 0));
		vector<vector<double>> U(gNodSize, vector<double>(gNodSize, 0));
		//Setup diagonal
		for (int i = 0; i < gNodSize; i++)
			L[i][i] = 1;

		for (int i = 0; i < gNodSize; i++)
		{
			for (int j = 0; j < gNodSize; j++)
			{
				double sum = 0;
				for (int k = 0; k <= i - 1; k++)
					sum += L[i][k] * U[k][j];

				(i <= j) ?
					U[i][j] = (agregatorH[i][j] + (agregatorC_2D[i][j] / gSimStepTime)) - sum
					:
					L[i][j] = (1.0 / U[j][j]) * ((agregatorH[i][j] + (agregatorC_2D[i][j] / gSimStepTime)) - sum);
			}
		}

		for (int i = 0; i < gNodSize; i++)
		{
			double sum = 0;
			for (int j = 0; j <= i - 1; j++)
				sum += L[i][j] * Y[j];

			Y[i] = P_C[i] - sum;
		}

		temperatures[gNodSize - 1] = Y[gNodSize - 1] / U[gNodSize - 1][gNodSize - 1];

		
		for (int j = gNodSize - 2; j >= 0; j--) {
			double summ = 0;
			for (int k = j + 1; k < gNodSize; k++)
				summ += U[j][k] * temperatures[k];
			temperatures[j] = 1 / U[j][j] * (Y[j] - summ);
		}

		//Wypisz na ekran temperature
		//Przystosowane do siatek 4x4, 31x31 juz nie, ale one i tak sie nie mieszcza w konsoli TODO
		cout << "\nT w czasie:" << currentTime << endl;
		for (int i = 0; i < gNodSize; i++)
			cout << temperatures[i] << ((i % 4 == 3) ? "\n" : "\t\t");
		cout << endl;

		//Zapisz do global, do pó¿niejszego u¿ycia 
		Saved_Temperatura.push_back(temperatures);

		for (int i = 0; i < gNodSize; i++)
		{
			P_C[i] = agregatorWektoraP[i];
			for (int j = 0; j < gNodSize; j++)
				P_C[i] += temperatures[j] * agregatorC_2D[i][j] / (gSimStepTime);
		}
		cout << P_C<<endl;
	}

}


//***************************************************************************	RESZTA
void setupPointsWeightsCalk() {
	switch (SCHEMAT) {
	case 2:
		GLOBAL_PKT_CALK = PktCalk2D;
		GLOBAL_WAGI_CALK = wagi2D;
		GLOBAL_PKT_SCIANA = PktCalkHBC_2D;
		GLOBAL_WAGI_SCIANA = weightsBC_2D;
		break;
	case 3:
		GLOBAL_PKT_CALK = PktCalk3D;
		GLOBAL_WAGI_CALK = wagi3D;
		GLOBAL_PKT_SCIANA = PktCalkHBC_3D;
		GLOBAL_WAGI_SCIANA = weightsBC_3D;
		break;
	case 4:
		GLOBAL_PKT_CALK = PktCalk4D;
		GLOBAL_PKT_SCIANA = PKtcalkHBC_4D;
		GLOBAL_WAGI_CALK = wagi4D;
		GLOBAL_WAGI_SCIANA = weightsBC_4D;
		break;

	defualt:
		cout << "ERR, NIE OBSLUGUJE TAKIEGO\n";
		exit(SCHEMAT);
	}
}
double nodeDistance(const Node node1, const Node node2) {

	if (node2.x == node1.x) return abs(node2.y - node1.y);
	if (node2.y == node1.y) return abs(node2.x - node1.x);
	double deltaX = node2.x - node1.x;
	double deltaY = node2.y - node1.y;

	return sqrt(deltaX * deltaX + deltaY * deltaY);
}
double determinant(vector<vector<double>> matrix) {
	double det = 0;

	int size = matrix.size();
	switch (size) {
	case 2:
		return (matrix[0][0] * matrix[1][1]) - (matrix[0][1] * matrix[1][0]);
	case 3:
		return matrix[0][0] * (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1])
			- matrix[0][1] * (matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0])
			+ matrix[0][2] * (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]);
	case 4:
		for (int i = 0; i < 4; ++i) {
			vector<vector<double>> subMat(3, vector<double>(3, 0.0));
			for (int j = 1; j < 4; j++) {
				int col = 0;
				for (int k = 0; k < 4; k++) {
					if (k != i)
						subMat[j - 1][col++] = matrix[j][k];
				}
			}
			det += (i % 2 == 0 ? 1 : -1) * matrix[0][i] * determinant(subMat);//Naprzemienne odejmowanie
		}
		return det;
	default:
		cout << "ERR, ZLY SIZE MATRIXA:" << size << "\n";
		exit(-999);
	}
}
void displayMinAndMax(const vector<double>& vecc) {

	double minVal = vecc[0];
	double maxVal = vecc[0];

	for (double value : vecc) {
		if (value < minVal)	minVal = value;
		if (value > maxVal)	maxVal = value;
	}


	cout << "Min: " << minVal << "\tMax: " << maxVal << endl;
}